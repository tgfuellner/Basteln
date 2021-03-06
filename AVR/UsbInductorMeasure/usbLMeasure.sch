EESchema Schematic File Version 2  date Fr 10 Sep 2010 14:31:13 CEST
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
LIBS:attiny
LIBS:usbLMeasure-cache
EELAYER 24  0
EELAYER END
$Descr A4 11700 8267
Sheet 1 1
Title ""
Date "10 sep 2010"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Connection ~ 3500 2650
Wire Wire Line
	8400 4950 8250 4950
Connection ~ 9450 3300
Wire Wire Line
	9450 3300 9750 3300
Wire Wire Line
	9750 3300 9750 3550
Wire Wire Line
	9450 3550 9450 3250
Wire Wire Line
	4450 6050 4450 6300
Wire Wire Line
	5500 5250 5700 5250
Wire Wire Line
	7650 5800 7650 6000
Connection ~ 7650 5200
Wire Wire Line
	7650 5400 7650 5200
Wire Wire Line
	8100 4450 8100 4300
Wire Wire Line
	8400 5200 6200 5200
Wire Wire Line
	6200 5200 6200 5850
Wire Wire Line
	6200 5850 5100 5850
Wire Wire Line
	4600 5850 4450 5850
Wire Wire Line
	5250 2550 4500 2550
Wire Wire Line
	3000 3500 3000 3750
Wire Wire Line
	1350 3250 1350 3750
Wire Wire Line
	4150 3300 4500 3300
Wire Wire Line
	3400 3450 3400 3750
Wire Wire Line
	2100 3750 2100 3450
Wire Wire Line
	4900 2550 4900 2300
Connection ~ 4500 2550
Wire Wire Line
	4500 3300 4500 1850
Wire Wire Line
	4500 1850 4150 1850
Wire Wire Line
	3650 3300 3650 2800
Wire Wire Line
	3650 2800 3500 2800
Wire Wire Line
	3500 2800 3500 2650
Wire Wire Line
	3400 2450 3500 2450
Wire Wire Line
	3000 3000 3000 2300
Wire Wire Line
	3900 2950 3900 3050
Wire Wire Line
	3500 2650 2800 2650
Wire Wire Line
	3900 2050 3900 2150
Connection ~ 3000 2650
Wire Wire Line
	3650 1850 3400 1850
Connection ~ 3400 2450
Wire Wire Line
	4900 1800 4900 1600
Wire Wire Line
	2100 2650 2100 3050
Wire Wire Line
	1950 2650 2400 2650
Connection ~ 2100 2650
Wire Wire Line
	3400 1850 3400 3050
Wire Wire Line
	1350 3050 1350 2650
Wire Wire Line
	3000 1800 3000 1600
Wire Wire Line
	1300 5900 1100 5900
Connection ~ 4900 2550
Wire Wire Line
	4450 5450 4700 5450
Wire Wire Line
	9150 5050 9500 5050
Wire Wire Line
	4600 5050 4450 5050
Wire Wire Line
	5100 5050 8400 5050
Connection ~ 8100 5050
Wire Wire Line
	8100 5400 8100 4950
Wire Wire Line
	8100 5800 8100 6000
Wire Wire Line
	4450 5250 4900 5250
Wire Wire Line
	1100 6100 1300 6100
Wire Wire Line
	9150 5200 9500 5200
Wire Wire Line
	9450 3950 9450 4400
Wire Wire Line
	9750 3950 9750 4150
Wire Wire Line
	9750 4150 9450 4150
Connection ~ 9450 4150
Wire Wire Line
	9300 4950 9150 4950
$Comp
L GND #PWR?
U 1 1 4C865719
P 8250 4950
F 0 "#PWR?" H 8250 4950 30  0001 C CNN
F 1 "GND" H 8250 4880 30  0001 C CNN
	1    8250 4950
	0    1    1    0   
$EndComp
$Comp
L GND #PWR?
U 1 1 4C865712
P 9300 4950
F 0 "#PWR?" H 9300 4950 30  0001 C CNN
F 1 "GND" H 9300 4880 30  0001 C CNN
	1    9300 4950
	0    -1   -1   0   
$EndComp
$Comp
L C C4
U 1 1 4C864DEB
P 9450 3750
F 0 "C4" H 9500 3850 50  0000 L CNN
F 1 "10u" H 9500 3650 50  0000 L CNN
	1    9450 3750
	1    0    0    -1  
$EndComp
$Comp
L C C5
U 1 1 4C864DE7
P 9750 3750
F 0 "C5" H 9800 3850 50  0000 L CNN
F 1 "100n" H 9800 3650 50  0000 L CNN
	1    9750 3750
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR01
U 1 1 4C864DBA
P 9450 3250
F 0 "#PWR01" H 9450 3340 20  0001 C CNN
F 1 "+5V" H 9450 3340 30  0000 C CNN
	1    9450 3250
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR02
U 1 1 4C864D9B
P 9450 4400
F 0 "#PWR02" H 9450 4400 30  0001 C CNN
F 1 "GND" H 9450 4330 30  0001 C CNN
	1    9450 4400
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR03
U 1 1 4C864C59
P 4450 6300
F 0 "#PWR03" H 4450 6390 20  0001 C CNN
F 1 "+5V" H 4450 6390 30  0000 C CNN
	1    4450 6300
	-1   0    0    1   
