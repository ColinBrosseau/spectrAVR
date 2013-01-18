// ATmega16 @ 12 MHz
//
//based on https://sites.google.com/site/qeewiki/books/avr-guide/timer-on-the-atmega8
// 
//Possible improvements:
//    Use PWM for easier pulses generation. See http://enricorossi.org/blog/2010/avr_atmega16_fast_pwm/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h> //for LCD
#include <avr/pgmspace.h>  //for LCD
#include <util/delay.h>
#include "def.h" //common definitions
#include "lcd.h" //for LCD

#define step2position 200 //convert number of steps in physical position (here Angstroms)

#define PUSH PD2 // Define push-button pin on PD2 (Int0)
#define DIRECTION PD6 // Spectrometer direction pin on PD6
#define PULSES PD5 // Spectrometer pulses pin on PD5 (for future PWM conversion)
#define LED PA0 // Display led pin on PA0 
#define HIGH 1 //logic level high
#define LOW 0 //logic level low
#define DUTY 10 //duty cycle (in %)

#define PORT_PULSES PORTD //PORT of pulse pin
#define PORT_DIRECTION PORTD //PORT of direction pin
#define PORT_LED PORTA //PORT of led pin

int pulses = HIGH; //a pulse is when logic level goes to low
long Position = 0; //Position of the motor (steps)
double Position_A; //Position of the spetrometer (Angtroms)
unsigned long switchToDo = 0; //number of PULSES state change remaining
int pulseDuration; //duration of the pulse (timer units)
volatile int period = 12000; //duration between pulses (timer units) (16000 = 1 ms)
                             //this variable needs to be volatile because it is changed by an interrupt function
volatile unsigned long i = 0;//used for pulse generation
long j;//general purpose

//#define setPulse(x) switchToDo = x*2 //set number of pulses to send 
#define setPulseDuration(x) pulseDuration = period*DUTY/100;

void setPulse(unsigned long steps) {
  switchToDo = 2 * steps;
}

int main(void) {
  //for LCD
  char buffer[16];

  //Turn on Push button pin interrupt on falling edge.
  #if defined(__AVR_ATmega8__) 
  GIMSK |= _BV(INT0);  //Enable INT0, Pin PD2 (arduino digital 2)
  MCUCR |= _BV(ISC01); //Trigger on falling edge of INT0 //works for mega8 (manual p. 66) 
  #else
  GICR |= _BV(INT0);  //Enable INT0
  MCUCR |= _BV(ISC01); //Trigger on falling edge of INT0 //works for mega16 (manual p. 69) 
  #endif

  sei();//turn on interrupts

  setPulse(3141); //@ 200 pulses/Angstrom
  setPulseDuration(period);

  //Direction pin
  OUT(PORT_DIRECTION,DIRECTION); // Set output on DIRECTION pin
  SET(PORT_DIRECTION,DIRECTION); // DIRECTION goes high 

  //Pulses pin
  OUT(PORT_PULSES,PULSES); // Set output on PULSES pin
  //setup pulse pin at default level
  if (pulses)
    CLR(PORT_PULSES,PULSES); // PULSE pin goes low
  else 
    SET(PORT_PULSES,PULSES); // PULSE pin goes high 
  
  //Led pin
  OUT(PORT_LED,LED); // Set output on LED pin
  SET(PORT_LED,LED); // LED goes high 

  OCR1A = 1; 
  TCCR1B |= (1 << WGM12); // Mode 4, CTC on OCR1A
  TIMSK |= (1 << OCIE1A); //Set interrupt on compare match   
  TCCR1B |= (1 << CS10); // set prescaler to 1 and start the timer    
  
  sei(); // enable interrupts

  //LCD
  lcd_init(LCD_DISP_ON); /* initialize display, cursor off */
  lcd_clrscr(); /* clear display and home cursor */
  lcd_puts("AVR U1000"); /* put string to display (line 1) with linefeed */ 
        
  for (j=0;j<100000;j++) {
    _delay_ms(200);
    Position_A = (double)Position/step2position;
    dtostrf(Position_A,9,3,buffer); //this line takes a lot of memory! //could be a good idea to remplace this code.
    lcd_gotoxy(0,1); 
    lcd_puts(buffer);
    lcd_puts(" A");
  }
}

int N = 50; //number of pulses to fully accelerate. 50 semble correct
int speedLow = 11000; //minimum speed (actually period)
int speedFast = 7000; //maxmimum speed (actually period). <6500 too low, 7000 correct

ISR (TIMER1_COMPA_vect) {
  if (switchToDo % 2 == 0) {
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
    if (switchToDo % 2 == 0) {
      Position += 1;
    }
    switchToDo -= 1;
    TOGL(PORT_PULSES,PULSES); //toggle PULSES pin 
  }
  else
    IN(PORT_DIRECTION,DIRECTION); // DIRECTION pin to input (high impedance). It allow to control it for the company's controler.
}

ISR(INT0_vect) {
  TOGL(PORT_LED,LED); //toggle LED pin 
  setPulse(200); //@ 200 pulses/Angstrom
} 
