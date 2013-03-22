// ATmega16 @ 12 MHz
//
//based on https://sites.google.com/site/qeewiki/books/avr-guide/timer-on-the-atmega8
// 
//Possible improvements:
//    Remove RW pin for LCD usage (need to change lcd.h, lcd.c) (Useless to read from lcd)
//    Remove bug "(up) number\r" same as "A number\r"
//    Remove bug "(up)number\r" same as "A 0\r"
//    Merge pin assigment for lcd for U1000 and HR320
//    Merge pin assigment for U1000 and HR320
//    Backlash doesn't work (?) for HR320
//    put all spectrometers code in firmware. Select at boot from jumpers or command selectable? Auto detect?
//    Use PWM for easier pulses generation. See http://enricorossi.org/blog/2010/avr_atmega16_fast_pwm/

#define LCD

#define UART_BAUD_RATE 57600 //uart speed

#define Version "1.98f" //firmware version

#include <avr/io.h>
#include <avr/interrupt.h> //for uart
#include <stdlib.h> //for LCD
#include <avr/pgmspace.h>  //for LCD //for uart
#include <util/delay.h>
#include <string.h>
#include "def.h" //common definitions
#include "lcd.h" //for LCD
#include "uartParser.h"
#include "uart.h"

long Position = 0; //Position of the motor (steps)
double Position_A; //Position of the spetrometer (Angtroms)
unsigned long switchToDo = 0; //number of PULSES state change remaining
int pulseDuration; //duration of the pulse (timer units)
volatile unsigned long i = 0;//used for pulse generation
unsigned char Moving = 0; //set to 1 if spectrometer is moving

//for command from uart
#define bufferLength 20
char command_in[bufferLength];

#if defined(ADC)
unsigned int ADC_read(unsigned char);
unsigned int adc; //output from ADC
#endif

//spectrometer characteristics
#if defined(U1000)
  #define step2position 200 //convert number of steps in physical position (here Angstroms)
  //pulses speed and acceleration
  int N = 50; //number of pulses to fully accelerate. 50 semble correct
  int speedLow = 11000; //minimum speed (actually period)
  int speedFast = 7000; //maxmimum speed (actually period). <6500 too low, 7000 correct
  #define DUTY 10 //duty cycle for pulses (in %)
  volatile int period = 12000; //duration between pulses (timer units) (16000 = 1 ms)
                             //this variable needs to be volatile because it is changed by an interrupt function

  #define INPUT_PULSES PD2 // Define INPUT_PULSE pin on PD2 (Int0)
  #define INPUT_DIRECTION PD4 // Define INPUT_DIRECTION pin on PD4 
  #define PULSES PD5 // Spectrometer pulses pin on PD5 (for future PWM conversion)
  #define DIRECTION PD6 // Spectrometer direction pin on PD6
  #define LED PA0 // Display led pin on PA0 

  #define PORT_INPUT_PULSES PORTD //PORT of INPUT_PULSES pin
  #define PORT_INPUT_DIRECTION PORTD //PORT of INPUT_DIRECTION pin
  #define PORT_PULSES PORTD //PORT of pulse pin
  #define PORT_DIRECTION PORTD //PORT of direction pin
  #define PORT_LED PORTA //PORT of led pin

  #define AVANCE CLR(PORT_DIRECTION,DIRECTION); //set pin to increase wavelength
  #define RECULE SET(PORT_DIRECTION,DIRECTION); //set pin to decrease wavelength

  #define Ndigit 3 //number of digits for wavelength display
  #define Ncharacters 9 //number of characters for wavelength display

