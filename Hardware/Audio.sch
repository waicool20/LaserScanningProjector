EESchema Schematic File Version 4
LIBS:LaserScanningProjector-cache
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 3
Title "Laser Scanning Projector"
Date ""
Rev "v0.1"
Comp "ELEC 3300 Group 26"
Comment1 "Mak Ching Hang David ( 20389036 )"
Comment2 "Wai Paulo Valerio Wang ( 20432417 )"
Comment3 ""
Comment4 ""
$EndDescr
Wire Notes Line
	5150 2400 2950 2400
Wire Notes Line
	5150 5250 5150 2400
Wire Notes Line
	2250 5250 5150 5250
Wire Notes Line
	2250 2500 2250 5250
Text Notes 2250 2450 0    50   ~ 0
Headphone Input
Wire Wire Line
	3050 4550 3300 4550
Wire Wire Line
	3050 3900 3050 4550
Wire Wire Line
	2850 3900 3050 3900
Wire Wire Line
	3050 3150 3300 3150
Wire Wire Line
	3050 3800 3050 3150
Wire Wire Line
	2850 3800 3050 3800
$Comp
L power:+3V3 #PWR?
U 1 1 5DC0762D
P 3700 4100
AR Path="/5DC0762D" Ref="#PWR?"  Part="1" 
AR Path="/5DBEF41C/5DC0762D" Ref="#PWR034"  Part="1" 
F 0 "#PWR034" H 3700 3950 50  0001 C CNN
F 1 "+3V3" H 3715 4273 50  0000 C CNN
F 2 "" H 3700 4100 50  0001 C CNN
F 3 "" H 3700 4100 50  0001 C CNN
	1    3700 4100
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5DC07633
P 4100 5000
AR Path="/5DC07633" Ref="#PWR?"  Part="1" 
AR Path="/5DBEF41C/5DC07633" Ref="#PWR037"  Part="1" 
F 0 "#PWR037" H 4100 4750 50  0001 C CNN
F 1 "GND" H 4105 4827 50  0000 C CNN
F 2 "" H 4100 5000 50  0001 C CNN
F 3 "" H 4100 5000 50  0001 C CNN
	1    4100 5000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5DC07639
P 3700 5000
AR Path="/5DC07639" Ref="#PWR?"  Part="1" 
AR Path="/5DBEF41C/5DC07639" Ref="#PWR035"  Part="1" 
F 0 "#PWR035" H 3700 4750 50  0001 C CNN
F 1 "GND" H 3705 4827 50  0000 C CNN
F 2 "" H 3700 5000 50  0001 C CNN
F 3 "" H 3700 5000 50  0001 C CNN
	1    3700 5000
	1    0    0    -1  
$EndComp
Wire Wire Line
	4100 4550 4450 4550
Connection ~ 4100 4550
Wire Wire Line
	4100 4700 4100 4550
Wire Wire Line
	3700 4550 4100 4550
Connection ~ 3700 4550
Wire Wire Line
	3700 4550 3700 4400
Wire Wire Line
	3700 4550 3700 4700
Wire Wire Line
	3600 4550 3700 4550
$Comp
L Device:C C?
U 1 1 5DC07647
P 4100 4850
AR Path="/5DC07647" Ref="C?"  Part="1" 
AR Path="/5DBEF41C/5DC07647" Ref="C20"  Part="1" 
F 0 "C20" H 3985 4804 50  0000 R CNN
F 1 "10n" H 3985 4895 50  0000 R CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 4138 4700 50  0001 C CNN
F 3 "~" H 4100 4850 50  0001 C CNN
	1    4100 4850
	-1   0    0    1   
$EndComp
$Comp
L Device:R R?
U 1 1 5DC0764D
P 3700 4850
AR Path="/5DC0764D" Ref="R?"  Part="1" 
AR Path="/5DBEF41C/5DC0764D" Ref="R17"  Part="1" 
F 0 "R17" H 3770 4896 50  0000 L CNN
F 1 "100k" H 3770 4805 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" V 3630 4850 50  0001 C CNN
F 3 "~" H 3700 4850 50  0001 C CNN
	1    3700 4850
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 5DC07653
P 3700 4250
AR Path="/5DC07653" Ref="R?"  Part="1" 
AR Path="/5DBEF41C/5DC07653" Ref="R16"  Part="1" 
F 0 "R16" H 3770 4296 50  0000 L CNN
F 1 "100k" H 3770 4205 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" V 3630 4250 50  0001 C CNN
F 3 "~" H 3700 4250 50  0001 C CNN
	1    3700 4250
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 5DC07659
P 3450 4550
AR Path="/5DC07659" Ref="C?"  Part="1" 
AR Path="/5DBEF41C/5DC07659" Ref="C18"  Part="1" 
F 0 "C18" V 3198 4550 50  0000 C CNN
F 1 "10u" V 3289 4550 50  0000 C CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 3488 4400 50  0001 C CNN
F 3 "~" H 3450 4550 50  0001 C CNN
	1    3450 4550
	0    1    1    0   
