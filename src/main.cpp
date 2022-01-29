/*
Arduino library not used because it cannot be used in my homework,
also arduino's uint8_t is 16bit but AVR's is 8 bit.
*/ 
#include <Arduino.h> //only used for Serial communication on arduino boards
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

extern "C"
{      //this one is used with atmega 128 serial line, the board that is used in my school doesn't have 
  #include "usart.h"
}

const uint8_t MAX_MESSAGE_SIZE = 14;
uint8_t channel1Signal[256], channel2Signal[256];
int incomingByte = 0; // for incoming serial data

void createSquare(uint8_t channelID)
{
  for (int16_t i = 0; i <= 0xff; i++)
  {
    if (channelID == 1)
    {
      if (i <= 0x7f)
        channel1Signal[i] = 0;
      else
        channel1Signal[i] = 0xff;
    }

    else
      channel2Signal[i] = i;
  }
}

void createSinus(uint8_t channelID)
{
  for (int16_t i = 0; i <= 0xff; i++)
  {
    if (channelID == 1)
        channel1Signal[i] = 0x7f + (0x7f * sin(2* M_PI / 0xff * (i+1)));

    else
        channel2Signal[i] = 0x7f + (0x7f * sin(2* M_PI / 0xff * (i+1)));
  }
}

void createTriangle(uint8_t channelID)
{
  for (int16_t i = 0; i <= 0xff; i++)
  {
    if (channelID == 1)
    {
      if (i <= 0x7f)
        channel1Signal[i] = i*2;
      
      else
        channel1Signal[i] = (0xff - i) *2;
    }

    else
      channel2Signal[i] = i;
  }
  
}

void createSawtooth(uint8_t channelID)
{
    for (int16_t i = 0; i <= 0xff; i++)
    {
      if (channelID == 1)
        channel1Signal[i] = i;

      else
        channel2Signal[i] = i;
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

void setup()
{
  Serial.begin(9600);
  //usart_setup(); for atmega 128

  createSinus(1);

  DDRA = 0xff; //set whole Port A as output CHANNEL 1 8bits
  DDRC = 0xff; //set whole Port C as output CHANNEL 2 8bits
}

int main(void)
{
  init();
  setup();
  uint8_t i = 0;

while (1)
  {
    if (Serial.available())
    {
      static char message[14];
      static uint8_t message_pos = 0;

      char newChar = Serial.read();

      if ( newChar != '\n' && (message_pos < 14 - 1) )
      {
        message[message_pos] = newChar;
        message_pos++;
        Serial.println(newChar);
      }

      else
      {
        message[message_pos] = '\0';
        Serial.println(message);
        message_pos = 0;
      }
    }

    OutputUpdate(channel1Signal[i]);
    i++;
  }

  return 0;
}
