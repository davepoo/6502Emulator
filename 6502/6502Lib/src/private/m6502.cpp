#include "m6502.h"

m6502::s32 m6502::CPU::Execute( s32 Cycles, Mem & memory )
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
			printf( "Instruction %d not handled\n", Ins );
			throw - 1;
		} break;
		}
	}

	const s32 NumCyclesUsed = CyclesRequested - Cycles;
	return NumCyclesUsed;
}
