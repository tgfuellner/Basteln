#!/bin/sh

# crontab -e
# 0-59/5 * * * * /home/thomas/tmp/tempLogger.sh

LOG=/home/thomas/tmp/temperatur.log

date "+%c|" | tr -d "\n" >> $LOG

wget -q -t 1 -O - http://192.168.0.93:8080/index.html | sed -e 's#^.*ADC 00</td><td class=....##' |sed -e 's/<.*/\n/' >> $LOG
