#include "m6502.h"

m6502::s32 m6502::CPU::Execute( s32 Cycles, Mem & memory )
{
	/** Load a Register with the value from the memory address */
	auto LoadRegister = 
		[&Cycles,&memory,this]
		( Word Address, Byte& Register )
	{
		Register = ReadByte( Cycles, Address, memory );
		LoadRegisterSetStatus( Register );
	};

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
			LoadRegister( Address, A );	
		} break;
		case INS_LDX_ZP:
		{
			Word Address = AddrZeroPage( Cycles, memory );
			LoadRegister( Address, X );
		} break;
		case INS_LDX_ZPY:
		{
			Word Address = AddrZeroPageY( Cycles, memory );
			LoadRegister( Address, X );
		} break;
		case INS_LDY_ZP:
		{
			Word Address = AddrZeroPage( Cycles, memory );
			LoadRegister( Address, Y );
		} break;
		case INS_LDA_ZPX:
		{
			Word Address = AddrZeroPageX( Cycles, memory );
			LoadRegister( Address, A );
		} break;
		case INS_LDY_ZPX:
		{
			Word Address = AddrZeroPageX( Cycles, memory );
			LoadRegister( Address, Y );
		} break;
		case INS_LDA_ABS:
		{
			Word Address = AddrAbsolute( Cycles, memory );
			LoadRegister( Address, A );
		} break;
		case INS_LDX_ABS:
		{
			Word Address = AddrAbsolute( Cycles, memory );
			LoadRegister( Address, X );
		} break;
		case INS_LDY_ABS:
		{
			Word Address = AddrAbsolute( Cycles, memory );
			LoadRegister( Address, Y );
		} break;
		case INS_LDA_ABSX:
		{
			Word Address = AddrAbsoluteX( Cycles, memory );
			LoadRegister( Address, A );
		} break;
		case INS_LDY_ABSX:
		{
			Word Address = AddrAbsoluteX( Cycles, memory );
			LoadRegister( Address, Y );
		} break;
		case INS_LDA_ABSY:
		{
			Word Address = AddrAbsoluteY( Cycles, memory );
			LoadRegister( Address, A );
		} break;
		case INS_LDX_ABSY:
		{
			Word Address = AddrAbsoluteY( Cycles, memory );
			LoadRegister( Address, X );
		} break;
		case INS_LDA_INDX:
		{
			Word Address = AddrIndirectX( Cycles, memory );
			LoadRegister( Address, A );
		} break;
		case INS_STA_INDX:
		{
			Word Address = AddrIndirectX( Cycles, memory );
			WriteByte( A, Cycles, Address, memory );
		} break;
		case INS_LDA_INDY:
		{
			Word Address = AddrIndirectY( Cycles, memory );
			LoadRegister( Address, A );
		} break;
		case INS_STA_INDY:
		{
			Word Address = AddrIndirectY_6( Cycles, memory );
			WriteByte( A, Cycles, Address, memory );
		} break;
		case INS_STA_ZP:
		{
			Word Address = AddrZeroPage( Cycles, memory );
			WriteByte( A, Cycles, Address, memory );
		} break;
		case INS_STX_ZP:
		{
			Word Address = AddrZeroPage( Cycles, memory );
			WriteByte( X, Cycles, Address, memory );
		} break;
		case INS_STY_ZP:
		{
			Word Address = AddrZeroPage( Cycles, memory );
			WriteByte( Y, Cycles, Address, memory );
		} break;
		case INS_STA_ABS:
		{
			Word Address = AddrAbsolute( Cycles, memory );
			WriteByte( A, Cycles, Address, memory );
		} break;
		case INS_STX_ABS:
		{
			Word Address = AddrAbsolute( Cycles, memory );
			WriteByte( X, Cycles, Address, memory );
		} break;
		case INS_STY_ABS:
		{
			Word Address = AddrAbsolute( Cycles, memory );
			WriteByte( Y, Cycles, Address, memory );
		} break;
		case INS_STA_ZPX:
		{
			Word Address = AddrZeroPageX( Cycles, memory );
			WriteByte( A, Cycles, Address, memory );
		} break;
		case INS_STY_ZPX:
		{
			Word Address = AddrZeroPageX( Cycles, memory );
			WriteByte( Y, Cycles, Address, memory );
		} break;
		case INS_STA_ABSX:
		{
			Word Address = AddrAbsoluteX_5( Cycles, memory );
			WriteByte( A, Cycles, Address, memory );
		} break;
		case INS_STA_ABSY:
		{
			Word Address = AddrAbsoluteY_5( Cycles, memory );
			WriteByte( A, Cycles, Address, memory );
		} break;		
		case INS_JSR:
		{
			Word SubAddr = FetchWord( Cycles, memory );
			PushPCToStack( Cycles, memory );	
			PC = SubAddr;
			Cycles--;
		} break;
		case INS_RTS:
		{
			Word ReturnAddress = PopWordFromStack( Cycles, memory );
			PC = ReturnAddress + 1;	
			Cycles -= 2;
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


m6502::Word m6502::CPU::AddrZeroPage( s32& Cycles, const Mem& memory )
{
	Byte ZeroPageAddr = FetchByte( Cycles, memory );
	return ZeroPageAddr;
}

m6502::Word m6502::CPU::AddrZeroPageX( s32& Cycles, const Mem& memory )
{
	Byte ZeroPageAddr = FetchByte( Cycles, memory );
	ZeroPageAddr += X;
	Cycles--;
	return ZeroPageAddr;
}

m6502::Word m6502::CPU::AddrZeroPageY( s32& Cycles, const Mem& memory )
{
	Byte ZeroPageAddr = FetchByte( Cycles, memory );
	ZeroPageAddr += Y;
	Cycles--;
	return ZeroPageAddr;
}

m6502::Word m6502::CPU::AddrAbsolute( s32& Cycles, const Mem& memory )
{
	Word AbsAddress = FetchWord( Cycles, memory );
	return AbsAddress;
}

m6502::Word m6502::CPU::AddrAbsoluteX( s32& Cycles, const Mem& memory )
{
	Word AbsAddress = FetchWord( Cycles, memory );
	Word AbsAddressX = AbsAddress + X;
	if ( AbsAddressX - AbsAddress >= 0xFF )
	{
		Cycles--;
	}

	return AbsAddressX;
}

m6502::Word m6502::CPU::AddrAbsoluteX_5( s32& Cycles, const Mem& memory )
{
	Word AbsAddress = FetchWord( Cycles, memory );
	Word AbsAddressX = AbsAddress + X;
	Cycles--;
	return AbsAddressX;
}

m6502::Word m6502::CPU::AddrAbsoluteY( s32& Cycles, const Mem& memory )
{
	Word AbsAddress = FetchWord( Cycles, memory );
	Word AbsAddressY = AbsAddress + Y;
	if ( AbsAddressY - AbsAddress >= 0xFF )
	{
		Cycles--;
	}

	return AbsAddressY;
}

m6502::Word m6502::CPU::AddrAbsoluteY_5( s32& Cycles, const Mem& memory )
{
	Word AbsAddress = FetchWord( Cycles, memory );
	Word AbsAddressY = AbsAddress + Y;	
	Cycles--;
	return AbsAddressY;
}

m6502::Word m6502::CPU::AddrIndirectX( s32& Cycles, const Mem& memory )
{
	Byte ZPAddress = FetchByte( Cycles, memory );
	ZPAddress += X;
	Cycles--;
	Word EffectiveAddr = ReadWord( Cycles, ZPAddress, memory );
	return EffectiveAddr;
}

m6502::Word m6502::CPU::AddrIndirectY( s32& Cycles, const Mem& memory )
{
	Byte ZPAddress = FetchByte( Cycles, memory );
	Word EffectiveAddr = ReadWord( Cycles, ZPAddress, memory );
	Word EffectiveAddrY = EffectiveAddr + Y;
	if ( EffectiveAddrY - EffectiveAddr >= 0xFF )
	{
		Cycles--;
	}
	return EffectiveAddrY;
}

m6502::Word m6502::CPU::AddrIndirectY_6( s32& Cycles, const Mem& memory )
{
	Byte ZPAddress = FetchByte( Cycles, memory );
	Word EffectiveAddr = ReadWord( Cycles, ZPAddress, memory );
	Word EffectiveAddrY = EffectiveAddr + Y;
	Cycles--;
	return EffectiveAddrY;
}

