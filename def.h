//#define U1000
#define HR320

//see http://www.avr-asm-tutorial.net/avr_en/beginner/PDETAIL.html#IOPORTS                                                    
#define PORT(x) (*(&x - 0))      /* address of data register of port x */
#define DDR(x) (*(&x - 1))      /* address of data direction register of port x */
#if defined(__AVR_ATmega64__) || defined(__AVR_ATmega128__)
/* on ATmega64/128 PINF is on port 0x00 and not 0x60 */
#define PIN(x) ( &PORTF==&(x) ? _SFR_IO8(0x00) : (*(&x - 2)) )
#else
#define PIN(x) (*(&x - 2))    /* address of input register of port x          */
#endif

#define OUT(port,pin) (DDR(port) |=  (1<<pin))  //set pin as output
#define IN(port,pin)  (DDR(port) &= ~(1<<pin))  //set pin as input
#define SET(port,pin) (PORT(port) |=  (1<<pin)) //set pin to high state
#define CLR(port,pin) (PORT(port) &= ~(1<<pin)) //set pin to low state
#define TOGL(port,pin)(PORT(port) ^=  (1<<pin)) //toggle pin state
#define READ(port,pin)(PIN(port) &   (1<<pin))  //read pin state

#define TRUE 1
#define FALSE 0
#define CHAR_NEWLINE '\n'
#define CHAR_RETURN '\r'
#define RETURN_NEWLINE "\r\n"
#define HIGH 1 //logic level high
#define LOW 0 //logic level low
