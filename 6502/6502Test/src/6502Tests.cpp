#include <gtest/gtest.h>
#include "m6502.h"

class M6502Test1 : public testing::Test
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

static void VerfifyUnmodifiedFlagsFromLDA( 
	const m6502::CPU& cpu, 
	const m6502::CPU& CPUCopy )
{
	EXPECT_EQ( cpu.C, CPUCopy.C );
	EXPECT_EQ( cpu.I, CPUCopy.I );
	EXPECT_EQ( cpu.D, CPUCopy.D );
	EXPECT_EQ( cpu.B, CPUCopy.B );
	EXPECT_EQ( cpu.V, CPUCopy.V );
}

TEST_F( M6502Test1, TheCPUDoesNothingWhenWeExecuteZeroCycles )
{
	//given:
	using namespace m6502;
	constexpr s32 NUM_CYCLES = 0;

	//when:
	s32 CyclesUsed = cpu.Execute( NUM_CYCLES, mem );

	//then:
	EXPECT_EQ( CyclesUsed, 0 );
}

TEST_F( M6502Test1, CPUCanExecuteMoreCyclesThanRequestedIfRequiredByTheInstruction )
{
	// given:
	using namespace m6502;
	mem[0xFFFC] = CPU::INS_LDA_IM;
	mem[0xFFFD] = 0x84;
	CPU CPUCopy = cpu;
	constexpr s32 NUM_CYCLES = 1;

	//when:
	s32 CyclesUsed = cpu.Execute( NUM_CYCLES, mem );

	//then:
	EXPECT_EQ( CyclesUsed, 2 );
}

TEST_F( M6502Test1, LDAImmediateCanLoadAValueIntoTheARegister )
{
	// given:
	using namespace m6502;
	mem[0xFFFC] = CPU::INS_LDA_IM;
	mem[0xFFFD] = 0x84;

	//when:
	CPU CPUCopy = cpu;
	s32 CyclesUsed = cpu.Execute( 2, mem );

	//then:
	EXPECT_EQ( cpu.A, 0x84 );
	EXPECT_EQ( CyclesUsed, 2 );
	EXPECT_FALSE( cpu.Z );
	EXPECT_TRUE( cpu.N );
	VerfifyUnmodifiedFlagsFromLDA( cpu, CPUCopy );
}

TEST_F( M6502Test1, LDAImmediateCanAffectTheZeroFlag )
{
	// given:
	using namespace m6502;
	cpu.A = 0x44;
	mem[0xFFFC] = CPU::INS_LDA_IM;
	mem[0xFFFD] = 0x0;
	CPU CPUCopy = cpu;

	//when:
	cpu.Execute( 2, mem );

	//then:
	EXPECT_TRUE( cpu.Z );
	EXPECT_FALSE( cpu.N );
	VerfifyUnmodifiedFlagsFromLDA( cpu, CPUCopy );
}

TEST_F( M6502Test1, LDAZeroPageCanLoadAValueIntoTheARegister )
{
	// given:
	using namespace m6502;
	mem[0xFFFC] = CPU::INS_LDA_ZP;
	mem[0xFFFD] = 0x42;
	mem[0x0042] = 0x37;

	//when:
	CPU CPUCopy = cpu;
	s32 CyclesUsed = cpu.Execute( 3, mem );

	//then:
	EXPECT_EQ( cpu.A, 0x37 );
	EXPECT_EQ( CyclesUsed, 3 );
	EXPECT_FALSE( cpu.Z );
	EXPECT_FALSE( cpu.N );
	VerfifyUnmodifiedFlagsFromLDA( cpu, CPUCopy );
}

TEST_F( M6502Test1, LDAZeroPageXCanLoadAValueIntoTheARegister )
{
	// given:
	using namespace m6502;
	cpu.X = 5;
	mem[0xFFFC] = CPU::INS_LDA_ZPX;
	mem[0xFFFD] = 0x42;
	mem[0x0047] = 0x37;

	//when:
	CPU CPUCopy = cpu;
	s32 CyclesUsed = cpu.Execute( 4, mem );

	//then:
	EXPECT_EQ( cpu.A, 0x37 );
	EXPECT_EQ( CyclesUsed, 4 );
	EXPECT_FALSE( cpu.Z );
	EXPECT_FALSE( cpu.N );
	VerfifyUnmodifiedFlagsFromLDA( cpu, CPUCopy );
}