$EndComp
$Comp
L GND #PWR04
U 1 1 4C864A2E
P 5700 5250
F 0 "#PWR04" H 5700 5250 30  0001 C CNN
F 1 "GND" H 5700 5180 30  0001 C CNN
	1    5700 5250
	0    -1   -1   0   
$EndComp
$Comp
L SW_PUSH SW1
U 1 1 4C864A1B
P 5200 5250
F 0 "SW1" H 5350 5360 50  0000 C CNN
F 1 "SW_PUSH" H 5200 5170 50  0000 C CNN
	1    5200 5250
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR05
U 1 1 4C864910
P 7650 6000
F 0 "#PWR05" H 7650 6000 30  0001 C CNN
F 1 "GND" H 7650 5930 30  0001 C CNN
	1    7650 6000
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR06
U 1 1 4C86490B
P 8100 6000
F 0 "#PWR06" H 8100 6000 30  0001 C CNN
F 1 "GND" H 8100 5930 30  0001 C CNN
	1    8100 6000
	1    0    0    -1  
$EndComp
$Comp
L ZENER D1
U 1 1 4C8648F3
P 7650 5600
F 0 "D1" H 7650 5700 50  0000 C CNN
F 1 "3.6" H 7650 5500 40  0000 C CNN
	1    7650 5600
	0    -1   -1   0   
$EndComp
$Comp
L ZENER D2
U 1 1 4C8648DD
P 8100 5600
F 0 "D2" H 8100 5700 50  0000 C CNN
F 1 "3.6" H 8100 5500 40  0000 C CNN
	1    8100 5600
	0    -1   -1   0   
$EndComp
$Comp
L +5V #PWR07
U 1 1 4C864883
P 8100 4300
F 0 "#PWR07" H 8100 4390 20  0001 C CNN
F 1 "+5V" H 8100 4390 30  0000 C CNN
	1    8100 4300
	1    0    0    -1  
$EndComp
$Comp
L R R8
U 1 1 4C864863
P 8100 4700
F 0 "R8" V 8180 4700 50  0000 C CNN
F 1 "1.5k" V 8100 4700 50  0000 C CNN
	1    8100 4700
	-1   0    0    1   
$EndComp
$Comp
L R R7
U 1 1 4C864815
P 4850 5850
F 0 "R7" V 4930 5850 50  0000 C CNN
F 1 "68" V 4850 5850 50  0000 C CNN
	1    4850 5850
	0    1    1    0   
$EndComp
$Comp
L R R6
U 1 1 4C8647DF
P 4850 5050
F 0 "R6" V 4930 5050 50  0000 C CNN
F 1 "68" V 4850 5050 50  0000 C CNN
	1    4850 5050
	0    1    1    0   
$EndComp
$Comp
L +5V #PWR08
U 1 1 4C8647A6
P 9500 5200
F 0 "#PWR08" H 9500 5290 20  0001 C CNN
F 1 "+5V" H 9500 5290 30  0000 C CNN
	1    9500 5200
	0    1    1    0   
$EndComp
$Comp
L GND #PWR09
U 1 1 4C864784
P 9500 5050
F 0 "#PWR09" H 9500 5050 30  0001 C CNN
F 1 "GND" H 9500 4980 30  0001 C CNN
	1    9500 5050
	0    -1   -1   0   
$EndComp
$Comp
L USB_1 J1
U 1 1 4C864753
P 8750 5400
F 0 "J1" H 8600 5800 60  0000 C CNN
F 1 "USB_1" H 8725 4800 60  0001 C CNN
	1    8750 5400
	-1   0    0    1   
$EndComp
Text GLabel 5250 2550 2    60   Output ~ 0
Count
Text GLabel 4700 5450 2    60   Input ~ 0
Count
$Comp
L +5V #PWR010
U 1 1 4C752CC7
P 1100 5900
F 0 "#PWR010" H 1100 5990 20  0001 C CNN
F 1 "+5V" H 1100 5990 30  0000 C CNN
	1    1100 5900
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR011
U 1 1 4C752CA8
P 1100 6100
F 0 "#PWR011" H 1100 6100 30  0001 C CNN
F 1 "GND" H 1100 6030 30  0001 C CNN
	1    1100 6100
	0    1    1    0   
