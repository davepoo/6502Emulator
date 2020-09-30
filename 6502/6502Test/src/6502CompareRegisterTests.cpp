#include <gtest/gtest.h>
#include "m6502.h"

class M6502CompareRegisterTests : public testing::Test
{
public:
	m6502::Mem mem;
	m6502::CPU cpu;

	virtual void SetUp()
	{
		cpu.Reset( mem );
	}

	virtual void TearDown()
	{
	}

	void ExpectUnaffectedRegisters( m6502::CPU CPUBefore )
	{
		EXPECT_EQ( CPUBefore.Flag.I, cpu.Flag.I );
		EXPECT_EQ( CPUBefore.Flag.D, cpu.Flag.D );
		EXPECT_EQ( CPUBefore.Flag.B, cpu.Flag.B );
		EXPECT_EQ( CPUBefore.Flag.V, cpu.Flag.V );
	}

	struct CMPTestData
	{
		m6502::Byte RegisterValue;
		m6502::Byte Operand;

		bool ExpectC;
		bool ExpectZ;
		bool ExpectN;
	};

	CMPTestData CompareTwoIdenticalValues()
	{
		CMPTestData Test;
		Test.RegisterValue = 26;
		Test.Operand = 26;
		Test.ExpectZ = true;
		Test.ExpectN = false;
		Test.ExpectC = true;
		return Test;
	}

	CMPTestData CompareALargePositiveToASmallPositive()
	{
		CMPTestData Test;
		Test.RegisterValue = 48;
		Test.Operand = 26;
		Test.ExpectZ = false;
		Test.ExpectN = false;
		Test.ExpectC = true;
		return Test;
	}

	CMPTestData CompareANegativeNumberToAPositive()
	{
		CMPTestData Test;
		Test.RegisterValue = 130; //Negative number!
		Test.Operand = 26;
		Test.ExpectZ = false;
		Test.ExpectN = false;
		Test.ExpectC = true;
		return Test;
	}

	CMPTestData CompareTwoValuesThatResultInANegativeFlagSet()
	{
		CMPTestData Test;
		Test.RegisterValue = 8;
		Test.Operand = 26;
		Test.ExpectZ = false;
		Test.ExpectN = true;
		Test.ExpectC = false;
		return Test;
	}

	enum class ERegister
	{
		A, X, Y
	};

	void CompareImmediate( CMPTestData Test, ERegister RegisterToCompare )
	{
		// given:
		using namespace m6502;
		cpu.Reset( 0xFF00, mem );
		cpu.Flag.C = !Test.ExpectC;
		cpu.Flag.Z = !Test.ExpectZ;
		cpu.Flag.N = !Test.ExpectN;
		Byte* Register = &cpu.A;
		Byte Opcode = CPU::INS_CMP;
		switch ( RegisterToCompare )
		{
		case ERegister::X:
			Register = &cpu.X;
			Opcode = CPU::INS_CPX;
			break;
		case ERegister::Y:
			Register = &cpu.Y;
			Opcode = CPU::INS_CPY;
			break;
		};
		*Register = Test.RegisterValue;

		mem[0xFF00] = Opcode;
		mem[0xFF01] = Test.Operand;
		constexpr s32 EXPECTED_CYCLES = 2;
		CPU CPUCopy = cpu;

		// when:
		const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

		// then:
		EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
		EXPECT_EQ( *Register, Test.RegisterValue );
		EXPECT_EQ( cpu.Flag.Z, Test.ExpectZ );
		EXPECT_EQ( cpu.Flag.N, Test.ExpectN );
		EXPECT_EQ( cpu.Flag.C, Test.ExpectC );
		ExpectUnaffectedRegisters( CPUCopy );
	}