#elif defined(HR320)
  #define step2position 10 //convert number of steps in physical position (here Angstroms)
  #define HighPulse  // Pulses are HIGH. 
  //pulses speed and acceleration
  int N = 500; //number of pulses to fully accelerate. 50 semble correct
  int speedLow = 64000; //minimum speed (actually period) 64000 = 187 Hz
  int speedFast = 40000; //maxmimum speed (actually period). 40000 = 300 Hz
  #define DUTY 20 //duty cycle for pulses (in %)
  volatile int period = 30000; //duration between pulses (timer units) (16000 = 1 ms)
                             //this variable needs to be volatile because it is changed by an interrupt function

  #define INPUT_PULSES_UP PD2 // Define INPUT_PULSE pin on PD2 (Int0)
  #define INPUT_PULSES_DOWN PB2 // Define INPUT_DIRECTION pin on PD4 
  #define PULSES_UP PD5 // Spectrometer pulses pin on PD5 
  #define PULSES_DOWN PB1 // Spectrometer direction pin on PD6 //will have to change to int2 pin
  #define LED PA0 // Display led pin on PA0 
  #define PHOTODIODE1 PA1 // Voltage Input related to photodiode

  #define PORT_INPUT_PULSES_UP PORTD //PORT of INPUT_PULSES_UP pin
  #define PORT_INPUT_PULSES_DOWN PORTB //PORT of INPUT_PULSES_DOWN pin
  #define PORT_PULSES_UP PORTD //PORT of PULSES_UP pin
  #define PORT_PULSES_DOWN PORTB //PORT of PULSES_DOWN pin //will have to change to int2 pin
  #define PORT_LED PORTA //PORT of led pin
  #define PORT_PHOTODIODE1 PORTA // PORT of Input related to photodiode

  #define Ndigit 1 //number of digits for wavelength
  #define Ncharacters 7 //number of characters for wavelength display

  unsigned char IncreasePosition;
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

//initialise UART communication
void initUART(void) {
  //  Initialize UART library
  uart_init( UART_BAUD_SELECT(UART_BAUD_RATE,F_CPU) );
  sei(); //enable interrupt, since UART library is interrupt controlled
  uart_puts("\r\n");
  uart_puts("-----"); uart_puts("\r\n");
  uart_puts("SpectrAVR ");
  uart_puts(Version); 
#if defined(U1000)
  uart_puts(" U1000"); 
#elif defined(HR320)
  uart_puts(" HR320"); 
#endif
  uart_puts("\r\n");
}

//initialise LCD display
void initLCD(void) {
  //LCD
  lcd_init(LCD_DISP_ON); /* initialize display, cursor off */
  lcd_clrscr(); /* clear display and home cursor */
  lcd_puts("SpectrAVR ");
  lcd_puts(Version); /* put string to display (line 1) with linefeed */
#if defined(U1000)
  lcd_puts("U"); 
#elif defined(HR320)
  lcd_puts("H"); 
#endif
}

