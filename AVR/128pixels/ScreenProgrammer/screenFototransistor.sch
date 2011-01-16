EESchema Schematic File Version 2  date So 16 Jan 2011 12:00:59 CET
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:special
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
EELAYER 43  0
EELAYER END
$Descr A4 11700 8267
Sheet 1 1
Title ""
Date "16 jan 2011"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Wire Wire Line
	2350 2200 2350 2850
Wire Wire Line
	2350 2850 3050 2850
Wire Wire Line
	3350 3050 3350 3450
Wire Wire Line
	3350 1950 3350 1700
Wire Wire Line
	3350 2450 3350 2650
Wire Wire Line
	2350 3250 2350 3450
Wire Wire Line
	3800 2550 3350 2550
Connection ~ 3350 2550
Wire Wire Line
	2500 1950 2500 1700
Wire Wire Line
	2500 1700 3350 1700
$Comp
L POT 344k
U 1 1 4D32CEE1
P 2500 2200
F 0 "344k" H 2500 2100 50  0000 C CNN
F 1 "POT" H 2500 2200 50  0000 C CNN
	1    2500 2200
	0    -1   -1   0   
$EndComp
$Comp
L CONN_1 Out
U 1 1 4D32CE22
P 3950 2550
F 0 "Out" H 4030 2550 40  0000 L CNN
F 1 "CONN_1" H 3950 2605 30  0001 C CNN
	1    3950 2550
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 4D32CDB9
P 2350 3450
F 0 "#PWR?" H 2350 3450 30  0001 C CNN
F 1 "GND" H 2350 3380 30  0001 C CNN
	1    2350 3450
	1    0    0    -1  
$EndComp
$Comp
L OPTO_NPN SFH309
U 1 1 4D32CCF6
P 2250 3050
F 0 "SFH309" H 2400 3100 50  0000 L CNN
F 1 "OPTO_NPN" H 2400 2950 50  0000 L CNN
	1    2250 3050
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 4D32CC74
P 3350 3450
F 0 "#PWR?" H 3350 3450 30  0001 C CNN
F 1 "GND" H 3350 3380 30  0001 C CNN
	1    3350 3450
	1    0    0    -1  
$EndComp
$Comp
L NPN BC548
U 1 1 4D32CC49
P 3250 2850
F 0 "BC548" H 3250 2700 50  0000 R CNN
F 1 "NPN" H 3250 3000 50  0000 R CNN
	1    3250 2850
	1    0    0    -1  
$EndComp
$Comp
L R 47k
U 1 1 4D32CBFE
P 3350 2200
F 0 "47k" V 3430 2200 50  0000 C CNN
F 1 "R" V 3350 2200 50  0000 C CNN
	1    3350 2200
	1    0    0    -1  
$EndComp
$Comp
L +2,5V #PWR?
U 1 1 4D32CB68
P 3350 1700
F 0 "#PWR?" H 3350 1650 20  0001 C CNN
F 1 "+2,5V" H 3350 1800 30  0001 C CNN
	1    3350 1700
	1    0    0    -1  
$EndComp
$EndSCHEMATC