	void CompareZeroPage( CMPTestData Test, ERegister RegisterToCompare )
	{
		// given:
		using namespace m6502;
		cpu.Reset( 0xFF00, mem );
		cpu.Flag.C = !Test.ExpectC;
		cpu.Flag.Z = !Test.ExpectZ;
		cpu.Flag.N = !Test.ExpectN;
		
		Byte* Register = &cpu.A;
		Byte Opcode = CPU::INS_CMP_ZP;
		switch ( RegisterToCompare )
		{
		case ERegister::X:
			Register = &cpu.X;
			Opcode = CPU::INS_CPX_ZP;
			break;
		case ERegister::Y:
			Register = &cpu.Y;
			Opcode = CPU::INS_CPY_ZP;
			break;
		};
		*Register = Test.RegisterValue;
		mem[0xFF00] = Opcode;
		mem[0xFF01] = 0x42;
		mem[0x0042] = Test.Operand;
		constexpr s32 EXPECTED_CYCLES = 3;
		CPU CPUCopy = cpu;

		// when:
		const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

		// then:
		EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
		EXPECT_EQ( *Register, Test.RegisterValue );
		EXPECT_EQ( cpu.Flag.Z, Test.ExpectZ );
		EXPECT_EQ( cpu.Flag.N, Test.ExpectN );
		EXPECT_EQ( cpu.Flag.C, Test.ExpectC );
		ExpectUnaffectedRegisters( CPUCopy );
	}

	void CMPZeroPageX( CMPTestData Test )
	{
		// given:
		using namespace m6502;
		cpu.Reset( 0xFF00, mem );
		cpu.Flag.C = !Test.ExpectC;
		cpu.Flag.Z = !Test.ExpectZ;
		cpu.Flag.N = !Test.ExpectN;
		cpu.A = Test.RegisterValue;
		cpu.X = 4;
		mem[0xFF00] = CPU::INS_CMP_ZPX;
		mem[0xFF01] = 0x42;
		mem[0x0042+0x4] = Test.Operand;
		constexpr s32 EXPECTED_CYCLES = 4;
		CPU CPUCopy = cpu;

		// when:
		const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

		// then:
		EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
		EXPECT_EQ( cpu.A, Test.RegisterValue );
		EXPECT_EQ( cpu.X, 4 );
		EXPECT_EQ( cpu.Flag.Z, Test.ExpectZ );
		EXPECT_EQ( cpu.Flag.N, Test.ExpectN );
		EXPECT_EQ( cpu.Flag.C, Test.ExpectC );
		ExpectUnaffectedRegisters( CPUCopy );
	}

	void CompareAbsolute( CMPTestData Test, ERegister RegisterToCompare )
	{
		// given:
		using namespace m6502;
		cpu.Reset( 0xFF00, mem );
		cpu.Flag.C = !Test.ExpectC;
		cpu.Flag.Z = !Test.ExpectZ;
		cpu.Flag.N = !Test.ExpectN;

		Byte* Register = &cpu.A;
		Byte Opcode = CPU::INS_CMP_ABS;
		switch ( RegisterToCompare )
		{
		case ERegister::X:
			Register = &cpu.X;
			Opcode = CPU::INS_CPX_ABS;
			break;
		case ERegister::Y:
			Register = &cpu.Y;
			Opcode = CPU::INS_CPY_ABS;
			break;
		};
		*Register = Test.RegisterValue;

		mem[0xFF00] = Opcode;
		mem[0xFF01] = 0x00;
		mem[0xFF02] = 0x80;
		mem[0x8000] = Test.Operand;
		constexpr s32 EXPECTED_CYCLES = 4;
		CPU CPUCopy = cpu;

		// when:
		const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

		// then:
		EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
		EXPECT_EQ( *Register, Test.RegisterValue );
		EXPECT_EQ( cpu.Flag.Z, Test.ExpectZ );
		EXPECT_EQ( cpu.Flag.N, Test.ExpectN );
		EXPECT_EQ( cpu.Flag.C, Test.ExpectC );
		ExpectUnaffectedRegisters( CPUCopy );
	}