$EndComp
$Comp
L power:+3V3 #PWR?
U 1 1 5DC07660
P 3700 2700
AR Path="/5DC07660" Ref="#PWR?"  Part="1" 
AR Path="/5DBEF41C/5DC07660" Ref="#PWR032"  Part="1" 
F 0 "#PWR032" H 3700 2550 50  0001 C CNN
F 1 "+3V3" H 3715 2873 50  0000 C CNN
F 2 "" H 3700 2700 50  0001 C CNN
F 3 "" H 3700 2700 50  0001 C CNN
	1    3700 2700
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5DC07666
P 4100 3600
AR Path="/5DC07666" Ref="#PWR?"  Part="1" 
AR Path="/5DBEF41C/5DC07666" Ref="#PWR036"  Part="1" 
F 0 "#PWR036" H 4100 3350 50  0001 C CNN
F 1 "GND" H 4105 3427 50  0000 C CNN
F 2 "" H 4100 3600 50  0001 C CNN
F 3 "" H 4100 3600 50  0001 C CNN
	1    4100 3600
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5DC0766C
P 3700 3600
AR Path="/5DC0766C" Ref="#PWR?"  Part="1" 
AR Path="/5DBEF41C/5DC0766C" Ref="#PWR033"  Part="1" 
F 0 "#PWR033" H 3700 3350 50  0001 C CNN
F 1 "GND" H 3705 3427 50  0000 C CNN
F 2 "" H 3700 3600 50  0001 C CNN
F 3 "" H 3700 3600 50  0001 C CNN
	1    3700 3600
	1    0    0    -1  
$EndComp
Wire Wire Line
	4100 3150 4450 3150
Connection ~ 4100 3150
Wire Wire Line
	4100 3300 4100 3150
Wire Wire Line
	3700 3150 4100 3150
Connection ~ 3700 3150
Wire Wire Line
	3700 3150 3700 3000
Wire Wire Line
	3700 3150 3700 3300
Wire Wire Line
	3600 3150 3700 3150
$Comp
L Device:C C?
U 1 1 5DC0767A
P 4100 3450
AR Path="/5DC0767A" Ref="C?"  Part="1" 
AR Path="/5DBEF41C/5DC0767A" Ref="C19"  Part="1" 
F 0 "C19" H 3985 3404 50  0000 R CNN
F 1 "10n" H 3985 3495 50  0000 R CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 4138 3300 50  0001 C CNN
F 3 "~" H 4100 3450 50  0001 C CNN
	1    4100 3450
	-1   0    0    1   
$EndComp
$Comp
L Device:R R?
U 1 1 5DC07680
P 3700 3450
AR Path="/5DC07680" Ref="R?"  Part="1" 
AR Path="/5DBEF41C/5DC07680" Ref="R15"  Part="1" 
F 0 "R15" H 3770 3496 50  0000 L CNN
F 1 "100k" H 3770 3405 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" V 3630 3450 50  0001 C CNN
F 3 "~" H 3700 3450 50  0001 C CNN
	1    3700 3450
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 5DC07686
P 3700 2850
AR Path="/5DC07686" Ref="R?"  Part="1" 
AR Path="/5DBEF41C/5DC07686" Ref="R14"  Part="1" 
F 0 "R14" H 3770 2896 50  0000 L CNN
F 1 "100k" H 3770 2805 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" V 3630 2850 50  0001 C CNN
F 3 "~" H 3700 2850 50  0001 C CNN
	1    3700 2850
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 5DC0768C
P 3450 3150
AR Path="/5DC0768C" Ref="C?"  Part="1" 
AR Path="/5DBEF41C/5DC0768C" Ref="C17"  Part="1" 
F 0 "C17" V 3198 3150 50  0000 C CNN
F 1 "10u" V 3289 3150 50  0000 C CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 3488 3000 50  0001 C CNN
F 3 "~" H 3450 3150 50  0001 C CNN
	1    3450 3150
	0    1    1    0   
$EndComp
Wire Wire Line
	2900 4000 2900 4050
Wire Wire Line
	2850 4000 2900 4000
