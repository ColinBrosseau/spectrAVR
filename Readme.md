To use

make
<reset the controller while holding the program button>
make program
<release the program button>
<reset the controller>

------

Create impulsions (an impulsion is low logic level) on a pin of the uC. Pin is alternativelly low and high (variable period, duty-cycle adjusted to 10%). Uses interrupt (non blocking). Create 10000 pulses and then stops.

There is now an auto acceleration feature to test spectrometer capabilities.

On falling edge on PD2 we toggle pin PA0. This is NOT debounced.

Define pin PD6 as output for Direction of the spectrometer. Set it to low (increase wavelength).
-> It allows to control the spectrometer with the external controler (from company) with out the need of an additional switch (like before)!

At present state, works on an Atmega16 @ 12 MHz). The impultions pin is PD5.

test_lcd.c
print things on the LCD

ATmega16     LCD

PC3	     RS
PD7	     R/W
PC4	     bit 4
PC5	     bit 5
PC6	     bit 6
PC7	     bit 7
PC2	     Enable (Clk)