$EndComp
$Comp
L ATTINY25-45-85/DIP-SO U2
U 1 1 4C752C87
P 2850 5550
F 0 "U2" H 2825 6175 60  0000 C CNN
F 1 "ATTINY25-45-85/DIP-SO" H 2850 6325 60  0000 C CNN
	1    2850 5550
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR012
U 1 1 4C752B97
P 3000 1600
F 0 "#PWR012" H 3000 1690 20  0001 C CNN
F 1 "+5V" H 3000 1690 30  0000 C CNN
	1    3000 1600
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR013
U 1 1 4C752B8D
P 3000 3750
F 0 "#PWR013" H 3000 3750 30  0001 C CNN
F 1 "GND" H 3000 3680 30  0001 C CNN
	1    3000 3750
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR014
U 1 1 4C752B02
P 1350 3750
F 0 "#PWR014" H 1350 3750 30  0001 C CNN
F 1 "GND" H 1350 3680 30  0001 C CNN
	1    1350 3750
	1    0    0    -1  
$EndComp
$Comp
L CONN_2 P1
U 1 1 4C752AF5
P 1000 3150
F 0 "P1" V 950 3150 40  0000 C CNN
F 1 "Lx" V 1050 3150 40  0000 C CNN
	1    1000 3150
	-1   0    0    1   
$EndComp
$Comp
L INDUCTOR L1
U 1 1 4C752A74
P 1650 2650
F 0 "L1" V 1600 2650 40  0000 C CNN
F 1 "10u" V 1750 2650 40  0000 C CNN
	1    1650 2650
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR015
U 1 1 4C752A1A
P 2100 3750
F 0 "#PWR015" H 2100 3750 30  0001 C CNN
F 1 "GND" H 2100 3680 30  0001 C CNN
	1    2100 3750
	1    0    0    -1  
$EndComp
$Comp
L C C1
U 1 1 4C752A05
P 2100 3250
F 0 "C1" H 2150 3350 50  0000 L CNN
F 1 "10n" H 2150 3150 50  0000 L CNN
	1    2100 3250
	-1   0    0    1   
$EndComp
$Comp
L +5V #PWR016
U 1 1 4C7529C1
P 4900 1600
F 0 "#PWR016" H 4900 1690 20  0001 C CNN
F 1 "+5V" H 4900 1690 30  0000 C CNN
	1    4900 1600
	1    0    0    -1  
$EndComp
$Comp
L R R5
U 1 1 4C752999
P 4900 2050
F 0 "R5" V 4980 2050 50  0000 C CNN
F 1 "1k" V 4900 2050 50  0000 C CNN
	1    4900 2050
	1    0    0    -1  
$EndComp
$Comp
L R R4
U 1 1 4C752908
P 3900 3300
F 0 "R4" V 3980 3300 50  0000 C CNN
F 1 "100k" V 3900 3300 50  0000 C CNN
	1    3900 3300
	0    1    1    0   
$EndComp
$Comp
L R R3
U 1 1 4C752863
P 3900 1850
F 0 "R3" V 3980 1850 50  0000 C CNN
F 1 "47k" V 3900 1850 50  0000 C CNN
	1    3900 1850
	0    1    1    0   
$EndComp
$Comp
L GND #PWR017
U 1 1 4C7527D7
P 3400 3750
F 0 "#PWR017" H 3400 3750 30  0001 C CNN
F 1 "GND" H 3400 3680 30  0001 C CNN
	1    3400 3750
	1    0    0    -1  
$EndComp
$Comp
L C C3
U 1 1 4C7527C5
P 3400 3250
F 0 "C3" H 3450 3350 50  0000 L CNN
F 1 "10u" H 3450 3150 50  0000 L CNN
	1    3400 3250
	-1   0    0    1   
$EndComp
$Comp
L R R2
U 1 1 4C75275B
P 3000 3250
F 0 "R2" V 3080 3250 50  0000 C CNN
F 1 "100k" V 3000 3250 50  0000 C CNN
	1    3000 3250
	1    0    0    -1  
$EndComp
$Comp
L R R1
U 1 1 4C75272F
P 3000 2050
F 0 "R1" V 3080 2050 50  0000 C CNN
F 1 "100k" V 3000 2050 50  0000 C CNN
	1    3000 2050
	1    0    0    -1  
$EndComp
$Comp
L C C2
U 1 1 4C7525DD
P 2600 2650
F 0 "C2" V 2650 2750 50  0000 L CNN
F 1 "10u" V 2650 2550 50  0000 L CNN
	1    2600 2650
	0    1    1    0   
$EndComp
$Comp
L +5V #PWR018
U 1 1 4C7525A1
P 3900 3050
F 0 "#PWR018" H 3900 3140 20  0001 C CNN
F 1 "+5V" H 3900 3140 30  0000 C CNN
	1    3900 3050
	-1   0    0    1   
$EndComp
$Comp
L GND #PWR019
U 1 1 4C75257A
P 3900 2050
F 0 "#PWR019" H 3900 2050 30  0001 C CNN
F 1 "GND" H 3900 1980 30  0001 C CNN
	1    3900 2050
	-1   0    0    1   
$EndComp
$Comp
L LM393 U1
U 1 1 4C752531
P 4000 2550
F 0 "U1" H 4150 2700 60  0000 C CNN
F 1 "LM393" H 4200 2350 60  0000 C CNN
	1    4000 2550
	1    0    0    1   
$EndComp
$EndSCHEMATC
