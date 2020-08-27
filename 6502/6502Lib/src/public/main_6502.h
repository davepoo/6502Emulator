#pragma once
#include <stdio.h>
#include <stdlib.h>

// http://www.obelisk.me.uk/6502/

using Byte = unsigned char;
using Word = unsigned short;

using u32 = unsigned int;
using s32 = signed int;

struct Mem
{
	static constexpr u32 MAX_MEM = 1024 * 64;
	Byte Data[MAX_MEM];

	void Initialise()
	{
		for ( u32 i = 0; i < MAX_MEM; i++ )
		{
			Data[i] = 0;
		}
	}

	/** read 1 byte */
	Byte operator[]( u32 Address ) const
	{
		// assert here Address is < MAX_MEM
		return Data[Address];
	}

	/** write 1 byte */
	Byte& operator[]( u32 Address )
	{
		// assert here Address is < MAX_MEM
		return Data[Address];
	}

	/** write 2 bytes */
	void WriteWord( 
		Word Value, 
		u32 Address, 
		s32& Cycles )
	{
		Data[Address]		= Value & 0xFF;
		Data[Address + 1]   = (Value >> 8);
		Cycles -= 2;
	}
};

struct CPU
{
	Word PC;		//program counter
	Word SP;		//stack pointer

	Byte A, X, Y;	//registers

	Byte C : 1;	//status flag
	Byte Z : 1;	//status flag
	Byte I : 1;//status flag
	Byte D : 1;//status flag
	Byte B : 1;//status flag
	Byte V : 1;//status flag
	Byte N : 1;//status flag

	void Reset( Mem& memory )
	{
		Reset( 0xFFFC, memory );
	}

	void Reset( Word ResetVector, Mem& memory )
	{
		PC = ResetVector;
		SP = 0x0100;
		C = Z = I = D = B = V = N = 0;
		A = X = Y = 0;
		memory.Initialise();
	}

	Byte FetchByte( s32& Cycles, Mem& memory )
	{
		Byte Data = memory[PC];
		PC++;
		Cycles--;
		return Data;
	}

	Word FetchWord( s32& Cycles, Mem& memory )
	{
		// 6502 is little endian
		Word Data = memory[PC];
		PC++;
		
		Data |= (memory[PC] << 8 );
		PC++;

		Cycles -= 2;

		// if you wanted to handle endianness
		// you would have to swap bytes here
		// if ( PLATFORM_BIG_ENDIAN )
		//	SwapBytesInWord(Data)

		return Data;
	}

	Byte ReadByte(
		s32& Cycles,
		Word Address,
		Mem& memory )
	{
		Byte Data = memory[Address];
		Cycles--;
		return Data;
	}

	Word ReadWord(
		s32& Cycles,
		Word Address,
		Mem& memory )
	{
		Byte LoByte = ReadByte( Cycles, Address, memory );
		Byte HiByte = ReadByte( Cycles, Address + 1, memory );
		return LoByte | (HiByte << 8);
	}

	// opcodes
	static constexpr Byte
		INS_LDA_IM = 0xA9,
		INS_LDA_ZP = 0xA5,
		INS_LDA_ZPX = 0xB5,
		INS_LDA_ABS = 0xAD,
		INS_LDA_ABSX = 0xBD,
		INS_LDA_ABSY = 0xB9,
		INS_LDA_INDX = 0xA1,
		INS_LDA_INDY = 0xB1,
		INS_JSR = 0x20;

	void LDASetStatus()
	{
		Z = (A == 0);
		N = (A & 0b10000000) > 0;
	}

	/** @return the number of cycles that were used */
	s32 Execute( s32 Cycles, Mem& memory )
	{
		const s32 CyclesRequested = Cycles;
		while ( Cycles > 0 )
		{
			Byte Ins = FetchByte( Cycles, memory );			
			switch ( Ins )
			{
			case INS_LDA_IM:
			{
				Byte Value = 
					FetchByte( Cycles, memory );
				A = Value;
				LDASetStatus();
			} break;
			case INS_LDA_ZP:
			{
				Byte ZeroPageAddr =
					FetchByte( Cycles, memory );
				A = ReadByte( 
					Cycles, ZeroPageAddr, memory );
				LDASetStatus();
			} break;
			case INS_LDA_ZPX:
			{
				Byte ZeroPageAddr =
					FetchByte( Cycles, memory );
				ZeroPageAddr += X;
				Cycles--;
				A = ReadByte(
					Cycles, ZeroPageAddr, memory );
				LDASetStatus();
			} break;
			case INS_LDA_ABS:
			{
				Word AbsAddress = FetchWord( Cycles, memory );
				A = ReadByte( Cycles, AbsAddress, memory );
			} break;
			case INS_LDA_ABSX:
			{
				Word AbsAddress = FetchWord( Cycles, memory );
				Word AbsAddressX = AbsAddress + X;
				A = ReadByte( Cycles, AbsAddressX, memory );
				if ( AbsAddressX - AbsAddress >= 0xFF )
				{
					Cycles--;
				}
			} break;
			case INS_LDA_ABSY:
			{
				Word AbsAddress = FetchWord( Cycles, memory );
				Word AbsAddressY = AbsAddress + Y;
				A = ReadByte( Cycles, AbsAddressY, memory );
				if ( AbsAddressY - AbsAddress >= 0xFF )
				{
					Cycles--;
				}
			} break;
			case INS_LDA_INDX:
			{
				Byte ZPAddress = FetchByte( Cycles, memory );
				ZPAddress += X;
				Cycles--;
				Word EffectiveAddr = ReadWord( Cycles, ZPAddress, memory );
				A = ReadByte( Cycles, EffectiveAddr, memory );
			} break;
			case INS_LDA_INDY:
			{
				Byte ZPAddress = FetchByte( Cycles, memory );
				Word EffectiveAddr = ReadWord( Cycles, ZPAddress, memory );
				Word EffectiveAddrY = EffectiveAddr + Y;
				A = ReadByte( Cycles, EffectiveAddrY, memory );
				if ( EffectiveAddrY - EffectiveAddr >= 0xFF )
				{
					Cycles--;
				}
			} break;
			case INS_JSR:
			{
				Word SubAddr = 
					FetchWord( Cycles, memory );
				memory.WriteWord( 
					PC - 1, SP, Cycles );
				SP += 2;				//(forgot to do this during the video, probably need a function to push onto the stack!)
				PC = SubAddr;
				Cycles--;
			} break;
			default:
			{
				printf("Instruction %d not handled\n", Ins );
				throw -1;
			} break;
			}
		}

		const s32 NumCyclesUsed = CyclesRequested - Cycles;
		return NumCyclesUsed;
	}
};