//initialise IO on uC
#if defined(U1000)
void initIO(void) {
  //Turn  INPUT_DIRECTION pin to input
  IN(PORT_INPUT_DIRECTION,INPUT_DIRECTION); // Set INPUT_DIRECTION pin as input

  //Direction pin
  OUT(PORT_DIRECTION,DIRECTION); // Set output on DIRECTION pin
  SET(PORT_DIRECTION,DIRECTION); // DIRECTION goes high

  //Pulses pin
  OUT(PORT_PULSES,PULSES); // Set output on PULSES pin
  //setup pulse pin at default level
  SET(PORT_PULSES,PULSES); // PULSE pin goes high
			     
  //set timer 1
  //for pulse generation
  OCR1A = 50000;
  TCCR1B |= (1 << WGM12); // Mode 4, CTC on OCR1A
  TIMSK |= (1 << OCIE1A); //Set interrupt on compare match
  TCCR1B |= (1 << CS10); // set prescaler to 1 and start the timer
  sei(); // enable interrupts
   
  //Turn on INPUT_PULSE pin interrupt on falling edge.
  IN(PORT_INPUT_PULSES,INPUT_PULSES); // Set INPUT_PULSES pin as input
  /* #if defined(__AVR_ATmega8__) */
  /* GIMSK |= _BV(INT0);  //Enable INT0, Pin PD2 (arduino digital 2) */
  /* MCUCR |= _BV(ISC01); //Trigger on falling edge of INT0 //works for mega8 (manual p. 66) */
  /* #else */
  GICR |= _BV(INT0);  //Enable INT0
  MCUCR |= _BV(ISC01) | _BV(ISC00); //Trigger on raising edge of INT0 //works for mega16 (manual p. 69)
  sei();//turn on interrupts

  //Initialization of ADC
  //ADMUX=(1<<REFS0); // AVcc with external capacitor at AREF
  //ADCSRA=(1<<ADEN)|(1<<ADPS2)|(1<<ADPS0); // Enable ADC and set Prescaler division factor as 32
}
#elif defined(HR320)
void initIO(void) {
  //to be completed

  //Pulses pins
  OUT(PORT_PULSES_UP,PULSES_UP); // Set output on PULSES_UP pin
  //setup pulse_UP pin at default level
  CLR(PORT_PULSES_UP,PULSES_UP); // PULSE_UP pin goes high
  OUT(PORT_PULSES_DOWN,PULSES_DOWN); // Set output on PULSES_UP pin
  //setup pulse_DOWN pin at default level
  CLR(PORT_PULSES_DOWN,PULSES_DOWN); // PULSE_DOWN pin goes high

  //set timer 1
  //for pulse generation
  OCR1A = 50000;
  TCCR1B |= (1 << WGM12); // Mode 4, CTC on OCR1A
  TIMSK |= (1 << OCIE1A); //Set interrupt on compare match
  TCCR1B |= (1 << CS10); // set prescaler to 1 and start the timer (manual p. 113)
  //TCCR1B |= (1 << CS11); // set prescaler to 8 and start the timer (manual p. 113)
  sei(); // enable interrupts
   
  //Turn on INPUT_PULSE_UP pin interrupt on falling edge.
  IN(PORT_INPUT_PULSES_UP,INPUT_PULSES_UP); // Set INPUT_PULSES_UP pin as input
  GICR |= _BV(INT0);  //Enable INT0
  //MCUCR |= _BV(ISC01) | _BV(ISC00); //Trigger raising edge of INT0 //works for mega16 (manual p. 69)
  MCUCR |= _BV(ISC01) ; //Trigger on falling edge of INT0 //works for mega16 (manual p. 69)
  sei();//turn on interrupts

  //Turn on INPUT_PULSE_DOWN pin interrupt on falling edge.
  IN(PORT_INPUT_PULSES_DOWN,INPUT_PULSES_DOWN); // Set INPUT_PULSES_DOWN pin as input
  GICR |= _BV(INT2);  //Enable INT2
  //MCUCSR |= _BV(ISC2) ; //Trigger on raising (?) edge of INT2 //works for mega16 
  MCUCSR &= ~_BV(ISC2) ; //Trigger on falling (?) edge of INT2 //works for mega16 
  sei();//turn on interrupts

  //Initialization of ADC
  //ADMUX=(1<<REFS0); // AVcc with external capacitor at AREF
  //ADCSRA=(1<<ADEN)|(1<<ADPS2)|(1<<ADPS0); // Enable ADC and set Prescaler division factor as 32
}
#endif


/* unsigned int ADC_read(unsigned char ch) */
/* { */
/*   ch= ch & 0b00000111;	// channel must be b/w 0 to 7 */
/*   ADMUX |= ch;	// selecting channel */
  
/*   ADCSRA|=(1<<ADSC);	// start conversion */
/*   while(!(ADCSRA & (1<<ADIF)));	// waiting for ADIF, conversion complete */
/*   ADCSRA|=(1<<ADIF);	// clearing of ADIF, it is done by writing 1 to it */
 
/*   return (ADC); */
/* } */

char buffer[16];

#if defined(U1000)
 char bufferLCD[20];
#elif defined(HR320)
char bufferLCD[16];
#endif


