LightMotion Firmware for MX3 / Arduino
=============================================

Version: 1.08
---------------

This is the LightMotion Firmware for the Arduino/MX3 Platform.

Version: 1.08

### Required Libraries

You must install the following libraries to compile the MotionEngine:

 * [OMLibraries](https://github.com/DynamicPerception/OMLibraries) 
 * [MsTimer2](http://www.pjrc.com/teensy/td_libs_MsTimer2.html)
 * [TimerOne](http://code.google.com/p/arduino-timerone/downloads/list)

For MsTimer2 you must replace all lines of:

"#if defined (__AVR_ATmega168__) || defined (__AVR_ATmega48__) || defined (__AVR_ATmega88__) || defined (__AVR_ATmega328P__) || defined (__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) || defined(__AVR_AT90USB646__) || defined(__AVR_AT90USB1286__)"

with line:

"#if defined (__AVR_ATmega168__) || defined (__AVR_ATmega48__) || defined (__AVR_ATmega88__) || defined (__AVR_ATmega328P__)  || defined(__AVR_ATmega644p__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) || defined(__AVR_AT90USB646__) || defined(__AVR_AT90USB647__) || defined(__AVR_AT90USB1286__) || defined(__AVR_AT90USB1287__)"

This will allow the MsTimer2 to work with the chip used in the MX3.

For more information on installing libraries in Arduino, see [Arduino Libraries Guide](http://arduino.cc/en/Guide/Libraries).

### Arduino Requirements

You must use at least Arduino version 1.5.2 (nightly build is recommended), and must use the Dynamic Perception AT90USB Core for Arduino.  This can be downloaded [here](https://github.com/DynamicPerception/ArduinoAT90USB)

### Uploading to MX3

Key tips for uploading to the MX3:

 * Ensure you select 'AT90USB128x 16Mhz' as the Board Type (do not select the 'USB Serial' variant!)
 * You must press and hold the [Enter] key before plugging in the USB port on the MX3
     - Note that the LCD backlight will flash showing it is ready for firmware upload

### Bootloader

Use the LUFA CDC bootloader from the ArduinoAT90USB repo.

### More Information
 
More information can be found at http://www.dynamicperception.com/

### Copyright and License

(c) 2008-2014 Dynamic Perception LLC

The LightMotion firmware is distributed under a GPLv3 License. See LICENSE.txt for licensing information.

