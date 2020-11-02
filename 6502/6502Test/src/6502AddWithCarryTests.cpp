#include <gtest/gtest.h>
#include "m6502.h"

class M6502AddSubWithCarryTests : public testing::Test
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
	}

	struct ADCTestData
	{
		bool Carry;
		m6502::Byte A;
		m6502::Byte Operand;
		m6502::Byte Answer;

		bool ExpectC;
		bool ExpectZ;
		bool ExpectN;
		bool ExpectV;
	};

	enum class EOperation
	{
		Add, Subtract
	};

	void TestSBCAbsolute( ADCTestData Test )
	{		
		TestADCAbsolute( Test, EOperation::Subtract );
	}

	void TestADCAbsolute( ADCTestData Test, 
		EOperation Operation = EOperation::Add )
	{
		// given:
		using namespace m6502;
		cpu.Reset( 0xFF00, mem );
		cpu.Flag.C = Test.Carry;
		cpu.A = Test.A;
		cpu.Flag.Z = !Test.ExpectZ;
		cpu.Flag.N = !Test.ExpectN;
		cpu.Flag.V = !Test.ExpectV;
		mem[0xFF00] = 
			(Operation == EOperation::Add) ? 
			CPU::INS_ADC_ABS : CPU::INS_SBC_ABS;
		mem[0xFF01] = 0x00;
		mem[0xFF02] = 0x80;
		mem[0x8000] = Test.Operand;
		constexpr s32 EXPECTED_CYCLES = 4;
		CPU CPUCopy = cpu;

		// when:
		const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

		// then:
		EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
		EXPECT_EQ( cpu.A, Test.Answer );
		EXPECT_EQ( cpu.Flag.C, Test.ExpectC );
		EXPECT_EQ( cpu.Flag.Z, Test.ExpectZ );
		EXPECT_EQ( cpu.Flag.N, Test.ExpectN );
		EXPECT_EQ( cpu.Flag.V, Test.ExpectV );
		ExpectUnaffectedRegisters( CPUCopy );
	}

	void TestSBCAbsoluteX( ADCTestData Test )
	{
		TestADCAbsoluteX( Test, EOperation::Subtract );
	}

	void TestADCAbsoluteX( ADCTestData Test,
		EOperation Operation = EOperation::Add )
	{
		// given:
		using namespace m6502;
		cpu.Reset( 0xFF00, mem );
		cpu.Flag.C = Test.Carry;
		cpu.X = 0x10;
		cpu.A = Test.A;
		cpu.Flag.Z = !Test.ExpectZ;
		cpu.Flag.N = !Test.ExpectN;
		cpu.Flag.V = !Test.ExpectV;
		mem[0xFF00] =
			(Operation == EOperation::Add) ?
			CPU::INS_ADC_ABSX : CPU::INS_SBC_ABSX;
		mem[0xFF01] = 0x00;
		mem[0xFF02] = 0x80;
		mem[0x8000+0x10] = Test.Operand;
		constexpr s32 EXPECTED_CYCLES = 4;
		CPU CPUCopy = cpu;

		// when:
		const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

		// then:
		EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
		EXPECT_EQ( cpu.A, Test.Answer );
		EXPECT_EQ( cpu.Flag.C, Test.ExpectC );
		EXPECT_EQ( cpu.Flag.Z, Test.ExpectZ );
		EXPECT_EQ( cpu.Flag.N, Test.ExpectN );
		EXPECT_EQ( cpu.Flag.V, Test.ExpectV );
		ExpectUnaffectedRegisters( CPUCopy );
	}

	void TestSBCAbsoluteY( ADCTestData Test )
	{
		TestADCAbsoluteY( Test, EOperation::Subtract );
	}

	void TestADCAbsoluteY( ADCTestData Test,
		EOperation Operation = EOperation::Add )
	{
		// given:
		using namespace m6502;
		cpu.Reset( 0xFF00, mem );
		cpu.Flag.C = Test.Carry;
		cpu.Y = 0x10;
		cpu.A = Test.A;
		cpu.Flag.Z = !Test.ExpectZ;
		cpu.Flag.N = !Test.ExpectN;
		cpu.Flag.V = !Test.ExpectV;
		mem[0xFF00] =
			(Operation == EOperation::Add) ?
			CPU::INS_ADC_ABSY : CPU::INS_SBC_ABSY;
		mem[0xFF01] = 0x00;
		mem[0xFF02] = 0x80;
		mem[0x8000 + 0x10] = Test.Operand;
		constexpr s32 EXPECTED_CYCLES = 4;
		CPU CPUCopy = cpu;

		// when:
		const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

		// then:
		EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
		EXPECT_EQ( cpu.A, Test.Answer );
		EXPECT_EQ( cpu.Flag.C, Test.ExpectC );
		EXPECT_EQ( cpu.Flag.Z, Test.ExpectZ );
		EXPECT_EQ( cpu.Flag.N, Test.ExpectN );
		EXPECT_EQ( cpu.Flag.V, Test.ExpectV );
		ExpectUnaffectedRegisters( CPUCopy );
	}

	void TestSBCImmediate( ADCTestData Test )
	{
		TestADCImmediate( Test, EOperation::Subtract );
	}

	void TestADCImmediate( ADCTestData Test, 
		EOperation Operation = EOperation::Add )
	{
		// given:
		using namespace m6502;
		cpu.Reset( 0xFF00, mem );
		cpu.Flag.C = Test.Carry;
		cpu.A = Test.A;
		cpu.Flag.Z = !Test.ExpectZ;
		cpu.Flag.N = !Test.ExpectN;
		cpu.Flag.V = !Test.ExpectV;
		mem[0xFF00] =
			(Operation == EOperation::Add) ?
			CPU::INS_ADC : CPU::INS_SBC;
		mem[0xFF01] = Test.Operand;
		constexpr s32 EXPECTED_CYCLES = 2;
		CPU CPUCopy = cpu;

		// when:
		const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

		// then:
		EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
		EXPECT_EQ( cpu.A, Test.Answer );
		EXPECT_EQ( cpu.Flag.C, Test.ExpectC );
		EXPECT_EQ( cpu.Flag.Z, Test.ExpectZ );
		EXPECT_EQ( cpu.Flag.N, Test.ExpectN );
		EXPECT_EQ( cpu.Flag.V, Test.ExpectV );
		ExpectUnaffectedRegisters( CPUCopy );
	}

	void TestSBCZeroPage( ADCTestData Test )
	{
		TestADCZeroPage( Test, EOperation::Subtract );
	}

	void TestADCZeroPage( ADCTestData Test,
		EOperation Operation = EOperation::Add )
	{
		// given:
		using namespace m6502;
		cpu.Reset( 0xFF00, mem );
		cpu.Flag.C = Test.Carry;
		cpu.A = Test.A;
		cpu.Flag.Z = !Test.ExpectZ;
		cpu.Flag.N = !Test.ExpectN;
		cpu.Flag.V = !Test.ExpectV;
		mem[0xFF00] = 
			(Operation == EOperation::Add) ?
			CPU::INS_ADC_ZP : CPU::INS_SBC_ZP;
		mem[0xFF01] = 0x42;
		mem[0x0042] = Test.Operand;
		constexpr s32 EXPECTED_CYCLES = 3;
		CPU CPUCopy = cpu;

		// when:
		const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

		// then:
		EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
		EXPECT_EQ( cpu.A, Test.Answer );
		EXPECT_EQ( cpu.Flag.C, Test.ExpectC );
		EXPECT_EQ( cpu.Flag.Z, Test.ExpectZ );
		EXPECT_EQ( cpu.Flag.N, Test.ExpectN );
		EXPECT_EQ( cpu.Flag.V, Test.ExpectV );
		ExpectUnaffectedRegisters( CPUCopy );
	}

	void TestSBCZeroPageX( ADCTestData Test )
	{
		TestADCZeroPageX( Test, EOperation::Subtract );
	}

	void TestADCZeroPageX( ADCTestData Test, 
		EOperation Operation = EOperation::Add )
	{
		// given:
		using namespace m6502;
		cpu.Reset( 0xFF00, mem );
		cpu.Flag.C = Test.Carry;
		cpu.X = 0x10;
		cpu.A = Test.A;
		cpu.Flag.Z = !Test.ExpectZ;
		cpu.Flag.N = !Test.ExpectN;
		cpu.Flag.V = !Test.ExpectV;
		mem[0xFF00] =
			(Operation == EOperation::Add) ?
			CPU::INS_ADC_ZPX : CPU::INS_SBC_ZPX;
		mem[0xFF01] = 0x42;
		mem[0x0042+0x10] = Test.Operand;
		constexpr s32 EXPECTED_CYCLES = 4;
		CPU CPUCopy = cpu;

		// when:
		const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

		// then:
		EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
		EXPECT_EQ( cpu.A, Test.Answer );
		EXPECT_EQ( cpu.Flag.C, Test.ExpectC );
		EXPECT_EQ( cpu.Flag.Z, Test.ExpectZ );
		EXPECT_EQ( cpu.Flag.N, Test.ExpectN );
		EXPECT_EQ( cpu.Flag.V, Test.ExpectV );
		ExpectUnaffectedRegisters( CPUCopy );
	}

	void TestSBCIndirectX( ADCTestData Test )
	{
		TestADCIndirectX( Test, EOperation::Subtract );
	}

	void TestADCIndirectX( ADCTestData Test, 
		EOperation Operation = EOperation::Add )
	{
		// given:
		using namespace m6502;
		cpu.Reset( 0xFF00, mem );
		cpu.Flag.C = Test.Carry;
		cpu.X = 0x04;
		cpu.A = Test.A;
		cpu.Flag.Z = !Test.ExpectZ;
		cpu.Flag.N = !Test.ExpectN;
		cpu.Flag.V = !Test.ExpectV;
		mem[0xFF00] =
			(Operation == EOperation::Add) ?
			CPU::INS_ADC_INDX : CPU::INS_SBC_INDX;
		mem[0xFF01] = 0x02;
		mem[0x0006] = 0x00;	//0x2 + 0x4
		mem[0x0007] = 0x80;
		mem[0x8000] = Test.Operand;
		constexpr s32 EXPECTED_CYCLES = 6;
		CPU CPUCopy = cpu;

		// when:
		const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

		// then:
		EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
		EXPECT_EQ( cpu.A, Test.Answer );
		EXPECT_EQ( cpu.Flag.C, Test.ExpectC );
		EXPECT_EQ( cpu.Flag.Z, Test.ExpectZ );
		EXPECT_EQ( cpu.Flag.N, Test.ExpectN );
		EXPECT_EQ( cpu.Flag.V, Test.ExpectV );
		ExpectUnaffectedRegisters( CPUCopy );
	}

	void TestSBCIndirectY( ADCTestData Test )
	{
		TestADCIndirectY( Test, EOperation::Subtract );
	}

	void TestADCIndirectY( ADCTestData Test, 
		EOperation Operation = EOperation::Add )
	{
		// given:
		using namespace m6502;
		cpu.Reset( 0xFF00, mem );
		cpu.Flag.C = Test.Carry;
		cpu.Y = 0x04;
		cpu.A = Test.A;
		cpu.Flag.Z = !Test.ExpectZ;
		cpu.Flag.N = !Test.ExpectN;
		cpu.Flag.V = !Test.ExpectV;
		mem[0xFF00] =
			(Operation == EOperation::Add) ?
			CPU::INS_ADC_INDY : CPU::INS_SBC_INDY;
		mem[0xFF01] = 0x02;
		mem[0x0002] = 0x00;
		mem[0x0003] = 0x80;
		mem[0x8000 + 0x04] = Test.Operand;
		constexpr s32 EXPECTED_CYCLES = 5;
		CPU CPUCopy = cpu;

		// when:
		const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

		// then:
		EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
		EXPECT_EQ( cpu.A, Test.Answer );
		EXPECT_EQ( cpu.Flag.C, Test.ExpectC );
		EXPECT_EQ( cpu.Flag.Z, Test.ExpectZ );
		EXPECT_EQ( cpu.Flag.N, Test.ExpectN );
		EXPECT_EQ( cpu.Flag.V, Test.ExpectV );
		ExpectUnaffectedRegisters( CPUCopy );
	}
};