$Comp
L power:GND #PWR?
U 1 1 5DC07694
P 2900 4050
AR Path="/5DC07694" Ref="#PWR?"  Part="1" 
AR Path="/5DBEF41C/5DC07694" Ref="#PWR031"  Part="1" 
F 0 "#PWR031" H 2900 3800 50  0001 C CNN
F 1 "GND" H 2905 3877 50  0000 C CNN
F 2 "" H 2900 4050 50  0001 C CNN
F 3 "" H 2900 4050 50  0001 C CNN
	1    2900 4050
	1    0    0    -1  
$EndComp
$Comp
L Connector:AudioJack3 J?
U 1 1 5DC0769A
P 2650 3900
AR Path="/5DC0769A" Ref="J?"  Part="1" 
AR Path="/5DBEF41C/5DC0769A" Ref="J8"  Part="1" 
F 0 "J8" H 2500 3550 50  0000 R CNN
F 1 "HEADPHONES" H 2900 3650 50  0000 R CNN
F 2 "waicool20 - Connectors:JST_XH_B3B-XH-A_1x03_P2.50mm_Vertical_RST_AUDIO" H 2650 3900 50  0001 C CNN
F 3 "~" H 2650 3900 50  0001 C CNN
	1    2650 3900
	1    0    0    1   
$EndComp
$Comp
L Device:Microphone MK?
U 1 1 5DC1A46E
P 5550 3500
AR Path="/5DC1A46E" Ref="MK?"  Part="1" 
AR Path="/5DBEF41C/5DC1A46E" Ref="MK1"  Part="1" 
F 0 "MK1" H 5680 3546 50  0000 L CNN
F 1 "Mic" H 5680 3455 50  0000 L CNN
F 2 "Connector_JST:JST_XH_B2B-XH-A_1x02_P2.50mm_Vertical" V 5550 3600 50  0001 C CNN
F 3 "~" V 5550 3600 50  0001 C CNN
	1    5550 3500
	1    0    0    -1  
$EndComp
$Comp
L Amplifier_Operational:LM321 U?
U 1 1 5DC1A474
P 7050 3300
AR Path="/5DC1A474" Ref="U?"  Part="1" 
AR Path="/5DBEF41C/5DC1A474" Ref="U4"  Part="1" 
F 0 "U4" H 7100 3550 50  0000 L CNN
F 1 "LM321" H 7100 3450 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23-5" H 7050 3300 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/lm321.pdf" H 7050 3300 50  0001 C CNN
	1    7050 3300
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR?
U 1 1 5DC1A47A
P 6950 3000
AR Path="/5DC1A47A" Ref="#PWR?"  Part="1" 
AR Path="/5DBEF41C/5DC1A47A" Ref="#PWR041"  Part="1" 
F 0 "#PWR041" H 6950 2850 50  0001 C CNN
F 1 "+5V" H 6965 3173 50  0000 C CNN
F 2 "" H 6950 3000 50  0001 C CNN
F 3 "" H 6950 3000 50  0001 C CNN
	1    6950 3000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5DC1A480
P 6150 3700
AR Path="/5DC1A480" Ref="#PWR?"  Part="1" 
AR Path="/5DBEF41C/5DC1A480" Ref="#PWR040"  Part="1" 
F 0 "#PWR040" H 6150 3450 50  0001 C CNN
F 1 "GND" H 6155 3527 50  0000 C CNN
F 2 "" H 6150 3700 50  0001 C CNN
F 3 "" H 6150 3700 50  0001 C CNN
	1    6150 3700
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR?
U 1 1 5DC1A486
P 5550 2750
AR Path="/5DC1A486" Ref="#PWR?"  Part="1" 
AR Path="/5DBEF41C/5DC1A486" Ref="#PWR038"  Part="1" 
F 0 "#PWR038" H 5550 2600 50  0001 C CNN
F 1 "+3V3" H 5565 2923 50  0000 C CNN
F 2 "" H 5550 2750 50  0001 C CNN
F 3 "" H 5550 2750 50  0001 C CNN
	1    5550 2750
	1    0    0    -1  
$EndComp
Wire Wire Line
	5550 3300 5550 3200
$Comp
L power:GND #PWR?
U 1 1 5DC1A48D
P 5550 3700
AR Path="/5DC1A48D" Ref="#PWR?"  Part="1" 
AR Path="/5DBEF41C/5DC1A48D" Ref="#PWR039"  Part="1" 
F 0 "#PWR039" H 5550 3450 50  0001 C CNN
F 1 "GND" H 5555 3527 50  0000 C CNN
F 2 "" H 5550 3700 50  0001 C CNN
F 3 "" H 5550 3700 50  0001 C CNN
	1    5550 3700
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 5DC1A493
P 5900 3200
AR Path="/5DC1A493" Ref="C?"  Part="1" 
AR Path="/5DBEF41C/5DC1A493" Ref="C21"  Part="1" 
F 0 "C21" V 5648 3200 50  0000 C CNN
F 1 "10u" V 5739 3200 50  0000 C CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 5938 3050 50  0001 C CNN
F 3 "~" H 5900 3200 50  0001 C CNN
	1    5900 3200
	0    1    1    0   
