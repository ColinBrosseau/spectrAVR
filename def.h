#define PORT(x) (*(&x - 0))      /* address of data register of port x */
#define DDR(x) (*(&x - 1))      /* address of data direction register of port x */

#define OUT(port,pin) (DDR(port) |=  (1<<pin))
