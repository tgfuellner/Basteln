#!/bin/sh
# \
exec wish "$0" ${1+"$@"}

set timeInMs 50


wm protocol . WM_DELETE_WINDOW exit


canvas .c

.c config -bg black
wm title . "$timeInMs ms ([expr 1000.0/$timeInMs]Hz)"

pack .c

proc setColor {timeMs primeColor nextColor} {
    .c config -bg $primeColor
    after $timeMs setColor $timeMs $nextColor $primeColor
}

setColor $timeInMs black white
vwait tt
