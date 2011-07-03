#!/bin/sh
## \
exec tclsh "$0" ${1+"$@"}

set fileToFeed [lindex $argv 0]


set fdIn [open $fileToFeed]
set fdOut [open /dev/ttyUSB1 r+]


while {[gets $fdIn line] >= 0} {
    puts $fdOut $line
    flush $fdOut
    after 2000
}

close $fdIn
close $fdOut
