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
LIBS:conn_boosterpack-cache
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
P 1900 2800
F 0 "J1" H 1900 3200 50  0000 C CNN
F 1 "Conn_01x08" H 1900 2300 50  0000 C CNN
F 2 "Connectors_Molex:Molex_Microfit3_Header_02x04_Angled_43045-080x" H 1900 2800 50  0001 C CNN
F 3 "" H 1900 2800 50  0001 C CNN
	1    1900 2800
	-1   0    0    1   
$EndComp
$Comp
L GND #PWR01
U 1 1 5A734483
P 2200 3200
F 0 "#PWR01" H 2200 2950 50  0001 C CNN
F 1 "GND" H 2200 3050 50  0000 C CNN
F 2 "" H 2200 3200 50  0001 C CNN
F 3 "" H 2200 3200 50  0001 C CNN
	1    2200 3200
	1    0    0    -1  
$EndComp
$Comp
L +3V3 #PWR02
U 1 1 5A734499
P 2400 3200
F 0 "#PWR02" H 2400 3050 50  0001 C CNN
F 1 "+3V3" H 2400 3340 50  0000 C CNN
F 2 "" H 2400 3200 50  0001 C CNN
F 3 "" H 2400 3200 50  0001 C CNN
	1    2400 3200
	-1   0    0    1   
$EndComp
Wire Wire Line
	2100 3100 2200 3100
Wire Wire Line
	2200 3000 2200 3200
Wire Wire Line
	2100 3000 2200 3000
Connection ~ 2200 3100
Wire Wire Line
	2100 2900 2400 2900
Wire Wire Line
	2400 2800 2400 3200
Wire Wire Line
	2100 2800 2400 2800
Connection ~ 2400 2900
Wire Wire Line
	2100 2600 2400 2600
Wire Wire Line
	2100 2500 2400 2500
Wire Wire Line
	2100 2400 2400 2400
Text Label 2400 2700 0    60   ~ 0
SCL
Text Label 2400 2600 0    60   ~ 0
SDA
Text Label 2400 2400 0    60   ~ 0
RST
Wire Wire Line
	2100 2700 2400 2700
Text Label 2400 2500 0    60   ~ 0
TRIG
$Comp
L Conn_01x08 J2
U 1 1 5A734646
P 1900 4300
F 0 "J2" H 1900 4700 50  0000 C CNN
F 1 "Conn_01x08" H 1900 3800 50  0000 C CNN
F 2 "Connectors_Molex:Molex_Microfit3_Header_02x04_Angled_43045-080x" H 1900 4300 50  0001 C CNN
F 3 "" H 1900 4300 50  0001 C CNN
	1    1900 4300
	-1   0    0    1   
$EndComp
$Comp
L GND #PWR03
U 1 1 5A73464C
P 2200 4700
F 0 "#PWR03" H 2200 4450 50  0001 C CNN
F 1 "GND" H 2200 4550 50  0000 C CNN
F 2 "" H 2200 4700 50  0001 C CNN
F 3 "" H 2200 4700 50  0001 C CNN
	1    2200 4700
	1    0    0    -1  
$EndComp
$Comp
L +3V3 #PWR04
U 1 1 5A734652
P 2400 4700
F 0 "#PWR04" H 2400 4550 50  0001 C CNN
F 1 "+3V3" H 2400 4840 50  0000 C CNN
F 2 "" H 2400 4700 50  0001 C CNN
F 3 "" H 2400 4700 50  0001 C CNN
	1    2400 4700
	-1   0    0    1   
$EndComp
Wire Wire Line
	2100 4600 2200 4600
Wire Wire Line
	2200 4500 2200 4700
Wire Wire Line
	2100 4500 2200 4500
Connection ~ 2200 4600
Wire Wire Line
	2100 4400 2400 4400
Wire Wire Line
	2400 4300 2400 4700
Wire Wire Line
	2100 4300 2400 4300
Connection ~ 2400 4400
Wire Wire Line
	2100 4100 2400 4100
