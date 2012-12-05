// ATmega8 @ 16 MHz
// setup timer1 for 0.5 s clock cycle
// blink a led (pin 13 on Arduino)
//
//based on https://sites.google.com/site/qeewiki/books/avr-guide/timer-on-the-atmega8

#include <avr/io.h>
#include <avr/interrupt.h>

#define LED PB5 // Define led pin on PB5
#define HIGH 1 //logic level high
#define LOW 0 //logic level low

#define setPulse(x) switchToDo = x*2 //set number of pulses to send 

int pulses = HIGH; //a pulse is when logic level goes to high
int switchToDo = 0; //number of LED state change remaining

int main(void)
{
  setPulse(10); //create 10 pulses on the led and then stops.
  
  DDRB |= (1 << LED); // Set output on LED pin
  //setup led at default level
  if (pulses){
    PORTB &= ~(1 << LED); // led goes low 
  }
  else {
    PORTB |= (1 << LED); // led goes high 
  }
  
  OCR1A = 14062; //at 0.9 s LED low
  OCR1B = 15624; //at 1 s LED high (pulse duration = 0.1 s)
  TCCR1A |= (1 << WGM12); // Mode 4, CTC on OCR1A
  TCCR1B |= (1 << WGM12); // Mode 4, CTC on OCR1B
  TIMSK |= (1 << OCIE1A); //Set interrupt on compare match   
  TIMSK |= (1 << OCIE1B); //Set interrupt on compare match   
  TCCR1A |= (1 << CS12) | (1 << CS10); // set prescaler to 1024 and start the timer    
  TCCR1B |= (1 << CS12) | (1 << CS10); // set prescaler to 1024 and start the timer    
  
  sei(); // enable interrupts
  
  while (1);
  {
  }
}

ISR (TIMER1_COMPA_vect)
{
  if (switchToDo > 0) {
    PORTB^= _BV(LED); //toggle LED pin 
  }
}

ISR (TIMER1_COMPB_vect)
{
  if (switchToDo > 0) {
    switchToDo -= 1;
    PORTB^= _BV(LED); //toggle LED pin 
  }
}
