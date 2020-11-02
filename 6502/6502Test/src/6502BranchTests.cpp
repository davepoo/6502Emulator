#include <gtest/gtest.h>
#include "m6502.h"

class M6502BranchTests : public testing::Test
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
};

TEST_F( M6502BranchTests, BEQCanBranchForwardsWhenZeroIsSet )
{
	// given:
	using namespace m6502;
	cpu.Reset( 0xFF00, mem );
	cpu.Flag.Z = true;
	mem[0xFF00] = CPU::INS_BEQ;
	mem[0xFF01] = 0x1;
	constexpr s32 EXPECTED_CYCLES = 3;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

	// then:
	EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
	EXPECT_EQ( cpu.PC, 0xFF03 );
	EXPECT_EQ( cpu.PS, CPUCopy.PS );
}

TEST_F( M6502BranchTests, BEQDoesNotBranchForwardsWhenZeroIsNotSet )
{
	// given:
	using namespace m6502;
	cpu.Reset( 0xFF00, mem );
	cpu.Flag.Z = false;
	mem[0xFF00] = CPU::INS_BEQ;
	mem[0xFF01] = 0x1;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

	// then:
	EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
	EXPECT_EQ( cpu.PC, 0xFF02 );
	EXPECT_EQ( cpu.PS, CPUCopy.PS );
}

TEST_F( M6502BranchTests, BEQCanBranchForwardsIntoANewPageWhenZeroIsSet )
{
	// given:
	using namespace m6502;
	cpu.Reset( 0xFEFD, mem );
	cpu.Flag.Z = true;
	mem[0xFEFD] = CPU::INS_BEQ;
	mem[0xFEFE] = 0x1;
	constexpr s32 EXPECTED_CYCLES = 4;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

	// then:
	EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
	EXPECT_EQ( cpu.PC, 0xFF00 );
	EXPECT_EQ( cpu.PS, CPUCopy.PS );
}

TEST_F( M6502BranchTests, BEQCanBranchBackwardsWhenZeroIsSet )
{
	// given:
	using namespace m6502;
	cpu.Reset( 0xFFCC, mem );
	cpu.Flag.Z = true;
	mem[0xFFCC] = CPU::INS_BEQ;
	mem[0xFFCD] = static_cast<Byte>( -0x2 );
	constexpr s32 EXPECTED_CYCLES = 3;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

	// then:
	EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
	EXPECT_EQ( cpu.PC, 0xFFCC );
	EXPECT_EQ( cpu.PS, CPUCopy.PS );
}

TEST_F( M6502BranchTests, BEQCanBranchBackwardsWhenZeroIsSetFromAssembleCode )
{
	// given:
	using namespace m6502;
	cpu.Reset( 0xFFCC, mem );
	cpu.Flag.Z = true;
	/*
	loop
	lda #0
	beq loop
	*/
	mem[0xFFCC] = 0xA9;
	mem[0xFFCC+1] = 0x00;
	mem[0xFFCC+2] = 0xF0;
	mem[0xFFCC+3] = 0xFC;
	constexpr s32 EXPECTED_CYCLES = 2 + 3;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

	// then:
	EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
	EXPECT_EQ( cpu.PC, 0xFFCC );
	EXPECT_EQ( cpu.PS, CPUCopy.PS );
}

TEST_F( M6502BranchTests, BNECanBranchForwardsWhenZeroIsNotSet )
{
	// given:
	using namespace m6502;
	cpu.Reset( 0xFF00, mem );
	cpu.Flag.Z = false;
	mem[0xFF00] = CPU::INS_BNE;
	mem[0xFF01] = 0x1;
	constexpr s32 EXPECTED_CYCLES = 3;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

	// then:
	EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
	EXPECT_EQ( cpu.PC, 0xFF03 );
	EXPECT_EQ( cpu.PS, CPUCopy.PS );
}


TEST_F( M6502BranchTests, BCSCanBranchForwardsWhenCarryFlagIsSet )
{
	// given:
	using namespace m6502;
	cpu.Reset( 0xFF00, mem );
	cpu.Flag.C = true;
	mem[0xFF00] = CPU::INS_BCS;
	mem[0xFF01] = 0x1;
	constexpr s32 EXPECTED_CYCLES = 3;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

	// then:
	EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
	EXPECT_EQ( cpu.PC, 0xFF03 );
	EXPECT_EQ( cpu.PS, CPUCopy.PS );
}

TEST_F( M6502BranchTests, BCCCanBranchForwardsWhenCarryFlagIsNotSet )
{
	// given:
	using namespace m6502;
	cpu.Reset( 0xFF00, mem );
	cpu.Flag.C = false;
	mem[0xFF00] = CPU::INS_BCC;
	mem[0xFF01] = 0x1;
	constexpr s32 EXPECTED_CYCLES = 3;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

	// then:
	EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
	EXPECT_EQ( cpu.PC, 0xFF03 );
	EXPECT_EQ( cpu.PS, CPUCopy.PS );
}

TEST_F( M6502BranchTests, BMICanBranchForwardsWhenNegativeFlagIsSet )
{
	// given:
	using namespace m6502;
	cpu.Reset( 0xFF00, mem );
	cpu.Flag.N = true;
	mem[0xFF00] = CPU::INS_BMI;
	mem[0xFF01] = 0x1;
	constexpr s32 EXPECTED_CYCLES = 3;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

	// then:
	EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
	EXPECT_EQ( cpu.PC, 0xFF03 );
	EXPECT_EQ( cpu.PS, CPUCopy.PS );
}

TEST_F( M6502BranchTests, BPLCanBranchForwardsWhenCarryNegativeIsNotSet )
{
	// given:
	using namespace m6502;
	cpu.Reset( 0xFF00, mem );
	cpu.Flag.N = false;
	mem[0xFF00] = CPU::INS_BPL;
	mem[0xFF01] = 0x1;
	constexpr s32 EXPECTED_CYCLES = 3;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

	// then:
	EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
	EXPECT_EQ( cpu.PC, 0xFF03 );
	EXPECT_EQ( cpu.PS, CPUCopy.PS );
}

TEST_F( M6502BranchTests, BVSCanBranchForwardsWhenOverflowFlagIsSet )
{
	// given:
	using namespace m6502;
	cpu.Reset( 0xFF00, mem );
	cpu.Flag.V = true;
	mem[0xFF00] = CPU::INS_BVS;
	mem[0xFF01] = 0x1;
	constexpr s32 EXPECTED_CYCLES = 3;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

	// then:
	EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
	EXPECT_EQ( cpu.PC, 0xFF03 );
	EXPECT_EQ( cpu.PS, CPUCopy.PS );
}

TEST_F( M6502BranchTests, BVCCanBranchForwardsWhenOverflowNegativeIsNotSet )
{
	// given:
	using namespace m6502;
	cpu.Reset( 0xFF00, mem );
	cpu.Flag.V = false;
	mem[0xFF00] = CPU::INS_BVC;
	mem[0xFF01] = 0x1;
	constexpr s32 EXPECTED_CYCLES = 3;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

	// then:
	EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
	EXPECT_EQ( cpu.PC, 0xFF03 );
	EXPECT_EQ( cpu.PS, CPUCopy.PS );
}