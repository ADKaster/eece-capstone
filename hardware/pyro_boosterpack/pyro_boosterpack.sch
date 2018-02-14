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
Text Notes 8250 5900 0    60   ~ 0
Pins are in order of the\n40-pin Booster Pack pinout
$Comp
L GND #PWR01
U 1 1 5A7347F0
P 9200 3550
F 0 "#PWR01" H 9200 3300 50  0001 C CNN
F 1 "GND" H 9200 3400 50  0000 C CNN
F 2 "" H 9200 3550 50  0001 C CNN
F 3 "" H 9200 3550 50  0001 C CNN
	1    9200 3550
	1    0    0    -1  
$EndComp
$Comp
L Q_NMOS_GSD Q1
U 1 1 5A80B0E0
P 4600 1800
F 0 "Q1" H 4800 1850 50  0000 L CNN
F 1 "Q_NMOS_GSD" H 4800 1750 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-23_Handsoldering" H 4800 1900 50  0001 C CNN
F 3 "" H 4600 1800 50  0001 C CNN
	1    4600 1800
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR02
U 1 1 5A80B1ED
P 4700 2150
F 0 "#PWR02" H 4700 1900 50  0001 C CNN
F 1 "GND" H 4700 2000 50  0000 C CNN
F 2 "" H 4700 2150 50  0001 C CNN
F 3 "" H 4700 2150 50  0001 C CNN
	1    4700 2150
	1    0    0    -1  
$EndComp
Text Label 3900 1800 0    60   ~ 0
PYRO_1_EN
Text Label 4700 1450 0    60   ~ 0
PYRO_1_OUT
$Comp
L Q_NMOS_GSD Q3
U 1 1 5A80B329
P 4550 3150
F 0 "Q3" H 4750 3200 50  0000 L CNN
F 1 "Q_NMOS_GSD" H 4750 3100 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-23_Handsoldering" H 4750 3250 50  0001 C CNN
F 3 "" H 4550 3150 50  0001 C CNN
	1    4550 3150
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR03
U 1 1 5A80B32F
P 4650 3500
F 0 "#PWR03" H 4650 3250 50  0001 C CNN
F 1 "GND" H 4650 3350 50  0000 C CNN
F 2 "" H 4650 3500 50  0001 C CNN
F 3 "" H 4650 3500 50  0001 C CNN
	1    4650 3500
	1    0    0    -1  
$EndComp
Text Label 3850 3150 0    60   ~ 0
PYRO_3_EN
Text Label 4650 2800 0    60   ~ 0
PYRO_3_OUT
$Comp
L Q_NMOS_GSD Q2
U 1 1 5A80B43B
P 6450 1800
F 0 "Q2" H 6650 1850 50  0000 L CNN
F 1 "Q_NMOS_GSD" H 6650 1750 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-23_Handsoldering" H 6650 1900 50  0001 C CNN
F 3 "" H 6450 1800 50  0001 C CNN
	1    6450 1800
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR04
U 1 1 5A80B441
P 6550 2150
F 0 "#PWR04" H 6550 1900 50  0001 C CNN
F 1 "GND" H 6550 2000 50  0000 C CNN
F 2 "" H 6550 2150 50  0001 C CNN
F 3 "" H 6550 2150 50  0001 C CNN
	1    6550 2150
	1    0    0    -1  
$EndComp
Text Label 5750 1800 0    60   ~ 0
PYRO_2_EN
Text Label 6550 1450 0    60   ~ 0
PYRO_2_OUT
$Comp
L Q_NMOS_GSD Q4
U 1 1 5A80B44C
P 6400 3150
F 0 "Q4" H 6600 3200 50  0000 L CNN
F 1 "Q_NMOS_GSD" H 6600 3100 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-23_Handsoldering" H 6600 3250 50  0001 C CNN
F 3 "" H 6400 3150 50  0001 C CNN
	1    6400 3150
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR05
U 1 1 5A80B452
P 6500 3500
F 0 "#PWR05" H 6500 3250 50  0001 C CNN
F 1 "GND" H 6500 3350 50  0000 C CNN
F 2 "" H 6500 3500 50  0001 C CNN
F 3 "" H 6500 3500 50  0001 C CNN
	1    6500 3500
	1    0    0    -1  