Wire Wire Line
	2100 4000 2400 4000
Wire Wire Line
	2100 3900 2400 3900
Text Label 2400 4200 0    60   ~ 0
SCL
Text Label 2400 4100 0    60   ~ 0
SDA
Text Label 2400 3900 0    60   ~ 0
RST
Wire Wire Line
	2100 4200 2400 4200
Text Label 2400 4000 0    60   ~ 0
TRIG
$Comp
L Conn_01x40 J3
U 1 1 5A73471C
P 8800 3350
F 0 "J3" H 8800 5350 50  0000 C CNN
F 1 "Conn_01x40" H 8800 1250 50  0000 C CNN
F 2 "capstone_board_components:ti_boosterpack_40pin" H 8800 3350 50  0001 C CNN
F 3 "" H 8800 3350 50  0001 C CNN
	1    8800 3350
	-1   0    0    1   
$EndComp
Text Notes 8350 5800 0    60   ~ 0
Pins are in order of the\n40-pin Booster Pack pinout
$Comp
L +3V3 #PWR05
U 1 1 5A7347B6
P 9300 5350
F 0 "#PWR05" H 9300 5200 50  0001 C CNN
F 1 "+3V3" H 9300 5490 50  0000 C CNN
F 2 "" H 9300 5350 50  0001 C CNN
F 3 "" H 9300 5350 50  0001 C CNN
	1    9300 5350
	-1   0    0    1   
$EndComp
$Comp
L GND #PWR06
U 1 1 5A7347F0
P 9300 3450
F 0 "#PWR06" H 9300 3200 50  0001 C CNN
F 1 "GND" H 9300 3300 50  0000 C CNN
F 2 "" H 9300 3450 50  0001 C CNN
F 3 "" H 9300 3450 50  0001 C CNN
	1    9300 3450
	1    0    0    -1  
$EndComp
Wire Wire Line
	9000 5250 9300 5250
Wire Wire Line
	9000 3350 9300 3350
Wire Wire Line
	9300 3150 9300 3450
Wire Wire Line
	9000 3150 9300 3150
Connection ~ 9300 3350
Wire Wire Line
	9000 3750 9450 3750
Text Label 9450 3750 0    60   ~ 0
RST
Text Label 9450 4850 0    60   ~ 0
GPIO_P5
Wire Wire Line
	9300 5250 9300 5350
Wire Wire Line
	9000 4450 9450 4450
Wire Wire Line
	9000 4350 9450 4350
Text Label 9450 4450 0    60   ~ 0
UCB1_SCL
Text Label 9450 4350 0    60   ~ 0
UCB1_SDA
Wire Wire Line
	9550 1750 9000 1750
Text Label 9550 1750 0    60   ~ 0
UCB3_SDA
Wire Wire Line
	9000 1850 9550 1850
Text Label 9550 1850 0    60   ~ 0
UCB3_SCL
Text Notes 10050 4400 0    60   ~ 0
BoosterPack "I2C"
Wire Wire Line
	9000 2250 9550 2250
Text Label 9450 4200 0    60   ~ 0
UCB2_SDA
Wire Wire Line
	9000 4250 9450 4250
Wire Notes Line
	10000 4250 10000 4450
Wire Wire Line
	9450 4250 9450 4200
Text Label 9550 2250 0    60   ~ 0
UCB2_SCL
Wire Wire Line
	9450 4850 9000 4850
$Comp
L Jumper JP1
U 1 1 5A790ADD
P 5450 2150
F 0 "JP1" H 5450 2300 50  0000 C CNN
F 1 "Jumper" H 5450 2070 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x02_Pitch2.54mm" H 5450 2150 50  0001 C CNN
F 3 "" H 5450 2150 50  0001 C CNN
	1    5450 2150
	1    0    0    -1  
$EndComp
$Comp
L Jumper JP2
U 1 1 5A790BF2
P 5450 2500
F 0 "JP2" H 5450 2650 50  0000 C CNN
F 1 "Jumper" H 5450 2420 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x02_Pitch2.54mm" H 5450 2500 50  0001 C CNN
F 3 "" H 5450 2500 50  0001 C CNN
	1    5450 2500
	1    0    0    -1  
