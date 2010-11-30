# Name: Makefile
# Author: Thomas Gfuellner thomas.gfuellner@gmx.de
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
PROGRAMMER = -c usbasp -P usb 
#PROGRAMMER = -c avrispv2 -P usb 
# PROGRAMMER = -c usbtiny 
# Fuses: internal oscilator 4 MHz, no prescale, BOD 1.8V
FUSES      =  -U hfuse:w:0xDD:m -U lfuse:w:0xE2:m

# Reset disabled
#FUSES      =  -U hfuse:w:0xDC:m -U lfuse:w:0xE2:m

# Tune the lines below only if you know what you are doing:

AVRDUDE = avrdude $(PROGRAMMER) -p $(DEVICE)
OBJDUMP = avr-objdump
#COMPILE = avr-gcc -Wall -Os -DF_CPU=$(CLOCK) -mmcu=$(DEVICE)
COMPILE = avr-gcc -Wall -Os -fno-inline-small-functions -DF_CPU=$(CLOCK) -mmcu=$(DEVICE)

# symbolic targets:
all:	128pixel.hex 128pixel.lss


128pixel.o: 128pixel.c
	$(COMPILE) -c 128pixel.c -o 128pixel.o

.S.o:
	$(COMPILE) -x assembler-with-cpp -c $< -o $@
# "-x assembler-with-cpp" should not be necessary since this is the default
# file type for the .S (with capital S) extension. However, upper case
# characters are not always preserved on Windows. To ensure WinAVR
# compatibility define the file type manually.

.c.s:
	$(COMPILE) -S $< -o $@

flash:	128pixel.hex
	$(AVRDUDE) -U flash:w:128pixel.hex:i

fuse:
	$(AVRDUDE) $(FUSES)

# Xcode uses the Makefile targets "", "clean" and "install"
install: flash fuse

clean:
	rm -f *.hex *.elf *.lss *.o

# file targets:
128pixel.elf: 128pixel.o
	$(COMPILE) -o 128pixel.elf 128pixel.o

128pixel.hex: 128pixel.elf
	rm -f 128pixel.hex
	avr-objcopy -j .text -j .data -O ihex 128pixel.elf 128pixel.hex
	./checksize 128pixel.elf 2048 128

# If you have an EEPROM section, you must also create a hex file for the
# EEPROM and add it to the "flash" target.

# Targets for code debugging and analysis:
disasm:	128pixel.elf
	avr-objdump -d 128pixel.elf

%.lss: %.elf
	@echo
	$(OBJDUMP) -h -S $< > $@

cpp:
	$(COMPILE) -E 128pixel.c
