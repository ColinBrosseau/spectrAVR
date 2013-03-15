// ATmega16 @ 12 MHz
//
//based on https://sites.google.com/site/qeewiki/books/avr-guide/timer-on-the-atmega8
// 
//Possible improvements:
//    Use PWM for easier pulses generation. See http://enricorossi.org/blog/2010/avr_atmega16_fast_pwm/

#define U1000

#define UART_BAUD_RATE 57600 //uart speed

#include <avr/io.h>
#include <avr/interrupt.h> //for uart
#include <stdlib.h> //for LCD
#include <avr/pgmspace.h>  //for LCD //for uart
#include <util/delay.h>
#include "def.h" //common definitions
#include "lcd.h" //for LCD
#include "uartParser.h"
#include "uart.h"
#include <string.h>

#define HIGH 1 //logic level high
#define LOW 0 //logic level low


//#define HighPulse  // uncomment if pulses are HIGH. Stay commented if pulses are LOW.

long Position = 0; //Position of the motor (steps)
double Position_A; //Position of the spetrometer (Angtroms)
unsigned long switchToDo = 0; //number of PULSES state change remaining
int pulseDuration; //duration of the pulse (timer units)
volatile int period = 12000; //duration between pulses (timer units) (16000 = 1 ms)
                             //this variable needs to be volatile because it is changed by an interrupt function
volatile unsigned long i = 0;//used for pulse generation
unsigned char Moving = 0; //set to 1 if spectrometer is moving

//for command from uart
#define bufferLength 20
char command_in[bufferLength];

#if defined(U1000)
  //spectrometer characteristics
  #define step2position 200 //convert number of steps in physical position (here Angstroms)
  //pulses speed and acceleration
  int N = 50; //number of pulses to fully accelerate. 50 semble correct
  int speedLow = 11000; //minimum speed (actually period)
  int speedFast = 7000; //maxmimum speed (actually period). <6500 too low, 7000 correct
  #define DUTY 10 //duty cycle for pulses (in %)

  #define INPUT_PULSES PD2 // Define INPUT_PULSE pin on PD2 (Int0)
  #define INPUT_DIRECTION PD4 // Define INPUT_DIRECTION pin on PD4 
  #define DIRECTION PD6 // Spectrometer direction pin on PD6
  #define PULSES PD5 // Spectrometer pulses pin on PD5 (for future PWM conversion)
  #define LED PA0 // Display led pin on PA0 

  #define PORT_INPUT_DIRECTION PORTD //PORT of INPUT_DIRECTION pin
  #define PORT_INPUT_PULSES PORTD //PORT of INPUT_PULSES pin
  #define PORT_PULSES PORTD //PORT of pulse pin
  #define PORT_DIRECTION PORTD //PORT of direction pin
  #define PORT_LED PORTA //PORT of led pin

  #define AVANCE CLR(PORT_DIRECTION,DIRECTION); //set pin to increase wavelength
  #define RECULE SET(PORT_DIRECTION,DIRECTION); //set pin to decrease wavelength

#elif defined(HR320)
  #define HighPulse  // Pulses are HIGH. 
#endif

void backlash(void);

void uart_ok() {
  uart_puts("OK");
  uart_puts(RETURN_NEWLINE);
}

double parse_assignment (char input[bufferLength]) {
  char *pch;
  char cmdValue[bufferLength];
  // Find the position the equals sign is
  // in the string, keep a pointer to it
  pch = strchr(input, ' ');
  // Copy everything after that point into
  // the buffer variable
  strcpy(cmdValue, pch+1);
  // Now turn this value into an integer and
  // return it to the caller.
  return atof(cmdValue);
}

// Process commands get from uart
void process_command() {
  long Position2go;
  double Position2go_A;

  if(strcasestr(command_in,"A") != NULL){
    if(strcasestr(command_in,"?") != NULL)
      print_value("A", Position_A);
    else {
      Position_A = parse_assignment(command_in);
      Position = Position_A*step2position;
      uart_ok();
    }
  }
  else if(strcasestr(command_in,"GOTO") != NULL){
    char buffer[16];

    Position2go_A = parse_assignment(command_in);
    Position2go = Position2go_A*step2position;

    OUT(PORT_PULSES,PULSES);
    OUT(PORT_DIRECTION,DIRECTION);

    Moving = 1;

    i=0;
    backlash();

    i=0;
    if (Position2go > Position) {
      switchToDo = Position2go - Position;
      AVANCE;
    }
    else {
      switchToDo = - Position2go + Position;
      RECULE;
    }
  }
  else if(strcasestr(command_in,"STOP") != NULL){
    i = N; //suppose motor is already moving at full speed
    switchToDo = N;
  }

  memset(command_in, 0, bufferLength); //erase the command
}

char buffer[16];

