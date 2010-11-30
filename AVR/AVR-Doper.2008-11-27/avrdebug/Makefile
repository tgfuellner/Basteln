# Name: Makefile
# Project: AVR Programmer
# Author: Christian Starkjohann
# Creation Date: 2006-07-10
# Tabsize: 4
# Copyright: (c) 2006 by OBJECTIVE DEVELOPMENT Software GmbH
# License: GNU GPL v2 (see License.txt) or proprietary (CommercialLicense.txt)
# This Revision: $Id: Makefile 406 2007-11-20 19:38:35Z cs $


CC              = gcc
LIBUSB_CONFIG   = libusb-config
# Make sure that libusb-config is in the search path or specify a full path.
# On Windows, there is no libusb-config and you must configure the options
# below manually. See examples.
USBFLAGS        = `$(LIBUSB_CONFIG) --cflags`
# On Windows replace `$(LIBUSB_CONFIG) --cflags` with appropriate "-I..."
# option to ensure that usb.h is found
USBLIBS         = `$(LIBUSB_CONFIG) --libs`
# You may need "-framework CoreFoundation" on Mac OS X and Darwin.
#USBLIBS        = -L/usr/local/libusb/lib/gcc -lusb
# On Windows use somthing similar to the line above.
EXE_SUFFIX =

CFLAGS          = $(USBFLAGS) -O -Wall
LIBS            = $(USBLIBS)

PROGRAM = avrdebug$(EXE_SUFFIX)


all: $(PROGRAM)

.c.o:
	$(CC) $(CFLAGS) -c $<

$(PROGRAM): avrdebug.o
	$(CC) -o $(PROGRAM) avrdebug.o $(LIBS)

strip: $(PROGRAM)
	strip $(PROGRAM)

clean:
	rm -f *.o
	rm -f avrdebug avrdebug.exe
