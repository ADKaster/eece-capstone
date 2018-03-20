EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:switches
LIBS:relays
LIBS:motors
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
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
LIBS:capstone_board_components
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Conn_01x40 J2
U 1 1 5A73471C
P 1400 3800
F 0 "J2" H 1400 5800 50  0000 C CNN
F 1 "Conn_01x40" H 1400 1700 50  0000 C CNN
F 2 "capstone_board_components:ti_boosterpack_40pin" H 1400 3800 50  0001 C CNN
F 3 "" H 1400 3800 50  0001 C CNN
	1    1400 3800
	-1   0    0    1   
$EndComp
Text Notes 950  6250 0    60   ~ 0
Pins are in order of the\n40-pin Booster Pack pinout
$Comp
L +3V3 #PWR01
U 1 1 5A7347B6
P 1900 5800
F 0 "#PWR01" H 1900 5650 50  0001 C CNN
F 1 "+3V3" H 1900 5940 50  0000 C CNN
F 2 "" H 1900 5800 50  0001 C CNN
F 3 "" H 1900 5800 50  0001 C CNN
	1    1900 5800
	-1   0    0    1   
$EndComp
$Comp
L GND #PWR02
U 1 1 5A7347F0
P 1900 3900
F 0 "#PWR02" H 1900 3650 50  0001 C CNN
F 1 "GND" H 1900 3750 50  0000 C CNN
F 2 "" H 1900 3900 50  0001 C CNN
F 3 "" H 1900 3900 50  0001 C CNN
	1    1900 3900
	1    0    0    -1  
$EndComp
Wire Wire Line
	1600 5700 1900 5700
Wire Wire Line
	1600 3800 1900 3800
Wire Wire Line
	1900 3600 1900 3900
Wire Wire Line
	1600 3600 1900 3600
Connection ~ 1900 3800
Wire Wire Line
	1900 5700 1900 5800
Wire Wire Line
	1600 4900 2050 4900
Wire Wire Line
	1600 4800 2050 4800
Text Label 2050 4900 0    60   ~ 0
SCL
Text Label 2050 4800 0    60   ~ 0
SDA
$Comp
L adafruit_bno055 U2
U 1 1 5A734C43
P 5550 4250
F 0 "U2" H 5850 4150 60  0000 C CNN
F 1 "adafruit_bno055" H 6000 4850 60  0000 C CNN
F 2 "capstone_board_components:adafruit_bno055" H 5550 4250 60  0001 C CNN
F 3 "" H 5550 4250 60  0001 C CNN
	1    5550 4250
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR03
U 1 1 5A734CD2
P 7400 3400
F 0 "#PWR03" H 7400 3150 50  0001 C CNN
F 1 "GND" H 7400 3250 50  0000 C CNN
F 2 "" H 7400 3400 50  0001 C CNN
F 3 "" H 7400 3400 50  0001 C CNN
	1    7400 3400
	1    0    0    -1  
$EndComp
$Comp
L +3V3 #PWR04
U 1 1 5A734CEE
P 7600 3400
F 0 "#PWR04" H 7600 3250 50  0001 C CNN
F 1 "+3V3" H 7600 3540 50  0000 C CNN
F 2 "" H 7600 3400 50  0001 C CNN
F 3 "" H 7600 3400 50  0001 C CNN
	1    7600 3400
	-1   0    0    1   
$EndComp
Wire Wire Line
	6800 3300 7600 3300
Wire Wire Line
	7600 3300 7600 3400
Wire Wire Line
	6800 3400 7400 3400
Wire Wire Line
	6800 3500 7000 3500
Wire Wire Line
	6800 3600 7000 3600
Text Label 7000 3500 0    60   ~ 0
SDA
Text Label 7000 3600 0    60   ~ 0
SCL
$Comp
L adafruit_bmp280 U1
U 1 1 5A73517C
P 5300 5450
F 0 "U1" H 5400 5350 60  0000 C CNN
F 1 "adafruit_bmp280" H 5750 5900 60  0000 C CNN
F 2 "capstone_board_components:adafruit_bmp280" H 5300 5450 60  0001 C CNN
F 3 "" H 5300 5450 60  0001 C CNN
	1    5300 5450
	1    0    0    -1  
