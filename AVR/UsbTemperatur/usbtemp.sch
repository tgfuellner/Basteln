EESchema Schematic File Version 2  date Mo 09 Aug 2010 20:29:04 CEST
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
LIBS:Dmitri_Belimovl
EELAYER 43  0
EELAYER END
$Descr A4 11700 8267
Sheet 1 1
Title ""
Date "9 aug 2010"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Connection ~ 6650 4100
Wire Wire Line
	6650 4000 6650 4100
Wire Wire Line
	6950 4100 6500 4100
Wire Wire Line
	6200 3400 6950 3400
Wire Wire Line
	6950 3400 6950 3950
Connection ~ 7600 2900
Wire Wire Line
	6850 3200 7300 3200
Wire Wire Line
	7300 3200 7300 2900
Wire Wire Line
	7300 2900 8200 2900
Wire Wire Line
	8200 2150 8200 2700
Wire Wire Line
	8200 3350 8200 3000
Connection ~ 7950 2800
Wire Wire Line
	8200 2800 6850 2800
Wire Wire Line
	6350 3200 6200 3200
Connection ~ 2150 3850
Wire Wire Line
	1800 3850 3050 3850
Connection ~ 2150 3450
Wire Wire Line
	1800 3450 3050 3450
Connection ~ 2700 3850
Connection ~ 2700 3450
Wire Wire Line
	3050 3450 3050 3650
Wire Wire Line
	6350 2800 6200 2800
Wire Wire Line
	6350 3000 6200 3000
Wire Wire Line
	6950 3000 7050 3000
Wire Wire Line
	7950 3250 7950 3350
Wire Wire Line
	7950 2250 7950 2150
Wire Wire Line
	7950 2750 7950 2850
Wire Wire Line
	7600 3300 7600 3350
Wire Wire Line
	6950 4200 6950 4400
Wire Wire Line
	6650 3500 6650 3400
Connection ~ 6650 3400
$Comp
L R R3
U 1 1 4C6043AC
P 6650 3750
F 0 "R3" V 6730 3750 50  0000 C CNN
F 1 "10k" V 6650 3750 50  0000 C CNN
	1    6650 3750
	-1   0    0    1   
$EndComp
$Comp
L +5V #PWR3
U 1 1 4C604390
P 6500 4100
F 0 "#PWR3" H 6500 4190 20  0001 C CNN
F 1 "+5V" H 6500 4190 30  0000 C CNN
	1    6500 4100
	0    -1   1    0   
$EndComp
$Comp
L GND #PWR4
U 1 1 4C604351
P 6950 4400
F 0 "#PWR4" H 6950 4400 30  0001 C CNN
F 1 "GND" H 6950 4330 30  0001 C CNN
	1    6950 4400
	-1   0    0    -1  
$EndComp
$Comp
L DS18S20 U2
U 1 1 4C604304
P 7350 4050
F 0 "U2" H 7300 4350 60  0000 C CNN
F 1 "DS18S20" H 7350 3750 60  0000 C CNN
	1    7350 4050
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR6
U 1 1 4C603F88
P 7600 3350
F 0 "#PWR6" H 7600 3350 30  0001 C CNN
F 1 "GND" H 7600 3280 30  0001 C CNN
	1    7600 3350
	-1   0    0    -1  
$EndComp
$Comp
L ZENER D1
U 1 1 4C603F6E
P 7600 3100
F 0 "D1" H 7600 3200 50  0000 C CNN
F 1 "3.6" H 7600 3000 40  0000 C CNN
	1    7600 3100
	0    -1   -1   0   
$EndComp
$Comp
L +5V #PWR7
U 1 1 4C603F23
P 7950 2150
F 0 "#PWR7" H 7950 2240 20  0001 C CNN
F 1 "+5V" H 7950 2240 30  0000 C CNN
	1    7950 2150
	-1   0    0    -1  
$EndComp
$Comp
L R R4
U 1 1 4C603F0B
P 7950 2500
F 0 "R4" V 8030 2500 50  0000 C CNN
F 1 "1.5k" V 7950 2500 50  0000 C CNN
	1    7950 2500
	-1   0    0    1   