#define BYTE( A ) ( (m6502::Byte)A )

TEST_F( M6502AddSubWithCarryTests, ADCAbsCanAddZeroToZeroAndGetZero )
{
	ADCTestData Test;
	Test.Carry = false;
	Test.A = 0;
	Test.Operand = 0;
	Test.Answer = 0;
	Test.ExpectC = false;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = true;
	TestADCAbsolute( Test );
}

TEST_F( M6502AddSubWithCarryTests, ADCAbsCanAddCarryAndZeroToZeroAndGetOne )
{
	ADCTestData Test;
	Test.Carry = true;
	Test.A = 0;
	Test.Operand = 0;
	Test.Answer = 1;
	Test.ExpectC = false;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	TestADCAbsolute( Test );
}

TEST_F( M6502AddSubWithCarryTests, ADCAbsCanAddTwoUnsignedNumbers )
{
	ADCTestData Test;
	Test.Carry = true;
	Test.A = 20;
	Test.Operand = 17;
	Test.Answer = 38;
	Test.ExpectC = false;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	TestADCAbsolute( Test );
}

TEST_F( M6502AddSubWithCarryTests, ADCAbsCanAddAPositiveAndNegativeNumber )
{
	// A: 00010100 20   
	// O: 11101111 -17
	// =: 00000011
	// C:1 N:0 V:0 Z:0

	ADCTestData Test;
	Test.Carry = true;
	Test.A = 20;
	Test.Operand = BYTE(-17);
	Test.Answer = 4;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	TestADCAbsolute( Test );
}

