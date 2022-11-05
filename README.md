![](../../workflows/gds/badge.svg) ![](../../workflows/docs/badge.svg) ![](../../workflows/test/badge.svg)

# Tiny Tapeout SRAM

Tiny Tapeout #1 included a 4-bit CPU, a 6-bit CORDIC, an FPGA, and other tiny
circuits.  It seemed like an SRAM was missing, so this is my attempt to remedy
that.  There is a 4-bit address and 8-bit output, which implies up to 16
addressable bytes.

## Circuit Logic

There are three modes: Write, Read, and Stream.  The diagram below demonstrates
how to use the write and read modes.  Because there are only eight pins for
input and four of the pins are used for signalling, the remaining four input
pins define the data and address over three cycles.  Reads

```
┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
┃                                                         ╭╭               ┃
┃           ┌───┐   ┌───┐   ┌───┐   ┌───┐   ┌───┐   ┌───┐ ││ ┌───┐   ┌───┐ ┃
┃       clk     └───┘   └───┘   └───┘   └───┘   └───┘   └─││─┘   └───┘   └─┃
┃                     _______                             ││               ┃
┃      reset ________╱       ╲____________________________││_______________┃
┃                             _______________________     ││               ┃
┃        we  ________________╱                       ╲____││_______________┃
┃                                                         ││   _______     ┃
┃        oe  _____________________________________________││__╱       ╲____┃
┃                             _______ _______ _______     ││   _______     ┃
┃ input[7:4] ________________╱__lo___╳__hi___╳_addr__╲____││__╱_addr__╲____┃
┃                                                         ││   _______     ┃
┃output[7:0] _____________________________________________││__╱_data__╲____┃
┃                                                         ╯╯               ┃
┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
```

There is also a streaming mode which can be used as a small digital signal
generator. When both Output Enable (`oe`) and Write Enable (`we`) are high, the
output cycles through the values in memory at each clock cycle, but without
specifying the input address.  This seemed like it could be useful for a
programmable message on the 7-segment display used by other circuits.

```
┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
┃                                                                 ┃
┃           ┌───┐   ┌───┐   ┌───┐   ┌───┐   ┌───┐   ┌───┐   ┌───┐ ┃
┃       clk     └───┘   └───┘   └───┘   └───┘   └───┘   └───┘   └─┃
┃                                                                 ┃
┃      reset _____________________________________________________┃
┃                     _______________________________             ┃
┃        we  ________╱                               ╲____________┃
┃                     _______________________________             ┃
┃        oe  ________╱                               ╲____________┃
┃                                                                 ┃
┃ input[7:4] _____________________________________________________┃
┃                     _______ _______ _______ _______             ┃
┃output[7:0] ________╱_data__╳_data__╳_data__╳_data__╲____________┃
┃                                                                 ┃
┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
```

## What is Tiny Tapeout?

TinyTapeout is an educational project that aims to make it easier and cheaper
than ever to get your digital designs manufactured on a real chip!

Go to https://tinytapeout.com for instructions!