int main(void) {
  //clean command input from outside world (uart) buffer
  memset(command_in, 0, bufferLength);

  initUART(); //initialise UART communication
#if defined(LCD)
  initLCD(); //Initialise LCD display
#endif

  pulseDuration = period*DUTY/100;   // set pulseDuration
    
  //Led pin
  OUT(PORT_LED,LED); // Set output on LED pin
  SET(PORT_LED,LED); // LED goes high 

  initIO();

  switchToDo = 0;
  Position = 0;

  while(1) {
    _delay_ms(25);
    TOGL(PORT_LED,LED); // toggle LED
    //TOGL(PORT_PULSES,PULSES); //for testing
    Position_A = (double)Position/step2position;
#if defined(LCD)
    // LCD display
    dtostrf(Position_A,Ncharacters,Ndigit,bufferLCD); //this line takes a lot of memory! //could be a good idea to remplace this code.
    lcd_gotoxy(0,1);
    lcd_puts(bufferLCD);
    lcd_puts(" A");
#endif
    // LCD Power
    //if (switchToDo == 0) {
    // if (j-- == 0){
    //j = 13;
    //	adc = ADC_read(1);
    //	itoa(adc, bufferLCD, 10);
    //	lcd_gotoxy(12,1); //erase
    //	lcd_puts("    ");   
    //	lcd_gotoxy(12,1);
    //	lcd_puts(bufferLCD);
    //}
    //}
    //else {
    //lcd_gotoxy(12,1); //erase
    //lcd_puts("    ");        
    //}
    // process commands from uart
    process_uart();
    process_command();
    // put PULSES and DIRECTIN pins to input if not moving (for external controler compatibility)
    if (switchToDo == 0) {
#if defined(U1000)
      IN(PORT_PULSES,PULSES);
      IN(PORT_DIRECTION,DIRECTION);
#elif defined(HR320)
      //IN(PORT_PULSES_UP,PULSES_UP);
      //IN(PORT_PULSES_DOWN,PULSES_DOWN);
#endif

    }
  }
}


void calculate_period(int i) {
  //period:
  if (i<N) //acceleration
    period = speedLow - (speedLow-speedFast)/N * i;
  else if (switchToDo < N*2) //deceleration
    period =  speedLow - (speedLow-speedFast)/N * switchToDo/2;
  else
    period = speedFast;
}


//Pulses generation
ISR (TIMER1_COMPA_vect) {
  //Important note: don put any uart_put() here. It takes too much time and freeze the uC)
#if defined(U1000)
  if (switchToDo > 0) {
    if (READ(PORT_PULSES,PULSES)) { //so its high (has to be written like that)
      i++;
      calculate_period(i);
      OCR1A = pulseDuration;      // Duration of the pulses
      CLR(PORT_PULSES,PULSES); // PULSE pin goes low 
    }
    else {
      OCR1A = period - pulseDuration;     // Duration of inter-pulses
      SET(PORT_PULSES,PULSES); // PULSE pin goes high 
    }
  }
#elif defined(HR320)
  if (switchToDo > 0) {
    if (IncreasePosition != NULL) {
      if (READ(PORT_PULSES_UP,PULSES_UP) == LOW) {  
 	i++;
	calculate_period(i);
	OCR1A = pulseDuration;      // Duration of the pulses
	SET(PORT_PULSES_UP,PULSES_UP);
      }
      else {
	//uart_puts("is high...\n\r");
	OCR1A = period - pulseDuration;     // Duration of inter-pulses
	CLR(PORT_PULSES_UP,PULSES_UP);
      }
    }
    else {
      if (READ(PORT_PULSES_DOWN,PULSES_DOWN) == LOW) {  
	i++;
	calculate_period(i);
	OCR1A = pulseDuration;      // Duration of the pulses
	SET(PORT_PULSES_DOWN,PULSES_DOWN);
      }
      else {
	OCR1A = period - pulseDuration;     // Duration of inter-pulses
	CLR(PORT_PULSES_DOWN,PULSES_DOWN);
      }
    }
  }
#endif

  else {
    if (Moving) { //spectrometer just finished its movement
      Moving = 0;
      uart_ok();
    }
  }
}


