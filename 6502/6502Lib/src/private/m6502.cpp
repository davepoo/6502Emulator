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
			A = FetchByte( Cycles, memory );
			LoadRegisterSetStatus( A );
		} break;
		case INS_LDX_IM:
		{
			X = FetchByte( Cycles, memory );
			LoadRegisterSetStatus( X );
		} break;
		case INS_LDY_IM:
		{
			Y = FetchByte( Cycles, memory );
			LoadRegisterSetStatus( Y );
		} break;
		case INS_LDA_ZP:
		{
			Word Address = AddrZeroPage( Cycles, memory );
			A = ReadByte( Cycles, Address, memory );
			LoadRegisterSetStatus( A );
		} break;
		case INS_LDX_ZP:
		{
			Word Address = AddrZeroPage( Cycles, memory );
			X = ReadByte( Cycles, Address, memory );
			LoadRegisterSetStatus( X );
		} break;
		case INS_LDX_ZPY:
		{
			Word Address = AddrZeroPageY( Cycles, memory );
			X = ReadByte( Cycles, Address, memory );
			LoadRegisterSetStatus( X );
		} break;
		case INS_LDY_ZP:
		{
			Word Address = AddrZeroPage( Cycles, memory );
			Y = ReadByte( Cycles, Address, memory );
			LoadRegisterSetStatus( Y );
		} break;
		case INS_LDA_ZPX:
		{
			Word Address = AddrZeroPageX( Cycles, memory );
			A = ReadByte( Cycles, Address, memory );
			LoadRegisterSetStatus( A );
		} break;
		case INS_LDY_ZPX:
		{
			Word Address = AddrZeroPageX( Cycles, memory );
			Y = ReadByte( Cycles, Address, memory );
			LoadRegisterSetStatus( Y );
		} break;
		case INS_LDA_ABS:
		{
			Word Address = AddrAbsolute( Cycles, memory );
			A = ReadByte( Cycles, Address, memory );
			LoadRegisterSetStatus( A );
		} break;
		case INS_LDX_ABS:
		{
			Word Address = AddrAbsolute( Cycles, memory );
			X = ReadByte( Cycles, Address, memory );
			LoadRegisterSetStatus( X );
		} break;
		case INS_LDY_ABS:
		{
			Word Address = AddrAbsolute( Cycles, memory );
			Y = ReadByte( Cycles, Address, memory );
			LoadRegisterSetStatus( Y );
		} break;
		case INS_LDA_ABSX:
		{
			Word Address = AddrAbsoluteX( Cycles, memory );
			A = ReadByte( Cycles, Address, memory );
			LoadRegisterSetStatus( A );
		} break;
		case INS_LDY_ABSX:
		{
			Word Address = AddrAbsoluteX( Cycles, memory );
			Y = ReadByte( Cycles, Address, memory );
			LoadRegisterSetStatus( Y );
		} break;
		case INS_LDA_ABSY:
		{
			Word Address = AddrAbsoluteY( Cycles, memory );
			A = ReadByte( Cycles, Address, memory );
			LoadRegisterSetStatus( A );
		} break;
		case INS_LDX_ABSY:
		{
			Word Address = AddrAbsoluteY( Cycles, memory );
			X = ReadByte( Cycles, Address, memory );
			LoadRegisterSetStatus( X );
		} break;
		case INS_LDA_INDX:
		{
			Byte ZPAddress = FetchByte( Cycles, memory );
			ZPAddress += X;
			Cycles--;
			Word EffectiveAddr = ReadWord( Cycles, ZPAddress, memory );
			A = ReadByte( Cycles, EffectiveAddr, memory );
			LoadRegisterSetStatus( A );
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
			LoadRegisterSetStatus( A );
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


m6502::Word m6502::CPU::AddrZeroPage( s32& Cycles, Mem& memory )
{
	Byte ZeroPageAddr = FetchByte( Cycles, memory );
	return ZeroPageAddr;
}

m6502::Word m6502::CPU::AddrZeroPageX( s32& Cycles, Mem& memory )
{
	Byte ZeroPageAddr = FetchByte( Cycles, memory );
	ZeroPageAddr += X;
	Cycles--;
	return ZeroPageAddr;
}

m6502::Word m6502::CPU::AddrZeroPageY( s32& Cycles, Mem& memory )
{
	Byte ZeroPageAddr = FetchByte( Cycles, memory );
	ZeroPageAddr += Y;
	Cycles--;
	return ZeroPageAddr;
}

m6502::Word m6502::CPU::AddrAbsolute( s32& Cycles, Mem& memory )
{
	Word AbsAddress = FetchWord( Cycles, memory );
	return AbsAddress;
}

m6502::Word m6502::CPU::AddrAbsoluteX( s32& Cycles, Mem& memory )
{
	Word AbsAddress = FetchWord( Cycles, memory );
	Word AbsAddressX = AbsAddress + X;
	if ( AbsAddressX - AbsAddress >= 0xFF )
	{
		Cycles--;
	}

	return AbsAddressX;
}

m6502::Word m6502::CPU::AddrAbsoluteY( s32& Cycles, Mem& memory )
{
	Word AbsAddress = FetchWord( Cycles, memory );
	Word AbsAddressY = AbsAddress + Y;
	if ( AbsAddressY - AbsAddress >= 0xFF )
	{
		Cycles--;
	}

	return AbsAddressY;
}