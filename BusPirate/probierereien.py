#!/usr/bin/env python
# encoding: utf-8

import os, sys
import serial

from time import sleep

from pyBusPirateLite.UART import *
from pyBusPirateLite.BitBang import *

#change this path
BUS_PIRATE_DEV = "/dev/ttyUSB-buspirate"

bp = UART(BUS_PIRATE_DEV,115200)

print "Entering binmode: ",
if bp.BBmode():
	print "OK."
else:
	print "failed."
	sys.exit()
	
bp.raw_set_pins(BBIOPins.POWER|BBIOPins.PULLUP)


def on():
    bp.raw_cfg_pins(PinCfg.CS)
def off():
    bp.raw_cfg_pins(0)

tWait = 0.020
while 1:
    on()
    sleep(tWait)
    off()
    sleep(tWait)
