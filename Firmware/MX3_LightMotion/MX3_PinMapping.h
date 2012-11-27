
/* 

   MX3 LightMotion Firmware
   
   (c) 2008-2012 C.A. Church / Dynamic Perception LLC
   
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.


*/


/*

  ========================================
  Pin Mapping
  ========================================
  
*/

  
const byte   SHUTTER_PIN     = 22;
const byte   FOCUS_PIN       = 21;

    // lcd pins
const byte   LCD_RS          = 7;
const byte   LCD_EN          = 6;
const byte   LCD_D4          = 2;
const byte   LCD_D5          = 3;
const byte   LCD_D6          = 4;
const byte   LCD_D7          = 5;
const byte   LCD_BKL         = 19;

 // which input is our button
const byte   BUT_PIN         = 23;

  // first alt i/o pin (digital #)
const byte   ALT_START_PIN   = 2;

  // leave motor definitions as defines!

  // motor driving pin register and first pin mask
#define MOTOR_DRV_PREG      PORTB
#define MOTOR_DRV_FMASK     PORTB1

  // first motor direction pin
const byte MOTOR_DIR_PINSTART = 7;

 // digital pin #s for motor output
 
const byte MOTOR_PWM_0   = 14;
const byte MOTOR_PWM_1   = 15;
const byte MOTOR_PWM_2   = 16;