	void CMPAbsoluteX( CMPTestData Test )
	{
		// given:
		using namespace m6502;
		cpu.Reset( 0xFF00, mem );
		cpu.Flag.C = !Test.ExpectC;
		cpu.Flag.Z = !Test.ExpectZ;
		cpu.Flag.N = !Test.ExpectN;
		cpu.A = Test.RegisterValue;
		cpu.X = 4;
		mem[0xFF00] = CPU::INS_CMP_ABSX;
		mem[0xFF01] = 0x00;
		mem[0xFF02] = 0x80;
		mem[0x8000+4] = Test.Operand;
		constexpr s32 EXPECTED_CYCLES = 4;
		CPU CPUCopy = cpu;

		// when:
		const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

		// then:
		EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
		EXPECT_EQ( cpu.A, Test.RegisterValue );
		EXPECT_EQ( cpu.X, 4 );
		EXPECT_EQ( cpu.Flag.Z, Test.ExpectZ );
		EXPECT_EQ( cpu.Flag.N, Test.ExpectN );
		EXPECT_EQ( cpu.Flag.C, Test.ExpectC );
		ExpectUnaffectedRegisters( CPUCopy );
	}

	void CMPAbsoluteY( CMPTestData Test )
	{
		// given:
		using namespace m6502;
		cpu.Reset( 0xFF00, mem );
		cpu.Flag.C = !Test.ExpectC;
		cpu.Flag.Z = !Test.ExpectZ;
		cpu.Flag.N = !Test.ExpectN;
		cpu.A = Test.RegisterValue;
		cpu.Y = 4;
		mem[0xFF00] = CPU::INS_CMP_ABSY;
		mem[0xFF01] = 0x00;
		mem[0xFF02] = 0x80;
		mem[0x8000 + 4] = Test.Operand;
		constexpr s32 EXPECTED_CYCLES = 4;
		CPU CPUCopy = cpu;

		// when:
		const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

		// then:
		EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
		EXPECT_EQ( cpu.A, Test.RegisterValue );
		EXPECT_EQ( cpu.Y, 4 );
		EXPECT_EQ( cpu.Flag.Z, Test.ExpectZ );
		EXPECT_EQ( cpu.Flag.N, Test.ExpectN );
		EXPECT_EQ( cpu.Flag.C, Test.ExpectC );
		ExpectUnaffectedRegisters( CPUCopy );
	}

	void CMPIndirectX( CMPTestData Test )
	{
		// given:
		using namespace m6502;
		cpu.Reset( 0xFF00, mem );
		cpu.Flag.C = !Test.ExpectC;
		cpu.Flag.Z = !Test.ExpectZ;
		cpu.Flag.N = !Test.ExpectN;
		cpu.A = Test.RegisterValue;
		cpu.X = 4;
		mem[0xFF00] = CPU::INS_CMP_INDX;
		mem[0xFF01] = 0x42;
		mem[0x42 + 4] = 0x00;
		mem[0x42 + 5] = 0x80;
		mem[0x8000] = Test.Operand;
		constexpr s32 EXPECTED_CYCLES = 6;
		CPU CPUCopy = cpu;

		// when:
		const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

		// then:
		EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
		EXPECT_EQ( cpu.A, Test.RegisterValue );
		EXPECT_EQ( cpu.X, 4 );
		EXPECT_EQ( cpu.Flag.Z, Test.ExpectZ );
		EXPECT_EQ( cpu.Flag.N, Test.ExpectN );
		EXPECT_EQ( cpu.Flag.C, Test.ExpectC );
		ExpectUnaffectedRegisters( CPUCopy );
	}

