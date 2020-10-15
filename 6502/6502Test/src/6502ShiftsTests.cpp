#include <gtest/gtest.h>
#include "m6502.h"

class M6502ShiftsTests : public testing::Test
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

TEST_F( M6502ShiftsTests, ASLCanShiftTheValueOfOne )
{
	// given:
	using namespace m6502;
	cpu.Reset( 0xFF00, mem );
	cpu.Flag.C = true;
	cpu.Flag.Z = true;
	cpu.Flag.N = true;
	cpu.A = 1;
	mem[0xFF00] = CPU::INS_ASL;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

	// then:
	EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
	EXPECT_EQ( cpu.A, 2 );
	EXPECT_FALSE( cpu.Flag.C );
	EXPECT_FALSE( cpu.Flag.Z );
	EXPECT_FALSE( cpu.Flag.N );	
}

TEST_F( M6502ShiftsTests, ASLCanShiftANegativeValue )
{
	// given:
	using namespace m6502;
	cpu.Reset( 0xFF00, mem );
	cpu.Flag.C = false;
	cpu.Flag.Z = true;
	cpu.Flag.N = false;
	cpu.A = 0b11000010;
	mem[0xFF00] = CPU::INS_ASL;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

	// then:
	EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
	EXPECT_EQ( cpu.A, 0b10000100 );
	EXPECT_TRUE( cpu.Flag.C );
	EXPECT_FALSE( cpu.Flag.Z );
	EXPECT_TRUE( cpu.Flag.N );
}

TEST_F( M6502ShiftsTests, ASLZeroPageCanShiftTheValueOfOne )
{
	// given:
	using namespace m6502;
	cpu.Reset( 0xFF00, mem );
	cpu.Flag.C = true;
	cpu.Flag.Z = true;
	cpu.Flag.N = true;
	mem[0xFF00] = CPU::INS_ASL_ZP;
	mem[0xFF01] = 0x42;
	mem[0x0042] = 1;
	constexpr s32 EXPECTED_CYCLES = 5;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

	// then:
	EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
	EXPECT_EQ( mem[0x0042], 2 );
	EXPECT_FALSE( cpu.Flag.C );
	EXPECT_FALSE( cpu.Flag.Z );
	EXPECT_FALSE( cpu.Flag.N );
}

TEST_F( M6502ShiftsTests, ASLZeroPageCanShiftANegativeValue )
{
	// given:
	using namespace m6502;
	cpu.Reset( 0xFF00, mem );
	cpu.Flag.C = false;
	cpu.Flag.Z = true;
	cpu.Flag.N = false;
	mem[0xFF00] = CPU::INS_ASL_ZP;
	mem[0xFF01] = 0x42;
	mem[0x0042] = 0b11000010;
	constexpr s32 EXPECTED_CYCLES = 5;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

	// then:
	EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
	EXPECT_EQ( mem[0x0042], 0b10000100 );
	EXPECT_TRUE( cpu.Flag.C );
	EXPECT_FALSE( cpu.Flag.Z );
	EXPECT_TRUE( cpu.Flag.N );
}

TEST_F( M6502ShiftsTests, ASLZeroPageXCanShiftTheValueOfOne )
{
	// given:
	using namespace m6502;
	cpu.Reset( 0xFF00, mem );
	cpu.Flag.C = true;
	cpu.Flag.Z = true;
	cpu.Flag.N = true;
	cpu.X = 0x10;
	mem[0xFF00] = CPU::INS_ASL_ZPX;
	mem[0xFF01] = 0x42;
	mem[0x0042 + 0x10] = 1;
	constexpr s32 EXPECTED_CYCLES = 6;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

	// then:
	EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
	EXPECT_EQ( mem[0x0042 + 0x10], 2 );
	EXPECT_FALSE( cpu.Flag.C );
	EXPECT_FALSE( cpu.Flag.Z );
	EXPECT_FALSE( cpu.Flag.N );
}

