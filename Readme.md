Create impulsions (an impulsion is low logic level) on a pin of the uC. Pin is alternativelly low and high (period of 1 ms, duty-cycle adjusted to 10%). Uses interrupt (non blocking). Create 10000 pulses and then stops.

There is now an auto acceleration feature to test spectrometer capabilities.

On falling edge on PD2 (Arduino Digital #2) we toggle pin PB5 (Arduino Digital #13, with led). This is NOT debounced.

Define pin PB3 as output for Direction of the spectrometer. Set it to low (increase wavelength).

At present state, works on an Arduino NG (Atmega8 @ 16 MHz). The impultions pin is PB4 (Arduino pin 12).
