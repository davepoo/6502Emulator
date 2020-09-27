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
		m6502::Byte A;
		m6502::Byte Operand;

		bool ExpectC;
		bool ExpectZ;
		bool ExpectN;
	};

	CMPTestData CompareTwoIdenticalValues()
	{
		CMPTestData Test;
		Test.A = 26;
		Test.Operand = 26;
		Test.ExpectZ = true;
		Test.ExpectN = false;
		Test.ExpectC = true;
		return Test;
	}

	CMPTestData CompareALargePositiveToASmallPositive()
	{
		CMPTestData Test;
		Test.A = 48;
		Test.Operand = 26;
		Test.ExpectZ = false;
		Test.ExpectN = false;
		Test.ExpectC = true;
		return Test;
	}

	CMPTestData CompareANegativeNumberToAPositive()
	{
		CMPTestData Test;
		Test.A = 130; //Negative number!
		Test.Operand = 26;
		Test.ExpectZ = false;
		Test.ExpectN = false;
		Test.ExpectC = true;
		return Test;
	}

	CMPTestData CompareTwoValuesThatResultInANegativeFlagSet()
	{
		CMPTestData Test;
		Test.A = 8;
		Test.Operand = 26;
		Test.ExpectZ = false;
		Test.ExpectN = true;
		Test.ExpectC = false;
		return Test;
	}

	void CMPImmediate( CMPTestData Test )
	{
		// given:
		using namespace m6502;
		cpu.Reset( 0xFF00, mem );
		cpu.Flag.C = !Test.ExpectC;
		cpu.Flag.Z = !Test.ExpectZ;
		cpu.Flag.N = !Test.ExpectN;
		cpu.A = Test.A;
		mem[0xFF00] = CPU::INS_CMP;
		mem[0xFF01] = Test.Operand;
		constexpr s32 EXPECTED_CYCLES = 2;
		CPU CPUCopy = cpu;

		// when:
		const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

		// then:
		EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
		EXPECT_EQ( cpu.A, Test.A );
		EXPECT_EQ( cpu.Flag.Z, Test.ExpectZ );
		EXPECT_EQ( cpu.Flag.N, Test.ExpectN );
		EXPECT_EQ( cpu.Flag.C, Test.ExpectC );
		ExpectUnaffectedRegisters( CPUCopy );
	}

	void CMPZeroPage( CMPTestData Test )
	{
		// given:
		using namespace m6502;
		cpu.Reset( 0xFF00, mem );
		cpu.Flag.C = !Test.ExpectC;
		cpu.Flag.Z = !Test.ExpectZ;
		cpu.Flag.N = !Test.ExpectN;
		cpu.A = Test.A;
		mem[0xFF00] = CPU::INS_CMP_ZP;
		mem[0xFF01] = 0x42;
		mem[0x0042] = Test.Operand;
		constexpr s32 EXPECTED_CYCLES = 3;
		CPU CPUCopy = cpu;

		// when:
		const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

		// then:
		EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
		EXPECT_EQ( cpu.A, Test.A );
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
		cpu.A = Test.A;
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
		EXPECT_EQ( cpu.A, Test.A );
		EXPECT_EQ( cpu.X, 4 );
		EXPECT_EQ( cpu.Flag.Z, Test.ExpectZ );
		EXPECT_EQ( cpu.Flag.N, Test.ExpectN );
		EXPECT_EQ( cpu.Flag.C, Test.ExpectC );
		ExpectUnaffectedRegisters( CPUCopy );
	}

	void CMPAbsolute( CMPTestData Test )
	{
		// given:
		using namespace m6502;
		cpu.Reset( 0xFF00, mem );
		cpu.Flag.C = !Test.ExpectC;
		cpu.Flag.Z = !Test.ExpectZ;
		cpu.Flag.N = !Test.ExpectN;
		cpu.A = Test.A;
		mem[0xFF00] = CPU::INS_CMP_ABS;
		mem[0xFF01] = 0x00;
		mem[0xFF02] = 0x80;
		mem[0x8000] = Test.Operand;
		constexpr s32 EXPECTED_CYCLES = 4;
		CPU CPUCopy = cpu;

		// when:
		const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

		// then:
		EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
		EXPECT_EQ( cpu.A, Test.A );
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
		cpu.A = Test.A;
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
		EXPECT_EQ( cpu.A, Test.A );
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
		cpu.A = Test.A;
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
		EXPECT_EQ( cpu.A, Test.A );
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
		cpu.A = Test.A;
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
		EXPECT_EQ( cpu.A, Test.A );
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
		cpu.A = Test.A;
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
		EXPECT_EQ( cpu.A, Test.A );
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
	CMPImmediate( Test );
}

TEST_F( M6502CompareRegisterTests, CMPImmediateCanCompareALargePositiveToASmallPositive )
{
	CMPTestData Test = CompareALargePositiveToASmallPositive();
	CMPImmediate( Test );
}

TEST_F( M6502CompareRegisterTests, CMPImmediateCanCompareANegativeNumberToAPositive )
{
	CMPTestData Test = CompareANegativeNumberToAPositive();
	CMPImmediate( Test );
}

TEST_F( M6502CompareRegisterTests, CMPImmediateCanCompareTwoValuesThatResultInANegativeFlagSet )
{
	CMPTestData Test = CompareTwoValuesThatResultInANegativeFlagSet();
	CMPImmediate( Test );
}

//-- Zero Page

TEST_F( M6502CompareRegisterTests, CMPZeroPageCanCompareTwoIdenticalValues )
{
	CMPTestData Test = CompareTwoIdenticalValues();
	CMPZeroPage( Test );
}

TEST_F( M6502CompareRegisterTests, CMPZeroPageCanCompareALargePositiveToASmallPositive )
{
	CMPTestData Test = CompareALargePositiveToASmallPositive();
	CMPZeroPage( Test );
}

TEST_F( M6502CompareRegisterTests, CMPZeroPageCanCompareANegativeNumberToAPositive )
{
	CMPTestData Test = CompareANegativeNumberToAPositive();
	CMPZeroPage( Test );
}

TEST_F( M6502CompareRegisterTests, CMPZeroPageCanCompareTwoValuesThatResultInANegativeFlagSet )
{
	CMPTestData Test = CompareTwoValuesThatResultInANegativeFlagSet();
	CMPZeroPage( Test );
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
	CMPAbsolute( Test );
}

TEST_F( M6502CompareRegisterTests, CMPAbsoluteCanCompareALargePositiveToASmallPositive )
{
	CMPTestData Test = CompareALargePositiveToASmallPositive();
	CMPAbsolute( Test );
}

TEST_F( M6502CompareRegisterTests, CMPAbsoluteCanCompareANegativeNumberToAPositive )
{
	CMPTestData Test = CompareANegativeNumberToAPositive();
	CMPAbsolute( Test );
}

TEST_F( M6502CompareRegisterTests, CMPAbsoluteCanCompareTwoValuesThatResultInANegativeFlagSet )
{
	CMPTestData Test = CompareTwoValuesThatResultInANegativeFlagSet();
	CMPAbsolute( Test );
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