TEST_F( M6502ShiftsTests, ASLZeroPageXCanShiftANegativeValue )
{
	// given:
	using namespace m6502;
	cpu.Reset( 0xFF00, mem );
	cpu.Flag.C = false;
	cpu.Flag.Z = true;
	cpu.Flag.N = false;
	cpu.X = 0x10;
	mem[0xFF00] = CPU::INS_ASL_ZPX;
	mem[0xFF01] = 0x42;
	mem[0x0042 + 0x10] = 0b11000010;
	constexpr s32 EXPECTED_CYCLES = 6;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

	// then:
	EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
	EXPECT_EQ( mem[0x0042 + 0x10], 0b10000100 );
	EXPECT_TRUE( cpu.Flag.C );
	EXPECT_FALSE( cpu.Flag.Z );
	EXPECT_TRUE( cpu.Flag.N );
}

TEST_F( M6502ShiftsTests, ASLAbsCanShiftTheValueOfOne )
{
	// given:
	using namespace m6502;
	cpu.Reset( 0xFF00, mem );
	cpu.Flag.C = true;
	cpu.Flag.Z = true;
	cpu.Flag.N = true;
	mem[0xFF00] = CPU::INS_ASL_ABS;
	mem[0xFF01] = 0x00;
	mem[0xFF02] = 0x80;
	mem[0x8000] = 1;
	constexpr s32 EXPECTED_CYCLES = 6;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

	// then:
	EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
	EXPECT_EQ( mem[0x8000], 2 );
	EXPECT_FALSE( cpu.Flag.C );
	EXPECT_FALSE( cpu.Flag.Z );
	EXPECT_FALSE( cpu.Flag.N );
}

TEST_F( M6502ShiftsTests, ASLAbsCanShiftANegativeValue )
{
	// given:
	using namespace m6502;
	cpu.Reset( 0xFF00, mem );
	cpu.Flag.C = false;
	cpu.Flag.Z = true;
	cpu.Flag.N = false;
	mem[0xFF00] = CPU::INS_ASL_ABS;
	mem[0xFF01] = 0x00;
	mem[0xFF02] = 0x80;
	mem[0x8000] = 0b11000010;
	constexpr s32 EXPECTED_CYCLES = 6;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

	// then:
	EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
	EXPECT_EQ( mem[0x8000], 0b10000100 );
	EXPECT_TRUE( cpu.Flag.C );
	EXPECT_FALSE( cpu.Flag.Z );
	EXPECT_TRUE( cpu.Flag.N );
}

TEST_F( M6502ShiftsTests, ASLAbsXCanShiftTheValueOfOne )
{
	// given:
	using namespace m6502;
	cpu.Reset( 0xFF00, mem );
	cpu.Flag.C = true;
	cpu.Flag.Z = true;
	cpu.Flag.N = true;
	cpu.X = 0x10;
	mem[0xFF00] = CPU::INS_ASL_ABSX;
	mem[0xFF01] = 0x00;
	mem[0xFF02] = 0x80;
	mem[0x8000 + 0x10] = 1;
	constexpr s32 EXPECTED_CYCLES = 7;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

	// then:
	EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
	EXPECT_EQ( mem[0x8000 + 0x10], 2 );
	EXPECT_FALSE( cpu.Flag.C );
	EXPECT_FALSE( cpu.Flag.Z );
	EXPECT_FALSE( cpu.Flag.N );
}

TEST_F( M6502ShiftsTests, ASLAbsXCanShiftANegativeValue )
{
	// given:
	using namespace m6502;
	cpu.Reset( 0xFF00, mem );
	cpu.Flag.C = false;
	cpu.Flag.Z = true;
	cpu.Flag.N = false;
	cpu.X = 0x10;
	mem[0xFF00] = CPU::INS_ASL_ABSX;
	mem[0xFF01] = 0x00;
	mem[0xFF02] = 0x80;
	mem[0x8000+0x10] = 0b11000010;
	constexpr s32 EXPECTED_CYCLES = 7;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

	// then:
	EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
	EXPECT_EQ( mem[0x8000 + 0x10], 0b10000100 );
	EXPECT_TRUE( cpu.Flag.C );
	EXPECT_FALSE( cpu.Flag.Z );
	EXPECT_TRUE( cpu.Flag.N );
}

