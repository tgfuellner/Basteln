#!/usr/bin/env python

# Generate an SVG document containing a geometric pattern intended for
# plotting with the Eggbot, http://www.egg-bot.com/ .

# Written by Thomas Gfuellner
# thomas dot gfuellner at gmx dot de
# 2001-12-03

# Written by Daniel C. Newman
# dan dot newman at mtbaldy dot us
# 30 January 2011

# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

import sys
from math import sin, pi

# Scale to mm
MM = 3.5433068

# Document height x width
Height = 140.0
Width  = 50.0
#Height *= 3
#Width  *= 3

# n is number of steps-1!
def frange(start, stop, n):
    L = [0.0] * (n-1)
    nm1 = n - 1
    nm1inv = 1.0 / nm1
    for i in range(n-1):
        L[i] = nm1inv * (start*(nm1 - i) + stop*i)
    return L

def sinf(x):
    return 8*sin(x+pi/2.0)

def draw(func, periods, xStart, periodParts=20, reverse=False):
    dy = Height / (periods*periodParts)
    if reverse:
        dy *= -1
        YStart = Height
    else:
        YStart = 0.0

    listOfAngles = []
    for p in range(0,periods):
        listOfAngles += frange(0,2*pi, periodParts+1)

    xPre = func(listOfAngles[0])
    path = 'M %f,%f' % ( xStart+xPre, YStart )

    if reverse:
        listOfAngles.reverse()
    else:
        # move first to the end to close
        listOfAngles = listOfAngles[1:] + listOfAngles[0:1]
            
    for t in listOfAngles:
        x = func(t)
        path += ' l %f,%f' % (x-xPre, dy)
        xPre = x

    print('<path d="%s"/>' % path)

def drawLine(x, reverse=False):
    if reverse:
        print( '<path d="M %s,%s l 0,-%s"/>' %(x, Height, Height))
    else:
        print( '<path d="M %s,0 l 0,%s"/>' % (x,Height))

def printLayer(color):
    print('<g inkscape:groupmode="layer" inkscape:label="L-%s"' % color )
    print('   transform="scale(%f,%f)" fill="none" stroke="%s"' % ( MM, MM, color ) )
    print('   stroke-width="%f">\n' % ( 0.2 ) )


print(
'<svg xmlns="http://www.w3.org/2000/svg"\n' +
'     xmlns:inkscape="http://www.inkscape.org/namespaces/inkscape"\n' +
'     width="%d" height="%d">\n' % ( Width*MM, Height*MM ) )

printLayer('black')


drawLine(5)
draw(func=lambda x: 2.5*sin(x+pi/2), periods=40,xStart=7.5,periodParts=2,reverse=True)
drawLine(10)
drawLine(15,reverse=True)

drawLine(35)
drawLine(40,reverse=True)
draw(func=lambda x: 2.5*sin(x+pi/2), periods=40,xStart=42.5,periodParts=2)
drawLine(45,reverse=True)

print('\n</g>')

printLayer('red')

draw(func=lambda x: 10*sin(x), periods=10,xStart=25)
draw(func=lambda x: 10*sin(x+pi), periods=10,xStart=25,reverse=True)
drawLine(25)

draw(func=lambda x: 5*sin(x), periods=10,xStart=25,reverse=True)
draw(func=lambda x: 5*sin(x+pi), periods=10,xStart=25)
draw(func=lambda x: 2.5*sin(x), periods=10,xStart=25,reverse=True)
draw(func=lambda x: 2.5*sin(x+pi), periods=10,xStart=25)

print( '\n</g>\n</svg>' )
