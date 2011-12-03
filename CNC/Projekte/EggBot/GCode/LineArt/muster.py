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

# n is number of steps!
def frange(start, stop, n):
    L = [0.0] * n
    nm1 = n - 1
    nm1inv = 1.0 / nm1
    for i in range(n):
        L[i] = nm1inv * (start*(nm1 - i) + stop*i)
    return L

def sinf(x):
    return 3*sin(x)

def draw_sin(xStart):
    path = 'M %f,%f' % ( xStart, 0 )
    Periods = 10
    NumberOfSinParts = 40
    dy = Height / (Periods*NumberOfSinParts)
    xPre = 0

    for p in range(0,Periods):
        for t in frange(0,2*pi, NumberOfSinParts):
            x = sinf(t)
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


print( '<path d="M 5,0 l 0,140"/>')
print( '<path d="M 45,140 l 0,-140"/>')
draw_sin(10)


print( '\n</g>\n</svg>' )
