#!/usr/bin/python

# Feeds a gcode File to the SphereBot.
# 
# Write a line to the serial device and wait for an "ok" response.

# prerequisite:  http://pyserial.sourceforge.net
#   Installation on Ubuntu: sudo aptitude install python-serial




# Configure:
BAUDRATE = 57600
DEVICE = "/dev/tty.PL2303-00001004"
DEVICE = "/dev/tty.PL2303-00004006"
DEVICE = "/dev/ttyUSB1"

# End configuration



import sys
import serial
import re
from optparse import OptionParser

def y_displacement(x):
    # look into file egg-displace.dat for documentation
    return (0.00795338*x*x + 0.0734545*x + 0.15711)

lastX = 0.0

def correctDisplacement(lineIn):
    # extract x and y
    # calculate new y
    # return line with alter y

    global lastX
    foundY = False

    line = lineIn.upper()
    words = pattern.findall(line)
    for word in words:
        if word[0] == 'X':
            lastX = eval(word[1:])

        if word[0] == 'Y':
            y = eval(word[1:])
            foundY=True

    if foundY:
        y = y + y_displacement(lastX)
    else:
        return lineIn

    lineOut=""
    for word in words:
        if word[0] == 'Y':
            lineOut = lineOut + "Y{0}".format(y)
        else:
            lineOut = lineOut + word

    return lineOut

def penChange(lineIn):
    # Test Line for a Pen change request (M1)
    # If true, wait for user input

    if penChangePattern.match(lineIn):
        raw_input('Change pen ... press <Return> when finished ')
    
def waitForOkResponse(sphereBotInFile):
    response = sphereBotInFile.readline()

    while response[:3] != "ok:":
        print "  ", response,
        response = sphereBotInFile.readline()


penIsUp=None
def penUpAcceleration(line):
    global penIsUp

    if penUpIndication.search(line):
        penIsUp=True
    if penDownIndication.search(line):
        penIsUp=False

    if options.liftOffSpeed and penIsUp and line[0:2] == "G1":
        line = penSpeedPattern.sub("F{0}".format(options.liftOffSpeed), line)

    return line


######################## Main #########################

parser = OptionParser(usage="usage: %prog [options] gcode-file")
parser.add_option("-e", "--egg-displace", dest="wantDisplaceCorrection",
                  action="store_true", default=False,
                  help="Correct displacement if drawn on a egg. The tip of the egg is pointing right hand.")
parser.add_option("-d", "--dont-send", dest="wantToSend",
                  action="store_false", default=True,
                  help="Dont send GCode to SphereBot")
parser.add_option("-s", "--servo-angle", dest="servoAngle", type="float",
                  help="Set angle of Servo and exit. 0 is pen down.")
parser.add_option("-l", "--liftOff-speed", dest="liftOffSpeed", type="float", default=None,
                  help="Force speed if pen is up (600 is good).")

(options, args) = parser.parse_args()

if options.wantToSend:
    sphereBot = serial.Serial(DEVICE, BAUDRATE, timeout=30)

    if options.servoAngle != None:
        line = "M300 S{0}\n".format(options.servoAngle)
        print line,
        sphereBot.write(line)
        waitForOkResponse(sphereBot)
        sys.exit()



if len(args) != 1:
    parser.error("incorrect number of arguments: need one gcode file to send to the sphereBot!")


if options.wantDisplaceCorrection:
    pattern = re.compile('([(!;].*|\s+|[a-zA-Z0-9_:](?:[+-])?\d*(?:\.\d*)?|\w\#\d+|\(.*?\)|\#\d+\=(?:[+-])?\d*(?:\.\d*)?)')

penChangePattern = re.compile('^M01')
penUpIndication =  re.compile('pen up')
penDownIndication =  re.compile('pen down')
penSpeedPattern = re.compile('F[0-9\.]+')

blockedLinePattern = re.compile('turn off servo')

fileToFeed = args[0]
gcode = open(fileToFeed, "r")

currentLine = 0.0
lines = gcode.readlines()
totalLines = len(lines)
for line in lines:
    currentLine = currentLine + 1

    if blockedLinePattern.search(line):
        continue

    line = penUpAcceleration(line)
    print line, "({0:.1f}%)".format((currentLine / totalLines)*100),

    penChange(line)

    if options.wantDisplaceCorrection:
        line = correctDisplacement(line)
        print ">> ", line,


    if options.wantToSend:
        sphereBot.write(line)
        waitForOkResponse(sphereBot)

