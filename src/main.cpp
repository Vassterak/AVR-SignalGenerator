/*
Arduino library not used because it cannot be used in my homework,
also arduino's int is 16bit but AVR's is 8 bit.
*/ 
//#include <Arduino.h>
#include <avr/io.h>
#include <util/delay.h> 
#include "usart.h"

int channel1 = 0, channel2 = 0;

void setup()
{
  DDRA = 0xff; //set whole Port A as output CHANNEL 1 8bits
  DDRC = 0xff; //set whole Port C as output CHANNEL 2 8bits
}

void OutputUpdate(int channel1)
{
  PORTA = channel1;
}

void OutputUpdate(int channel1, int channel2)
{
  PORTA = channel1;
  PORTC = channel2;
}

int main(void)
{
  setup();

  while (1)
  {
    _delay_ms(1);
    //channel1 = channel1 << 1;
    channel1++;;
    OutputUpdate(channel1);
  }
  
  return 0;
}