TEST_F( M6502ShiftsTests, LSRCanShiftTheValueOfOne )
{
	// given:
	using namespace m6502;
	cpu.Reset( 0xFF00, mem );
	cpu.Flag.C = false;
	cpu.Flag.Z = false;
	cpu.Flag.N = true;
	cpu.A = 1;
	mem[0xFF00] = CPU::INS_LSR;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

	// then:
	EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
	EXPECT_EQ( cpu.A, 0 );
	EXPECT_TRUE( cpu.Flag.C );
	EXPECT_TRUE( cpu.Flag.Z );
	EXPECT_FALSE( cpu.Flag.N );
}

TEST_F( M6502ShiftsTests, LSRCanShiftAZeroIntoTheCarryFlag )
{
	// given:
	using namespace m6502;
	cpu.Reset( 0xFF00, mem );
	cpu.Flag.C = true;
	cpu.Flag.Z = true;
	cpu.Flag.N = true;
	cpu.A = 8;
	mem[0xFF00] = CPU::INS_LSR;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

	// then:
	EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
	EXPECT_EQ( cpu.A, 4 );
	EXPECT_FALSE( cpu.Flag.C );
	EXPECT_FALSE( cpu.Flag.Z );
	EXPECT_FALSE( cpu.Flag.N );
}

TEST_F( M6502ShiftsTests, LSRZeroPageCanShiftTheValueOfOne )
{
	// given:
	using namespace m6502;
	cpu.Reset( 0xFF00, mem );
	cpu.Flag.C = false;
	cpu.Flag.Z = false;
	cpu.Flag.N = true;
	mem[0xFF00] = CPU::INS_LSR_ZP;
	mem[0xFF01] = 0x42;
	mem[0x0042] = 1;
	constexpr s32 EXPECTED_CYCLES = 5;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

	// then:
	EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
	EXPECT_EQ( mem[0x0042], 0 );
	EXPECT_TRUE( cpu.Flag.C );
	EXPECT_TRUE( cpu.Flag.Z );
	EXPECT_FALSE( cpu.Flag.N );
}

TEST_F( M6502ShiftsTests, LSRZeroPageCanShiftAZeroIntoTheCarryFlag )
{
	// given:
	using namespace m6502;
	cpu.Reset( 0xFF00, mem );
	cpu.Flag.C = true;
	cpu.Flag.Z = true;
	cpu.Flag.N = true;
	mem[0xFF00] = CPU::INS_LSR_ZP;
	mem[0xFF01] = 0x42;
	mem[0x0042] = 8;
	constexpr s32 EXPECTED_CYCLES = 5;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

	// then:
	EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
	EXPECT_EQ( mem[0x0042], 4 );
	EXPECT_FALSE( cpu.Flag.C );
	EXPECT_FALSE( cpu.Flag.Z );
	EXPECT_FALSE( cpu.Flag.N );
}

TEST_F( M6502ShiftsTests, LSRZeroPageXCanShiftTheValueOfOne )
{
	// given:
	using namespace m6502;
	cpu.Reset( 0xFF00, mem );
	cpu.Flag.C = false;
	cpu.Flag.Z = false;
	cpu.Flag.N = true;
	cpu.X = 0x10;
	mem[0xFF00] = CPU::INS_LSR_ZPX;
	mem[0xFF01] = 0x42;
	mem[0x0042+ 0x10] = 1;
	constexpr s32 EXPECTED_CYCLES = 6;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

	// then:
	EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
	EXPECT_EQ( mem[0x0042+0x10], 0 );
	EXPECT_TRUE( cpu.Flag.C );
	EXPECT_TRUE( cpu.Flag.Z );
	EXPECT_FALSE( cpu.Flag.N );
}

