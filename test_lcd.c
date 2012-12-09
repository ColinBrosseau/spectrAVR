/*************************************************************************
Title:    testing output to a HD44780 based LCD display.
Author:   Peter Fleury  <pfleury@gmx.ch>  http://jump.to/fleury
File:     $Id: test_lcd.c,v 1.6 2004/12/10 13:53:59 peter Exp $
Software: AVR-GCC 3.3
Hardware: HD44780 compatible LCD text display
          ATS90S8515/ATmega if memory-mapped LCD interface is used
          any AVR with 7 free I/O pins if 4-bit IO port mode is used
**************************************************************************/
#include <stdlib.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include "lcd.h"


/*
** constant definitions
*/
static const PROGMEM unsigned char copyRightChar[] =
{
	0x07, 0x08, 0x13, 0x14, 0x14, 0x13, 0x08, 0x07,
	0x00, 0x10, 0x08, 0x08, 0x08, 0x08, 0x10, 0x00
};


/*
** function prototypes
*/ 
//void wait_until_key_pressed(void);

/* void wait_until_key_pressed(void) */
/* { */
/*     unsigned char temp1, temp2; */
/*     unsigned int i; */
    
/*     do { */
/*         temp1 = PIND;                  // read input */
/*         for(i=0;i<65535;i++); */
/*         temp2 = PIND;                  // read input */
/*         temp1 = (temp1 & temp2);       // debounce input */
/*     } while ( temp1 & _BV(PIND2) ); */
    
/*     loop_until_bit_is_set(PIND,PIND2);            /\* wait until key is released *\/ */
/* } */

#define LED PB5 // Display led pin on PB4 (Arduino Digital #13, led)

int main(void)
{
    char buffer[7];
    int  num=134;
    unsigned char i;
    
    DDRB |= (1 << LED); // Set output on LED pin
    PORTB |= (1 << LED); // LED goes high 

    //DDRD &=~ (1 << PD2);        /* Pin PD2 input              */
    //PORTD |= (1 << PD2);        /* Pin PD2 pull-up enabled    */
    //

    /* initialize display, cursor off */
    lcd_init(LCD_DISP_ON);

        /* clear display and home cursor */
        lcd_clrscr();
        
        /* put string to display (line 1) with linefeed */
        lcd_puts("LCD Test Line 1\n");

        /* cursor is now on second line, write second line */
        lcd_puts("Line 2");
        
        /* move cursor to position 8 on line 2 */
        lcd_gotoxy(7,1);  
        
        /* write single char to display */
        lcd_putc(':');
        

    for (;;) {                           /* loop forever */
    }
}
