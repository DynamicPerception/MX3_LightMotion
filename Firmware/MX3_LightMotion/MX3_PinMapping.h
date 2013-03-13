
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

  
const byte   SHUTTER_PIN     = 13;
const byte   FOCUS_PIN       = 15;

    // lcd pins
const byte   LCD_RS          = 21;
const byte   LCD_EN          = 20;
const byte   LCD_D4          = 16;
const byte   LCD_D5          = 17;
const byte   LCD_D6          = 18;
const byte   LCD_D7          = 19;
const byte   LCD_BKL         = 22;

 // which input is our button
const byte   BUT_PIN         = 40;

  // first alt i/o pin (digital #)
const byte   ALT_START_PIN   = 36;

  // leave motor definitions as defines!

  // motor driving pin register and first pin mask
#define MOTOR_DRV_PREG      PORTA
#define MOTOR_DRV_FMASK     PORTA0

  // first motor direction pin
const byte MOTOR_DIR_PINSTART = 0;

 // digital pin #s for motor inhibit
 
const byte MOTOR_INH_0   = 2;
const byte MOTOR_INH_1   = 5;
const byte MOTOR_INH_2   = 8;

 // analog pins for sensors
 
const byte CUR_SENSOR  = 41;
const byte VOL_SENSOR  = 42;
const byte TEMP_SENSOR_0 = 45;
const byte TEMP_SENSOR_1 = 44;
const byte TEMP_SENSOR_2 = 43;
