#!/usr/bin/env python


import sys


# Parse the commandline
columns = sys.argv[1:]

if len(columns) == 0:
    print "need Arguments: 1,2,3,4 or 1 2 3 4"

if len(columns) == 1:
    columns = columns[0].split(',')


for bit in xrange(8):
    row = '';

    for col in columns:
        if int(col)&(1<<bit) > 0:
            row += 'X'
        else:
            row += ' '

    print row



print 'Swaped'


for bit in xrange(7, -1, -1):
    row = '';

    for col in columns:
        if int(col)&(1<<bit) > 0:
            row += 'X'
        else:
            row += ' '

    print row

