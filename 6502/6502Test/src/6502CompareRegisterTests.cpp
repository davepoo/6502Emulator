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
};

TEST_F( M6502CompareRegisterTests, CMPImmediateCanCompareTwoIdenticalValues  )
{
	// given:
	using namespace m6502;
	cpu.Reset( 0xFF00, mem );
	cpu.Flag.C = false;
	cpu.Flag.Z = false;
	cpu.Flag.N = true;
	cpu.A = 26;
	mem[0xFF00] = CPU::INS_CMP;
	mem[0xFF01] = 26;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

	// then:
	EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
	EXPECT_EQ( cpu.A, 26 );
	EXPECT_TRUE( cpu.Flag.Z );
	EXPECT_FALSE( cpu.Flag.N );
	EXPECT_TRUE( cpu.Flag.C );
	ExpectUnaffectedRegisters( CPUCopy );
}

TEST_F( M6502CompareRegisterTests, CMPImmediateCanCompareALargePositiveToASmallPositive )
{
	// given:
	using namespace m6502;
	cpu.Reset( 0xFF00, mem );
	cpu.Flag.Z = true;
	cpu.Flag.N = true;
	cpu.Flag.C = false;
	cpu.A = 48;
	mem[0xFF00] = CPU::INS_CMP;
	mem[0xFF01] = 26;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

	// then:
	EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
	EXPECT_EQ( cpu.A, 48 );
	EXPECT_FALSE( cpu.Flag.Z );
	EXPECT_FALSE( cpu.Flag.N );
	EXPECT_TRUE( cpu.Flag.C );
	ExpectUnaffectedRegisters( CPUCopy );
}

TEST_F( M6502CompareRegisterTests, CMPImmediateCanCompareANegativeNumberToAPositive )
{
	// given:
	using namespace m6502;
	cpu.Reset( 0xFF00, mem );
	cpu.Flag.Z = true;
	cpu.Flag.N = true;
	cpu.Flag.C = false;
	cpu.A = 130;	//Negative number!
	mem[0xFF00] = CPU::INS_CMP;
	mem[0xFF01] = 26;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

	// then:
	EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
	EXPECT_EQ( cpu.A, 130 );
	EXPECT_FALSE( cpu.Flag.Z );
	EXPECT_FALSE( cpu.Flag.N );
	EXPECT_TRUE( cpu.Flag.C );
	ExpectUnaffectedRegisters( CPUCopy );
}

TEST_F( M6502CompareRegisterTests, CMPImmediateCanCompareTwoValuesThatResultInANegativeFlagSet )
{
	// given:
	using namespace m6502;
	cpu.Reset( 0xFF00, mem );
	cpu.Flag.Z = true;
	cpu.Flag.N = false;
	cpu.Flag.C = true;
	cpu.A = 8;
	mem[0xFF00] = CPU::INS_CMP;
	mem[0xFF01] = 26;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

	// then:
	EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
	EXPECT_EQ( cpu.A, 8 );
	EXPECT_FALSE( cpu.Flag.Z );
	EXPECT_TRUE( cpu.Flag.N );
	EXPECT_FALSE( cpu.Flag.C );
	ExpectUnaffectedRegisters( CPUCopy );
}