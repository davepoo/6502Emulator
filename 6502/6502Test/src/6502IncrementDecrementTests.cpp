#include <gtest/gtest.h>
#include "m6502.h"

class M6502IncrementDecrementTests : public testing::Test
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

	void ExpectUnaffectedFlags( m6502::CPU CPUBefore )
	{
		EXPECT_EQ( CPUBefore.Flag.C, cpu.Flag.C );
		EXPECT_EQ( CPUBefore.Flag.I, cpu.Flag.I );
		EXPECT_EQ( CPUBefore.Flag.D, cpu.Flag.D );
		EXPECT_EQ( CPUBefore.Flag.B, cpu.Flag.B );
		EXPECT_EQ( CPUBefore.Flag.V, cpu.Flag.V );
	}
};

TEST_F( M6502IncrementDecrementTests, INXCanIncrementAZeroValue )
{
	// given:
	using namespace m6502;
	cpu.Reset( 0xFF00, mem );
	cpu.X = 0x0;
	cpu.Flag.Z = true;
	cpu.Flag.N = true;
	mem[0xFF00] = CPU::INS_INX;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

	// then:
	EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
	EXPECT_EQ( cpu.X, 0x1 );
	EXPECT_FALSE( cpu.Flag.Z );
	EXPECT_FALSE( cpu.Flag.N );
	ExpectUnaffectedFlags( CPUCopy );
}

TEST_F( M6502IncrementDecrementTests, INXCanIncrement255 )
{
	// given:
	using namespace m6502;
	cpu.Reset( 0xFF00, mem );
	cpu.X = 0xFF;
	cpu.Flag.Z = false;
	cpu.Flag.N = true;
	mem[0xFF00] = CPU::INS_INX;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

	// then:
	EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
	EXPECT_EQ( cpu.X, 0x0 );	//NOTE: does this instruction actually wrap?
	EXPECT_TRUE( cpu.Flag.Z );
	EXPECT_FALSE( cpu.Flag.N );
	ExpectUnaffectedFlags( CPUCopy );
}

TEST_F( M6502IncrementDecrementTests, INXCanIncrementANegativeValue )
{
	// given:
	using namespace m6502;
	cpu.Reset( 0xFF00, mem );
	cpu.X = 0b10001000;
	cpu.Flag.Z = true;
	cpu.Flag.N = false;
	mem[0xFF00] = CPU::INS_INX;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

	// then:
	EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
	EXPECT_EQ( cpu.X, 0b10001001 );
	EXPECT_FALSE( cpu.Flag.Z );
	EXPECT_TRUE( cpu.Flag.N );
	ExpectUnaffectedFlags( CPUCopy );
}


TEST_F( M6502IncrementDecrementTests, INYCanIncrementAZeroValue )
{
	// given:
	using namespace m6502;
	cpu.Reset( 0xFF00, mem );
	cpu.Y = 0x0;
	cpu.Flag.Z = true;
	cpu.Flag.N = true;
	mem[0xFF00] = CPU::INS_INY;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

	// then:
	EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
	EXPECT_EQ( cpu.Y, 0x1 );
	EXPECT_FALSE( cpu.Flag.Z );
	EXPECT_FALSE( cpu.Flag.N );
	ExpectUnaffectedFlags( CPUCopy );
}

TEST_F( M6502IncrementDecrementTests, INYCanIncrement255 )
{
	// given:
	using namespace m6502;
	cpu.Reset( 0xFF00, mem );
	cpu.Y = 0xFF;
	cpu.Flag.Z = false;
	cpu.Flag.N = true;
	mem[0xFF00] = CPU::INS_INY;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

	// then:
	EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
	EXPECT_EQ( cpu.Y, 0x0 );	//NOTE: does this instruction actually wrap?
	EXPECT_TRUE( cpu.Flag.Z );
	EXPECT_FALSE( cpu.Flag.N );
	ExpectUnaffectedFlags( CPUCopy );
}