$EndComp
Text Label 5700 3150 0    60   ~ 0
PYRO_4_EN
Text Label 6500 2800 0    60   ~ 0
PYRO_4_OUT
$Comp
L Conn_01x02 J1
U 1 1 5A80B5A5
P 1600 1750
F 0 "J1" H 1600 1850 50  0000 C CNN
F 1 "Conn_01x02" H 1600 1550 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x02_Pitch2.54mm" H 1600 1750 50  0001 C CNN
F 3 "" H 1600 1750 50  0001 C CNN
	1    1600 1750
	-1   0    0    1   
$EndComp
$Comp
L GND #PWR06
U 1 1 5A80B62C
P 2050 1850
F 0 "#PWR06" H 2050 1600 50  0001 C CNN
F 1 "GND" H 2050 1700 50  0000 C CNN
F 2 "" H 2050 1850 50  0001 C CNN
F 3 "" H 2050 1850 50  0001 C CNN
	1    2050 1850
	1    0    0    -1  
$EndComp
$Comp
L +BATT #PWR07
U 1 1 5A80B64C
P 2350 1850
F 0 "#PWR07" H 2350 1700 50  0001 C CNN
F 1 "+BATT" H 2350 1990 50  0000 C CNN
F 2 "" H 2350 1850 50  0001 C CNN
F 3 "" H 2350 1850 50  0001 C CNN
	1    2350 1850
	-1   0    0    1   
$EndComp
$Comp
L Conn_01x08 J2
U 1 1 5A80B7A4
P 1600 2800
F 0 "J2" H 1600 3200 50  0000 C CNN
F 1 "Conn_01x08" H 1600 2300 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x08_Pitch2.54mm" H 1600 2800 50  0001 C CNN
F 3 "" H 1600 2800 50  0001 C CNN
	1    1600 2800
	-1   0    0    1   
$EndComp
Text Label 2250 3100 0    60   ~ 0
PYRO_1_OUT
Text Label 2250 2900 0    60   ~ 0
PYRO_2_OUT
Text Label 2250 2700 0    60   ~ 0
PYRO_3_OUT
Text Label 2250 2500 0    60   ~ 0
PYRO_4_OUT
Text Label 9400 1750 0    60   ~ 0
PYRO_1_EN
Text Label 9400 1550 0    60   ~ 0
PYRO_3_EN
Text Label 9400 1450 0    60   ~ 0
PYRO_4_EN
Text Label 9400 1650 0    60   ~ 0
PYRO_2_EN
NoConn ~ 8900 1850
NoConn ~ 8900 1950
NoConn ~ 8900 2050
NoConn ~ 8900 2150
NoConn ~ 8900 2250
NoConn ~ 8900 2350
NoConn ~ 8900 2450
NoConn ~ 8900 2550
NoConn ~ 8900 2650
NoConn ~ 8900 2750
NoConn ~ 8900 2850
NoConn ~ 8900 2950
NoConn ~ 8900 3050
NoConn ~ 8900 3150
NoConn ~ 8900 3350
NoConn ~ 8900 3550
NoConn ~ 8900 3650
NoConn ~ 8900 3750
NoConn ~ 8900 3850
NoConn ~ 8900 3950
NoConn ~ 8900 4050
NoConn ~ 8900 4150
NoConn ~ 8900 4250
NoConn ~ 8900 4350
NoConn ~ 8900 4650
NoConn ~ 8900 4750
NoConn ~ 8900 4850
NoConn ~ 8900 4950
NoConn ~ 8900 5050
NoConn ~ 8900 5150
NoConn ~ 8900 5250
$Comp
L ina260 U1
U 1 1 5A839AFF
P 4100 5350
F 0 "U1" H 3850 5300 60  0000 C CNN
F 1 "ina260" H 4250 5300 60  0000 C CNN
F 2 "capstone_board_components:tssop-16" H 4300 5350 60  0001 C CNN
F 3 "" H 4300 5350 60  0001 C CNN
	1    4100 5350
	1    0    0    -1  
