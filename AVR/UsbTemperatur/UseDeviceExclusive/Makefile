# Name: Makefile
# Project: Templogger command line tool
# Author: Thomas Gfuellner
# Creation Date: 2010-08-15
# Tabsize: 4
# License: GNU GPL v2 (see License.txt) or proprietary (CommercialLicense.txt)



CC		= gcc
CFLAGS	= -O -Wall
LIBS	= -L/usr/lib -lhid

PROGRAM = templogger


all: $(PROGRAM)

.c.o:
	$(CC) $(CFLAGS) -c $<

$(PROGRAM): templogger.o
	$(CC) -o $(PROGRAM) templogger.o $(LIBS)

strip: $(PROGRAM)
	strip $(PROGRAM)

clean:
	rm -f *.o $(PROGRAM)