TEST_F( M6502IncrementDecrementTests, INYCanIncrementANegativeValue )
{
	// given:
	using namespace m6502;
	cpu.Reset( 0xFF00, mem );
	cpu.Y = 0b10001000;
	cpu.Flag.Z = true;
	cpu.Flag.N = false;
	mem[0xFF00] = CPU::INS_INY;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

	// then:
	EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
	EXPECT_EQ( cpu.Y, 0b10001001 );
	EXPECT_FALSE( cpu.Flag.Z );
	EXPECT_TRUE( cpu.Flag.N );
	ExpectUnaffectedFlags( CPUCopy );
}


TEST_F( M6502IncrementDecrementTests, DEYCanDecementAZeroValue )
{
	// given:
	using namespace m6502;
	cpu.Reset( 0xFF00, mem );
	cpu.Y = 0x0;
	cpu.Flag.Z = true;
	cpu.Flag.N = false;
	mem[0xFF00] = CPU::INS_DEY;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

	// then:
	EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
	EXPECT_EQ( cpu.Y, 0xFF );	//NOTE: Does this wrap?
	EXPECT_FALSE( cpu.Flag.Z );
	EXPECT_TRUE( cpu.Flag.N );
	ExpectUnaffectedFlags( CPUCopy );
}

TEST_F( M6502IncrementDecrementTests, DEYCanDecrement255 )
{
	// given:
	using namespace m6502;
	cpu.Reset( 0xFF00, mem );
	cpu.Y = 0xFF;
	cpu.Flag.Z = true;
	cpu.Flag.N = false;
	mem[0xFF00] = CPU::INS_DEY;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

	// then:
	EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
	EXPECT_EQ( cpu.Y, 0xFE );
	EXPECT_FALSE( cpu.Flag.Z );
	EXPECT_TRUE( cpu.Flag.N );
	ExpectUnaffectedFlags( CPUCopy );
}

TEST_F( M6502IncrementDecrementTests, DEYCanDecrementANegativeValue )
{
	// given:
	using namespace m6502;
	cpu.Reset( 0xFF00, mem );
	cpu.Y = 0b10001001;
	cpu.Flag.Z = true;
	cpu.Flag.N = false;
	mem[0xFF00] = CPU::INS_DEY;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

	// then:
	EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
	EXPECT_EQ( cpu.Y, 0b10001000 );
	EXPECT_FALSE( cpu.Flag.Z );
	EXPECT_TRUE( cpu.Flag.N );
	ExpectUnaffectedFlags( CPUCopy );
}

TEST_F( M6502IncrementDecrementTests, DEXCanDecementAZeroValue )
{
	// given:
	using namespace m6502;
	cpu.Reset( 0xFF00, mem );
	cpu.X = 0x0;
	cpu.Flag.Z = true;
	cpu.Flag.N = false;
	mem[0xFF00] = CPU::INS_DEX;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

	// then:
	EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
	EXPECT_EQ( cpu.X, 0xFF );	//NOTE: Does this wrap?
	EXPECT_FALSE( cpu.Flag.Z );
	EXPECT_TRUE( cpu.Flag.N );
	ExpectUnaffectedFlags( CPUCopy );
}

TEST_F( M6502IncrementDecrementTests, DEXCanDecrement255 )
{
	// given:
	using namespace m6502;
	cpu.Reset( 0xFF00, mem );
	cpu.X = 0xFF;
	cpu.Flag.Z = true;
	cpu.Flag.N = false;
	mem[0xFF00] = CPU::INS_DEX;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

	// then:
	EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
	EXPECT_EQ( cpu.X, 0xFE );
	EXPECT_FALSE( cpu.Flag.Z );
	EXPECT_TRUE( cpu.Flag.N );
	ExpectUnaffectedFlags( CPUCopy );
}

