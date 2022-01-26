# AVR-SignalGenerator

This program is meant for AVR based boards aspecially for 8bit MCUs from Atmel (ATmega 128, ATmega 328p, ATmega 2560)

Some MCUs have different register naming for timers, port mapping etc... please check it before usage!
This program is developed on Arduino Mega2560 and later is inported into ATmega 128.

## What does it do?

It's meant to be a 2 Channel signal generator.
To change waveforms you will need to communicate via serial line (RS232)
You will be able to change frequency and waveform type and max aplitude
It's unable to output AC signal because it's missing OP-Amp so max pK-pK voltage is 5V (0V-5V)
It's only using 8bit DAC so it's not super high res. but good enought.

For my testing purposes I do not have DAC converting IC, so I'm using only resistor ladder to create an analog signal on output.

##Communication commands
yet to be released...

##Licence
MIT so do with it what ever you want
