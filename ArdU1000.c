// ATmega8 @ 16 MHz
// setup timer1 for 0.5 s clock cycle
// blink a led (pin 13 on Arduino)
//
//based on https://sites.google.com/site/qeewiki/books/avr-guide/timer-on-the-atmega8

#include <avr/io.h>
#include <avr/interrupt.h>

#define PUSH PD2 // Define push-button pin on PD2 (Arduino Digital #2)
#define DIRECTION PB3 // Spectrometer direction pin on PB4 (Arduino Digital #11)
#define PULSES PB4 // Spectrometer pulses pin on PB4 (Arduino Digital #12)
#define LED PB5 // Display led pin on PB4 (Arduino Digital #13, led)
#define HIGH 1 //logic level high
#define LOW 0 //logic level low
#define DUTY 10 //duty cycle (in %)

int pulses = LOW; //a pulse is when logic level goes to low
unsigned long switchToDo = 0; //number of PULSES state change remaining
int pulseDuration; //duration of the pulse (timer units)
volatile int period = 12000; //duration between pulses (timer units) (16000 = 1 ms)
                             //this variable needs to be volatile because it is changed by an interrupt function
volatile unsigned long i = 0;

//#define setPulse(x) switchToDo = x*2 //set number of pulses to send 
#define setPulseDuration(x) pulseDuration = period*DUTY/100;

void setPulse(unsigned long steps)
{
switchToDo = 2 * steps;
}

int main(void)
{
 
  //Turn on Push button pin interrupt on falling edge.
  GIMSK |= _BV(INT0);  //Enable INT0, Pin PD2 (arduino digital 2)
  MCUCR |= _BV(ISC01); //Trigger on falling edge of INT0 //works for mega8 (manual p. 66)
  sei();//turn on interrupts

  setPulse(10000); //@ 200 pulses/Angstrom
  setPulseDuration(period);

  DDRB |= (1 << DIRECTION); // Set output on DIRECTION pin
  DDRB |= (1 << PULSES); // Set output on PULSES pin
  DDRB |= (1 << LED); // Set output on LED pin

  PORTB &= ~(1 << DIRECTION); // DIRECTION goes high 
  PORTB |= (1 << LED); // LED goes high 
  //setup led at default level
  if (pulses)
    PORTB &= ~(1 << PULSES); // PULSE pin goes low 
  else 
    PORTB |= (1 << PULSES); // PULSE pin goes high 
  
  OCR1A = 1; 
  TCCR1B |= (1 << WGM12); // Mode 4, CTC on OCR1A
  TIMSK |= (1 << OCIE1A); //Set interrupt on compare match   
  TCCR1B |= (1 << CS10); // set prescaler to 1 and start the timer    
  
  sei(); // enable interrupts

  while (1);
  {
  }
}

int N = 50; //number of pulses to fully accelerate. 50 semble correct
int speedLow = 11000; //minimum speed (actually period)
int speedFast = 7000; //maxmimum speed (actually period). <6500 too low, 7000 correct
//int acceleration = (speedLow-speedFast)/N;

ISR (TIMER1_COMPA_vect)
{
   if (switchToDo % 2 == 0){
     i++;
     //period: 
     if (i<N) //acceleration
       period = speedLow - (speedLow-speedFast)/N * i; 
     if (switchToDo < N*2) //deceleration
       period = speedLow - (speedLow-speedFast)/N * switchToDo/2;
     OCR1A = pulseDuration;      // Duration of the pulses
    }
    else
        OCR1A = period - pulseDuration;     // Duration of inter-pulses
  if (switchToDo > 0) {
    switchToDo -= 1;
    PORTB ^= _BV(PULSES); //toggle PULSES pin 
  }
  else
    DDRB &= ~(1 << DIRECTION); // DIRECTION pin to input (high impedance). It allow to control it for the company's controler.
}

ISR(INT0_vect)
{
  PORTB ^= _BV(LED); //toggle LED pin 
  //period /= 100;
  //period *= 99;
  //setPulseDuration(period);
} 
