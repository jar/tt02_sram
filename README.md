![](../../workflows/gds/badge.svg) ![](../../workflows/docs/badge.svg) ![](../../workflows/test/badge.svg)

# Tiny Tapeout SRAM

Tiny Tapeout #1 included a 4-bit CPU, a 6-bit CORDIC, an FPGA, and other tiny
circuits.  It seemed like an SRAM was missing, so this is my attempt to remedy
that in Tiny Tapeout #2.

## Circuit Logic

The circuit in this repository has a 4-bit address/data input and 8-bit output.
Due to die area limitations, only 8 bytes are addressable.  There are two
modes: Write and Read.  The diagram below demonstrates how to use the write and
read modes.  Because there are only eight pins for input and four of the pins
are used for signalling, the remaining four input pins define the data and
address over three cycles.  The high bit of the address is ignored.

```
                                                         ╭╭
           ┌───┐   ┌───┐   ┌───┐   ┌───┐   ┌───┐   ┌───┐ ││ ┌───┐   ┌───┐
       clk     └───┘   └───┘   └───┘   └───┘   └───┘   └─││─┘   └───┘   └─
                     _______                             ││
      reset ________╱       ╲____________________________││_______________
                             _______________________     ││
        we  ________________╱                       ╲____││_______________
                                                         ││   _______
        oe  _____________________________________________││__╱       ╲____
                             _______ _______ _______     ││   _______
 input[7:4] ________________╱__lo___╳__hi___╳_addr__╲____││__╱_addr__╲____
                                                         ││   _______
output[7:0] _____________________________________________││__╱_data__╲____
                                                         ╯╯
```

## What is Tiny Tapeout?

TinyTapeout is an educational project that aims to make it easier and cheaper
than ever to get your digital designs manufactured on a real chip!

Go to https://tinytapeout.com for instructions!