TEST_F( M6502Test1, LDAZeroPageXCanLoadAValueIntoTheARegisterWhenItWraps )
{
	// given:
	using namespace m6502;
	cpu.X = 0xFF;
	mem[0xFFFC] = CPU::INS_LDA_ZPX;
	mem[0xFFFD] = 0x80;
	mem[0x007F] = 0x37;

	//when:
	CPU CPUCopy = cpu;
	s32 CyclesUsed = cpu.Execute( 4, mem );

	//then:
	EXPECT_EQ( cpu.A, 0x37 );
	EXPECT_EQ( CyclesUsed, 4 );
	EXPECT_FALSE( cpu.Z );
	EXPECT_FALSE( cpu.N );
	VerfifyUnmodifiedFlagsFromLDA( cpu, CPUCopy );
}

TEST_F( M6502Test1, LDAAbsoluteCanLoadAValueIntoTheARegister )
{
	// given:
	using namespace m6502;
	mem[0xFFFC] = CPU::INS_LDA_ABS;
	mem[0xFFFD] = 0x80;
	mem[0xFFFE] = 0x44;	//0x4480
	mem[0x4480] = 0x37;
	constexpr s32 EXPECTED_CYCLES = 4;
	CPU CPUCopy = cpu;

	//when:
	s32 CyclesUsed = cpu.Execute( EXPECTED_CYCLES, mem );

	//then:
	EXPECT_EQ( cpu.A, 0x37 );
	EXPECT_EQ( CyclesUsed, EXPECTED_CYCLES );
	EXPECT_FALSE( cpu.Z );
	EXPECT_FALSE( cpu.N );
	VerfifyUnmodifiedFlagsFromLDA( cpu, CPUCopy );
}

TEST_F( M6502Test1, LDAAbsoluteXCanLoadAValueIntoTheARegister )
{
	// given:
	using namespace m6502;
	cpu.X = 1;
	mem[0xFFFC] = CPU::INS_LDA_ABSX;
	mem[0xFFFD] = 0x80;
	mem[0xFFFE] = 0x44;	//0x4480
	mem[0x4481] = 0x37;	
	constexpr s32 EXPECTED_CYCLES = 4;
	CPU CPUCopy = cpu;

	//when:
	s32 CyclesUsed = cpu.Execute( EXPECTED_CYCLES, mem );

	//then:
	EXPECT_EQ( cpu.A, 0x37 );
	EXPECT_EQ( CyclesUsed, EXPECTED_CYCLES );
	EXPECT_FALSE( cpu.Z );
	EXPECT_FALSE( cpu.N );
	VerfifyUnmodifiedFlagsFromLDA( cpu, CPUCopy );
}

TEST_F( M6502Test1, LDAAbsoluteXCanLoadAValueIntoTheARegisterWhenItCrossesAPageBoundary )
{
	// given:
	using namespace m6502;
	cpu.X = 0xFF;
	mem[0xFFFC] = CPU::INS_LDA_ABSX;
	mem[0xFFFD] = 0x02;
	mem[0xFFFE] = 0x44;	//0x4402
	mem[0x4501] = 0x37;	//0x4402+0xFF crosses page boundary!
	constexpr s32 EXPECTED_CYCLES = 5;
	CPU CPUCopy = cpu;

	//when:
	s32 CyclesUsed = cpu.Execute( EXPECTED_CYCLES, mem );

	//then:
	EXPECT_EQ( cpu.A, 0x37 );
	EXPECT_EQ( CyclesUsed, EXPECTED_CYCLES );
	EXPECT_FALSE( cpu.Z );
	EXPECT_FALSE( cpu.N );
	VerfifyUnmodifiedFlagsFromLDA( cpu, CPUCopy );
}

TEST_F( M6502Test1, LDAAbsoluteYCanLoadAValueIntoTheARegister )
{
	// given:
	using namespace m6502;
	cpu.Y = 1;
	mem[0xFFFC] = CPU::INS_LDA_ABSY;
	mem[0xFFFD] = 0x80;
	mem[0xFFFE] = 0x44;	//0x4480
	mem[0x4481] = 0x37;
	constexpr s32 EXPECTED_CYCLES = 4;
	CPU CPUCopy = cpu;

	//when:
	s32 CyclesUsed = cpu.Execute( EXPECTED_CYCLES, mem );

	//then:
	EXPECT_EQ( cpu.A, 0x37 );
	EXPECT_EQ( CyclesUsed, EXPECTED_CYCLES );
	EXPECT_FALSE( cpu.Z );
	EXPECT_FALSE( cpu.N );
	VerfifyUnmodifiedFlagsFromLDA( cpu, CPUCopy );
}