$EndComp
$Comp
L GND #PWR8
U 1 1 4C603ED7
P 7950 3350
F 0 "#PWR8" H 7950 3350 30  0001 C CNN
F 1 "GND" H 7950 3280 30  0001 C CNN
	1    7950 3350
	-1   0    0    -1  
$EndComp
$Comp
L ZENER D2
U 1 1 4C603EA8
P 7950 3050
F 0 "D2" H 7950 3150 50  0000 C CNN
F 1 "3.6" H 7950 2950 40  0000 C CNN
	1    7950 3050
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR5
U 1 1 4C603E34
P 7050 3000
F 0 "#PWR5" H 7050 3000 30  0001 C CNN
F 1 "GND" H 7050 2930 30  0001 C CNN
	1    7050 3000
	0    -1   1    0   
$EndComp
$Comp
L SW_PUSH SW1
U 1 1 4C603DF9
P 6650 3000
F 0 "SW1" H 6800 3110 50  0000 C CNN
F 1 "SW_PUSH" H 6650 2920 50  0000 C CNN
	1    6650 3000
	1    0    0    -1  
$EndComp
$Comp
L R R2
U 1 1 4C603D62
P 6600 3200
F 0 "R2" V 6680 3200 50  0000 C CNN
F 1 "68" V 6600 3200 50  0000 C CNN
	1    6600 3200
	0    1    1    0   
$EndComp
$Comp
L R R1
U 1 1 4C603D4E
P 6600 2800
F 0 "R1" V 6680 2800 50  0000 C CNN
F 1 "68" V 6600 2800 50  0000 C CNN
	1    6600 2800
	0    1    1    0   
$EndComp
$Comp
L CP1 10u1
U 1 1 4C603BDE
P 2150 3650
F 0 "10u1" H 2200 3750 50  0000 L CNN
F 1 "CP1" H 2200 3550 50  0000 L CNN
	1    2150 3650
	1    0    0    -1  
$EndComp
$Comp
L C 100n
U 1 1 4C603B34
P 2700 3650
F 0 "100n" H 2750 3750 50  0000 L CNN
F 1 "C" H 2750 3550 50  0000 L CNN
	1    2700 3650
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR9
U 1 1 4C603A6A
P 8200 2150
F 0 "#PWR9" H 8200 2240 20  0001 C CNN
F 1 "+5V" H 8200 2240 30  0000 C CNN
	1    8200 2150
	-1   0    0    -1  
$EndComp
$Comp
L +5V #PWR1
U 1 1 4C603A4D
P 1800 3450
F 0 "#PWR1" H 1800 3540 20  0001 C CNN
F 1 "+5V" H 1800 3540 30  0000 C CNN
	1    1800 3450
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR2
U 1 1 4C603A14
P 1800 3850
F 0 "#PWR2" H 1800 3850 30  0001 C CNN
F 1 "GND" H 1800 3780 30  0001 C CNN
	1    1800 3850
	0    1    1    0   
$EndComp
$Comp
L GND #PWR10
U 1 1 4C6039E5
P 8200 3350
F 0 "#PWR10" H 8200 3350 30  0001 C CNN
F 1 "GND" H 8200 3280 30  0001 C CNN
	1    8200 3350
	-1   0    0    -1  
$EndComp
$Comp
L USB_2 J1
U 1 1 4C603995
P 8400 2850
F 0 "J1" H 8325 3100 60  0000 C CNN
F 1 "USB_2" H 8450 2550 60  0001 C CNN
F 2 "VCC" H 8725 3000 50  0001 C CNN
F 3 "D+" H 8700 2900 50  0001 C CNN
F 4 "D-" H 8700 2800 50  0001 C CNN "Data-"
F 5 "GND" H 8725 2700 50  0001 C CNN "Ground"
	1    8400 2850
	-1   0    0    -1  
$EndComp
$Comp
L ATTINY25-45-85/DIP-SO U1
U 1 1 4C603707
P 4600 3300
F 0 "U1" H 4575 3925 60  0000 C CNN
F 1 "ATTINY45" H 4600 4075 60  0000 C CNN
	1    4600 3300
	1    0    0    -1  
$EndComp
$EndSCHEMATC
