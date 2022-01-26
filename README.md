# AVR-SignalGenerator

This program is meant for AVR based boards especially for 8bit MCUs from Atmel (ATmega 128, ATmega 328p, ATmega 2560... etc.)
Some MCUs have different register naming for timers, port mapping etc... please check it before usage!
This program is developed on Arduino Mega2560 and later is imported onto ATmega 128.

## What does it do?
1. It's meant to be a 2 Channel signal generator.
2. To change waveforms you will need to communicate via serial line (RS232)
3. You will be able to change frequency and waveform type and max amplitude
4. It's unable to output AC signal because it's missing OP-Amp so max pK-pK voltage is 5V (0V-5V)
5. It's only using 8bit DAC so it's not super high res. but good enough.

For my testing purposes I do not have DAC converting IC, so I'm using only resistor ladder to create an analog signal on output.

## Communication commands
yet to be released...

## Licence
All code is MIT so do with it what ever you want...
**EXCEPT**  
Not sure about my teacher's own serial communication library [header code](/include/usart.h) / [src code](/src/usart.c)
Feel free to contact him, he's usually a nice guy :D his email: kolousek@spscv.cz
