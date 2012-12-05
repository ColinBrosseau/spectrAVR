Create impulsions (an impulsion is low logic level) on a pin of the uC. Pin is alternativelly low and high (period of 0.2 s, duty-cycle adjusted to 10%). Uses interrupt (non blocking). Create 10 pulses and then stops.

At present state, works on an Arduino NG (Atmega8 @ 16 MHz). The pin is PB4 (Arduino pin 12).