TEST_F( M6502IncrementDecrementTests, DEXCanDecrementANegativeValue )
{
	// given:
	using namespace m6502;
	cpu.Reset( 0xFF00, mem );
	cpu.X = 0b10001001;
	cpu.Flag.Z = true;
	cpu.Flag.N = false;
	mem[0xFF00] = CPU::INS_DEX;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

	// then:
	EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
	EXPECT_EQ( cpu.X, 0b10001000 );
	EXPECT_FALSE( cpu.Flag.Z );
	EXPECT_TRUE( cpu.Flag.N );
	ExpectUnaffectedFlags( CPUCopy );
}

TEST_F( M6502IncrementDecrementTests, DECCanDecrementAValueInTheZeroPage )
{
	// given:
	using namespace m6502;
	cpu.Reset( 0xFF00, mem );
	cpu.Flag.Z = true;
	cpu.Flag.N = true;
	mem[0xFF00] = CPU::INS_DEC_ZP;
	mem[0xFF01] = 0x42;
	mem[0x0042] = 0x57;
	constexpr s32 EXPECTED_CYCLES = 5;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

	// then:
	EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
	EXPECT_EQ( mem[0x0042], 0x56 );
	EXPECT_FALSE( cpu.Flag.Z );
	EXPECT_FALSE( cpu.Flag.N );
	ExpectUnaffectedFlags( CPUCopy );
}

TEST_F( M6502IncrementDecrementTests, DECCanDecrementAValueInTheZeroPageX )
{
	// given:
	using namespace m6502;
	cpu.Reset( 0xFF00, mem );
	cpu.Flag.Z = true;
	cpu.Flag.N = true;
	cpu.X = 0x10;
	mem[0xFF00] = CPU::INS_DEC_ZPX;
	mem[0xFF01] = 0x42;
	mem[0x0042 + 0x10] = 0x57;
	constexpr s32 EXPECTED_CYCLES = 6;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

	// then:
	EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
	EXPECT_EQ( mem[0x0042 + 0x10], 0x56 );
	EXPECT_FALSE( cpu.Flag.Z );
	EXPECT_FALSE( cpu.Flag.N );
	ExpectUnaffectedFlags( CPUCopy );
}

TEST_F( M6502IncrementDecrementTests, DECCanDecrementAValueAbsolute )
{
	// given:
	using namespace m6502;
	cpu.Reset( 0xFF00, mem );
	cpu.Flag.Z = true;
	cpu.Flag.N = true;
	mem[0xFF00] = CPU::INS_DEC_ABS;
	mem[0xFF01] = 0x00;
	mem[0xFF02] = 0x80;
	mem[0x8000] = 0x57;
	constexpr s32 EXPECTED_CYCLES = 6;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

	// then:
	EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
	EXPECT_EQ( mem[0x8000], 0x56 );
	EXPECT_FALSE( cpu.Flag.Z );
	EXPECT_FALSE( cpu.Flag.N );
	ExpectUnaffectedFlags( CPUCopy );
}

TEST_F( M6502IncrementDecrementTests, DECCanDecrementAValueAbsoluteX )
{
	// given:
	using namespace m6502;
	cpu.Reset( 0xFF00, mem );
	cpu.Flag.Z = true;
	cpu.Flag.N = true;
	cpu.X = 0x10;
	mem[0xFF00] = CPU::INS_DEC_ABSX;
	mem[0xFF01] = 0x00;
	mem[0xFF02] = 0x80;
	mem[0x8000 + 0x10] = 0x57;
	constexpr s32 EXPECTED_CYCLES = 7;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

	// then:
	EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
	EXPECT_EQ( mem[0x8000 + 0x10], 0x56 );
	EXPECT_FALSE( cpu.Flag.Z );
	EXPECT_FALSE( cpu.Flag.N );
	ExpectUnaffectedFlags( CPUCopy );
}