//count pulses (input) so it knows where are the motors
ISR(INT0_vect) {

#if defined(U1000)
  if (READ(PORT_INPUT_DIRECTION,INPUT_DIRECTION) == LOW) {
    Position += 1;
  }
  else {
    Position -= 1;
  }
#elif defined(HR320)
  Position += 1;
#endif

  if (switchToDo > 0)
    switchToDo -= 1;
}


//count pulses (input) so it knows where are the motors
ISR(INT2_vect) {

#if defined(HR320)
  Position -= 1;
  if (switchToDo > 0)
    switchToDo -= 1;
#endif

}


void backlash(void) {
#if defined(U1000)
  //revient sur ses pas
  _delay_ms(100);
  TOGL(PORT_DIRECTION,DIRECTION); //toggle DIRECTION pin
  switchToDo = 5;
  //retourne a sa position initiale
  _delay_ms(100);
  TOGL(PORT_DIRECTION,DIRECTION); //toggle DIRECTION pin
  switchToDo = 5;
#elif defined(HR320)
  //revient sur ses pas
  _delay_ms(100);
  if (IncreasePosition) {
    IncreasePosition = 0;
  }
  else {
    IncreasePosition = 1;
  }
  switchToDo = 5;

  _delay_ms(100);
  if (IncreasePosition) {
    IncreasePosition = 0;
  }
  else {
    IncreasePosition = 1;
  }
  switchToDo = 5;
#endif
}


// Process commands get from uart
void process_command() {
  long Position2go;
  double Position2go_A;

  if(strcasestr(command_in,"A") != NULL){
    if(strcasestr(command_in,"?") != NULL){
      uart_puts(":A? "); uart_puts("\r\n");
      print_value("A", Position_A);
    }
    else {
      uart_puts(":A "); uart_puts("\r\n");
      Position_A = parse_assignment(command_in);
      Position = Position_A*step2position;
      uart_ok();
    }
  }

  else if(strcasestr(command_in,"GOTO") != NULL){
    uart_puts(":GOTO \r\n"); 
    Position2go_A = parse_assignment(command_in);
    Position2go = Position2go_A*step2position;

#if defined(U1000)
    OUT(PORT_PULSES,PULSES);
    OUT(PORT_DIRECTION,DIRECTION);
#elif defined(HR320)
    //OUT(PORT_PULSES_UP,PULSES_UP);
    //OUT(PORT_PULSES_DOWN,PULSES_DOWN);
#endif

    i=0;
    backlash();

    i=0;
    if (Position2go > Position) {
      switchToDo = Position2go - Position;
#if defined(U1000)
      AVANCE;
#elif defined(HR320)
      IncreasePosition = 1;
      OUT(PORT_PULSES_UP,PULSES_UP);
      IN(PORT_PULSES_DOWN,PULSES_DOWN);
#endif
    }
    else {
      switchToDo = - Position2go + Position;
#if defined(U1000)
      RECULE;
#elif defined(HR320)
      IncreasePosition = 0;     
      IN(PORT_PULSES_UP,PULSES_UP);
      OUT(PORT_PULSES_DOWN,PULSES_DOWN);
#endif     
    }
    Moving = 1;
  }

  else if(strcasestr(command_in,"STOP") != NULL){
    uart_puts(":STOP "); uart_puts("\r\n");
    i = N; //suppose motor is already moving at full speed
    switchToDo = N;
  }

  else if(strcasestr(command_in,"POWER1") != NULL){
    uart_puts(":POWER1? "); uart_puts("\r\n");
   //adc = ADC_read(1);
   //print_value_int("POWER1", adc);
  }
  memset(command_in, 0, bufferLength); //erase the command
}