$EndComp
$Comp
L +3V3 #PWR05
U 1 1 5A7351E2
P 7600 4900
F 0 "#PWR05" H 7600 4750 50  0001 C CNN
F 1 "+3V3" H 7600 5040 50  0000 C CNN
F 2 "" H 7600 4900 50  0001 C CNN
F 3 "" H 7600 4900 50  0001 C CNN
	1    7600 4900
	-1   0    0    1   
$EndComp
$Comp
L GND #PWR06
U 1 1 5A735205
P 7400 4900
F 0 "#PWR06" H 7400 4650 50  0001 C CNN
F 1 "GND" H 7400 4750 50  0000 C CNN
F 2 "" H 7400 4900 50  0001 C CNN
F 3 "" H 7400 4900 50  0001 C CNN
	1    7400 4900
	1    0    0    -1  
$EndComp
Wire Wire Line
	6800 4900 7400 4900
Wire Wire Line
	6800 4800 7600 4800
Wire Wire Line
	7600 4800 7600 4900
Wire Wire Line
	6800 5000 7000 5000
Wire Wire Line
	6800 5200 7000 5200
Text Label 7000 5000 0    60   ~ 0
SCL
Text Label 7000 5200 0    60   ~ 0
SDA
NoConn ~ 1600 1800
NoConn ~ 1600 1900
NoConn ~ 1600 2000
NoConn ~ 1600 2100
NoConn ~ 1600 2200
NoConn ~ 1600 2500
NoConn ~ 1600 2600
NoConn ~ 1600 2400
NoConn ~ 1600 2300
NoConn ~ 1600 2700
NoConn ~ 1600 2800
NoConn ~ 1600 2900
NoConn ~ 1600 3000
NoConn ~ 1600 3100
NoConn ~ 1600 3200
NoConn ~ 1600 3300
NoConn ~ 1600 3400
NoConn ~ 1600 3500
NoConn ~ 1600 3700
NoConn ~ 1600 3900
NoConn ~ 1600 4000
NoConn ~ 1600 4100
NoConn ~ 1600 4200
NoConn ~ 1600 4300
NoConn ~ 1600 4400
NoConn ~ 1600 4500
NoConn ~ 1600 4600
NoConn ~ 1600 4700
NoConn ~ 1600 5000
NoConn ~ 1600 5100
NoConn ~ 1600 5200
NoConn ~ 1600 5300
NoConn ~ 1600 5400
NoConn ~ 1600 5500
NoConn ~ 1600 5600
NoConn ~ 6800 4700
NoConn ~ 6800 5100
NoConn ~ 6800 5300
NoConn ~ 6800 3700
NoConn ~ 6800 3200
NoConn ~ 6800 3800
NoConn ~ 6800 3900
NoConn ~ 6800 4000
NoConn ~ 6800 4100
$Comp
L R R1
U 1 1 5A846002
P 3550 3550
F 0 "R1" V 3630 3550 50  0000 C CNN
F 1 "4.7k" V 3550 3550 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 3480 3550 50  0001 C CNN
F 3 "" H 3550 3550 50  0001 C CNN
	1    3550 3550
	0    1    1    0   
$EndComp
$Comp
L R R2
U 1 1 5A84603F
P 3550 3800
F 0 "R2" V 3630 3800 50  0000 C CNN
F 1 "4.7k" V 3550 3800 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 3480 3800 50  0001 C CNN
F 3 "" H 3550 3800 50  0001 C CNN
	1    3550 3800
	0    1    1    0   
$EndComp
$Comp
L +3V3 #PWR07
U 1 1 5A846061
P 3850 4000
F 0 "#PWR07" H 3850 3850 50  0001 C CNN
F 1 "+3V3" H 3850 4140 50  0000 C CNN
F 2 "" H 3850 4000 50  0001 C CNN
F 3 "" H 3850 4000 50  0001 C CNN
	1    3850 4000
	-1   0    0    1   
$EndComp
Wire Wire Line
	3700 3800 3850 3800
Wire Wire Line
	3850 3550 3850 4000
Wire Wire Line
	3700 3550 3850 3550
Connection ~ 3850 3800
Wire Wire Line
	3400 3550 3200 3550
Wire Wire Line
	3200 3800 3400 3800
Text Label 3200 3550 0    60   ~ 0
SDA
Text Label 3200 3800 0    60   ~ 0
SCL
$EndSCHEMATC
