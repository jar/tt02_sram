ifeq ($(VERILATOR_ROOT),)
VERILATOR = verilator
else
export VERILATOR_ROOT
VERILATOR = $(VERILATOR_ROOT)/bin/verilator
endif

all:
	$(VERILATOR) -cc -CFLAGS "-O3 -Wall" --exe --build -j sram_top.v main.cpp

test: all
	./obj_dir/Vsram_top

.PHONY: clean distclean

clean:
	-rm -rf obj_dir

distclean: clean
