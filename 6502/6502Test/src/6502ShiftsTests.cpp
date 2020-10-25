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

 // ------------ ROL ----------------

TEST_F( M6502ShiftsTests, ROLCanShiftABitOutOfTheCarryFlag )
{
	// given:
	using namespace m6502;
	cpu.Reset( 0xFF00, mem );
	cpu.Flag.C = true;
	cpu.Flag.Z = true;
	cpu.Flag.N = true;
	cpu.A = 0;
	mem[0xFF00] = CPU::INS_ROL;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

	// then:
	EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
	EXPECT_EQ( cpu.A, 1 );
	EXPECT_FALSE( cpu.Flag.C );
	EXPECT_FALSE( cpu.Flag.Z );
	EXPECT_FALSE( cpu.Flag.N );
}

TEST_F( M6502ShiftsTests, ROLCanShiftABitIntoTheCarryFlag )
{
	// given:
	using namespace m6502;
	cpu.Reset( 0xFF00, mem );
	cpu.Flag.C = false;
	cpu.Flag.Z = false;
	cpu.Flag.N = true;
	cpu.A = 0b10000000;
	mem[0xFF00] = CPU::INS_ROL;
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

TEST_F( M6502ShiftsTests, ROLCanShiftZeroWithNoCarry )
{
	// given:
	using namespace m6502;
	cpu.Reset( 0xFF00, mem );
	cpu.Flag.C = false;
	cpu.Flag.Z = false;
	cpu.Flag.N = true;
	cpu.A = 0;
	mem[0xFF00] = CPU::INS_ROL;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

	// then:
	EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
	EXPECT_EQ( cpu.A, 0 );
	EXPECT_FALSE( cpu.Flag.C );
	EXPECT_TRUE( cpu.Flag.Z );
	EXPECT_FALSE( cpu.Flag.N );
}

TEST_F( M6502ShiftsTests, ROLCanShiftAValueThatResultInANegativeValue )
{
	// given:
	using namespace m6502;
	cpu.Reset( 0xFF00, mem );
	cpu.Flag.C = true;
	cpu.Flag.Z = false;
	cpu.Flag.N = false;
	cpu.A = 0b01110011;
	mem[0xFF00] = CPU::INS_ROL;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

	// then:
	EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
	EXPECT_EQ( cpu.A, 0b11100111 );
	EXPECT_FALSE( cpu.Flag.C );
	EXPECT_FALSE( cpu.Flag.Z );
	EXPECT_TRUE( cpu.Flag.N );
}

// ---------- Zero Page -------------

TEST_F( M6502ShiftsTests, ROLZeroPageCanShiftABitOutOfTheCarryFlag )
{
	// given:
	using namespace m6502;
	cpu.Reset( 0xFF00, mem );
	cpu.Flag.C = true;
	cpu.Flag.Z = true;
	cpu.Flag.N = true;
	mem[0xFF00] = CPU::INS_ROL_ZP;
	mem[0xFF01] = 0x42;
	mem[0x0042] = 0;
	constexpr s32 EXPECTED_CYCLES = 5;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

	// then:
	EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
	EXPECT_EQ( mem[0x0042], 1 );
	EXPECT_FALSE( cpu.Flag.C );
	EXPECT_FALSE( cpu.Flag.Z );
	EXPECT_FALSE( cpu.Flag.N );
}

TEST_F( M6502ShiftsTests, ROLZeroPageCanShiftABitIntoTheCarryFlag )
{
	// given:
	using namespace m6502;
	cpu.Reset( 0xFF00, mem );
	cpu.Flag.C = false;
	cpu.Flag.Z = false;
	cpu.Flag.N = true;
	mem[0xFF00] = CPU::INS_ROL_ZP;
	mem[0xFF01] = 0x42;
	mem[0x0042] = 0b10000000;
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

TEST_F( M6502ShiftsTests, ROLZeroPageCanShiftZeroWithNoCarry )
{
	// given:
	using namespace m6502;
	cpu.Reset( 0xFF00, mem );
	cpu.Flag.C = false;
	cpu.Flag.Z = false;
	cpu.Flag.N = true;
	mem[0xFF00] = CPU::INS_ROL_ZP;
	mem[0xFF01] = 0x42;
	mem[0x0042] = 0;
	constexpr s32 EXPECTED_CYCLES = 5;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

	// then:
	EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
	EXPECT_EQ( mem[0x0042], 0 );
	EXPECT_FALSE( cpu.Flag.C );
	EXPECT_TRUE( cpu.Flag.Z );
	EXPECT_FALSE( cpu.Flag.N );
}

TEST_F( M6502ShiftsTests, ROLZeroPageCanShiftAValueThatResultInANegativeValue )
{
	// given:
	using namespace m6502;
	cpu.Reset( 0xFF00, mem );
	cpu.Flag.C = true;
	cpu.Flag.Z = false;
	cpu.Flag.N = false;
	mem[0xFF00] = CPU::INS_ROL_ZP;
	mem[0xFF01] = 0x42;
	mem[0x0042] = 0b01110011;
	constexpr s32 EXPECTED_CYCLES = 5;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

	// then:
	EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
	EXPECT_EQ( mem[0x0042], 0b11100111 );
	EXPECT_FALSE( cpu.Flag.C );
	EXPECT_FALSE( cpu.Flag.Z );
	EXPECT_TRUE( cpu.Flag.N );
}

// ------------- Zero Page X --------------

TEST_F( M6502ShiftsTests, ROLZeroPageXCanShiftABitOutOfTheCarryFlag )
{
	// given:
	using namespace m6502;
	cpu.Reset( 0xFF00, mem );
	cpu.Flag.C = true;
	cpu.Flag.Z = true;
	cpu.Flag.N = true;
	cpu.X = 0x10;
	mem[0xFF00] = CPU::INS_ROL_ZPX;
	mem[0xFF01] = 0x42;
	mem[0x0042 + 0x10] = 0;
	constexpr s32 EXPECTED_CYCLES = 6;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

	// then:
	EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
	EXPECT_EQ( mem[0x0042+0x10], 1 );
	EXPECT_FALSE( cpu.Flag.C );
	EXPECT_FALSE( cpu.Flag.Z );
	EXPECT_FALSE( cpu.Flag.N );
}

TEST_F( M6502ShiftsTests, ROLZeroPageXCanShiftABitIntoTheCarryFlag )
{
	// given:
	using namespace m6502;
	cpu.Reset( 0xFF00, mem );
	cpu.Flag.C = false;
	cpu.Flag.Z = false;
	cpu.Flag.N = true;
	cpu.X = 0x10;
	mem[0xFF00] = CPU::INS_ROL_ZPX;
	mem[0xFF01] = 0x42;
	mem[0x0042+0x10] = 0b10000000;
	constexpr s32 EXPECTED_CYCLES = 6;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

	// then:
	EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
	EXPECT_EQ( mem[0x0042 + 0x10], 0 );
	EXPECT_TRUE( cpu.Flag.C );
	EXPECT_TRUE( cpu.Flag.Z );
	EXPECT_FALSE( cpu.Flag.N );
}

TEST_F( M6502ShiftsTests, ROLZeroPageXCanShiftZeroWithNoCarry )
{
	// given:
	using namespace m6502;
	cpu.Reset( 0xFF00, mem );
	cpu.Flag.C = false;
	cpu.Flag.Z = false;
	cpu.Flag.N = true;
	cpu.X = 0x10;
	mem[0xFF00] = CPU::INS_ROL_ZPX;
	mem[0xFF01] = 0x42;
	mem[0x0042 + 0x10] = 0;
	constexpr s32 EXPECTED_CYCLES = 6;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

	// then:
	EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
	EXPECT_EQ( mem[0x0042 + 0x10], 0 );
	EXPECT_FALSE( cpu.Flag.C );
	EXPECT_TRUE( cpu.Flag.Z );
	EXPECT_FALSE( cpu.Flag.N );
}

TEST_F( M6502ShiftsTests, ROLZeroPageXCanShiftAValueThatResultInANegativeValue )
{
	// given:
	using namespace m6502;
	cpu.Reset( 0xFF00, mem );
	cpu.Flag.C = true;
	cpu.Flag.Z = false;
	cpu.Flag.N = false;
	cpu.X = 0x10;
	mem[0xFF00] = CPU::INS_ROL_ZPX;
	mem[0xFF01] = 0x42;
	mem[0x0042 + 0x10] = 0b01110011;
	constexpr s32 EXPECTED_CYCLES = 6;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

	// then:
	EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
	EXPECT_EQ( mem[0x0042 + 0x10], 0b11100111 );
	EXPECT_FALSE( cpu.Flag.C );
	EXPECT_FALSE( cpu.Flag.Z );
	EXPECT_TRUE( cpu.Flag.N );
}

// ------------- Absolute --------------

TEST_F( M6502ShiftsTests, ROLAbsoluteCanShiftABitOutOfTheCarryFlag )
{
	// given:
	using namespace m6502;
	cpu.Reset( 0xFF00, mem );
	cpu.Flag.C = true;
	cpu.Flag.Z = true;
	cpu.Flag.N = true;
	mem[0xFF00] = CPU::INS_ROL_ABS;
	mem[0xFF01] = 0x00;
	mem[0xFF02] = 0x80;
	mem[0x8000] = 0;
	constexpr s32 EXPECTED_CYCLES = 6;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

	// then:
	EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
	EXPECT_EQ( mem[0x8000], 1 );
	EXPECT_FALSE( cpu.Flag.C );
	EXPECT_FALSE( cpu.Flag.Z );
	EXPECT_FALSE( cpu.Flag.N );
}

TEST_F( M6502ShiftsTests, ROLAbsoluteCanShiftABitIntoTheCarryFlag )
{
	// given:
	using namespace m6502;
	cpu.Reset( 0xFF00, mem );
	cpu.Flag.C = false;
	cpu.Flag.Z = false;
	cpu.Flag.N = true;
	mem[0xFF00] = CPU::INS_ROL_ABS;
	mem[0xFF01] = 0x00;
	mem[0xFF02] = 0x80;
	mem[0x8000] = 0b10000000;
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

TEST_F( M6502ShiftsTests, ROLAbsoluteCanShiftZeroWithNoCarry )
{
	// given:
	using namespace m6502;
	cpu.Reset( 0xFF00, mem );
	cpu.Flag.C = false;
	cpu.Flag.Z = false;
	cpu.Flag.N = true;
	mem[0xFF00] = CPU::INS_ROL_ABS;
	mem[0xFF01] = 0x00;
	mem[0xFF02] = 0x80;
	mem[0x8000] = 0;
	constexpr s32 EXPECTED_CYCLES = 6;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

	// then:
	EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
	EXPECT_EQ( mem[0x8000], 0 );
	EXPECT_FALSE( cpu.Flag.C );
	EXPECT_TRUE( cpu.Flag.Z );
	EXPECT_FALSE( cpu.Flag.N );
}

TEST_F( M6502ShiftsTests, ROLAbsoluteCanShiftAValueThatResultInANegativeValue )
{
	// given:
	using namespace m6502;
	cpu.Reset( 0xFF00, mem );
	cpu.Flag.C = true;
	cpu.Flag.Z = false;
	cpu.Flag.N = false;
	mem[0xFF00] = CPU::INS_ROL_ABS;
	mem[0xFF01] = 0x00;
	mem[0xFF02] = 0x80;
	mem[0x8000] = 0b01110011;
	constexpr s32 EXPECTED_CYCLES = 6;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

	// then:
	EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
	EXPECT_EQ( mem[0x8000], 0b11100111 );
	EXPECT_FALSE( cpu.Flag.C );
	EXPECT_FALSE( cpu.Flag.Z );
	EXPECT_TRUE( cpu.Flag.N );
}

// ------------- Absolute X --------------

TEST_F( M6502ShiftsTests, ROLAbsoluteXCanShiftABitOutOfTheCarryFlag )
{
	// given:
	using namespace m6502;
	cpu.Reset( 0xFF00, mem );
	cpu.Flag.C = true;
	cpu.Flag.Z = true;
	cpu.Flag.N = true;
	cpu.X = 0x10;
	mem[0xFF00] = CPU::INS_ROL_ABSX;
	mem[0xFF01] = 0x00;
	mem[0xFF02] = 0x80;
	mem[0x8000+0x10] = 0;
	constexpr s32 EXPECTED_CYCLES = 7;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

	// then:
	EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
	EXPECT_EQ( mem[0x8000 + 0x10], 1 );
	EXPECT_FALSE( cpu.Flag.C );
	EXPECT_FALSE( cpu.Flag.Z );
	EXPECT_FALSE( cpu.Flag.N );
}

TEST_F( M6502ShiftsTests, ROLAbsoluteXCanShiftABitIntoTheCarryFlag )
{
	// given:
	using namespace m6502;
	cpu.Reset( 0xFF00, mem );
	cpu.Flag.C = false;
	cpu.Flag.Z = false;
	cpu.Flag.N = true;
	cpu.X = 0x10;
	mem[0xFF00] = CPU::INS_ROL_ABSX;
	mem[0xFF01] = 0x00;
	mem[0xFF02] = 0x80;
	mem[0x8000 + 0x10] = 0b10000000;
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

TEST_F( M6502ShiftsTests, ROLAbsoluteXCanShiftZeroWithNoCarry )
{
	// given:
	using namespace m6502;
	cpu.Reset( 0xFF00, mem );
	cpu.Flag.C = false;
	cpu.Flag.Z = false;
	cpu.Flag.N = true;
	cpu.X = 0x10;
	mem[0xFF00] = CPU::INS_ROL_ABSX;
	mem[0xFF01] = 0x00;
	mem[0xFF02] = 0x80;
	mem[0x8000 + 0x10] = 0;
	constexpr s32 EXPECTED_CYCLES = 7;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

	// then:
	EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
	EXPECT_EQ( mem[0x8000 + 0x10], 0 );
	EXPECT_FALSE( cpu.Flag.C );
	EXPECT_TRUE( cpu.Flag.Z );
	EXPECT_FALSE( cpu.Flag.N );
}

TEST_F( M6502ShiftsTests, ROLAbsoluteXCanShiftAValueThatResultInANegativeValue )
{
	// given:
	using namespace m6502;
	cpu.Reset( 0xFF00, mem );
	cpu.Flag.C = true;
	cpu.Flag.Z = false;
	cpu.Flag.N = false;
	cpu.X = 0x10;
	mem[0xFF00] = CPU::INS_ROL_ABSX;
	mem[0xFF01] = 0x00;
	mem[0xFF02] = 0x80;
	mem[0x8000 + 0x10] = 0b01110011;
	constexpr s32 EXPECTED_CYCLES = 7;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

	// then:
	EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
	EXPECT_EQ( mem[0x8000 + 0x10], 0b11100111 );
	EXPECT_FALSE( cpu.Flag.C );
	EXPECT_FALSE( cpu.Flag.Z );
	EXPECT_TRUE( cpu.Flag.N );
}

// --------------- ROR --------------------

TEST_F( M6502ShiftsTests, RORCanShiftTheCarryFlagIntoTheOperand )
{
	// given:
	using namespace m6502;
	cpu.Reset( 0xFF00, mem );
	cpu.Flag.C = true;
	cpu.Flag.Z = false;
	cpu.Flag.N = false;
	cpu.A = 0;
	mem[0xFF00] = CPU::INS_ROR;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

	// then:
	EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
	EXPECT_EQ( cpu.A, 0b10000000 );
	EXPECT_FALSE( cpu.Flag.C );
	EXPECT_FALSE( cpu.Flag.Z );
	EXPECT_TRUE( cpu.Flag.N );
}

TEST_F( M6502ShiftsTests, RORCanShiftAValueIntoTheCarryFlag )
{
	// given:
	using namespace m6502;
	cpu.Reset( 0xFF00, mem );
	cpu.Flag.C = false;
	cpu.Flag.Z = false;
	cpu.Flag.N = false;
	cpu.A = 1;
	mem[0xFF00] = CPU::INS_ROR;
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

TEST_F( M6502ShiftsTests, RORCanRotateANumber )
{
	// given:
	using namespace m6502;
	cpu.Reset( 0xFF00, mem );
	cpu.Flag.C = true;
	cpu.Flag.Z = true;
	cpu.Flag.N = false;
	cpu.A = 0b01101101;
	mem[0xFF00] = CPU::INS_ROR;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

	// then:
	EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
	EXPECT_EQ( cpu.A, 0b10110110 );
	EXPECT_TRUE( cpu.Flag.C );
	EXPECT_FALSE( cpu.Flag.Z );
	EXPECT_TRUE( cpu.Flag.N );
}

// ZeroPage

TEST_F( M6502ShiftsTests, RORZeroPageCanShiftTheCarryFlagIntoTheOperand )
{
	// given:
	using namespace m6502;
	cpu.Reset( 0xFF00, mem );
	cpu.Flag.C = true;
	cpu.Flag.Z = false;
	cpu.Flag.N = false;
	mem[0xFF00] = CPU::INS_ROR_ZP;
	mem[0xFF01] = 0x42;
	mem[0x0042] = 0;
	constexpr s32 EXPECTED_CYCLES = 5;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

	// then:
	EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
	EXPECT_EQ( mem[0x0042], 0b10000000 );
	EXPECT_FALSE( cpu.Flag.C );
	EXPECT_FALSE( cpu.Flag.Z );
	EXPECT_TRUE( cpu.Flag.N );
}

TEST_F( M6502ShiftsTests, RORZeroPageCanShiftAValueIntoTheCarryFlag )
{
	// given:
	using namespace m6502;
	cpu.Reset( 0xFF00, mem );
	cpu.Flag.C = false;
	cpu.Flag.Z = false;
	cpu.Flag.N = false;
	mem[0xFF00] = CPU::INS_ROR_ZP;
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

TEST_F( M6502ShiftsTests, RORZeroPageCanRotateANumber )
{
	// given:
	using namespace m6502;
	cpu.Reset( 0xFF00, mem );
	cpu.Flag.C = true;
	cpu.Flag.Z = true;
	cpu.Flag.N = false;
	mem[0xFF00] = CPU::INS_ROR_ZP;
	mem[0xFF01] = 0x42;
	mem[0x0042] = 0b01101101;
	constexpr s32 EXPECTED_CYCLES = 5;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

	// then:
	EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
	EXPECT_EQ( mem[0x0042], 0b10110110 );
	EXPECT_TRUE( cpu.Flag.C );
	EXPECT_FALSE( cpu.Flag.Z );
	EXPECT_TRUE( cpu.Flag.N );
}

// Zero Page X

TEST_F( M6502ShiftsTests, RORZeroXPageCanShiftTheCarryFlagIntoTheOperand )
{
	// given:
	using namespace m6502;
	cpu.Reset( 0xFF00, mem );
	cpu.Flag.C = true;
	cpu.Flag.Z = false;
	cpu.Flag.N = false;
	cpu.X = 0x10;
	mem[0xFF00] = CPU::INS_ROR_ZPX;
	mem[0xFF01] = 0x42;
	mem[0x0042 + 0x10] = 0;
	constexpr s32 EXPECTED_CYCLES = 6;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

	// then:
	EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
	EXPECT_EQ( mem[0x0042 + 0x10], 0b10000000 );
	EXPECT_FALSE( cpu.Flag.C );
	EXPECT_FALSE( cpu.Flag.Z );
	EXPECT_TRUE( cpu.Flag.N );
}

TEST_F( M6502ShiftsTests, RORZeroXPageCanShiftAValueIntoTheCarryFlag )
{
	// given:
	using namespace m6502;
	cpu.Reset( 0xFF00, mem );
	cpu.Flag.C = false;
	cpu.Flag.Z = false;
	cpu.Flag.N = false;
	cpu.X = 0x10;
	mem[0xFF00] = CPU::INS_ROR_ZPX;
	mem[0xFF01] = 0x42;
	mem[0x0042+0x10] = 1;
	constexpr s32 EXPECTED_CYCLES = 6;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

	// then:
	EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
	EXPECT_EQ( mem[0x0042 + 0x10], 0 );
	EXPECT_TRUE( cpu.Flag.C );
	EXPECT_TRUE( cpu.Flag.Z );
	EXPECT_FALSE( cpu.Flag.N );
}

TEST_F( M6502ShiftsTests, RORZeroXPageCanRotateANumber )
{
	// given:
	using namespace m6502;
	cpu.Reset( 0xFF00, mem );
	cpu.Flag.C = true;
	cpu.Flag.Z = true;
	cpu.Flag.N = false;
	cpu.X = 0x10;
	mem[0xFF00] = CPU::INS_ROR_ZPX;
	mem[0xFF01] = 0x42;
	mem[0x0042 + 0x10] = 0b01101101;
	constexpr s32 EXPECTED_CYCLES = 6;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

	// then:
	EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
	EXPECT_EQ( mem[0x0042 + 0x10], 0b10110110 );
	EXPECT_TRUE( cpu.Flag.C );
	EXPECT_FALSE( cpu.Flag.Z );
	EXPECT_TRUE( cpu.Flag.N );
}

// Absolute

TEST_F( M6502ShiftsTests, RORAbsolutePageCanShiftTheCarryFlagIntoTheOperand )
{
	// given:
	using namespace m6502;
	cpu.Reset( 0xFF00, mem );
	cpu.Flag.C = true;
	cpu.Flag.Z = false;
	cpu.Flag.N = false;
	mem[0xFF00] = CPU::INS_ROR_ABS;
	mem[0xFF01] = 0x00;
	mem[0xFF02] = 0x80;
	mem[0x8000] = 0;
	constexpr s32 EXPECTED_CYCLES = 6;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

	// then:
	EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
	EXPECT_EQ( mem[0x8000], 0b10000000 );
	EXPECT_FALSE( cpu.Flag.C );
	EXPECT_FALSE( cpu.Flag.Z );
	EXPECT_TRUE( cpu.Flag.N );
}

TEST_F( M6502ShiftsTests, RORAbsolutePageCanShiftAValueIntoTheCarryFlag )
{
	// given:
	using namespace m6502;
	cpu.Reset( 0xFF00, mem );
	cpu.Flag.C = false;
	cpu.Flag.Z = false;
	cpu.Flag.N = false;
	mem[0xFF00] = CPU::INS_ROR_ABS;
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

TEST_F( M6502ShiftsTests, RORAbsolutePageCanRotateANumber )
{
	// given:
	using namespace m6502;
	cpu.Reset( 0xFF00, mem );
	cpu.Flag.C = true;
	cpu.Flag.Z = true;
	cpu.Flag.N = false;
	mem[0xFF00] = CPU::INS_ROR_ABS;
	mem[0xFF01] = 0x00;
	mem[0xFF02] = 0x80;
	mem[0x8000] = 0b01101101;
	constexpr s32 EXPECTED_CYCLES = 6;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

	// then:
	EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
	EXPECT_EQ( mem[0x8000], 0b10110110 );
	EXPECT_TRUE( cpu.Flag.C );
	EXPECT_FALSE( cpu.Flag.Z );
	EXPECT_TRUE( cpu.Flag.N );
}

// Absolute X

TEST_F( M6502ShiftsTests, RORAbsoluteXPageCanShiftTheCarryFlagIntoTheOperand )
{
	// given:
	using namespace m6502;
	cpu.Reset( 0xFF00, mem );
	cpu.Flag.C = true;
	cpu.Flag.Z = false;
	cpu.Flag.N = false;
	cpu.X = 0x10;
	mem[0xFF00] = CPU::INS_ROR_ABSX;
	mem[0xFF01] = 0x00;
	mem[0xFF02] = 0x80;
	mem[0x8000 + 0x10] = 0;
	constexpr s32 EXPECTED_CYCLES = 7;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

	// then:
	EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
	EXPECT_EQ( mem[0x8000 + 0x10], 0b10000000 );
	EXPECT_FALSE( cpu.Flag.C );
	EXPECT_FALSE( cpu.Flag.Z );
	EXPECT_TRUE( cpu.Flag.N );
}

TEST_F( M6502ShiftsTests, RORAbsoluteXPageCanShiftAValueIntoTheCarryFlag )
{
	// given:
	using namespace m6502;
	cpu.Reset( 0xFF00, mem );
	cpu.Flag.C = false;
	cpu.Flag.Z = false;
	cpu.Flag.N = false;
	cpu.X = 0x10;
	mem[0xFF00] = CPU::INS_ROR_ABSX;
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

TEST_F( M6502ShiftsTests, RORAbsoluteXPageCanRotateANumber )
{
	// given:
	using namespace m6502;
	cpu.Reset( 0xFF00, mem );
	cpu.Flag.C = true;
	cpu.Flag.Z = true;
	cpu.Flag.N = false;
	cpu.X = 0x10;
	mem[0xFF00] = CPU::INS_ROR_ABSX;
	mem[0xFF01] = 0x00;
	mem[0xFF02] = 0x80;
	mem[0x8000 + 0x10] = 0b01101101;
	constexpr s32 EXPECTED_CYCLES = 7;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

	// then:
	EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
	EXPECT_EQ( mem[0x8000 + 0x10], 0b10110110 );
	EXPECT_TRUE( cpu.Flag.C );
	EXPECT_FALSE( cpu.Flag.Z );
	EXPECT_TRUE( cpu.Flag.N );
}