$EndComp
$Comp
L +BATT #PWR08
U 1 1 5A839B93
P 3450 4450
F 0 "#PWR08" H 3450 4300 50  0001 C CNN
F 1 "+BATT" H 3450 4590 50  0000 C CNN
F 2 "" H 3450 4450 50  0001 C CNN
F 3 "" H 3450 4450 50  0001 C CNN
	1    3450 4450
	1    0    0    -1  
$EndComp
Text Label 4800 4400 0    60   ~ 0
BATT_LOAD
Text Label 2050 3350 0    60   ~ 0
BATT_LOAD
$Comp
L Conn_01x40 J3
U 1 1 5A73471C
P 8700 3450
F 0 "J3" H 8700 5450 50  0000 C CNN
F 1 "Conn_01x40" H 8700 1350 50  0000 C CNN
F 2 "capstone_board_components:ti_boosterpack_40pin" H 8700 3450 50  0001 C CNN
F 3 "" H 8700 3450 50  0001 C CNN
	1    8700 3450
	-1   0    0    1   
$EndComp
$Comp
L +3V3 #PWR09
U 1 1 5A839E06
P 9000 5450
F 0 "#PWR09" H 9000 5300 50  0001 C CNN
F 1 "+3V3" H 9000 5590 50  0000 C CNN
F 2 "" H 9000 5450 50  0001 C CNN
F 3 "" H 9000 5450 50  0001 C CNN
	1    9000 5450
	-1   0    0    1   
$EndComp
$Comp
L +3V3 #PWR010
U 1 1 5A839EBC
P 5050 5750
F 0 "#PWR010" H 5050 5600 50  0001 C CNN
F 1 "+3V3" H 5050 5890 50  0000 C CNN
F 2 "" H 5050 5750 50  0001 C CNN
F 3 "" H 5050 5750 50  0001 C CNN
	1    5050 5750
	-1   0    0    1   
$EndComp
Text Label 4700 5300 0    60   ~ 0
SCL
Wire Wire Line
	8900 3450 9200 3450
Wire Wire Line
	9200 3250 9200 3550
Wire Wire Line
	8900 3250 9200 3250
Connection ~ 9200 3450
Wire Wire Line
	4700 2000 4700 2150
Wire Wire Line
	3900 1800 4400 1800
Wire Wire Line
	4700 1450 4700 1600
Wire Wire Line
	3850 3150 4350 3150
Wire Wire Line
	4650 2800 4650 2950
Wire Wire Line
	6550 2000 6550 2150
Wire Wire Line
	5750 1800 6250 1800
Wire Wire Line
	6550 1450 6550 1600
Wire Wire Line
	6500 3350 6500 3500
Wire Wire Line
	5700 3150 6200 3150
Wire Wire Line
	6500 2800 6500 2950
Wire Wire Line
	1800 1650 2350 1650
Wire Wire Line
	1800 1750 2050 1750
Wire Wire Line
	2050 1750 2050 1850
Wire Wire Line
	1800 3100 2250 3100
Wire Wire Line
	1800 2900 2250 2900
Wire Wire Line
	1800 2500 2250 2500
Wire Wire Line
	1800 3000 2050 3000
Wire Wire Line
	2050 2400 2050 3350
Wire Wire Line
	1800 2800 2050 2800
Connection ~ 2050 3000
Wire Wire Line
	1800 2600 2050 2600
Connection ~ 2050 2800
Wire Wire Line
	1800 2400 2050 2400
Connection ~ 2050 2600
Wire Wire Line
	2250 2700 1800 2700
Wire Wire Line
	8900 1450 9400 1450
Wire Wire Line
	8900 1550 9400 1550
Wire Wire Line
	9400 1650 8900 1650
Wire Wire Line
	8900 1750 9400 1750
Wire Wire Line
	4650 3500 4650 3350
Wire Wire Line
	2350 1650 2350 1850
Wire Wire Line
	3600 4600 3450 4600
Wire Wire Line
	3450 4450 3450 4800
Wire Wire Line
	3450 4700 3600 4700
Connection ~ 3450 4600
Wire Wire Line
	3450 4800 3600 4800
Connection ~ 3450 4700
Wire Wire Line
	4600 4800 4800 4800
Wire Wire Line
	4800 4800 4800 4400
Wire Wire Line
	4600 4600 4800 4600
