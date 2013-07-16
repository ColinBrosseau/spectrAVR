# SpectrAVR

Controls a spectrometer. At present time, controls either a Instrument ISA U1000 or a HR320.

Create impulsions (an impulsion is low logic level) on a pin of the uC. Pin is alternativelly low and high (variable period, duty-cycle adjusted at compile time). Uses interrupt (non blocking).

There is an auto acceleration feature to allow for motor speed up in spectrometers.

The uC board is the link between the computer (via USB-UART converter) and the spectrometer's controler.

## Spectrometers

* U1000

    * Pin PD6 (output)

      Direction of the spectrometer (low=increase wavelength, high=decrease wavelength,high-Z)
      High-Z is set when not in use. It allows to control the spectrometer with the external controler (from company) with out the need of an additional switch (like before)!

	* Pin PD5 (output)
	
      Impulsions

    * Pin PD2 (input)

     Input for impulsions (from external controler) to accurately account spectrometer's mouvement from the external controler.

* HR320

    * Pin PD5 (output)
     Impulsions (increase wavelength)

    * Pin PD6 (output)
     Impulsions (decrease wavelength)

## Microcontroller board

At present state, works on an Atmega16 @ 12 MHz. Could be tweeked to work on an Arduino.

## To program

* From command line

	* $ make

	* *reset the controller while holding the program button*

	* $ make program

	* *release the program button*

	* *reset the controller*

## TODO

Create tutorial/software repo that contains everything to transfert firmware to the board (linux and windows).
