#include <verilated.h>
#include <cstdint>
#include "Vsram_top.h"

#define ADDRESS_WIDTH 4
#define BITS 8
#define DEPTH (1 << ADDRESS_WIDTH)

#define tick() do{io_in->clk=1;sram->eval();}while(0)
#define tock() do{io_in->clk=0;sram->eval();}while(0)
#define ticktock() do{tick();tock();}while(0)

typedef struct {
	union {
		uint8_t input;
		struct {
			uint8_t clk : 1;
			uint8_t rst : 1;
			uint8_t we  : 1;
			uint8_t oe  : 1;
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

	// Reset counter/data_tmp
	io_in->rst = 1;
	io_in->we = 0;
	io_in->oe = 0;
	io_in->addr_data = 0;
	ticktock();
	io_in->rst = 0;

	// Write Enable
	io_in->we = 1;
	for (uint8_t i = 0; i < DEPTH; i++) {
		uint8_t val = 3*i;
		io_in->addr_data = val & 0xf; // val.lo
		ticktock();
		io_in->addr_data = val >> 4; // val.hi
		ticktock();
		io_in->addr_data = i; // address
		ticktock();
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

#if 0
	// Enable Streaming Output
	io_in->oe = 1;
	io_in->we = 1;
	for (uint8_t i = 0; i < 2*DEPTH; i++) {
		tick();
		uint8_t val = 3*(i % DEPTH);
		printf("stream %2hhu: %2hhu\n", i, sram->io_out);
		if (sram->io_out != val) err = 1;
		tock();
	}
#endif

	sram->final();
	delete sram;

	if (err) puts("ERROR!");

	return err;
}
