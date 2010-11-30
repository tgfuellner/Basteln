# Name: Makefile
# Author: <insert your name here>
# Copyright: <insert your copyright message here>
# License: <insert your license reference here>

# This is a prototype Makefile. Modify it according to your needs.
# You should at least check the settings for
# DEVICE ....... The AVR device you compile for
# CLOCK ........ Target AVR clock rate in Hertz
# OBJECTS ...... The object files created from your source files. This list is
#                usually the same as the list of source files with suffix ".o".
# PROGRAMMER ... Options to avrdude which define the hardware you use for
#                uploading to the AVR and the interface where this hardware
#                is connected.
# FUSES ........ Parameters for avrdude to flash the fuses appropriately.

DEVICE     = attiny2313
CLOCK      = 4000000
PROGRAMMER = -c avrispv2 -P usb 
# PROGRAMMER = -c usbtiny 
OBJECTS    = matrix.o
# Fuses: internal oscilator 4 MHz, no prescale, BOD 1.8V
FUSES      =  -U hfuse:w:0xDD:m -U lfuse:w:0xE2:m

# Tune the lines below only if you know what you are doing:

AVRDUDE = avrdude $(PROGRAMMER) -p $(DEVICE)
OBJDUMP = avr-objdump
#COMPILE = avr-gcc -Wall -Os -DF_CPU=$(CLOCK) -mmcu=$(DEVICE)
COMPILE = avr-gcc -Wall -Os -fno-inline-small-functions -DF_CPU=$(CLOCK) -mmcu=$(DEVICE)

# symbolic targets:
all:	matrix.hex matrix.lss

.c.o:
	$(COMPILE) -c $< -o $@

.S.o:
	$(COMPILE) -x assembler-with-cpp -c $< -o $@
# "-x assembler-with-cpp" should not be necessary since this is the default
# file type for the .S (with capital S) extension. However, upper case
# characters are not always preserved on Windows. To ensure WinAVR
# compatibility define the file type manually.

.c.s:
	$(COMPILE) -S $< -o $@

flash:	all
	$(AVRDUDE) -U flash:w:matrix.hex:i

fuse:
	$(AVRDUDE) $(FUSES)

# Xcode uses the Makefile targets "", "clean" and "install"
install: flash fuse

clean:
	rm -f matrix.hex matrix.elf $(OBJECTS)

# file targets:
matrix.elf: $(OBJECTS)
	$(COMPILE) -o matrix.elf $(OBJECTS)

matrix.hex: matrix.elf
	rm -f matrix.hex
	avr-objcopy -j .text -j .data -O ihex matrix.elf matrix.hex
	./checksize matrix.elf 2048 128
# If you have an EEPROM section, you must also create a hex file for the
# EEPROM and add it to the "flash" target.

# Targets for code debugging and analysis:
disasm:	matrix.elf
	avr-objdump -d matrix.elf

%.lss: %.elf
	@echo
	$(OBJDUMP) -h -S $< > $@

cpp:
	$(COMPILE) -E matrix.c
