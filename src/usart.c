/* rs232.c - basic serial line access routines for ATmega128
   for PkDesign devel board, jumper JP4/JP5 must be set to 2-3 position!

   2013-09-28 Tomas Kolousek - updated, generalization of rs232 routines
   2013-04-08 Tomas Kolousek - created as support of B3t Mit lecture, basic config & stdio streams support
*/

#include "usart.h"
#include <util/setbaud.h>

// setup usart line to 9600bps,8,N,1
void usart_setup(void)
{
  // configure usart hardware registers (see documentation topic USART Registers, page 188)
  UCSR1B = (1<<RXEN1) | (1<<TXEN1);   // Enable Rx and TX, no interrupt
  UCSR1C = (1<<UCSZ01) | (1<<UCSZ00); // async mode, 8, N, 1 stop bit
  UBRR1L = UBRRL_VALUE;					// baud rate from setbaud.h based on BAUD and F_CPU symbols
  UBRR1H = UBRRH_VALUE;
  
  #ifdef USART_STDIO
    // stream config for usart IO
	fdev_setup_stream(&usart_io, usart_sputchar, usart_sgetchar, _FDEV_SETUP_RW);  // setup IO stream ...
	stdin = &usart_io;   // ... and use this stream for standard input ...
	stdout = &usart_io;  //  ... and output
  #endif
}

// @returns 0 if no new data byte is available on port, non-zero if new byte is present in usart data register
int usart_dataready(void) 
{
  return (UCSR1A & (1<<RXC1));
}

// send character via serial line. waits for empty tx buffer before sending
// @param c       Character to be sent to the serial line
void usart_putchar(char c) 
{
  while ( (UCSR1A & (1<<UDRE1)) == 0);		// wait until Tx buffer is empty. Note the empty command (;) mark!
  UDR1 = c;
}

// @returns character from serial line, waits for new character before retriaval
char usart_getchar(void) 
{
  while (!usart_dataready());		// wait until new byte is present on usart
  return(UDR1);
}


#ifdef USART_STDIO

// same as @see usart_putchar() 
void usart_sputchar(char c, FILE *stream) {
  usart_putchar(c);
}

// same as @see usart_getchar() 
char usart_sgetchar(FILE *stream) {
  while (!usart_dataready());  // wait for usart data
  return(usart_getchar());
}
#endif

