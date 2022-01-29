/*
Arduino library not used because it cannot be used in my homework,
also arduino's uint8_t is 16bit but AVR's is 8 bit.
*/ 
#include <Arduino.h> //only used for Serial communication
 #include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
extern "C" { 
#include "usart.h"
}

uint8_t channel1Singal[256], channel2Signal[256];

void setup()
{
  Serial.begin(9600);
  //usart_setup();

  DDRA = 0xff; //set whole Port A as output CHANNEL 1 8bits
  DDRC = 0xff; //set whole Port C as output CHANNEL 2 8bits
}

void createSinWave(uint8_t channelID)
{
  for (int16_t i = 0; i <= 0xff; i++)
  {
    if (channelID == 1)
        channel1Singal[i] = 0x7f + (0x7f * sin(2* M_PI / 0xff * (i+1)));

    else
        channel2Signal[i] = 0x7f + (0x7f * sin(2* M_PI / 0xff * (i+1)));
  }
}

void OutputUpdate(uint8_t channel1)
{
  PORTA = channel1;
}

void OutputUpdate(uint8_t channel1, uint8_t channel2)
{
  PORTA = channel1;
  PORTC = channel2;
}

/* void demo_chargen(void) {
  static uint8_t c=32;
  for (c=32;c<128;c++) {
    usart_putchar(c);
  }
  usart_putchar(10);
  usart_putchar(13);
} */


int main(void)
{
  setup();
  uint8_t a = 0;
  while (1)
  {
    PORTA = channel1Singal[a];
    a++;
    _delay_ms(5);
    //demo_chargen();
    //channel1 = channel1 << 1;
    //channel1++;
    //OutputUpdate(channel1);
    //PORTA++;
  }
  return 0;
}

