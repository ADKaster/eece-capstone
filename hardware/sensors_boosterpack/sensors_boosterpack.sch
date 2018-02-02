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
L Conn_01x08 J1
U 1 1 5A7342EE
P 1300 1550
F 0 "J1" H 1300 1950 50  0000 C CNN
F 1 "Conn_01x08" H 1300 1050 50  0000 C CNN
F 2 "Connectors_Molex:Molex_Microfit3_Header_02x04_Angled_43045-080x" H 1300 1550 50  0001 C CNN
F 3 "" H 1300 1550 50  0001 C CNN
	1    1300 1550
	-1   0    0    1   
$EndComp
$Comp
L GND #PWR01
U 1 1 5A734483
P 1600 1950
F 0 "#PWR01" H 1600 1700 50  0001 C CNN
F 1 "GND" H 1600 1800 50  0000 C CNN
F 2 "" H 1600 1950 50  0001 C CNN
F 3 "" H 1600 1950 50  0001 C CNN
	1    1600 1950
	1    0    0    -1  
$EndComp
$Comp
L +3V3 #PWR02
U 1 1 5A734499
P 1800 1950
F 0 "#PWR02" H 1800 1800 50  0001 C CNN
F 1 "+3V3" H 1800 2090 50  0000 C CNN
F 2 "" H 1800 1950 50  0001 C CNN
F 3 "" H 1800 1950 50  0001 C CNN
	1    1800 1950
	-1   0    0    1   
$EndComp
Wire Wire Line
	1500 1850 1600 1850
Wire Wire Line
	1600 1750 1600 1950
Wire Wire Line
	1500 1750 1600 1750
Connection ~ 1600 1850
Wire Wire Line
	1500 1650 1800 1650
Wire Wire Line
	1800 1550 1800 1950
Wire Wire Line
	1500 1550 1800 1550
Connection ~ 1800 1650
Wire Wire Line
	1500 1350 1800 1350
Wire Wire Line
	1500 1250 1800 1250
Wire Wire Line
	1500 1150 1800 1150
Text Label 1800 1450 0    60   ~ 0
SCL
Text Label 1800 1350 0    60   ~ 0
SDA
Text Label 1800 1150 0    60   ~ 0
RST
Wire Wire Line
	1500 1450 1800 1450
Text Label 1800 1250 0    60   ~ 0
TRIG
$Comp
L Conn_01x08 J3
U 1 1 5A734646
P 2700 1550
F 0 "J3" H 2700 1950 50  0000 C CNN
F 1 "Conn_01x08" H 2700 1050 50  0000 C CNN
F 2 "Connectors_Molex:Molex_Microfit3_Header_02x04_Angled_43045-080x" H 2700 1550 50  0001 C CNN
F 3 "" H 2700 1550 50  0001 C CNN
	1    2700 1550
	-1   0    0    1   
$EndComp
$Comp
L GND #PWR03
U 1 1 5A73464C
P 3000 1950
F 0 "#PWR03" H 3000 1700 50  0001 C CNN
F 1 "GND" H 3000 1800 50  0000 C CNN
F 2 "" H 3000 1950 50  0001 C CNN
F 3 "" H 3000 1950 50  0001 C CNN
	1    3000 1950
	1    0    0    -1  
$EndComp
$Comp
L +3V3 #PWR04
U 1 1 5A734652
P 3200 1950
F 0 "#PWR04" H 3200 1800 50  0001 C CNN
F 1 "+3V3" H 3200 2090 50  0000 C CNN
F 2 "" H 3200 1950 50  0001 C CNN
F 3 "" H 3200 1950 50  0001 C CNN
	1    3200 1950
	-1   0    0    1   
$EndComp
Wire Wire Line
	2900 1850 3000 1850
Wire Wire Line
	3000 1750 3000 1950
Wire Wire Line
	2900 1750 3000 1750
Connection ~ 3000 1850
Wire Wire Line
	2900 1650 3200 1650
Wire Wire Line
	3200 1550 3200 1950
Wire Wire Line
	2900 1550 3200 1550
Connection ~ 3200 1650
Wire Wire Line
	2900 1350 3200 1350
Wire Wire Line
	2900 1250 3200 1250
Wire Wire Line
	2900 1150 3200 1150
