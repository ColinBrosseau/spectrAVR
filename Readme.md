Create impulsions (an impulsion is low logic level) on a pin of the uC. Pin is alternativelly low and high (period of 1 ms, duty-cycle adjusted to 10%). Uses interrupt (non blocking). Create 10000 pulses and then stops.

There is now an auto acceleration feature to test spectrometer capabilities.

On falling edge on PD2 (Arduino Digital #2) we toggle pin PB5 (Arduino Digital #13, with led). This is NOT debounced.

Define pin PB3 as output for Direction of the spectrometer. Set it to low (increase wavelength).
-> It allows to control the spectrometer with the external controler (from company) with out the need of an additional switch (like before)!

At present state, works on an Arduino NG (Atmega8 @ 16 MHz). The impultions pin is PB4 (Arduino pin 12).

test_lcd.c
print things on the LCD

Arduino	     LCD

D2	     RS
D3	     R/W
D4	     bit 4
D5	     bit 5
D6	     bit 6
D7	     bit 7
D8	     Enable (Clk)