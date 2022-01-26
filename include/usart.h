/* rs232.h - header file rsr232.c serial line access library

  2013-09-28 Tomas Kolousek, created, support ATmega128 usart#1 only in fixed configuration
*/

// usart baud rate
#define BAUD 9600
// if defined, this symbols contains usart no which is used for printf() and scanf() functions over serial line
#define _USART_STDIO 1

#include <avr/io.h>
#ifdef USART_STDIO
  #include <stdio.h>
  FILE usart_io;	   // for usart stdio replacement
#endif


// setup usart line to 9600bps,8,N,1
void usart_setup(void);

// @returns 0 if no new data byte is available on port, non-zero if new byte is present in usart data register
int usart_dataready(void);

// send character via serial line. waits for empty tx buffer before sending
// @param c       Character to be sent to the serial line
void usart_putchar(char c);

// @returns character from serial line, waits for new character before retrieval
char usart_getchar(void);

#ifdef USART_STDIO
void usart_sputchar(char c, FILE *stream);
char usart_sgetchar(FILE *stream);
#endif