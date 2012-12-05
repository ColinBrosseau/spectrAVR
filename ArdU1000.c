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
#define DUTY 10 //duty cycle (in %)

int pulses = HIGH; //a pulse is when logic level goes to low
int switchToDo = 0; //number of LED state change remaining
int pulseDuration; //duration of the pulse (timer units)
int period = 3906; //duration between pulses (timer units) (0.2 s)

#define setPulse(x) switchToDo = x*2 //set number of pulses to send 
#define setPulseDuration(x) pulseDuration = period*DUTY/100;

//void setPulseDuration(int period)
//{
//  pulseDuration = period*DUTY;
// }

int main(void)
{
  setPulse(10);
  setPulseDuration(period);
  
  DDRB |= (1 << LED); // Set output on LED pin
  //setup led at default level
  if (pulses)
    PORTB &= ~(1 << LED); // led goes low 
  else 
    PORTB |= (1 << LED); // led goes high 
  
  OCR1A = 7812; //0.5 s per pulse
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
  if (switchToDo % 2 == 0)
        OCR1A = pulseDuration;      // Duration of the pulses
    else
        OCR1A = period - pulseDuration;     // Duration of inter-pulses
  if (switchToDo > 0) {
    switchToDo -= 1;
    PORTB^= _BV(LED); //toggle LED pin 
  }
}

