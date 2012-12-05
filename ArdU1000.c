// ATmega8 @ 16 MHz
// setup timer1 for 1 s clock cycle
// blink a led (pin 13 on Arduino)
//
//based on https://sites.google.com/site/qeewiki/books/avr-guide/timer-on-the-atmega8

#include <avr/io.h>
#include <avr/interrupt.h>

#define LED PB5 // Define led pin on PB5

int main(void)
{
    DDRB |= (1 << LED); // Set output on LED pin

    OCR1A = 15624;
    TCCR1B |= (1 << WGM12); // Mode 4, CTC on OCR1A
    TIMSK |= (1 << OCIE1A); //Set interrupt on compare match   
    TCCR1B |= (1 << CS12) | (1 << CS10); // set prescaler to 1024 and start the timer    

    sei(); // enable interrupts
    
    while (1);
    {
    }
}

ISR (TIMER1_COMPA_vect)
{
  PORTB^= _BV(LED); //toggle LED pin
}