	void CMPIndirectY( CMPTestData Test )
	{
		// given:
		using namespace m6502;
		cpu.Reset( 0xFF00, mem );
		cpu.Flag.C = !Test.ExpectC;
		cpu.Flag.Z = !Test.ExpectZ;
		cpu.Flag.N = !Test.ExpectN;
		cpu.A = Test.RegisterValue;
		cpu.Y = 4;
		mem[0xFF00] = CPU::INS_CMP_INDY;
		mem[0xFF01] = 0x42;
		mem[0x42] = 0x00;
		mem[0x43] = 0x80;
		mem[0x8000+4] = Test.Operand;
		constexpr s32 EXPECTED_CYCLES = 5;
		CPU CPUCopy = cpu;

		// when:
		const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

		// then:
		EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
		EXPECT_EQ( cpu.A, Test.RegisterValue );
		EXPECT_EQ( cpu.Y, 4 );
		EXPECT_EQ( cpu.Flag.Z, Test.ExpectZ );
		EXPECT_EQ( cpu.Flag.N, Test.ExpectN );
		EXPECT_EQ( cpu.Flag.C, Test.ExpectC );
		ExpectUnaffectedRegisters( CPUCopy );
	}
};

//-- Immediate

TEST_F( M6502CompareRegisterTests, CMPImmediateCanCompareTwoIdenticalValues  )
{
	CMPTestData Test = CompareTwoIdenticalValues();
	CompareImmediate( Test, ERegister::A );
}

TEST_F( M6502CompareRegisterTests, CMPImmediateCanCompareALargePositiveToASmallPositive )
{
	CMPTestData Test = CompareALargePositiveToASmallPositive();
	CompareImmediate( Test, ERegister::A );
}

TEST_F( M6502CompareRegisterTests, CMPImmediateCanCompareANegativeNumberToAPositive )
{
	CMPTestData Test = CompareANegativeNumberToAPositive();
	CompareImmediate( Test, ERegister::A );
}

TEST_F( M6502CompareRegisterTests, CMPImmediateCanCompareTwoValuesThatResultInANegativeFlagSet )
{
	CMPTestData Test = CompareTwoValuesThatResultInANegativeFlagSet();
	CompareImmediate( Test, ERegister::A );
}

//-- Zero Page

TEST_F( M6502CompareRegisterTests, CMPZeroPageCanCompareTwoIdenticalValues )
{
	CMPTestData Test = CompareTwoIdenticalValues();
	CompareZeroPage( Test, ERegister::A );
}

TEST_F( M6502CompareRegisterTests, CMPZeroPageCanCompareALargePositiveToASmallPositive )
{
	CMPTestData Test = CompareALargePositiveToASmallPositive();
	CompareZeroPage( Test, ERegister::A );
}

TEST_F( M6502CompareRegisterTests, CMPZeroPageCanCompareANegativeNumberToAPositive )
{
	CMPTestData Test = CompareANegativeNumberToAPositive();
	CompareZeroPage( Test, ERegister::A );
}

TEST_F( M6502CompareRegisterTests, CMPZeroPageCanCompareTwoValuesThatResultInANegativeFlagSet )
{
	CMPTestData Test = CompareTwoValuesThatResultInANegativeFlagSet();
	CompareZeroPage( Test, ERegister::A );
}

//-- Zero Page X

TEST_F( M6502CompareRegisterTests, CMPZeroPageXCanCompareTwoIdenticalValues )
{
	CMPTestData Test = CompareTwoIdenticalValues();
	CMPZeroPageX( Test );
}

TEST_F( M6502CompareRegisterTests, CMPZeroPageXCanCompareALargePositiveToASmallPositive )
{
	CMPTestData Test = CompareALargePositiveToASmallPositive();
	CMPZeroPageX( Test );
}

TEST_F( M6502CompareRegisterTests, CMPZeroPageXCanCompareANegativeNumberToAPositive )
{
	CMPTestData Test = CompareANegativeNumberToAPositive();
	CMPZeroPageX( Test );
}

TEST_F( M6502CompareRegisterTests, CMPZeroPageXCanCompareTwoValuesThatResultInANegativeFlagSet )
{
	CMPTestData Test = CompareTwoValuesThatResultInANegativeFlagSet();
	CMPZeroPageX( Test );
}

//-- Absolute

TEST_F( M6502CompareRegisterTests, CMPAbsoluteCanCompareTwoIdenticalValues )
{
	CMPTestData Test = CompareTwoIdenticalValues();
	CompareAbsolute( Test, ERegister::A );
}

