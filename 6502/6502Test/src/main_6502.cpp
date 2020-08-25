#include "main_6502.h"

int main()
{
	Mem mem;
	CPU cpu;
	cpu.Reset( mem );
	// start - inline a little program
	mem[0xFFFC] = CPU::INS_JSR;
	mem[0xFFFD] = 0x42;
	mem[0xFFFE] = 0x42;
	mem[0x4242] = CPU::INS_LDA_IM;
	mem[0x4243] = 0x84;
	// end - inline a little program
	cpu.Execute( 9, mem );
	return 0;
}