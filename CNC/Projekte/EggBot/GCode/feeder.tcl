#!/bin/sh
## \
exec tclsh "$0" ${1+"$@"}

set fileToFeed [lindex $argv 0]


set fdIn [open $fileToFeed]
set fdOut [open /dev/ttyUSB1 r+]


while {[gets $fdIn line] >= 0} {
    puts $line
    puts $fdOut $line
    flush $fdOut
    while {[gets $fdOut answer]} {
        puts "\t$answer"
        if {[regexp {^ok:} $answer]} {
            break
        }
    }
}

close $fdIn
close $fdOut
