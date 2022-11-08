#include <verilated.h>
#include <cstdint>
#include "Vsram_top.h"

#define ADDRESS_WIDTH 4
#define BITS 8
#define DEPTH 8

#define tick() do{io_in->clk=1;sram->eval();}while(0)
#define tock() do{io_in->clk=0;sram->eval();}while(0)
#define ticktock() do{tick();tock();}while(0)

typedef struct {
	union {
		uint8_t input;
		struct {
			uint8_t clk : 1;
			uint8_t we  : 1;
			uint8_t oe  : 1;
			uint8_t commit : 1;
			uint8_t addr_data : 4;
		};
	};
} input_t;

int main(int argc, char* argv[])
{
	VerilatedContext* vcp = new VerilatedContext;
	vcp->commandArgs(argc, argv);
	Vsram_top* sram = new Vsram_top{vcp};

	int err = 0;
	input_t* io_in = (input_t*)&(sram->io_in);

	// Write Enable
	for (uint8_t i = 0; i < DEPTH; i++) {
		uint8_t val = 3*i;
		io_in->we = 1;
		io_in->addr_data = val & 0xf; // val.lo
		ticktock();
		io_in->addr_data = val >> 4; // val.hi
		ticktock();
		io_in->we = 0;
		io_in->commit = 1; // commit is we = 1 & oe = 1
		io_in->addr_data = i; // address
		ticktock();
		io_in->commit = 0;
	}
	io_in->we = 0;

	// Output Enable
	io_in->oe = 1;
	for (uint8_t i = 0; i < DEPTH; i++) {
		uint8_t val = 3*i;
		io_in->addr_data = i;
		tick();
		printf("data[%2hhu] = %2hhu\n", i, sram->io_out);
		if (sram->io_out != val) err = 1;
		tock();
	}
	io_in->oe = 0;

	// Testing Fast Zeroing (mem[0] already zero'ed)
	io_in->oe = 1;
	io_in->addr_data = 0;
	ticktock(); // now data_tmp loaded into internal register
	io_in->oe = 0;
	io_in->commit = 1;
	for (uint8_t i = 1; i < DEPTH; i++) {
		io_in->addr_data = i;
		ticktock(); // commit
	}
	io_in->commit = 0;
	// Checking values are zero'ed
	io_in->oe = 1;
	for (uint8_t i = 0; i < DEPTH; i++) {
		io_in->addr_data = i;
		tick();
		printf("data[%2hhu] = %2hhu\n", i, sram->io_out);
		if (sram->io_out != 0) err = 1;
		tock();
	}

	sram->final();
	delete sram;

	if (err) puts("ERROR!");

	return err;
}