TEST_F( M6502CompareRegisterTests, CMPAbsoluteCanCompareALargePositiveToASmallPositive )
{
	CMPTestData Test = CompareALargePositiveToASmallPositive();
	CompareAbsolute( Test, ERegister::A );
}

TEST_F( M6502CompareRegisterTests, CMPAbsoluteCanCompareANegativeNumberToAPositive )
{
	CMPTestData Test = CompareANegativeNumberToAPositive();
	CompareAbsolute( Test, ERegister::A );
}

TEST_F( M6502CompareRegisterTests, CMPAbsoluteCanCompareTwoValuesThatResultInANegativeFlagSet )
{
	CMPTestData Test = CompareTwoValuesThatResultInANegativeFlagSet();
	CompareAbsolute( Test, ERegister::A );
}

//-- Absolute X

TEST_F( M6502CompareRegisterTests, CMPAbsoluteXCanCompareTwoIdenticalValues )
{
	CMPTestData Test = CompareTwoIdenticalValues();
	CMPAbsoluteX( Test );
}

TEST_F( M6502CompareRegisterTests, CMPAbsoluteXCanCompareALargePositiveToASmallPositive )
{
	CMPTestData Test = CompareALargePositiveToASmallPositive();
	CMPAbsoluteX( Test );
}

TEST_F( M6502CompareRegisterTests, CMPAbsoluteXCanCompareANegativeNumberToAPositive )
{
	CMPTestData Test = CompareANegativeNumberToAPositive();
	CMPAbsoluteX( Test );
}

TEST_F( M6502CompareRegisterTests, CMPAbsoluteXCanCompareTwoValuesThatResultInANegativeFlagSet )
{
	CMPTestData Test = CompareTwoValuesThatResultInANegativeFlagSet();
	CMPAbsoluteX( Test );
}

//-- Absolute Y

TEST_F( M6502CompareRegisterTests, CMPAbsoluteYCanCompareTwoIdenticalValues )
{
	CMPTestData Test = CompareTwoIdenticalValues();
	CMPAbsoluteY( Test );
}

TEST_F( M6502CompareRegisterTests, CMPAbsoluteYCanCompareALargePositiveToASmallPositive )
{
	CMPTestData Test = CompareALargePositiveToASmallPositive();
	CMPAbsoluteY( Test );
}

TEST_F( M6502CompareRegisterTests, CMPAbsoluteYCanCompareANegativeNumberToAPositive )
{
	CMPTestData Test = CompareANegativeNumberToAPositive();
	CMPAbsoluteY( Test );
}

TEST_F( M6502CompareRegisterTests, CMPAbsoluteYCanCompareTwoValuesThatResultInANegativeFlagSet )
{
	CMPTestData Test = CompareTwoValuesThatResultInANegativeFlagSet();
	CMPAbsoluteY( Test );
}

//-- Indirect X

TEST_F( M6502CompareRegisterTests, CMPIndirectXCanCompareTwoIdenticalValues )
{
	CMPTestData Test = CompareTwoIdenticalValues();
	CMPIndirectX( Test );
}

TEST_F( M6502CompareRegisterTests, CMPIndirectXCanCompareALargePositiveToASmallPositive )
{
	CMPTestData Test = CompareALargePositiveToASmallPositive();
	CMPIndirectX( Test );
}

TEST_F( M6502CompareRegisterTests, CMPIndirectXCanCompareANegativeNumberToAPositive )
{
	CMPTestData Test = CompareANegativeNumberToAPositive();
	CMPIndirectX( Test );
}

TEST_F( M6502CompareRegisterTests, CMPIndirectXCanCompareTwoValuesThatResultInANegativeFlagSet )
{
	CMPTestData Test = CompareTwoValuesThatResultInANegativeFlagSet();
	CMPIndirectX( Test );
}

//-- Indirect Y

