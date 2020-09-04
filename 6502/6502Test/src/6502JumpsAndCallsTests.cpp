#include <gtest/gtest.h>
#include "m6502.h"

class M6502JumpsAndCallsTests : public testing::Test
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

TEST_F( M6502JumpsAndCallsTests, CanJumpToASubroutineAndJumpBackAgain )
{
	// given:
	using namespace m6502;
	cpu.Reset( 0xFF00, mem );
	mem[0xFF00] = CPU::INS_JSR;
	mem[0xFF01] = 0x00;
	mem[0xFF02] = 0x80;
	mem[0x8000] = CPU::INS_RTS;
	mem[0xFF03] = CPU::INS_LDA_IM;
	mem[0xFF04] = 0x42;
	constexpr s32 EXPECTED_CYCLES = 6 + 6 + 2;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

	// then:
	EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
	EXPECT_EQ( cpu.A, 0x42 );
	EXPECT_EQ( cpu.SP, CPUCopy.SP );
}

TEST_F( M6502JumpsAndCallsTests, JSRDoesNotAffectTheProcessorStatus )
{
	// given:
	using namespace m6502;
	cpu.Reset( 0xFF00, mem );
	mem[0xFF00] = CPU::INS_JSR;
	mem[0xFF01] = 0x00;
	mem[0xFF02] = 0x80;
	constexpr s32 EXPECTED_CYCLES = 6;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

	// then:
	EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
	EXPECT_EQ( cpu.PS, CPUCopy.PS );
	EXPECT_NE( cpu.SP, CPUCopy.SP );
	EXPECT_EQ( cpu.PC, 0x8000 );
}

TEST_F( M6502JumpsAndCallsTests, RTSDoesNotAffectTheProcessorStatus )
{
	// given:
	using namespace m6502;
	cpu.Reset( 0xFF00, mem );
	mem[0xFF00] = CPU::INS_JSR;
	mem[0xFF01] = 0x00;
	mem[0xFF02] = 0x80;
	mem[0x8000] = CPU::INS_RTS;
	constexpr s32 EXPECTED_CYCLES = 6 + 6;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

	// then:
	EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
	EXPECT_EQ( cpu.PS, CPUCopy.PS );
	EXPECT_EQ( cpu.PC, 0xFF03 );
}

TEST_F( M6502JumpsAndCallsTests, JumpAbsoluteCanJumpToAnNewLocationInTheProgram )
{
	// given:
	using namespace m6502;
	cpu.Reset( 0xFF00, mem );
	mem[0xFF00] = CPU::INS_JMP_ABS;
	mem[0xFF01] = 0x00;
	mem[0xFF02] = 0x80;
	constexpr s32 EXPECTED_CYCLES = 3;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

	// then:
	EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
	EXPECT_EQ( cpu.PS, CPUCopy.PS );
	EXPECT_EQ( cpu.SP, CPUCopy.SP );
	EXPECT_EQ( cpu.PC, 0x8000 );
}

TEST_F( M6502JumpsAndCallsTests, JumpIndirectCanJumpToAnNewLocationInTheProgram )
{
	// given:
	using namespace m6502;
	cpu.Reset( 0xFF00, mem );
	mem[0xFF00] = CPU::INS_JMP_IND;
	mem[0xFF01] = 0x00;
	mem[0xFF02] = 0x80;
	mem[0x8000] = 0x00;
	mem[0x8001] = 0x90;
	constexpr s32 EXPECTED_CYCLES = 5;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

	// then:
	EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
	EXPECT_EQ( cpu.PS, CPUCopy.PS );
	EXPECT_EQ( cpu.SP, CPUCopy.SP );
	EXPECT_EQ( cpu.PC, 0x9000 );
}