TEST_F( M6502Test1, LDAAbsoluteYCanLoadAValueIntoTheARegisterWhenItCrossesAPageBoundary )
{
	// given:
	using namespace m6502;
	cpu.Y = 0xFF;
	mem[0xFFFC] = CPU::INS_LDA_ABSY;
	mem[0xFFFD] = 0x02;
	mem[0xFFFE] = 0x44;	//0x4402
	mem[0x4501] = 0x37;	//0x4402+0xFF crosses page boundary!
	constexpr s32 EXPECTED_CYCLES = 5;
	CPU CPUCopy = cpu;

	//when:
	s32 CyclesUsed = cpu.Execute( EXPECTED_CYCLES, mem );

	//then:
	EXPECT_EQ( cpu.A, 0x37 );
	EXPECT_EQ( CyclesUsed, EXPECTED_CYCLES );
	EXPECT_FALSE( cpu.Z );
	EXPECT_FALSE( cpu.N );
	VerfifyUnmodifiedFlagsFromLDA( cpu, CPUCopy );
}

TEST_F( M6502Test1, LDAIndirectXCanLoadAValueIntoTheARegister )
{
	// given:
	using namespace m6502;
	cpu.X = 0x04;
	mem[0xFFFC] = CPU::INS_LDA_INDX;
	mem[0xFFFD] = 0x02;
	mem[0x0006] = 0x00;	//0x2 + 0x4
	mem[0x0007] = 0x80;	
	mem[0x8000] = 0x37;
	constexpr s32 EXPECTED_CYCLES = 6;
	CPU CPUCopy = cpu;

	//when:
	s32 CyclesUsed = cpu.Execute( EXPECTED_CYCLES, mem );

	//then:
	EXPECT_EQ( cpu.A, 0x37 );
	EXPECT_EQ( CyclesUsed, EXPECTED_CYCLES );
	EXPECT_FALSE( cpu.Z );
	EXPECT_FALSE( cpu.N );
	VerfifyUnmodifiedFlagsFromLDA( cpu, CPUCopy );
}

TEST_F( M6502Test1, LDAIndirectYCanLoadAValueIntoTheARegister )
{
	// given:
	using namespace m6502;
	cpu.Y = 0x04;
	mem[0xFFFC] = CPU::INS_LDA_INDY;
	mem[0xFFFD] = 0x02;
	mem[0x0002] = 0x00;	
	mem[0x0003] = 0x80;
	mem[0x8004] = 0x37;	//0x8000 + 0x4
	constexpr s32 EXPECTED_CYCLES = 5;
	CPU CPUCopy = cpu;

	//when:
	s32 CyclesUsed = cpu.Execute( EXPECTED_CYCLES, mem );

	//then:
	EXPECT_EQ( cpu.A, 0x37 );
	EXPECT_EQ( CyclesUsed, EXPECTED_CYCLES );
	EXPECT_FALSE( cpu.Z );
	EXPECT_FALSE( cpu.N );
	VerfifyUnmodifiedFlagsFromLDA( cpu, CPUCopy );
}

TEST_F( M6502Test1, LDAIndirectYCanLoadAValueIntoTheARegisterWhenItCrossesAPage )
{
	// given:
	using namespace m6502;
	cpu.Y = 0xFF;
	mem[0xFFFC] = CPU::INS_LDA_INDY;
	mem[0xFFFD] = 0x02;
	mem[0x0002] = 0x02;
	mem[0x0003] = 0x80;
	mem[0x8101] = 0x37;	//0x8002 + 0xFF
	constexpr s32 EXPECTED_CYCLES = 6;
	CPU CPUCopy = cpu;

	//when:
	s32 CyclesUsed = cpu.Execute( EXPECTED_CYCLES, mem );

	//then:
	EXPECT_EQ( cpu.A, 0x37 );
	EXPECT_EQ( CyclesUsed, EXPECTED_CYCLES );
	EXPECT_FALSE( cpu.Z );
	EXPECT_FALSE( cpu.N );
	VerfifyUnmodifiedFlagsFromLDA( cpu, CPUCopy );
}