TEST_F( M6502CompareRegisterTests, CMPIndirectYCanCompareTwoIdenticalValues )
{
	CMPTestData Test = CompareTwoIdenticalValues();
	CMPIndirectY( Test );
}

TEST_F( M6502CompareRegisterTests, CMPIndirectYCanCompareALargePositiveToASmallPositive )
{
	CMPTestData Test = CompareALargePositiveToASmallPositive();
	CMPIndirectY( Test );
}

TEST_F( M6502CompareRegisterTests, CMPIndirectYCanCompareANegativeNumberToAPositive )
{
	CMPTestData Test = CompareANegativeNumberToAPositive();
	CMPIndirectY( Test );
}

TEST_F( M6502CompareRegisterTests, CMPIndirectYCanCompareTwoValuesThatResultInANegativeFlagSet )
{
	CMPTestData Test = CompareTwoValuesThatResultInANegativeFlagSet();
	CMPIndirectY( Test );
}

//-- CPX Immediate

TEST_F( M6502CompareRegisterTests, CPXImmediateCanCompareTwoIdenticalValues )
{
	CMPTestData Test = CompareTwoIdenticalValues();
	CompareImmediate( Test, ERegister::X );
}

TEST_F( M6502CompareRegisterTests, CPXImmediateCanCompareALargePositiveToASmallPositive )
{
	CMPTestData Test = CompareALargePositiveToASmallPositive();
	CompareImmediate( Test, ERegister::X );
}

TEST_F( M6502CompareRegisterTests, CPXImmediateCanCompareANegativeNumberToAPositive )
{
	CMPTestData Test = CompareANegativeNumberToAPositive();
	CompareImmediate( Test, ERegister::X );
}

TEST_F( M6502CompareRegisterTests, CPXImmediateCanCompareTwoValuesThatResultInANegativeFlagSet )
{
	CMPTestData Test = CompareTwoValuesThatResultInANegativeFlagSet();
	CompareImmediate( Test, ERegister::X );
}

//-- CPY Immediate

TEST_F( M6502CompareRegisterTests, CPYImmediateCanCompareTwoIdenticalValues )
{
	CMPTestData Test = CompareTwoIdenticalValues();
	CompareImmediate( Test, ERegister::Y );
}

TEST_F( M6502CompareRegisterTests, CPYImmediateCanCompareALargePositiveToASmallPositive )
{
	CMPTestData Test = CompareALargePositiveToASmallPositive();
	CompareImmediate( Test, ERegister::Y );
}

TEST_F( M6502CompareRegisterTests, CPYImmediateCanCompareANegativeNumberToAPositive )
{
	CMPTestData Test = CompareANegativeNumberToAPositive();
	CompareImmediate( Test, ERegister::Y );
}

TEST_F( M6502CompareRegisterTests, CPYImmediateCanCompareTwoValuesThatResultInANegativeFlagSet )
{
	CMPTestData Test = CompareTwoValuesThatResultInANegativeFlagSet();
	CompareImmediate( Test, ERegister::Y );
}

//-- CPX Zero Page

TEST_F( M6502CompareRegisterTests, CPXZeroPageCanCompareTwoIdenticalValues )
{
	CMPTestData Test = CompareTwoIdenticalValues();
	CompareZeroPage( Test, ERegister::X );
}

TEST_F( M6502CompareRegisterTests, CPXZeroPageCanCompareALargePositiveToASmallPositive )
{
	CMPTestData Test = CompareALargePositiveToASmallPositive();
	CompareZeroPage( Test, ERegister::X );
}

TEST_F( M6502CompareRegisterTests, CPXZeroPageCanCompareANegativeNumberToAPositive )
{
	CMPTestData Test = CompareANegativeNumberToAPositive();
	CompareZeroPage( Test, ERegister::X );
}

TEST_F( M6502CompareRegisterTests, CPXZeroPageCanCompareTwoValuesThatResultInANegativeFlagSet )
{
	CMPTestData Test = CompareTwoValuesThatResultInANegativeFlagSet();
	CompareZeroPage( Test, ERegister::X );
}

