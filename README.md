LightMotion Firmware for MX3 / Arduino
=============================================

Version: 1.03
---------------

This is the LightMotion Firmware for the Arduino/MX3 Platform.

Version: 1.03

### Required Libraries

You must install the following libraries to compile the MotionEngine:

 * [OMLibraries v1.13](https://github.com/DynamicPerception/OMLibraries/tree/v1.13) 
 * [MsTimer2](http://www.pjrc.com/teensy/td_libs_MsTimer2.html)
 * [TimerOne](http://code.google.com/p/arduino-timerone/downloads/list)


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

(c) 2008-2013 C.A. Church / Dynamic Perception LLC

The LightMotion firmware is distributed under a GPLv3 License. See LICENSE.txt for licensing information.

