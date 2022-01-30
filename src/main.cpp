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

static uint8_t channel1Signal[256], channel2Signal[256];
uint8_t selectedChannel, selectedAmplitude, selectedFrequency;

void createSquare(uint8_t channelID, uint8_t amplitude)
{
  for (int16_t i = 0; i <= 0xff; i++)
  {
    if (channelID == 1)
    {
      if (i <= 0xff / 2)
        channel1Signal[i] = 0;
      else
        channel1Signal[i] = amplitude;
    }

    else
      channel2Signal[i] = i;
  }
}

void createSinus(uint8_t channelID, uint8_t amplitude)
{
  for (int16_t i = 0; i <= 0xff; i++)
  {
    if (channelID == 1)
        channel1Signal[i] = amplitude/2 + (amplitude/2 * sin(2* M_PI / 0xff * (i+1)));

    else
        channel2Signal[i] = amplitude/2 + (amplitude/2 * sin(2* M_PI / 0xff * (i+1)));
  }
}

void createTriangle(uint8_t channelID, uint8_t amplitude)
{
  for (int16_t i = 0; i <= 0xff; i++)
  {
    if (channelID == 1)
    {
      if (i <= 0xff/2)
        channel1Signal[i] = (int)((amplitude / 127.0f) * i);
      
      else
        channel1Signal[i] = (0xff - i) * (amplitude / 127.0f);
    }

    else
      channel2Signal[i] = i;
  }
  
}

void createSawtooth(uint8_t channelID, uint8_t amplitude)
{
    for (int16_t i = 0; i <= 0xff; i++)
    {
      if (channelID == 1)
        channel1Signal[i] = (int)((amplitude / 255.0f) * i);

      else
        channel2Signal[i] = i;
    }
}

void OutputUpdate(uint8_t channel1, uint8_t channel2)
{
  PORTA = channel1;
  PORTC = channel2;
}

void inputManagement()
{
  static char message[MAX_MESSAGE_SIZE];
  static uint8_t message_pos = 0;
  char newChar = Serial.read(); //read first char in buffer, if read delete it in buffer

  if ( newChar != '\n' && (message_pos < MAX_MESSAGE_SIZE - 1)) 
  {
    message[message_pos] = newChar;
    message_pos++;
  }

  else //whole message block has been received
  {
    message[message_pos] = '\0';
    Serial.println(message); //for debug only
    message_pos = 0;

    if (isdigit(message[0]))
    {
      selectedChannel = (int)message[0] - (int)'0'; //convert char number to int, char to int return ASCII value, so we need to substract 0 in ASCII
      Serial.println("selected channel: "); //debug only
      Serial.println(selectedChannel); //debug only
    }

    char amplitude[3];
    memcpy(amplitude, &message[2],3); //extract amplitude values from incomming data
    selectedAmplitude = atoi(amplitude);
    
    switch (message[1])
    {
    case 's':
      createSinus(selectedChannel, selectedAmplitude);
      break;

    case 't':
      createTriangle(selectedChannel, selectedAmplitude);
      break;

    case 'w':
      createSawtooth(selectedChannel, selectedAmplitude);
      break;

    case 'e':
      createSquare(selectedChannel, selectedAmplitude);
      break;

    default:
      Serial.println("Wrong value");
      break;
    }
  }
}

void setup()
{
  Serial.begin(9600);
  //usart_setup(); for atmega 128

  DDRA = 0xff; //set whole Port A as output CHANNEL 1 8bits
  DDRC = 0xff; //set whole Port C as output CHANNEL 2 8bits
}

int main(void)
{
  init(); //Initialisation for serial line from arduino.h
  setup();
  uint8_t i = 0; //loop trought arrays

  while (1)
  {
    if (Serial.available()) //when there are data waiting in a buffer
    {
      inputManagement();
    }

    OutputUpdate(channel1Signal[i], channel2Signal[i]);
    i++;
  }

  return 0;
}