TEST_F( M6502AddSubWithCarryTests, ADCAbsCanAddOneToFFAndItWillCauseACarry )
{
	ADCTestData Test;
	Test.Carry = false;
	Test.A = 0xFF;			
	Test.Operand = 1;
	Test.Answer = 0;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = true;
	TestADCAbsolute( Test );
}

TEST_F( M6502AddSubWithCarryTests, ADCAbsWillSetTheNegativeFlagWhenTheResultIsNegative )
{
	ADCTestData Test;
	Test.Carry = false;
	Test.A = 0;
	Test.Operand = BYTE(-1);
	Test.Answer = BYTE(-1);
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	TestADCAbsolute( Test );
}

TEST_F( M6502AddSubWithCarryTests, ADCAbsWillSetTheOverflowFlagWhenSignedNegativeAddtionFails )
{
	// A: 10000000 -128
	// O: 11111111 -1
	// =: 01111111 127
	// C:1 N:0 V:1 Z:0
	ADCTestData Test;
	Test.Carry = false;
	Test.A = BYTE( -128 );
	Test.Operand = BYTE( -1 );
	Test.Answer = 127;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = true;
	Test.ExpectZ = false;
	TestADCAbsolute( Test );
}

TEST_F( M6502AddSubWithCarryTests, 
	ADCAbsWillSetTheOverflowFlagWhenSignedNegativeAddtionPassedDueToInitalCarryFlag )
{
	// C: 00000001
	// A: 10000000 -128
	// O: 11111111 -1
	// =: 10000000 -128
	// C:1 N:1 V:0 Z:0

	ADCTestData Test;
	Test.Carry = true;
	Test.A = BYTE( -128 );
	Test.Operand = BYTE( -1 );
	Test.Answer = BYTE(-128);
	Test.ExpectC = true;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	TestADCAbsolute( Test );
}