TEST_F( M6502IncrementDecrementTests, INCCanIncrementAValueInTheZeroPage )
{
	// given:
	using namespace m6502;
	cpu.Reset( 0xFF00, mem );
	cpu.Flag.Z = true;
	cpu.Flag.N = true;
	mem[0xFF00] = CPU::INS_INC_ZP;
	mem[0xFF01] = 0x42;
	mem[0x0042] = 0x57;
	constexpr s32 EXPECTED_CYCLES = 5;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

	// then:
	EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
	EXPECT_EQ( mem[0x0042], 0x58 );
	EXPECT_FALSE( cpu.Flag.Z );
	EXPECT_FALSE( cpu.Flag.N );
	ExpectUnaffectedFlags( CPUCopy );
}

TEST_F( M6502IncrementDecrementTests, INCCanIncrementAValueInTheZeroPageX )
{
	// given:
	using namespace m6502;
	cpu.Reset( 0xFF00, mem );
	cpu.Flag.Z = true;
	cpu.Flag.N = true;
	cpu.X = 0x10;
	mem[0xFF00] = CPU::INS_INC_ZPX;
	mem[0xFF01] = 0x42;
	mem[0x0042 + 0x10] = 0x57;
	constexpr s32 EXPECTED_CYCLES = 6;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

	// then:
	EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
	EXPECT_EQ( mem[0x0042 + 0x10], 0x58 );
	EXPECT_FALSE( cpu.Flag.Z );
	EXPECT_FALSE( cpu.Flag.N );
	ExpectUnaffectedFlags( CPUCopy );
}

TEST_F( M6502IncrementDecrementTests, INCCanIncrementAValueAbsolute )
{
	// given:
	using namespace m6502;
	cpu.Reset( 0xFF00, mem );
	cpu.Flag.Z = true;
	cpu.Flag.N = true;
	mem[0xFF00] = CPU::INS_INC_ABS;
	mem[0xFF01] = 0x00;
	mem[0xFF02] = 0x80;
	mem[0x8000] = 0x57;
	constexpr s32 EXPECTED_CYCLES = 6;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

	// then:
	EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
	EXPECT_EQ( mem[0x8000], 0x58 );
	EXPECT_FALSE( cpu.Flag.Z );
	EXPECT_FALSE( cpu.Flag.N );
	ExpectUnaffectedFlags( CPUCopy );
}

TEST_F( M6502IncrementDecrementTests, INCCanIncrementAValueAbsoluteX )
{
	// given:
	using namespace m6502;
	cpu.Reset( 0xFF00, mem );
	cpu.Flag.Z = true;
	cpu.Flag.N = true;
	cpu.X = 0x10;
	mem[0xFF00] = CPU::INS_INC_ABSX;
	mem[0xFF01] = 0x00;
	mem[0xFF02] = 0x80;
	mem[0x8000 + 0x10] = 0x57;
	constexpr s32 EXPECTED_CYCLES = 7;
	CPU CPUCopy = cpu;

	// when:
	const s32 ActualCycles = cpu.Execute( EXPECTED_CYCLES, mem );

	// then:
	EXPECT_EQ( ActualCycles, EXPECTED_CYCLES );
	EXPECT_EQ( mem[0x8000 + 0x10], 0x58 );
	EXPECT_FALSE( cpu.Flag.Z );
	EXPECT_FALSE( cpu.Flag.N );
	ExpectUnaffectedFlags( CPUCopy );
}

TEST_F( M6502IncrementDecrementTests, TestLoadAProgramThatCanIncMemory )
{
	// given:
	using namespace m6502;

	// when:
	/*
	* = $1000

	lda #00
	sta $42

	start
	inc $42
	ldx $42
	inx
	jmp start
	*/
	Byte TestPrg[] = 
		{ 0x0,0x10,0xA9,0x00,0x85,0x42,0xE6,0x42,
		0xA6,0x42,0xE8,0x4C,0x04,0x10 };

	Word StartAddress = cpu.LoadPrg( TestPrg, sizeof(TestPrg), mem );
	cpu.PC = StartAddress;

	//then:
	for ( m6502::s32 Clock = 1000; Clock > 0; )
	{
		Clock -= cpu.Execute( 1, mem );
	}
}