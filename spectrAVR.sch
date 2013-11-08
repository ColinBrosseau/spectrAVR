v 20121203 2
C 40000 40000 0 0 0 title-B.sym
C 45900 41800 1 0 0 ATmega16L-1.sym
{
T 48900 50400 5 10 0 1 0 6 1
refdes=U?
T 46300 50550 5 10 0 0 0 0 1
device=ATmega16L_DIP
T 46300 50750 5 10 0 0 0 0 1
footprint=DIP40
T 46300 51950 5 10 0 0 0 0 1
symversion=2.0
}
C 43600 45800 1 270 0 crystal-1.sym
{
T 43900 45600 5 10 0 2 270 0 1
refdes=U?
T 44300 45600 5 10 0 1 270 0 1
symversion=0.1
T 43900 45800 5 10 1 1 270 0 1
value=12 MHz
}
C 45500 45600 1 0 0 ground.sym
N 46000 45900 45700 45900 4
C 42800 44900 1 0 0 capacitor-1.sym
{
T 43000 45600 5 10 0 0 0 0 1
device=CAPACITOR
T 43000 45400 5 10 1 1 0 0 1
refdes=20p
T 43000 45800 5 10 0 0 0 0 1
symversion=0.1
}
C 42800 45600 1 0 0 capacitor-1.sym
{
T 43000 46300 5 10 0 0 0 0 1
device=CAPACITOR
T 43000 46100 5 10 1 1 0 0 1
refdes=20p
T 43000 46500 5 10 0 0 0 0 1
symversion=0.1
}
N 42800 45100 42800 45800 4
N 43700 45100 46000 45100 4
C 44400 49900 1 0 0 switch-pushbutton-no-1.sym
{
T 44800 50200 5 10 1 1 0 0 1
refdes=PROGRAM
T 44800 50500 5 10 0 0 0 0 1
device=SWITCH_PUSHBUTTON_NO
}
C 40500 46700 1 0 0 switch-pushbutton-no-1.sym
{
T 40900 47000 5 10 1 1 0 0 1
refdes=RESET
T 40900 47300 5 10 0 0 0 0 1
device=SWITCH_PUSHBUTTON_NO
}
C 44200 49600 1 0 0 ground.sym
C 49300 46000 1 0 0 ground.sym
N 49200 45100 49800 45100 4
{
T 49300 45150 5 10 1 1 0 0 1
netname=D2
}
N 49200 43500 49800 43500 4
{
T 49300 43550 5 10 1 1 0 0 1
netname=RS
}
N 49200 43900 49800 43900 4
{
T 49300 43950 5 10 1 1 0 0 1
netname=E
}
N 49200 44300 49800 44300 4
{
T 49300 44350 5 10 1 1 0 0 1
netname=D0
}
N 49200 44700 49800 44700 4
{
T 49300 44750 5 10 1 1 0 0 1
netname=D1
}
C 50500 48200 1 0 0 EMBEDDEDHD44780_con-1.sym
[
P 50700 48200 50700 48500 1 0 0
{
T 50650 48400 5 8 1 1 90 6 1
pinnumber=1
T 50750 48400 5 8 0 1 90 8 1
pinseq=1
T 50700 48550 9 8 1 1 90 0 1
pinlabel=GND
T 50700 48550 5 8 0 1 90 2 1
pintype=pwr
}
P 51100 48200 51100 48500 1 0 0
{
T 51050 48400 5 8 1 1 90 6 1
pinnumber=2
T 51150 48400 5 8 0 1 90 8 1
pinseq=2
T 51100 48550 9 8 1 1 90 0 1
pinlabel=Vcc
T 51100 48550 5 8 0 1 90 2 1
pintype=pwr
}
P 51500 48200 51500 48500 1 0 0
{
T 51450 48400 5 8 1 1 90 6 1
pinnumber=3
T 51550 48400 5 8 0 1 90 8 1
pinseq=3
T 51500 48550 9 8 1 1 90 0 1
pinlabel=Vo
T 51500 48550 5 8 0 1 90 2 1
pintype=pwr
}
P 51900 48200 51900 48500 1 0 0
{
T 51850 48400 5 8 1 1 90 6 1
pinnumber=4
T 51950 48400 5 8 0 1 90 8 1
pinseq=4
T 51900 48550 9 8 1 1 90 0 1
pinlabel=RS
T 51900 48550 5 8 0 1 90 2 1
pintype=in
}
P 52300 48200 52300 48500 1 0 0
{
T 52250 48400 5 8 1 1 90 6 1
pinnumber=5
T 52350 48400 5 8 0 1 90 8 1
pinseq=5
T 52300 48550 9 8 1 1 90 0 1
pinlabel=R/W
T 52300 48550 5 8 0 1 90 2 1
pintype=in
}
L 52176 48726 52176 48874 3 0 0 0 -1 -1
P 52700 48200 52700 48500 1 0 0
{
T 52650 48400 5 8 1 1 90 6 1
pinnumber=6
T 52750 48400 5 8 0 1 90 8 1
pinseq=6
T 52700 48550 9 8 1 1 90 0 1
pinlabel=E
T 52700 48550 5 8 0 1 90 2 1
pintype=in
}
P 53100 48200 53100 48500 1 0 0
{
T 53050 48400 5 8 1 1 90 6 1
pinnumber=7
T 53150 48400 5 8 0 1 90 8 1
pinseq=7
T 53100 48550 9 8 1 1 90 0 1
pinlabel=D0
T 53100 48550 5 8 0 1 90 2 1
pintype=io
}
P 53500 48200 53500 48500 1 0 0
{
T 53450 48400 5 8 1 1 90 6 1
pinnumber=8
T 53550 48400 5 8 0 1 90 8 1
pinseq=8
T 53500 48550 9 8 1 1 90 0 1
pinlabel=D1
T 53500 48550 5 8 0 1 90 2 1
pintype=io
}
P 53900 48200 53900 48500 1 0 0
{
T 53850 48400 5 8 1 1 90 6 1
pinnumber=9
T 53950 48400 5 8 0 1 90 8 1
pinseq=9
T 53900 48550 9 8 1 1 90 0 1
pinlabel=D2
T 53900 48550 5 8 0 1 90 2 1
pintype=io
}
P 54300 48200 54300 48500 1 0 0
{
T 54250 48400 5 8 1 1 90 6 1
pinnumber=10
T 54350 48400 5 8 0 1 90 8 1
pinseq=10
T 54300 48550 9 8 1 1 90 0 1
pinlabel=D3
T 54300 48550 5 8 0 1 90 2 1
pintype=io
}
P 54700 48200 54700 48500 1 0 0
{
T 54650 48400 5 8 1 1 90 6 1
pinnumber=11
T 54750 48400 5 8 0 1 90 8 1
pinseq=11
T 54700 48550 9 8 1 1 90 0 1
pinlabel=D4
T 54700 48550 5 8 0 1 90 2 1
pintype=io
}
P 55100 48200 55100 48500 1 0 0
{
T 55050 48400 5 8 1 1 90 6 1
pinnumber=12
T 55150 48400 5 8 0 1 90 8 1
pinseq=12
T 55100 48550 9 8 1 1 90 0 1
pinlabel=D5
T 55100 48550 5 8 0 1 90 2 1
pintype=io
}
P 55500 48200 55500 48500 1 0 0
{
T 55450 48400 5 8 1 1 90 6 1
pinnumber=13
T 55550 48400 5 8 0 1 90 8 1
pinseq=13
T 55500 48550 9 8 1 1 90 0 1
pinlabel=D6
T 55500 48550 5 8 0 1 90 2 1
pintype=io
}
P 55900 48200 55900 48500 1 0 0
{
T 55850 48400 5 8 1 1 90 6 1
pinnumber=14
T 55950 48400 5 8 0 1 90 8 1
pinseq=14
T 55900 48550 9 8 1 1 90 0 1
pinlabel=D7
T 55900 48550 5 8 0 1 90 2 1
pintype=io
}
P 56300 48200 56300 48500 1 0 0
{
T 56250 48400 5 8 1 1 90 6 1
pinnumber=15
T 56350 48400 5 8 0 1 90 8 1
pinseq=15
T 56300 48550 9 8 1 1 90 0 1
pinlabel=LED+
T 56300 48550 5 8 0 1 90 2 1
pintype=pas
}
P 56700 48200 56700 48500 1 0 0
{
T 56650 48400 5 8 1 1 90 6 1
pinnumber=16
T 56750 48400 5 8 0 1 90 8 1
pinseq=16
T 56700 48550 9 8 1 1 90 0 1
pinlabel=LED-
T 56700 48550 5 8 0 1 90 2 1
pintype=pas
}
B 50500 48500 6400 500 3 0 0 0 -1 -1 0 -1 -1 -1 -1 -1
T 56900 49100 8 10 0 1 0 6 1
refdes=X?
T 50500 49050 9 10 1 0 0 0 1
HD44780
T 50500 49250 5 10 0 0 0 0 1
device=HD44780_con
T 50500 49450 5 10 0 0 0 0 1
footprint=non
T 50500 49650 5 10 0 0 0 0 1
numslots=0
T 50500 49850 5 10 0 0 0 0 1
description=LCD-display connector for HD44780 and compatible display controllers
]
{
T 56900 49100 5 10 0 2 0 6 1
refdes=X?
}
U 51800 47700 56100 47700 10 1
N 54700 48200 54700 47900 4
{
T 54650 47900 5 10 1 1 90 0 1
netname=D0
}
C 54700 47900 1 270 0 busripper-1.sym
C 55100 47900 1 270 0 busripper-1.sym
N 55100 48200 55100 47900 4
{
T 55050 47900 5 10 1 1 90 0 1
netname=D1
}
C 55500 47900 1 270 0 busripper-1.sym
N 55500 48200 55500 47900 4
{
T 55450 47900 5 10 1 1 90 0 1
netname=D2
}
C 55900 47900 1 270 0 busripper-1.sym
N 55900 48200 55900 47900 4
{
T 55850 47900 5 10 1 1 90 0 1
netname=D3
}
C 50600 47900 1 0 0 gnd-1.sym
N 52700 48200 52700 47900 4
{
T 52650 47900 5 10 1 1 90 0 1
netname=E
}
C 52700 47900 1 270 0 busripper-1.sym
C 51900 47900 1 270 0 busripper-1.sym
N 51100 47300 51100 48200 4
N 49200 45500 49800 45500 4
{
T 49300 45550 5 10 0 1 0 0 1
netname=D3
T 49300 45550 5 10 1 1 0 0 1
netname=D3
}
U 49800 45500 49800 42300 10 0
C 52300 47900 1 270 0 busripper-1.sym
{
T 52700 47900 5 8 0 0 270 0 1
device=none
}
N 52300 48200 52300 47900 4
{
T 52250 47900 5 10 1 1 90 0 1
netname=RW
}
N 45400 49900 46000 49900 4
C 40600 47500 1 0 0 resistor-1.sym
{
T 40900 47900 5 10 0 0 0 0 1
device=RESISTOR
T 40800 47800 5 10 1 1 0 0 1
refdes=10K
}
N 41500 46700 46000 46700 4
N 41500 47600 41500 46700 4
C 40300 47600 1 0 0 5V-plus-1.sym
N 46000 46300 45700 46300 4
C 40300 46400 1 0 0 ground.sym
C 42600 44800 1 0 0 ground.sym
N 49200 46300 49500 46300 4
C 49200 49800 1 0 0 led-2.sym
{
T 50000 50100 5 10 0 2 0 0 1
refdes=LED1
T 49300 50400 5 10 0 0 0 0 1
device=LED
}
C 50100 49800 1 0 0 resistor-1.sym
{
T 50400 50200 5 10 0 0 0 0 1
device=RESISTOR
T 50300 50100 5 10 1 1 0 0 1
refdes=220R
}
C 50800 49600 1 0 0 ground.sym
N 45200 42300 45200 49100 4
N 46000 43100 45200 43100 4
N 45200 42300 46000 42300 4
{
T 45500 42350 5 10 1 1 0 0 1
netname=1
}
N 45200 49100 46000 49100 4
N 45500 42700 45500 43900 4
N 45500 43900 46000 43900 4
C 42300 41300 1 0 1 output-2.sym
{
T 41400 41500 5 10 1 1 0 6 1
net=CLEAR = H
T 42100 42000 5 10 0 0 0 6 1
device=none
T 41400 41400 5 10 1 1 0 7 1
value=GROUND
}
C 42300 40800 1 0 1 output-2.sym
{
T 41400 41000 5 10 1 1 0 6 1
net=RED = E
T 42100 41500 5 10 0 0 0 6 1
device=none
T 41400 40900 5 10 1 1 0 7 1
value=FORWARD
}
C 42300 40300 1 0 1 output-2.sym
{
T 41400 40500 5 10 1 1 0 6 1
net=BLACK = D
T 42100 41000 5 10 0 0 0 6 1
device=none
T 41400 40400 5 10 1 1 0 7 1
value=REVERSE
}
C 42300 43400 1 0 1 output-2.sym
{
T 42100 44100 5 10 0 0 0 6 1
device=none
T 41400 43600 5 10 1 0 0 6 1
pinlabel=2
T 41400 43500 5 10 1 1 0 7 1
value=GROUND
}
C 42300 42900 1 0 1 output-2.sym
{
T 42100 43600 5 10 0 0 0 6 1
device=none
T 41400 43100 5 10 1 0 0 6 1
pinlabel=3
T 41400 43000 5 10 1 1 0 7 1
value=Pulses / Clock
}
C 42300 42400 1 0 1 output-2.sym
{
T 42100 43100 5 10 0 0 0 6 1
device=none
T 41400 42600 5 10 1 0 0 6 1
pinlabel=1
T 41400 42500 5 10 1 1 0 7 1
value=DIRECTION
}
C 42600 41100 1 0 0 ground.sym
C 42600 43200 1 0 0 ground.sym
N 42400 42200 42400 43900 4
N 42400 43900 40100 43900 4
{
T 42300 43950 5 10 1 1 0 6 1
device=U1000
}
N 40100 43900 40100 42200 4
N 40100 42200 42400 42200 4
U 42800 43000 42800 42500 10 0
N 42800 43000 42300 43000 4
{
T 42700 43050 5 10 1 1 0 6 1
netname=2
}
N 42300 42500 42800 42500 4
{
T 42400 42550 5 10 1 1 0 0 1
netname=1
}
C 44500 41800 1 0 1 switch-spdt-1.sym
{
T 44100 42600 5 10 0 0 0 6 1
device=SPDT
T 44100 42400 5 10 1 1 0 6 1
refdes=Spectrometer
}
U 43600 42200 42800 42200 10 0
U 42800 42200 42800 42500 10 0
U 43600 41800 43600 41100 10 0
N 42800 43500 42300 43500 4
N 42400 40100 42400 41800 4
N 42400 41800 40100 41800 4
{
T 42300 41850 5 10 1 1 0 6 1
device=HR320
}
N 40100 41800 40100 40100 4
N 40100 40100 42400 40100 4
U 42800 40900 42800 40400 10 0
N 42800 40900 42300 40900 4
{
T 42700 40950 5 10 1 1 0 6 1
netname=2
}
N 42300 40400 42800 40400 4
{
T 42400 40450 5 10 1 1 0 0 1
netname=1
}
N 42800 41400 42300 41400 4
U 42800 40900 43600 40900 10 0
U 43600 40900 43600 41100 10 0
N 40500 47600 40600 47600 4
C 49900 46500 1 270 1 5V-plus-1.sym
N 49900 46700 49200 46700 4
N 49900 45900 49200 45900 4
N 51900 48200 51900 47900 4
{
T 51850 47900 5 10 1 1 90 0 1
netname=RS
}
U 50600 42300 51600 42300 10 0
U 51600 42300 51900 42300 10 0
U 51900 42300 51900 47700 10 0
N 46000 42700 45500 42700 4
{
T 45900 42750 5 10 1 1 0 6 1
netname=2
}
U 44500 42000 45700 42000 10 0
U 45700 42000 45700 42700 10 0
N 49200 42300 49800 42300 4
{
T 49300 42350 5 10 1 1 0 0 1
netname=RW
}
U 50600 42300 49800 42300 10 0
C 56800 47300 1 90 0 resistor-1.sym
{
T 56400 47600 5 10 0 0 90 0 1
device=RESISTOR
T 56500 47500 5 10 1 1 90 0 1
refdes=47R
}
C 55800 46900 1 0 0 5V-plus-1.sym
C 56600 47000 1 0 0 gnd-1.sym
N 56300 48200 56300 46900 4
N 56300 46900 56000 46900 4
C 51000 46400 1 270 1 resistor-variable-1.sym
{
T 51900 47200 5 10 0 0 90 2 1
device=VARIABLE_RESISTOR
T 51400 47000 5 10 1 1 90 2 1
refdes=10K
}
C 51000 46100 1 0 0 gnd-1.sym
N 50800 47300 51100 47300 4
C 50800 47100 1 90 0 5V-plus-1.sym
C 45700 46100 1 90 0 5V-plus-1.sym
C 49900 45700 1 270 1 5V-plus-1.sym
N 43700 45800 44200 45800 4
N 44200 45800 44200 45500 4
N 46000 45500 44200 45500 4
N 46000 44700 40700 44700 4
N 46000 44300 40700 44300 4
N 51500 48200 51500 47400 4
N 51500 47400 51600 47400 4
N 51600 47400 51600 46900 4
N 40400 44900 41200 44900 4
{
T 40500 44950 5 10 1 1 0 0 1
netname=PC
}
N 41200 44900 41200 44700 4
N 41200 44700 41200 44300 4
N 41200 44300 41200 44100 4
N 41200 44100 40400 44100 4
N 40400 44100 40400 44900 4