Text Label 3200 1450 0    60   ~ 0
SCL
Text Label 3200 1350 0    60   ~ 0
SDA
Text Label 3200 1150 0    60   ~ 0
RST
Wire Wire Line
	2900 1450 3200 1450
Text Label 3200 1250 0    60   ~ 0
TRIG
$Comp
L Conn_01x40 J2
U 1 1 5A73471C
P 1350 4800
F 0 "J2" H 1350 6800 50  0000 C CNN
F 1 "Conn_01x40" H 1350 2700 50  0000 C CNN
F 2 "capstone_board_components:ti_boosterpack_40pin" H 1350 4800 50  0001 C CNN
F 3 "" H 1350 4800 50  0001 C CNN
	1    1350 4800
	-1   0    0    1   
$EndComp
Text Notes 900  7250 0    60   ~ 0
Pins are in order of the\n40-pin Booster Pack pinout
$Comp
L +3V3 #PWR05
U 1 1 5A7347B6
P 1850 6800
F 0 "#PWR05" H 1850 6650 50  0001 C CNN
F 1 "+3V3" H 1850 6940 50  0000 C CNN
F 2 "" H 1850 6800 50  0001 C CNN
F 3 "" H 1850 6800 50  0001 C CNN
	1    1850 6800
	-1   0    0    1   
$EndComp
$Comp
L GND #PWR06
U 1 1 5A7347F0
P 1850 4900
F 0 "#PWR06" H 1850 4650 50  0001 C CNN
F 1 "GND" H 1850 4750 50  0000 C CNN
F 2 "" H 1850 4900 50  0001 C CNN
F 3 "" H 1850 4900 50  0001 C CNN
	1    1850 4900
	1    0    0    -1  
$EndComp
Wire Wire Line
	1550 6700 1850 6700
Wire Wire Line
	1550 4800 1850 4800
Wire Wire Line
	1850 4600 1850 4900
Wire Wire Line
	1550 4600 1850 4600
Connection ~ 1850 4800
Wire Wire Line
	1550 5200 2000 5200
Text Label 2000 5200 0    60   ~ 0
RST
Wire Wire Line
	1550 6600 2000 6600
Text Label 2000 6600 0    60   ~ 0
TRIG
Wire Wire Line
	1850 6700 1850 6800
Wire Wire Line
	1550 5900 2000 5900
Wire Wire Line
	1550 5800 2000 5800
Text Label 2000 5900 0    60   ~ 0
SCL
Text Label 2000 5800 0    60   ~ 0
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
L GND #PWR07
U 1 1 5A734CD2
P 7400 3400
F 0 "#PWR07" H 7400 3150 50  0001 C CNN
F 1 "GND" H 7400 3250 50  0000 C CNN
F 2 "" H 7400 3400 50  0001 C CNN
F 3 "" H 7400 3400 50  0001 C CNN
	1    7400 3400
	1    0    0    -1  
$EndComp
$Comp
L +3V3 #PWR08
U 1 1 5A734CEE
P 7600 3400
F 0 "#PWR08" H 7600 3250 50  0001 C CNN
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
S_SDA
Text Label 7000 3600 0    60   ~ 0
S_SCL
Wire Wire Line
	2100 3200 1550 3200
Text Label 2100 3200 0    60   ~ 0
S_SDA
Wire Wire Line
	1550 3300 2100 3300
Text Label 2100 3300 0    60   ~ 0
S_SCL
Text Notes 2500 3300 0    60   ~ 0
Internal Sensor Bus\nUCB3
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
L +3V3 #PWR09
U 1 1 5A7351E2
P 7600 4900
F 0 "#PWR09" H 7600 4750 50  0001 C CNN
F 1 "+3V3" H 7600 5040 50  0000 C CNN
F 2 "" H 7600 4900 50  0001 C CNN
F 3 "" H 7600 4900 50  0001 C CNN
	1    7600 4900
	-1   0    0    1   
$EndComp
$Comp
L GND #PWR010
U 1 1 5A735205
P 7400 4900
F 0 "#PWR010" H 7400 4650 50  0001 C CNN
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
S_SCL
Text Label 7000 5200 0    60   ~ 0
S_SDA
$EndSCHEMATC
