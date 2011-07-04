#!/usr/bin/python

# Feeds a gcode File to the SphereBot.
# 
# Write a line to the serial device and wait for an "ok" response.

# prerequisite:  http://pyserial.sourceforge.net
#   Installation on Ubuntu: sudo aptitude install python-serial




# Configure:
BAUDRATE = 57600
DEVICE = "/dev/ttyUSB1"

# End configuration



import sys
import serial


fileToFeed = sys.argv[1]
gcode = open(fileToFeed, "r")
sphereBot = serial.Serial(DEVICE, BAUDRATE, timeout=30)

line = gcode.readline()
while line:
    print line
    sphereBot.write(line)

    response = sphereBot.readline()
    while response[:3] != "ok:":
        print "  ", response
        response = sphereBot.readline()

    line = gcode.readline()

gcode.close()
sphereBot.close()