//-- CPY Zero Page

TEST_F( M6502CompareRegisterTests, CPYZeroPageCanCompareTwoIdenticalValues )
{
	CMPTestData Test = CompareTwoIdenticalValues();
	CompareZeroPage( Test, ERegister::Y );
}

TEST_F( M6502CompareRegisterTests, CPYZeroPageCanCompareALargePositiveToASmallPositive )
{
	CMPTestData Test = CompareALargePositiveToASmallPositive();
	CompareZeroPage( Test, ERegister::Y );
}

TEST_F( M6502CompareRegisterTests, CPYZeroPageCanCompareANegativeNumberToAPositive )
{
	CMPTestData Test = CompareANegativeNumberToAPositive();
	CompareZeroPage( Test, ERegister::Y );
}

TEST_F( M6502CompareRegisterTests, CPYZeroPageCanCompareTwoValuesThatResultInANegativeFlagSet )
{
	CMPTestData Test = CompareTwoValuesThatResultInANegativeFlagSet();
	CompareZeroPage( Test, ERegister::Y );
}

//-- CPX Absolute

TEST_F( M6502CompareRegisterTests, CPXAbsoluteCanCompareTwoIdenticalValues )
{
	CMPTestData Test = CompareTwoIdenticalValues();
	CompareAbsolute( Test, ERegister::X );
}

TEST_F( M6502CompareRegisterTests, CPXAbsoluteCanCompareALargePositiveToASmallPositive )
{
	CMPTestData Test = CompareALargePositiveToASmallPositive();
	CompareAbsolute( Test, ERegister::X );
}

TEST_F( M6502CompareRegisterTests, CPXAbsoluteCanCompareANegativeNumberToAPositive )
{
	CMPTestData Test = CompareANegativeNumberToAPositive();
	CompareAbsolute( Test, ERegister::X );
}

TEST_F( M6502CompareRegisterTests, CPXAbsoluteCanCompareTwoValuesThatResultInANegativeFlagSet )
{
	CMPTestData Test = CompareTwoValuesThatResultInANegativeFlagSet();
	CompareAbsolute( Test, ERegister::X );
}

//-- CPY Absolute

TEST_F( M6502CompareRegisterTests, CPYAbsoluteCanCompareTwoIdenticalValues )
{
	CMPTestData Test = CompareTwoIdenticalValues();
	CompareAbsolute( Test, ERegister::Y );
}

TEST_F( M6502CompareRegisterTests, CPYAbsoluteCanCompareALargePositiveToASmallPositive )
{
	CMPTestData Test = CompareALargePositiveToASmallPositive();
	CompareAbsolute( Test, ERegister::Y );
}

TEST_F( M6502CompareRegisterTests, CPYAbsoluteCanCompareANegativeNumberToAPositive )
{
	CMPTestData Test = CompareANegativeNumberToAPositive();
	CompareAbsolute( Test, ERegister::Y );
}

TEST_F( M6502CompareRegisterTests, CPYAbsoluteCanCompareTwoValuesThatResultInANegativeFlagSet )
{
	CMPTestData Test = CompareTwoValuesThatResultInANegativeFlagSet();
	CompareAbsolute( Test, ERegister::Y );
}

#if 0 // for loop test
TEST_F( M6502CompareRegisterTests, LoopTest )
{
	// given:
	using namespace m6502;
	/* 
	* = $1000

	lda #0
	clc
	loop
	   adc #8
	   cmp #24
	   bne loop

	ldx #20
	*/
	Byte TestPrg[] = { 0x0,0x10,0xA9,0x00,0x18,0x69,0x08,0xC9,0x18,0xD0,0xFA,0xA2,0x14 };

	// when:
	Word StartAddress = cpu.LoadPrg( TestPrg, sizeof( TestPrg ), mem );
	cpu.PC = StartAddress;

	//then:
	for ( m6502::s32 Clock = 1000; Clock > 0; )
	{
		Clock -= cpu.Execute( 1, mem );
	}
}
#endif