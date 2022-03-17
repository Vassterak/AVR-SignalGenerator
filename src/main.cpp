/*
Arduino library not used because it cannot be used in my homework,
also arduino's default int is 16bit but <avr/io.h> is 8 bit.
*/ 
#include <Arduino.h> //only used for Serial communication on arduino boards | PS: <math.h> is included in <Arduino.h>
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

/*extern "C" //this one is used with atmega 128 serial line, the board that is used in my school doesn't have 
{      
  #include "usart.h"
}*/

const uint8_t MAX_MESSAGE_SIZE = 14;

static uint8_t channel1Signal[256], channel2Signal[256];
static uint8_t channel1SampleRate, channel2SampleRate;

uint8_t selectedChannel, selectedAmplitude, selectedSampleRate, selectedDelay = 0;

void createSquare(uint8_t channelID, uint8_t amplitude, uint8_t sampleRate)
{
  for (int16_t i = 0; i <= sampleRate; i++)
  {
    if (channelID == 1)
    {
      if (i <= sampleRate / 2)
        channel1Signal[i] = 0;
      else
        channel1Signal[i] = amplitude;
    }

    else //for channel 2
    {
      if (i <= sampleRate / 2)
        channel2Signal[i] = 0;
      else
        channel2Signal[i] = amplitude;
    }
  }
}

void createSinus(uint8_t channelID, uint8_t amplitude, uint8_t sampleRate)
{
  for (int16_t i = 0; i <= sampleRate; i++)
  {
    if (channelID == 1)
        channel1Signal[i] = amplitude/2 + (amplitude/2 * sin(2* M_PI / sampleRate * (i+1)));

    else
        channel2Signal[i] = amplitude/2 + (amplitude/2 * sin(2* M_PI / sampleRate * (i+1)));
  }
}

void createTriangle(uint8_t channelID, uint8_t amplitude, uint8_t sampleRate)
{
  for (int16_t i = 0; i <= sampleRate; i++)
  {
    if (channelID == 1)
    {
      if (i <= sampleRate/2)
        channel1Signal[i] = (int)((amplitude / (sampleRate / 2.0f)) * i);
      
      else
        channel1Signal[i] = (sampleRate - i) * (amplitude / (sampleRate / 2.0f));
    }

    else //for channel 2
    {
      if (i <= sampleRate/2)
        channel2Signal[i] = (int)((amplitude / (sampleRate / 2.0f)) * i);
      
      else
        channel2Signal[i] = (sampleRate - i) * (amplitude / (sampleRate / 2.0f));
    }
  } 
}

void createRamp(uint8_t channelID, uint8_t amplitude, uint8_t sampleRate)
{
    for (int16_t i = 0; i <= sampleRate; i++)
    {
      if (channelID == 1)
        channel1Signal[i] = (int)((amplitude / sampleRate) * i);

      else //for channel 2
        channel2Signal[i] = (int)((amplitude / sampleRate) * i);
    }
}

void outputUpdate(uint8_t channel1, uint8_t channel2)
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
      Serial.print("selected channel: "); //debug only
      Serial.println(selectedChannel); //debug only
    }

    char amplitude[] = "255", sampleRate[] = "255", delayLocal[] = "255";

    memcpy(amplitude, &message[2],3); //extract amplitude values from incomming data
    memcpy(sampleRate, &message[6],3); //extract sample rate values from incomming data
    memcpy(delayLocal, &message[10],3); //extract sample rate values from incomming data

    selectedAmplitude = atoi(amplitude); //save data to int variable
    selectedSampleRate = atoi(sampleRate); //save data to int variable
    selectedDelay = atoi(delayLocal); //save data to int variable

    Serial.print("Amplitude: ");
    Serial.println(selectedAmplitude);

    Serial.print("Sample rate: ");
    Serial.println(selectedSampleRate);

    Serial.print("Delay: ");
    Serial.println(selectedDelay);


    switch (message[1])
    {
    case 's':
      createSinus(selectedChannel, selectedAmplitude, selectedSampleRate);
      break;

    case 't':
      createTriangle(selectedChannel, selectedAmplitude, selectedSampleRate);
      break;

    case 'w':
      createRamp(selectedChannel, selectedAmplitude, selectedSampleRate);
      break;

    case 'e':
      createSquare(selectedChannel, selectedAmplitude, selectedSampleRate);
      break;

    default:
      Serial.println("Wrong value");
      break;
    }

    if (selectedChannel == 1)
      channel1SampleRate = selectedSampleRate;

    else
      channel2SampleRate = selectedSampleRate;
  }
}

void setup()
{
  Serial.begin(9600);
  //usart_setup(); for atmega 128

  DDRA = 0xff; //set whole Port A as output CHANNEL 1 8bits
  DDRC = 0xff; //set whole Port C as output CHANNEL 2 8bits
  DDRL = 0xff; //dummy port to confuse compiler and add my delay to final compiled project because _delay_us takes to much time. and I don't want to be that slow
}

int main(void)
{
  init(); //Initialisation for serial line from arduino.h
  setup();
  uint8_t i,j = 0; //loop trought arrays

  while (1)
  {
    if (Serial.available()) //when there are data waiting in a buffer
      inputManagement();

    if (selectedDelay > 0) //custom delay affects both channels
    {
      for (int o = 0; o < 2 * selectedDelay; o++)
        PORTL = (uint8_t)o;
    }

    if (i < channel1SampleRate)
      i++;
    else
      i = 1;

    if (j < channel2SampleRate)
      j++;
    else
      j = 1;

    outputUpdate(channel1Signal[i], channel2Signal[j]);
  }
  return 0;
}
