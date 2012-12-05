// ATmega8 @ 16 MHz
// setup timer1 for 0.5 s clock cycle
// blink a led (pin 13 on Arduino)
//
//based on https://sites.google.com/site/qeewiki/books/avr-guide/timer-on-the-atmega8

#include <avr/io.h>
#include <avr/interrupt.h>

#define LED PB4 // Define output pin on PB4 (Arduino #12)
#define LED2 PB5 // Define output pin on PB4 (Arduino #13)
#define HIGH 1 //logic level high
#define LOW 0 //logic level low
#define DUTY 10 //duty cycle (in %)

int pulses = LOW; //a pulse is when logic level goes to low
unsigned long switchToDo = 0; //number of LED state change remaining
int pulseDuration; //duration of the pulse (timer units)
int period = 16000; //duration between pulses (timer units) (1 ms)

//#define setPulse(x) switchToDo = x*2 //set number of pulses to send 
#define setPulseDuration(x) pulseDuration = period*DUTY/100;

// this is just a program that 'kills time' in a calibrated method
void delay_ms(uint16_t ms) {
  uint16_t delay_count = F_CPU / 17500;
  volatile uint16_t i;

  while (ms != 0) {
    for (i=0; i != delay_count; i++);
    ms--;
  }
}


void setPulse(unsigned long steps)
{
switchToDo = 2 * steps;
}

int main(void)
{
  setPulse(500000);
  setPulseDuration(period);

  DDRB |= (1 << LED); // Set output on LED pin
  DDRB |= (1 << LED2); // Set output on LED2 pin
  PORTB |= (1 << LED2); // led2 goes high 
  //setup led at default level
  if (pulses)
    PORTB &= ~(1 << LED); // led goes low 
  else 
    PORTB |= (1 << LED); // led goes high 
  
  OCR1A = 1; 
  TCCR1B |= (1 << WGM12); // Mode 4, CTC on OCR1A
  TIMSK |= (1 << OCIE1A); //Set interrupt on compare match   
  TCCR1B |= (1 << CS10); // set prescaler to 1 and start the timer    
  
  sei(); // enable interrupts

  while (1);
  {
    delay_ms(500);
    PORTB ^= _BV(LED2); //toggle LED2 pin 
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
    PORTB ^= _BV(LED); //toggle LED pin 
  }
}