$EndComp
$Comp
L Device:R R?
U 1 1 5DC1A499
P 6150 3550
AR Path="/5DC1A499" Ref="R?"  Part="1" 
AR Path="/5DBEF41C/5DC1A499" Ref="R19"  Part="1" 
F 0 "R19" H 6220 3596 50  0000 L CNN
F 1 "100k" H 6220 3505 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" V 6080 3550 50  0001 C CNN
F 3 "~" H 6150 3550 50  0001 C CNN
	1    6150 3550
	1    0    0    -1  
$EndComp
Wire Wire Line
	5750 3200 5550 3200
Wire Wire Line
	6150 3200 6050 3200
Wire Wire Line
	6150 3400 6150 3200
Connection ~ 6150 3200
$Comp
L power:GND #PWR?
U 1 1 5DC1A4A3
P 6950 3600
AR Path="/5DC1A4A3" Ref="#PWR?"  Part="1" 
AR Path="/5DBEF41C/5DC1A4A3" Ref="#PWR042"  Part="1" 
F 0 "#PWR042" H 6950 3350 50  0001 C CNN
F 1 "GND" H 6955 3427 50  0000 C CNN
F 2 "" H 6950 3600 50  0001 C CNN
F 3 "" H 6950 3600 50  0001 C CNN
	1    6950 3600
	1    0    0    -1  
$EndComp
Wire Wire Line
	6750 3200 6150 3200
$Comp
L Device:R R?
U 1 1 5DC1A4AA
P 5550 3000
AR Path="/5DC1A4AA" Ref="R?"  Part="1" 
AR Path="/5DBEF41C/5DC1A4AA" Ref="R18"  Part="1" 
F 0 "R18" H 5620 3046 50  0000 L CNN
F 1 "10k" H 5620 2955 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" V 5480 3000 50  0001 C CNN
F 3 "~" H 5550 3000 50  0001 C CNN
	1    5550 3000
	1    0    0    -1  
$EndComp
Wire Wire Line
	5550 3150 5550 3200
Connection ~ 5550 3200
Wire Wire Line
	5550 2850 5550 2750
$Comp
L Device:R_POT RV?
U 1 1 5DC1A4B3
P 7600 3600
AR Path="/5DC1A4B3" Ref="RV?"  Part="1" 
AR Path="/5DBEF41C/5DC1A4B3" Ref="RV2"  Part="1" 
F 0 "RV2" H 7530 3554 50  0000 R CNN
F 1 "10k" H 7530 3645 50  0000 R CNN
F 2 "Potentiometer_THT:Potentiometer_Bourns_3296W_Vertical" H 7600 3600 50  0001 C CNN
F 3 "~" H 7600 3600 50  0001 C CNN
	1    7600 3600
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5DC1A4B9
P 7600 3750
AR Path="/5DC1A4B9" Ref="#PWR?"  Part="1" 
AR Path="/5DBEF41C/5DC1A4B9" Ref="#PWR043"  Part="1" 
F 0 "#PWR043" H 7600 3500 50  0001 C CNN
F 1 "GND" H 7605 3577 50  0000 C CNN
F 2 "" H 7600 3750 50  0001 C CNN
F 3 "" H 7600 3750 50  0001 C CNN
	1    7600 3750
	1    0    0    -1  
$EndComp
Wire Wire Line
	7350 3300 7600 3300
Wire Wire Line
	7600 3300 7600 3450
Wire Wire Line
	7450 3600 7300 3600
Wire Wire Line
	7300 3600 7300 3850
Wire Wire Line
	7300 3850 6700 3850
Wire Wire Line
	6700 3850 6700 3400
Wire Wire Line
	6700 3400 6750 3400
Wire Wire Line
	7600 3300 7750 3300
Connection ~ 7600 3300
Text Notes 5400 2450 0    50   ~ 0
Microphone Input
Wire Notes Line
	5400 2500 5400 4000
Wire Notes Line
	5400 4000 8300 4000
Wire Notes Line
	8300 4000 8300 2400
Wire Notes Line
	8300 2400 6100 2400
Text HLabel 4450 3150 2    50   Output ~ 0
ADC_JACK_L
Text HLabel 4450 4550 2    50   Output ~ 0
ADC_JACK_R
Text HLabel 7750 3300 2    50   Output ~ 0
ADC_MIC
$EndSCHEMATC