Connection ~ 4800 4600
Wire Wire Line
	4600 4700 4800 4700
Connection ~ 4800 4700
Wire Wire Line
	8900 5350 9000 5350
Wire Wire Line
	9000 5350 9000 5450
Wire Wire Line
	4600 5200 5050 5200
Wire Wire Line
	4600 5300 4700 5300
Wire Wire Line
	3600 5300 3300 5300
Text Label 3300 5300 0    60   ~ 0
SDA
Wire Wire Line
	3600 5100 2950 5100
Wire Wire Line
	2950 5100 2950 5300
Wire Wire Line
	4600 5100 5450 5100
$Comp
L GND #PWR011
U 1 1 5A83A16B
P 5450 5550
F 0 "#PWR011" H 5450 5300 50  0001 C CNN
F 1 "GND" H 5450 5400 50  0000 C CNN
F 2 "" H 5450 5550 50  0001 C CNN
F 3 "" H 5450 5550 50  0001 C CNN
	1    5450 5550
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR012
U 1 1 5A83A195
P 2950 5300
F 0 "#PWR012" H 2950 5050 50  0001 C CNN
F 1 "GND" H 2950 5150 50  0000 C CNN
F 2 "" H 2950 5300 50  0001 C CNN
F 3 "" H 2950 5300 50  0001 C CNN
	1    2950 5300
	1    0    0    -1  
$EndComp
Wire Wire Line
	4600 5000 5650 5000
Wire Wire Line
	5650 4750 5650 5050
$Comp
L +BATT #PWR013
U 1 1 5A83A216
P 5650 4750
F 0 "#PWR013" H 5650 4600 50  0001 C CNN
F 1 "+BATT" H 5650 4890 50  0000 C CNN
F 2 "" H 5650 4750 50  0001 C CNN
F 3 "" H 5650 4750 50  0001 C CNN
	1    5650 4750
	1    0    0    -1  
$EndComp
$Comp
L R R1
U 1 1 5A83A243
P 3600 5450
F 0 "R1" V 3680 5450 50  0000 C CNN
F 1 "4.7k" V 3600 5450 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 3530 5450 50  0001 C CNN
F 3 "" H 3600 5450 50  0001 C CNN
	1    3600 5450
	-1   0    0    1   
$EndComp
$Comp
L R R2
U 1 1 5A83A2A7
P 4600 5450
F 0 "R2" V 4680 5450 50  0000 C CNN
F 1 "4.7k" V 4600 5450 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 4530 5450 50  0001 C CNN
F 3 "" H 4600 5450 50  0001 C CNN
	1    4600 5450
	-1   0    0    1   
$EndComp
Wire Wire Line
	5050 5600 3600 5600
Connection ~ 4600 5600
Connection ~ 5050 5600
$Comp
L C C1
U 1 1 5A83A54F
P 5250 5350
F 0 "C1" H 5275 5450 50  0000 L CNN
F 1 "0.1uF" H 5275 5250 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603_HandSoldering" H 5288 5200 50  0001 C CNN
F 3 "" H 5250 5350 50  0001 C CNN
	1    5250 5350
	0    1    1    0   
$EndComp
Wire Wire Line
	8900 4550 9200 4550
Wire Wire Line
	8900 4450 9200 4450
Text Label 9200 4550 0    60   ~ 0
SCL
Text Label 9200 4450 0    60   ~ 0
SDA
NoConn ~ 3600 4900
NoConn ~ 3600 5000
NoConn ~ 3600 5200
Wire Wire Line
	5100 5350 5050 5350
Connection ~ 5050 5350
Wire Wire Line
	5400 5350 5650 5350
Connection ~ 5450 5350
$Comp
L C C2
U 1 1 5A83B224
P 5650 5200
F 0 "C2" H 5675 5300 50  0000 L CNN
F 1 "0.1uF" H 5675 5100 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603_HandSoldering" H 5688 5050 50  0001 C CNN
F 3 "" H 5650 5200 50  0001 C CNN
	1    5650 5200
	-1   0    0    1   
$EndComp
Wire Wire Line
	5450 5100 5450 5550
Connection ~ 5650 5000
Wire Wire Line
	5050 5200 5050 5750
$EndSCHEMATC