TEST_F( M6502AddSubWithCarryTests, ADCAbsWillSetTheOverflowFlagWhenSignedPositiveAddtionFails )
{
	// A: 01111111 127   
	// O: 00000001 1
	// =: 10000000 128
	// C:0 N:1 V:1 Z:0
	ADCTestData Test;
	Test.Carry = false;
	Test.A = 127;
	Test.Operand = 1;
	Test.Answer = 128;
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = true;
	Test.ExpectZ = false;
	TestADCAbsolute( Test );
}

TEST_F( M6502AddSubWithCarryTests, ADCImmediateCanAddTwoUnsignedNumbers )
{
	ADCTestData Test;
	Test.Carry = true;
	Test.A = 20;
	Test.Operand = 17;
	Test.Answer = 38;
	Test.ExpectC = false;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	TestADCImmediate( Test );
}

TEST_F( M6502AddSubWithCarryTests, ADCImmediateCanAddAPositiveAndNegativeNumber )
{
	// A: 00010100 20   
	// O: 11101111 -17
	// =: 00000011
	// C:1 N:0 V:0 Z:0

	ADCTestData Test;
	Test.Carry = true;
	Test.A = 20;
	Test.Operand = BYTE( -17 );
	Test.Answer = 4;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	TestADCImmediate( Test );
}

TEST_F( M6502AddSubWithCarryTests, ADCZeroPageCanAddTwoUnsignedNumbers )
{
	ADCTestData Test;
	Test.Carry = true;
	Test.A = 20;
	Test.Operand = 17;
	Test.Answer = 38;
	Test.ExpectC = false;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	TestADCZeroPage( Test );
}

TEST_F( M6502AddSubWithCarryTests, ADCZeroPageCanAddAPositiveAndNegativeNumber )
{
	// A: 00010100 20   
	// O: 11101111 -17
	// =: 00000011
	// C:1 N:0 V:0 Z:0

	ADCTestData Test;
	Test.Carry = true;
	Test.A = 20;
	Test.Operand = BYTE( -17 );
	Test.Answer = 4;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	TestADCZeroPage( Test );
}

TEST_F( M6502AddSubWithCarryTests, ADCZeroPageXCanAddTwoUnsignedNumbers )
{
	ADCTestData Test;
	Test.Carry = true;
	Test.A = 20;
	Test.Operand = 17;
	Test.Answer = 38;
	Test.ExpectC = false;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	TestADCZeroPageX( Test );
}

TEST_F( M6502AddSubWithCarryTests, ADCZeroPageXCanAddAPositiveAndNegativeNumber )
{
	// A: 00010100 20   
	// O: 11101111 -17
	// =: 00000011
	// C:1 N:0 V:0 Z:0

	ADCTestData Test;
	Test.Carry = true;
	Test.A = 20;
	Test.Operand = BYTE( -17 );
	Test.Answer = 4;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	TestADCZeroPageX( Test );
}

TEST_F( M6502AddSubWithCarryTests, ADCAbsXCanAddTwoUnsignedNumbers )
{
	ADCTestData Test;
	Test.Carry = true;
	Test.A = 20;
	Test.Operand = 17;
	Test.Answer = 38;
	Test.ExpectC = false;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	TestADCAbsoluteX( Test );
}

TEST_F( M6502AddSubWithCarryTests, ADCAbsXCanAddAPositiveAndNegativeNumber )
{
	// A: 00010100 20   
	// O: 11101111 -17
	// =: 00000011
	// C:1 N:0 V:0 Z:0

	ADCTestData Test;
	Test.Carry = true;
	Test.A = 20;
	Test.Operand = BYTE( -17 );
	Test.Answer = 4;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	TestADCAbsoluteX( Test );
}

TEST_F( M6502AddSubWithCarryTests, ADCAbsYCanAddTwoUnsignedNumbers )
{
	ADCTestData Test;
	Test.Carry = true;
	Test.A = 20;
	Test.Operand = 17;
	Test.Answer = 38;
	Test.ExpectC = false;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	TestADCAbsoluteY( Test );
}

TEST_F( M6502AddSubWithCarryTests, ADCAbsYCanAddAPositiveAndNegativeNumber )
{
	// A: 00010100 20   
	// O: 11101111 -17
	// =: 00000011
	// C:1 N:0 V:0 Z:0

	ADCTestData Test;
	Test.Carry = true;
	Test.A = 20;
	Test.Operand = BYTE( -17 );
	Test.Answer = 4;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	TestADCAbsoluteY( Test );
}

TEST_F( M6502AddSubWithCarryTests, ADCIndXCanAddTwoUnsignedNumbers )
{
	ADCTestData Test;
	Test.Carry = true;
	Test.A = 20;
	Test.Operand = 17;
	Test.Answer = 38;
	Test.ExpectC = false;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	TestADCIndirectX( Test );
}