int main(void) {
  //clean command input from outside world (uart) buffer
  memset(command_in, 0, bufferLength);

  //  Initialize UART library
  uart_init( UART_BAUD_SELECT(UART_BAUD_RATE,F_CPU) );
  sei(); //enable interrupt, since UART library is interrupt controlled
  uart_puts("-----"); uart_puts("\r\n");
  uart_puts("SpectrAVR Version 1.98"); uart_puts("\r\n");

  //Turn  INPUT_DIRECTION pin to input
  IN(PORT_INPUT_DIRECTION,INPUT_DIRECTION); // Set INPUT_DIRECTION pin as input

  pulseDuration = period*DUTY/100;   // set pulseDuration

  //Direction pin
  OUT(PORT_DIRECTION,DIRECTION); // Set output on DIRECTION pin
  SET(PORT_DIRECTION,DIRECTION); // DIRECTION goes high

  //Pulses pin
  OUT(PORT_PULSES,PULSES); // Set output on PULSES pin
  //setup pulse pin at default level
#if defined(HighPulse)
  CLR(PORT_PULSES,PULSES); // PULSE pin goes low
#else
  SET(PORT_PULSES,PULSES); // PULSE pin goes high
#endif
			     
  //Led pin
  OUT(PORT_LED,LED); // Set output on LED pin
  SET(PORT_LED,LED); // LED goes high 

  //set timer 1
  //for pulse generation
  OCR1A = 50000;
  TCCR1B |= (1 << WGM12); // Mode 4, CTC on OCR1A
  TIMSK |= (1 << OCIE1A); //Set interrupt on compare match
  TCCR1B |= (1 << CS10); // set prescaler to 1 and start the timer
  sei(); // enable interrupts

  //LCD
  char bufferLCD[16];
  lcd_init(LCD_DISP_ON); /* initialize display, cursor off */
  lcd_clrscr(); /* clear display and home cursor */
  lcd_puts("SpectrAVR 1.98"); /* put string to display (line 1) with linefeed */
        
  //Turn on INPUT_PULSE pin interrupt on falling edge.
  IN(PORT_INPUT_PULSES,INPUT_PULSES); // Set INPUT_PULSES pin as input
  /* #if defined(__AVR_ATmega8__) */
  /* GIMSK |= _BV(INT0);  //Enable INT0, Pin PD2 (arduino digital 2) */
  /* MCUCR |= _BV(ISC01); //Trigger on falling edge of INT0 //works for mega8 (manual p. 66) */
  /* #else */
  GICR |= _BV(INT0);  //Enable INT0
#if defined(HighPulse)
      MCUCR |= _BV(ISC01); //Trigger on falling edge of INT0 //works for mega16 (manual p. 69)
#else
      MCUCR |= _BV(ISC01) | _BV(ISC00); //Trigger on raising edge of INT0 //works for mega16 (manual p. 69)
#endif
  sei();//turn on interrupts

  switchToDo = 0;
  Position = 0;
  //switchToDo = 5000;

  while(1) {
    _delay_ms(200);
    TOGL(PORT_LED,LED); // toggle LED
    //TOGL(PORT_PULSES,PULSES); //for testing
    // LCD display
    Position_A = (double)Position/step2position;
    dtostrf(Position_A,9,3,bufferLCD); //this line takes a lot of memory! //could be a good idea to remplace this code.
    lcd_gotoxy(0,1);
    lcd_puts(bufferLCD);
    lcd_puts(" A");
    // process commands from uart
    process_uart();
    process_command();
    // put PULSES and DIRECTIN pins to input if not moving (for external controler compatibility)
    if (switchToDo == 0) {
      IN(PORT_PULSES,PULSES);
      IN(PORT_DIRECTION,DIRECTION);
    }
  }
}

//Pulses generation
ISR (TIMER1_COMPA_vect) {
  if (switchToDo > 0) {
#if defined(HighPulse)
    if (READ(PORT_PULSES,PULSES) == LOW) {
#else
    if (READ(PORT_PULSES,PULSES)) { //so its high (has to be written like that)
#endif
      i++;
      //period:
      if (i<N) //acceleration
	period = speedLow - (speedLow-speedFast)/N * i;
      if (switchToDo < N*2) //deceleration
	period = speedLow - (speedLow-speedFast)/N * switchToDo/2;
      OCR1A = pulseDuration;      // Duration of the pulses
#if defined(HighPulse)
      SET(PORT_PULSES,PULSES); // PULSE pin goes high
#else
      CLR(PORT_PULSES,PULSES); // PULSE pin goes low 
#endif
    }
    else {
      OCR1A = period - pulseDuration;     // Duration of inter-pulses
#if defined(HighPulse)
      CLR(PORT_PULSES,PULSES); // PULSE pin goes low
#else
      SET(PORT_PULSES,PULSES); // PULSE pin goes high 
#endif
    }
//TOGL(PORT_PULSES,PULSES); // PULSE pin goes low
  }
    else {
      if (Moving) { //spectrometer just finished its movement
	Moving = 0;
	uart_ok();
      }
    }
}

//count pulses (input) so it knows where are the motors
ISR(INT0_vect) {
  if (READ(PORT_INPUT_DIRECTION,INPUT_DIRECTION) == LOW) {
    Position += 1;
  }
  else {
    Position -= 1;
  }
  if (switchToDo > 0)
    switchToDo -= 1;
}

void backlash(void) {
  //revient sur ses pas
  _delay_ms(100);
  TOGL(PORT_DIRECTION,DIRECTION); //toggle DIRECTION pin
  switchToDo = 5;
  //retourne a sa position initiale
  _delay_ms(100);
  TOGL(PORT_DIRECTION,DIRECTION); //toggle DIRECTION pin
  switchToDo = 5;
}