TEST_F( M6502ShiftsTests, LSRZeroPageXCanShiftAZeroIntoTheCarryFlag )
{
	// given:
	using namespace m6502;
	cpu.Reset( 0xFF00, mem );
	cpu.Flag.C = true;
	cpu.Flag.Z = true;
	cpu.Flag.N = true;
	cpu.X = 0x10;
	mem[0xFF00] = CPU::INS_LSR_ZPX;
	mem[0xFF01] = 0x42;
	mem[0x0042+ 0x10] = 8;
	constexpr s32 EXPECTED_CYCLES = 6;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

	// then:
	EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
	EXPECT_EQ( mem[0x0042+ 0x10], 4 );
	EXPECT_FALSE( cpu.Flag.C );
	EXPECT_FALSE( cpu.Flag.Z );
	EXPECT_FALSE( cpu.Flag.N );
}

TEST_F( M6502ShiftsTests, LSRAbsCanShiftTheValueOfOne )
{
	// given:
	using namespace m6502;
	cpu.Reset( 0xFF00, mem );
	cpu.Flag.C = false;
	cpu.Flag.Z = false;
	cpu.Flag.N = true;
	mem[0xFF00] = CPU::INS_LSR_ABS;
	mem[0xFF01] = 0x00;
	mem[0xFF02] = 0x80;
	mem[0x8000] = 1;
	constexpr s32 EXPECTED_CYCLES = 6;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

	// then:
	EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
	EXPECT_EQ( mem[0x8000], 0 );
	EXPECT_TRUE( cpu.Flag.C );
	EXPECT_TRUE( cpu.Flag.Z );
	EXPECT_FALSE( cpu.Flag.N );
}

TEST_F( M6502ShiftsTests, LSRAbsCanShiftAZeroIntoTheCarryFlag )
{
	// given:
	using namespace m6502;
	cpu.Reset( 0xFF00, mem );
	cpu.Flag.C = true;
	cpu.Flag.Z = true;
	cpu.Flag.N = true;
	mem[0xFF00] = CPU::INS_LSR_ABS;
	mem[0xFF01] = 0x00;
	mem[0xFF02] = 0x80;
	mem[0x8000] = 8;
	constexpr s32 EXPECTED_CYCLES = 6;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

	// then:
	EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
	EXPECT_EQ( mem[0x8000], 4 );
	EXPECT_FALSE( cpu.Flag.C );
	EXPECT_FALSE( cpu.Flag.Z );
	EXPECT_FALSE( cpu.Flag.N );
}

TEST_F( M6502ShiftsTests, LSRAbsXCanShiftTheValueOfOne )
{
	// given:
	using namespace m6502;
	cpu.Reset( 0xFF00, mem );
	cpu.Flag.C = false;
	cpu.Flag.Z = false;
	cpu.Flag.N = true;
	cpu.X = 0x10;
	mem[0xFF00] = CPU::INS_LSR_ABSX;
	mem[0xFF01] = 0x00;
	mem[0xFF02] = 0x80;
	mem[0x8000+0x10] = 1;
	constexpr s32 EXPECTED_CYCLES = 7;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

	// then:
	EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
	EXPECT_EQ( mem[0x8000 + 0x10], 0 );
	EXPECT_TRUE( cpu.Flag.C );
	EXPECT_TRUE( cpu.Flag.Z );
	EXPECT_FALSE( cpu.Flag.N );
}

TEST_F( M6502ShiftsTests, LSRAbsXCanShiftAZeroIntoTheCarryFlag )
{
	// given:
	using namespace m6502;
	cpu.Reset( 0xFF00, mem );
	cpu.Flag.C = true;
	cpu.Flag.Z = true;
	cpu.Flag.N = true;
	cpu.X = 0x10;
	mem[0xFF00] = CPU::INS_LSR_ABSX;
	mem[0xFF01] = 0x00;
	mem[0xFF02] = 0x80;
	mem[0x8000 + 0x10] = 8;
	constexpr s32 EXPECTED_CYCLES = 7;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

	// then:
	EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
	EXPECT_EQ( mem[0x8000 + 0x10], 4 );
	EXPECT_FALSE( cpu.Flag.C );
	EXPECT_FALSE( cpu.Flag.Z );
	EXPECT_FALSE( cpu.Flag.N );
}