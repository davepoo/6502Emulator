#include <gtest/gtest.h>
#include "main_6502.h"

class M6502Test1 : public testing::Test
{
public:
	Mem mem;
	CPU cpu;

	virtual void SetUp()
	{
		cpu.Reset( mem );
	}

	virtual void TearDown()
	{
	}
};


TEST_F( M6502Test1, LDAImmediateCanLoadAValueIntoTheARegister )
{
	// given:
	// start - inline a little program
	mem[0xFFFC] = CPU::INS_LDA_IM;
	mem[0xFFFD] = 0x84;
	// end - inline a little program

	//when:
	CPU CPUCopy = cpu;
	s32 CyclesUsed = cpu.Execute( 2, mem );

	//then:
	EXPECT_EQ( cpu.A, 0x84 );
	EXPECT_EQ( CyclesUsed, 2 );
	EXPECT_FALSE( cpu.Z );
	EXPECT_TRUE( cpu.N );
	EXPECT_EQ( cpu.C, CPUCopy.C );
	EXPECT_EQ( cpu.I, CPUCopy.I );
	EXPECT_EQ( cpu.D, CPUCopy.D );
	EXPECT_EQ( cpu.B, CPUCopy.B );
	EXPECT_EQ( cpu.V, CPUCopy.V );
}

TEST_F( M6502Test1, LDAZeroPageCanLoadAValueIntoTheARegister )
{
	// given:
	// start - inline a little program
	mem[0xFFFC] = CPU::INS_LDA_ZP;
	mem[0xFFFD] = 0x42;
	mem[0x0042] = 0x37;
	// end - inline a little program

	//when:
	CPU CPUCopy = cpu;
	s32 CyclesUsed = cpu.Execute( 3, mem );

	//then:
	EXPECT_EQ( cpu.A, 0x37 );
	EXPECT_EQ( CyclesUsed, 3 );
	EXPECT_FALSE( cpu.Z );
	EXPECT_FALSE( cpu.N );
	EXPECT_EQ( cpu.C, CPUCopy.C );
	EXPECT_EQ( cpu.I, CPUCopy.I );
	EXPECT_EQ( cpu.D, CPUCopy.D );
	EXPECT_EQ( cpu.B, CPUCopy.B );
	EXPECT_EQ( cpu.V, CPUCopy.V );
}

TEST_F( M6502Test1, LDAZeroPageXCanLoadAValueIntoTheARegister )
{
	// given:
	cpu.X = 5;
	// start - inline a little program
	mem[0xFFFC] = CPU::INS_LDA_ZPX;
	mem[0xFFFD] = 0x42;
	mem[0x0047] = 0x37;
	// end - inline a little program

	//when:
	CPU CPUCopy = cpu;
	s32 CyclesUsed = cpu.Execute( 4, mem );

	//then:
	EXPECT_EQ( cpu.A, 0x37 );
	EXPECT_EQ( CyclesUsed, 4 );
	EXPECT_FALSE( cpu.Z );
	EXPECT_FALSE( cpu.N );
	EXPECT_EQ( cpu.C, CPUCopy.C );
	EXPECT_EQ( cpu.I, CPUCopy.I );
	EXPECT_EQ( cpu.D, CPUCopy.D );
	EXPECT_EQ( cpu.B, CPUCopy.B );
	EXPECT_EQ( cpu.V, CPUCopy.V );
}

TEST_F( M6502Test1, LDAZeroPageXCanLoadAValueIntoTheARegisterWhenItWraps )
{
	// given:
	cpu.X = 0xFF;
	// start - inline a little program
	mem[0xFFFC] = CPU::INS_LDA_ZPX;
	mem[0xFFFD] = 0x80;
	mem[0x007F] = 0x37;
	// end - inline a little program

	//when:
	CPU CPUCopy = cpu;
	s32 CyclesUsed = cpu.Execute( 4, mem );

	//then:
	EXPECT_EQ( cpu.A, 0x37 );
	EXPECT_EQ( CyclesUsed, 4 );
	EXPECT_FALSE( cpu.Z );
	EXPECT_FALSE( cpu.N );
	EXPECT_EQ( cpu.C, CPUCopy.C );
	EXPECT_EQ( cpu.I, CPUCopy.I );
	EXPECT_EQ( cpu.D, CPUCopy.D );
	EXPECT_EQ( cpu.B, CPUCopy.B );
	EXPECT_EQ( cpu.V, CPUCopy.V );
}