TEST_F( M6502AddSubWithCarryTests, ADCIndXCanAddAPositiveAndNegativeNumber )
{
	// A: 00010100 20   
	// O: 11101111 -17
	// =: 00000011
	// C:1 N:0 V:0 Z:0

	ADCTestData Test;
	Test.Carry = true;
	Test.A = 20;
	Test.Operand = BYTE( -17 );
	Test.Answer = 4;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	TestADCIndirectX( Test );
}

TEST_F( M6502AddSubWithCarryTests, ADCIndYCanAddTwoUnsignedNumbers )
{
	ADCTestData Test;
	Test.Carry = true;
	Test.A = 20;
	Test.Operand = 17;
	Test.Answer = 38;
	Test.ExpectC = false;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	TestADCIndirectY( Test );
}

TEST_F( M6502AddSubWithCarryTests, ADCIndYCanAddAPositiveAndNegativeNumber )
{
	// A: 00010100 20   
	// O: 11101111 -17
	// =: 00000011
	// C:1 N:0 V:0 Z:0

	ADCTestData Test;
	Test.Carry = true;
	Test.A = 20;
	Test.Operand = BYTE( -17 );
	Test.Answer = 4;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	TestADCIndirectY( Test );
}

// SBC Abs --------------

TEST_F( M6502AddSubWithCarryTests, SBCAbsCanSubtractZeroFromZeroAndGetZero )
{
	ADCTestData Test;
	Test.Carry = true;
	Test.A = 0;
	Test.Operand = 0;
	Test.Answer = 0;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = true;
	TestSBCAbsolute( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCAbsCanSubtractZeroFromZeroAndCarryAndGetMinusOne )
{
	ADCTestData Test;
	Test.Carry = false;
	Test.A = 0;
	Test.Operand = 0;
	Test.Answer = BYTE(-1);
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	TestSBCAbsolute( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCAbsCanSubtractOneFromZeroAndGetMinusOne )
{
	ADCTestData Test;
	Test.Carry = true;
	Test.A = 0;
	Test.Operand = 1;
	Test.Answer = BYTE(-1);
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	TestSBCAbsolute( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCAbsCanSubtractOneFromZeroWithCarryAndGetMinusTwo )
{
	ADCTestData Test;
	Test.Carry = false;
	Test.A = 0;
	Test.Operand = 1;
	Test.Answer = BYTE( -2 );
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	TestSBCAbsolute( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCAbsCanSubtractTwoNegativeNumbersAndGetSignedOverflow )
{
	ADCTestData Test;
	Test.Carry = true;
	Test.A = BYTE(-128);
	Test.Operand = 1;
	Test.Answer = 127;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = true;
	Test.ExpectZ = false;
	TestSBCAbsolute( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCAbsCanSubtractAPostitiveAndNegativeNumbersAndGetSignedOverflow )
{
	ADCTestData Test;
	Test.Carry = true;
	Test.A = 127;
	Test.Operand = BYTE(-1);
	Test.Answer = 128;
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = true;
	Test.ExpectZ = false;
	TestSBCAbsolute( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCAbsCanSubtractTwoUnsignedNumbers )
{
	ADCTestData Test;
	Test.Carry = true;
	Test.A = 20;
	Test.Operand = 17;
	Test.Answer = 3;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	TestSBCAbsolute( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCAbsCanSubtractTwoNegativeNumbers )
{
	ADCTestData Test;
	Test.Carry = true;
	Test.A = BYTE(-20);
	Test.Operand = BYTE(-17);
	Test.Answer = BYTE(-3);
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	TestSBCAbsolute( Test );
}

// SBC Zero Page

TEST_F( M6502AddSubWithCarryTests, SBCZeroPageCanSubtractZeroFromZeroAndGetZero )
{
	ADCTestData Test;
	Test.Carry = true;
	Test.A = 0;
	Test.Operand = 0;
	Test.Answer = 0;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = true;
	TestSBCZeroPage( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCZeroPageCanSubtractZeroFromZeroAndCarryAndGetMinusOne )
{
	ADCTestData Test;
	Test.Carry = false;
	Test.A = 0;
	Test.Operand = 0;
	Test.Answer = BYTE( -1 );
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	TestSBCZeroPage( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCZeroPageCanSubtractOneFromZeroAndGetMinusOne )
{
	ADCTestData Test;
	Test.Carry = true;
	Test.A = 0;
	Test.Operand = 1;
	Test.Answer = BYTE( -1 );
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	TestSBCZeroPage( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCZeroPageCanSubtractOneFromZeroWithCarryAndGetMinusTwo )
{
	ADCTestData Test;
	Test.Carry = false;
	Test.A = 0;
	Test.Operand = 1;
	Test.Answer = BYTE( -2 );
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	TestSBCZeroPage( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCZeroPageCanSubtractTwoNegativeNumbersAndGetSignedOverflow )
{
	ADCTestData Test;
	Test.Carry = true;
	Test.A = BYTE( -128 );
	Test.Operand = 1;
	Test.Answer = 127;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = true;
	Test.ExpectZ = false;
	TestSBCZeroPage( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCZeroPageCanSubtractAPostitiveAndNegativeNumbersAndGetSignedOverflow )
{
	ADCTestData Test;
	Test.Carry = true;
	Test.A = 127;
	Test.Operand = BYTE( -1 );
	Test.Answer = 128;
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = true;
	Test.ExpectZ = false;
	TestSBCZeroPage( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCZeroPageCanSubtractTwoUnsignedNumbers )
{
	ADCTestData Test;
	Test.Carry = true;
	Test.A = 20;
	Test.Operand = 17;
	Test.Answer = 3;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	TestSBCZeroPage( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCZeroPageCanSubtractTwoNegativeNumbers )
{
	ADCTestData Test;
	Test.Carry = true;
	Test.A = BYTE( -20 );
	Test.Operand = BYTE( -17 );
	Test.Answer = BYTE( -3 );
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	TestSBCZeroPage( Test );
}

// SBC Immediate ---------

TEST_F( M6502AddSubWithCarryTests, SBCImmediateCanSubtractZeroFromZeroAndGetZero )
{
	ADCTestData Test;
	Test.Carry = true;
	Test.A = 0;
	Test.Operand = 0;
	Test.Answer = 0;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = true;
	TestSBCImmediate( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCImmediateCanSubtractZeroFromZeroAndCarryAndGetMinusOne )
{
	ADCTestData Test;
	Test.Carry = false;
	Test.A = 0;
	Test.Operand = 0;
	Test.Answer = BYTE( -1 );
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	TestSBCImmediate( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCImmediateCanSubtractOneFromZeroAndGetMinusOne )
{
	ADCTestData Test;
	Test.Carry = true;
	Test.A = 0;
	Test.Operand = 1;
	Test.Answer = BYTE( -1 );
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	TestSBCImmediate( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCImmediateCanSubtractOneFromZeroWithCarryAndGetMinusTwo )
{
	ADCTestData Test;
	Test.Carry = false;
	Test.A = 0;
	Test.Operand = 1;
	Test.Answer = BYTE( -2 );
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	TestSBCImmediate( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCImmediateCanSubtractTwoNegativeNumbersAndGetSignedOverflow )
{
	ADCTestData Test;
	Test.Carry = true;
	Test.A = BYTE( -128 );
	Test.Operand = 1;
	Test.Answer = 127;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = true;
	Test.ExpectZ = false;
	TestSBCImmediate( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCImmediateCanSubtractAPostitiveAndNegativeNumbersAndGetSignedOverflow )
{
	ADCTestData Test;
	Test.Carry = true;
	Test.A = 127;
	Test.Operand = BYTE( -1 );
	Test.Answer = 128;
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = true;
	Test.ExpectZ = false;
	TestSBCImmediate( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCImmediateCanSubtractTwoUnsignedNumbers )
{
	ADCTestData Test;
	Test.Carry = true;
	Test.A = 20;
	Test.Operand = 17;
	Test.Answer = 3;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	TestSBCImmediate( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCImmediateCanSubtractTwoNegativeNumbers )
{
	ADCTestData Test;
	Test.Carry = true;
	Test.A = BYTE( -20 );
	Test.Operand = BYTE( -17 );
	Test.Answer = BYTE( -3 );
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	TestSBCImmediate( Test );
}

// SBC Zero Page, X -------

TEST_F( M6502AddSubWithCarryTests, SBCZeroPageXCanSubtractZeroFromZeroAndGetZero )
{
	ADCTestData Test;
	Test.Carry = true;
	Test.A = 0;
	Test.Operand = 0;
	Test.Answer = 0;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = true;
	TestSBCZeroPageX( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCZeroPageXCanSubtractZeroFromZeroAndCarryAndGetMinusOne )
{
	ADCTestData Test;
	Test.Carry = false;
	Test.A = 0;
	Test.Operand = 0;
	Test.Answer = BYTE( -1 );
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	TestSBCZeroPageX( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCZeroPageXCanSubtractOneFromZeroAndGetMinusOne )
{
	ADCTestData Test;
	Test.Carry = true;
	Test.A = 0;
	Test.Operand = 1;
	Test.Answer = BYTE( -1 );
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	TestSBCZeroPageX( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCZeroPageXCanSubtractOneFromZeroWithCarryAndGetMinusTwo )
{
	ADCTestData Test;
	Test.Carry = false;
	Test.A = 0;
	Test.Operand = 1;
	Test.Answer = BYTE( -2 );
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	TestSBCZeroPageX( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCZeroPageXCanSubtractTwoNegativeNumbersAndGetSignedOverflow )
{
	ADCTestData Test;
	Test.Carry = true;
	Test.A = BYTE( -128 );
	Test.Operand = 1;
	Test.Answer = 127;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = true;
	Test.ExpectZ = false;
	TestSBCZeroPageX( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCZeroPageXCanSubtractAPostitiveAndNegativeNumbersAndGetSignedOverflow )
{
	ADCTestData Test;
	Test.Carry = true;
	Test.A = 127;
	Test.Operand = BYTE( -1 );
	Test.Answer = 128;
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = true;
	Test.ExpectZ = false;
	TestSBCZeroPageX( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCZeroPageXCanSubtractTwoUnsignedNumbers )
{
	ADCTestData Test;
	Test.Carry = true;
	Test.A = 20;
	Test.Operand = 17;
	Test.Answer = 3;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	TestSBCZeroPageX( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCZeroPageXCanSubtractTwoNegativeNumbers )
{
	ADCTestData Test;
	Test.Carry = true;
	Test.A = BYTE( -20 );
	Test.Operand = BYTE( -17 );
	Test.Answer = BYTE( -3 );
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	TestSBCZeroPageX( Test );
}

// SBC Absolute, X -------

TEST_F( M6502AddSubWithCarryTests, SBCAbsoluteXCanSubtractZeroFromZeroAndGetZero )
{
	ADCTestData Test;
	Test.Carry = true;
	Test.A = 0;
	Test.Operand = 0;
	Test.Answer = 0;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = true;
	TestSBCAbsoluteX( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCAbsoluteXCanSubtractZeroFromZeroAndCarryAndGetMinusOne )
{
	ADCTestData Test;
	Test.Carry = false;
	Test.A = 0;
	Test.Operand = 0;
	Test.Answer = BYTE( -1 );
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	TestSBCAbsoluteX( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCAbsoluteXCanSubtractOneFromZeroAndGetMinusOne )
{
	ADCTestData Test;
	Test.Carry = true;
	Test.A = 0;
	Test.Operand = 1;
	Test.Answer = BYTE( -1 );
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	TestSBCAbsoluteX( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCAbsoluteXCanSubtractOneFromZeroWithCarryAndGetMinusTwo )
{
	ADCTestData Test;
	Test.Carry = false;
	Test.A = 0;
	Test.Operand = 1;
	Test.Answer = BYTE( -2 );
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	TestSBCAbsoluteX( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCAbsoluteXCanSubtractTwoNegativeNumbersAndGetSignedOverflow )
{
	ADCTestData Test;
	Test.Carry = true;
	Test.A = BYTE( -128 );
	Test.Operand = 1;
	Test.Answer = 127;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = true;
	Test.ExpectZ = false;
	TestSBCAbsoluteX( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCAbsoluteXCanSubtractAPostitiveAndNegativeNumbersAndGetSignedOverflow )
{
	ADCTestData Test;
	Test.Carry = true;
	Test.A = 127;
	Test.Operand = BYTE( -1 );
	Test.Answer = 128;
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = true;
	Test.ExpectZ = false;
	TestSBCAbsoluteX( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCAbsoluteXCanSubtractTwoUnsignedNumbers )
{
	ADCTestData Test;
	Test.Carry = true;
	Test.A = 20;
	Test.Operand = 17;
	Test.Answer = 3;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	TestSBCAbsoluteX( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCAbsoluteXCanSubtractTwoNegativeNumbers )
{
	ADCTestData Test;
	Test.Carry = true;
	Test.A = BYTE( -20 );
	Test.Operand = BYTE( -17 );
	Test.Answer = BYTE( -3 );
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	TestSBCAbsoluteX( Test );
}

// SBC Absolute, Y

TEST_F( M6502AddSubWithCarryTests, SBCAbsoluteYCanSubtractZeroFromZeroAndGetZero )
{
	ADCTestData Test;
	Test.Carry = true;
	Test.A = 0;
	Test.Operand = 0;
	Test.Answer = 0;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = true;
	TestSBCAbsoluteY( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCAbsoluteYCanSubtractZeroFromZeroAndCarryAndGetMinusOne )
{
	ADCTestData Test;
	Test.Carry = false;
	Test.A = 0;
	Test.Operand = 0;
	Test.Answer = BYTE( -1 );
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	TestSBCAbsoluteY( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCAbsoluteYCanSubtractOneFromZeroAndGetMinusOne )
{
	ADCTestData Test;
	Test.Carry = true;
	Test.A = 0;
	Test.Operand = 1;
	Test.Answer = BYTE( -1 );
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	TestSBCAbsoluteY( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCAbsoluteYCanSubtractOneFromZeroWithCarryAndGetMinusTwo )
{
	ADCTestData Test;
	Test.Carry = false;
	Test.A = 0;
	Test.Operand = 1;
	Test.Answer = BYTE( -2 );
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	TestSBCAbsoluteY( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCAbsoluteYCanSubtractTwoNegativeNumbersAndGetSignedOverflow )
{
	ADCTestData Test;
	Test.Carry = true;
	Test.A = BYTE( -128 );
	Test.Operand = 1;
	Test.Answer = 127;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = true;
	Test.ExpectZ = false;
	TestSBCAbsoluteY( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCAbsoluteYCanSubtractAPostitiveAndNegativeNumbersAndGetSignedOverflow )
{
	ADCTestData Test;
	Test.Carry = true;
	Test.A = 127;
	Test.Operand = BYTE( -1 );
	Test.Answer = 128;
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = true;
	Test.ExpectZ = false;
	TestSBCAbsoluteY( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCAbsoluteYCanSubtractTwoUnsignedNumbers )
{
	ADCTestData Test;
	Test.Carry = true;
	Test.A = 20;
	Test.Operand = 17;
	Test.Answer = 3;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	TestSBCAbsoluteY( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCAbsoluteYCanSubtractTwoNegativeNumbers )
{
	ADCTestData Test;
	Test.Carry = true;
	Test.A = BYTE( -20 );
	Test.Operand = BYTE( -17 );
	Test.Answer = BYTE( -3 );
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	TestSBCAbsoluteY( Test );
}

// SBC Indirect, X ---------

TEST_F( M6502AddSubWithCarryTests, SBCIndirectXCanSubtractZeroFromZeroAndGetZero )
{
	ADCTestData Test;
	Test.Carry = true;
	Test.A = 0;
	Test.Operand = 0;
	Test.Answer = 0;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = true;
	TestSBCIndirectX( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCIndirectXCanSubtractZeroFromZeroAndCarryAndGetMinusOne )
{
	ADCTestData Test;
	Test.Carry = false;
	Test.A = 0;
	Test.Operand = 0;
	Test.Answer = BYTE( -1 );
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	TestSBCIndirectX( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCIndirectXCanSubtractOneFromZeroAndGetMinusOne )
{
	ADCTestData Test;
	Test.Carry = true;
	Test.A = 0;
	Test.Operand = 1;
	Test.Answer = BYTE( -1 );
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	TestSBCIndirectX( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCIndirectXCanSubtractOneFromZeroWithCarryAndGetMinusTwo )
{
	ADCTestData Test;
	Test.Carry = false;
	Test.A = 0;
	Test.Operand = 1;
	Test.Answer = BYTE( -2 );
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	TestSBCIndirectX( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCIndirectXCanSubtractTwoNegativeNumbersAndGetSignedOverflow )
{
	ADCTestData Test;
	Test.Carry = true;
	Test.A = BYTE( -128 );
	Test.Operand = 1;
	Test.Answer = 127;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = true;
	Test.ExpectZ = false;
	TestSBCIndirectX( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCIndirectXCanSubtractAPostitiveAndNegativeNumbersAndGetSignedOverflow )
{
	ADCTestData Test;
	Test.Carry = true;
	Test.A = 127;
	Test.Operand = BYTE( -1 );
	Test.Answer = 128;
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = true;
	Test.ExpectZ = false;
	TestSBCIndirectX( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCIndirectXCanSubtractTwoUnsignedNumbers )
{
	ADCTestData Test;
	Test.Carry = true;
	Test.A = 20;
	Test.Operand = 17;
	Test.Answer = 3;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	TestSBCIndirectX( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCIndirectXCanSubtractTwoNegativeNumbers )
{
	ADCTestData Test;
	Test.Carry = true;
	Test.A = BYTE( -20 );
	Test.Operand = BYTE( -17 );
	Test.Answer = BYTE( -3 );
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	TestSBCIndirectX( Test );
}

// SBC Indirect, Y -----------

TEST_F( M6502AddSubWithCarryTests, SBCIndirectYCanSubtractZeroFromZeroAndGetZero )
{
	ADCTestData Test;
	Test.Carry = true;
	Test.A = 0;
	Test.Operand = 0;
	Test.Answer = 0;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = true;
	TestSBCIndirectY( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCIndirectYCanSubtractZeroFromZeroAndCarryAndGetMinusOne )
{
	ADCTestData Test;
	Test.Carry = false;
	Test.A = 0;
	Test.Operand = 0;
	Test.Answer = BYTE( -1 );
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	TestSBCIndirectY( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCIndirectYCanSubtractOneFromZeroAndGetMinusOne )
{
	ADCTestData Test;
	Test.Carry = true;
	Test.A = 0;
	Test.Operand = 1;
	Test.Answer = BYTE( -1 );
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	TestSBCIndirectY( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCIndirectYCanSubtractOneFromZeroWithCarryAndGetMinusTwo )
{
	ADCTestData Test;
	Test.Carry = false;
	Test.A = 0;
	Test.Operand = 1;
	Test.Answer = BYTE( -2 );
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	TestSBCIndirectY( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCIndirectYCanSubtractTwoNegativeNumbersAndGetSignedOverflow )
{
	ADCTestData Test;
	Test.Carry = true;
	Test.A = BYTE( -128 );
	Test.Operand = 1;
	Test.Answer = 127;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = true;
	Test.ExpectZ = false;
	TestSBCIndirectY( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCIndirectYCanSubtractAPostitiveAndNegativeNumbersAndGetSignedOverflow )
{
	ADCTestData Test;
	Test.Carry = true;
	Test.A = 127;
	Test.Operand = BYTE( -1 );
	Test.Answer = 128;
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = true;
	Test.ExpectZ = false;
	TestSBCIndirectY( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCIndirectYCanSubtractTwoUnsignedNumbers )
{
	ADCTestData Test;
	Test.Carry = true;
	Test.A = 20;
	Test.Operand = 17;
	Test.Answer = 3;
	Test.ExpectC = true;
	Test.ExpectN = false;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	TestSBCIndirectY( Test );
}

TEST_F( M6502AddSubWithCarryTests, SBCIndirectYCanSubtractTwoNegativeNumbers )
{
	ADCTestData Test;
	Test.Carry = true;
	Test.A = BYTE( -20 );
	Test.Operand = BYTE( -17 );
	Test.Answer = BYTE( -3 );
	Test.ExpectC = false;
	Test.ExpectN = true;
	Test.ExpectV = false;
	Test.ExpectZ = false;
	TestSBCIndirectY( Test );
}