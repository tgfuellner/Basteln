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


# Document height x width
Height = 140.0
Width  = 50.0
Height *= 3
Width  *= 3

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

def draw(func, periods, xStart, periodParts=20):
    dy = Height / (periods*periodParts)
    xPre = func(0)
    path = 'M %f,%f' % ( xStart+xPre, 0 )

    listOfAngles = []
    for p in range(0,periods):
        listOfAngles += frange(0,2*pi, periodParts+1)
    # move first to the end to close
    listOfAngles = listOfAngles[1:] + listOfAngles[0:1]
            
    for t in listOfAngles:
        x = func(t)
        path += ' l %f,%f' % (x-xPre, dy)
        xPre = x

    print('<path d="%s"/>' % path)


print(
'<svg xmlns="http://www.w3.org/2000/svg"\n' +
'     xmlns:inkscape="http://www.inkscape.org/namespaces/inkscape"\n' +
'     width="%d" height="%d">\n' % ( Width, Height ) )

color = 'black'

print('<g inkscape:groupmode="layer" inkscape:label="2 - %s"' % color )
print('   transform="scale(%f,%f)" fill="none" stroke="%s"' % ( 1, 1, color ) )
print('   stroke-width="%f">\n' % ( 0.2 ) )


print( '<path d="M 20,0 l 0,%s"/>' % Height)
print( '<path d="M 45,%s l 0,-%s"/>' %(Height, Height))
draw(func=lambda x: 4*sin(x), periods=40,xStart=20,periodParts=3)
draw(func=lambda x: 8*sin(x), periods=10,xStart=20)
draw(func=lambda x: 8*sin(x+pi), periods=10,xStart=20)
draw(func=lambda x: 8*sin(x+pi/2.0), periods=10,xStart=20)
draw(func=lambda x: 8*sin(x-pi/2.0), periods=10,xStart=20)


print( '\n</g>\n</svg>' )
