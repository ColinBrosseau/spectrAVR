/*************************************************************************
Parsing UART commands exemple

based on www.adnbr.co.uk/articles/parsing-simple-usart-commands

uses Peter Fleury's uart library http://homepage.hispeed.ch/peterfleury/avr-software.html#libs
for easier microcontroler change.
*************************************************************************/
#define bufferLength 20

#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <string.h>
#include "uart.h"

#include "uartParser.h"

#define TRUE 1
#define FALSE 0
#define CHAR_NEWLINE '\n'
#define CHAR_RETURN '\r'
#define RETURN_NEWLINE "\r\n"

unsigned char data_count = 0;
unsigned char data_in[bufferLength];
extern char command_in[bufferLength];

extern double Position_A; //variable from main program
extern long Position; //variable from main program
extern unsigned long switchToDo; //variable from main program



void copy_command () {
  // Copy the contents of data_in into command_in
  memcpy(command_in, data_in, bufferLength);
  // Now clear data_in, the UART can reuse it now
  memset(data_in, 0, bufferLength);
  //uart_puts("data_in cleared");
  //uart_puts(RETURN_NEWLINE);
  //uart_puts(data_in);
  //uart_puts(RETURN_NEWLINE);
}


void print_value (char *id, double value) {
  char buffer[bufferLength];
  dtostrf(value,0,3,buffer); //this line takes a lot of memory! //could be a good idea to remplace this code.
//ftoa(value, buffer, 10);
  uart_puts(id);
  uart_putc('=');
  uart_puts(buffer);
  uart_puts(RETURN_NEWLINE);
}

void print_value_int (char *id, int value) {
  char buffer[bufferLength];
  itoa(value, buffer, 10);
  uart_puts(id);
  uart_putc('=');
  uart_puts(buffer);
  uart_puts(RETURN_NEWLINE);
}


/* void uart_ok() { */
/*   uart_puts("OK"); */
/*   uart_puts(RETURN_NEWLINE); */
/* } */

void process_uart(){
  /* Get received characters from ringbuffer
   * read until no more character to read in ringbuffer or end of command ('\r') detected
   * uart_getc() returns in the lower byte the received character and 
   * in the higher byte (bitmask) the last receive error
   * UART_NO_DATA is returned when no data is available.   */
  char buffer[bufferLength];

  unsigned int c = uart_getc();
  char ok = !(c & UART_NO_DATA); //1 if there was a character to read in uart, 0 if not

  while(ok) { //loop until no more character to read in ringbuffer or end of command detected
    itoa(c, buffer, 10);
    //uart_puts(buffer);
    //uart_puts(" ");
    uart_putc( (unsigned char)c ); //echo back input character
    //uart_puts(RETURN_NEWLINE);

    // new data available from UART check for Frame or Overrun error
    if ( c & UART_FRAME_ERROR ) {
      /* Framing Error detected, i.e no stop bit detected */
      uart_puts_P("UART Frame Error: ");
    }
    if ( c & UART_OVERRUN_ERROR ) {
      /* Overrun, a character already present in the UART UDR register was 
       * not read by the interrupt handler before the next character arrived,
       * one or more received characters have been dropped */
      uart_puts_P("UART Overrun Error: ");
    }
    if ( c & UART_BUFFER_OVERFLOW ) {
      /* We are not reading the receive buffer fast enough,
       * one or more received character have been dropped  */
      uart_puts_P("Buffer overflow error: ");
    }
    
    // Add char to input buffer
    data_in[data_count] = c;
    
    // Return is signal for end of command input
    if (data_in[data_count] == CHAR_RETURN) {
      // Reset to 0, ready to go again
      data_count = 0;
      //uart_puts(RETURN_NEWLINE);
      //uart_puts(RETURN_NEWLINE);
      //uart_puts(RETURN_NEWLINE);

      //uart_puts("commande ->");
      //uart_puts(RETURN_NEWLINE);
      //uart_puts(RETURN_NEWLINE);
      //uart_puts(data_in);
      //uart_puts(RETURN_NEWLINE);
      //uart_puts(RETURN_NEWLINE);
      
      copy_command();
      //process_command();
      //uart_ok();
      ok = 0;
    } 
    else { //not end of command input. Try to read more characters
      data_count++;
      c = uart_getc();
      ok = !(c & UART_NO_DATA);     
    }
  }

}

/* int main(void) {  */
/*   /\*  Initialize UART library, pass baudrate and AVR cpu clock */
/*    *  with the macro  */
/*    *  UART_BAUD_SELECT() (normal speed mode ) */
/*    *  or  */
/*    *  UART_BAUD_SELECT_DOUBLE_SPEED() ( double speed mode)  *\/ */
/*   uart_init( UART_BAUD_SELECT(UART_BAUD_RATE,F_CPU) );  */
  
/*   // now enable interrupt, since UART library is interrupt controlled */
/*   sei(); */
  
/*   /\*  Transmit string to UART */
/*    *  The string is buffered by the uart library in a circular buffer */
/*    *  and one character at a time is transmitted to the UART using interrupts. */
/*    *  uart_puts() blocks if it can not write the whole string to the circular  */
/*    *  buffer *\/ */
/*   uart_puts("-----"); */
/*   uart_puts(RETURN_NEWLINE); */
/*   uart_puts("AVR UART"); */
/*   uart_puts(RETURN_NEWLINE); */
/*   uart_puts("Command parsing demo"); */
/*   uart_puts(RETURN_NEWLINE); */
/*   uart_puts(RETURN_NEWLINE); */
/*   uart_puts("End each input with enter key"); */
/*   uart_puts(RETURN_NEWLINE); */
/*   uart_puts("This demo has two parameters: a and goto."); */
/*   uart_puts(RETURN_NEWLINE); */
/*   uart_puts("To set a parameter: a=45 (or any number)"); */
/*   uart_puts(RETURN_NEWLINE); */
/*   uart_puts("To query a parameter: a?"); */
/*   uart_puts(RETURN_NEWLINE); */
/*   uart_puts("No difference with case: a? is the same thing as A?"); */
/*   uart_puts(RETURN_NEWLINE); */
/*   uart_puts("Illegal values set parameter to zero: a=t66"); */
/*   uart_puts(RETURN_NEWLINE); */
     
/*   while (1) { */
/*     process_uart(); */
/*   }  */
/* } */
