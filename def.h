#define PORT(x) (*(&x - 0))      /* address of data register of port x */
#define DDR(x) (*(&x - 1))      /* address of data direction register of port x */

#define OUT(port,pin) (DDR(port) |=  (1<<pin))  //set pin as output
#define IN(port,pin)  (DDR(port) &= ~(1<<pin))  //set pin as input
#define SET(port,pin) (PORT(port) |=  (1<<pin)) //set pin to high state
#define CLR(port,pin) (PORT(port) &= ~(1<<pin)) //set pin to low state
#define TOGL(port,pin)(PORT(port) ^=  (1<<pin)) //toggle pin state