$EndComp
Text Label 4950 2500 0    60   ~ 0
SDA
Wire Wire Line
	4950 2500 5150 2500
Text Label 4950 2150 0    60   ~ 0
SCL
Wire Wire Line
	4950 2150 5150 2150
Wire Wire Line
	5750 2150 5900 2150
Wire Wire Line
	5750 2500 5900 2500
$Comp
L Jumper JP3
U 1 1 5A791098
P 5450 3250
F 0 "JP3" H 5450 3400 50  0000 C CNN
F 1 "Jumper" H 5450 3170 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x02_Pitch2.54mm" H 5450 3250 50  0001 C CNN
F 3 "" H 5450 3250 50  0001 C CNN
	1    5450 3250
	1    0    0    -1  
$EndComp
$Comp
L Jumper JP4
U 1 1 5A79109E
P 5450 3600
F 0 "JP4" H 5450 3750 50  0000 C CNN
F 1 "Jumper" H 5450 3520 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x02_Pitch2.54mm" H 5450 3600 50  0001 C CNN
F 3 "" H 5450 3600 50  0001 C CNN
	1    5450 3600
	1    0    0    -1  
$EndComp
Text Label 4950 3600 0    60   ~ 0
SDA
Wire Wire Line
	4950 3600 5150 3600
Text Label 4950 3250 0    60   ~ 0
SCL
Wire Wire Line
	4950 3250 5150 3250
Wire Wire Line
	5750 3250 5900 3250
Wire Wire Line
	5750 3600 5900 3600
$Comp
L Jumper JP5
U 1 1 5A791198
P 5450 4250
F 0 "JP5" H 5450 4400 50  0000 C CNN
F 1 "Jumper" H 5450 4170 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x02_Pitch2.54mm" H 5450 4250 50  0001 C CNN
F 3 "" H 5450 4250 50  0001 C CNN
	1    5450 4250
	1    0    0    -1  
$EndComp
$Comp
L Jumper JP6
U 1 1 5A79119E
P 5450 4600
F 0 "JP6" H 5450 4750 50  0000 C CNN
F 1 "Jumper" H 5450 4520 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x02_Pitch2.54mm" H 5450 4600 50  0001 C CNN
F 3 "" H 5450 4600 50  0001 C CNN
	1    5450 4600
	1    0    0    -1  
$EndComp
Text Label 4950 4600 0    60   ~ 0
SDA
Wire Wire Line
	4950 4600 5150 4600
Text Label 4950 4250 0    60   ~ 0
SCL
Wire Wire Line
	4950 4250 5150 4250
Wire Wire Line
	5750 4250 5900 4250
Wire Wire Line
	5750 4600 5900 4600
$Comp
L Jumper JP7
U 1 1 5A7912B7
P 5450 5200
F 0 "JP7" H 5450 5350 50  0000 C CNN
F 1 "Jumper" H 5450 5120 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x02_Pitch2.54mm" H 5450 5200 50  0001 C CNN
F 3 "" H 5450 5200 50  0001 C CNN
	1    5450 5200
	1    0    0    -1  
$EndComp
Text Label 4950 5200 0    60   ~ 0
TRIG
Wire Wire Line
	4950 5200 5150 5200
Wire Wire Line
	5750 5200 5900 5200
Text Label 5900 5200 0    60   ~ 0
GPIO_P5
Text Label 5900 2500 0    60   ~ 0
UCB1_SDA
Text Label 5900 2150 0    60   ~ 0
UCB1_SCL
Text Label 5900 3600 0    60   ~ 0
UCB2_SDA
Text Label 5900 3250 0    60   ~ 0
UCB2_SCL
Text Label 5900 4600 0    60   ~ 0
UCB3_SDA
Text Label 5900 4250 0    60   ~ 0
UCB3_SCL
$EndSCHEMATC