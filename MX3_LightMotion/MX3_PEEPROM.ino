/*


MX3 LightMotion

See dynamicperception.com for more information


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
  EEPROM memory management functions
  ========================================
  
*/

 // stored memory layout version
const unsigned int MEMORY_VERSION    = 1;


/* Locations of each variable to be stored, note correct spacing
   based on data size */

const int EE_NONE      = -1; // do not store

const int EE_SMS       = 1; // motion_sms

const int EE_ALTDIR    = 2; // alt_direction


const int EE_MAXSHOT   = 4; // camera max shots

const int EE_CAMREP    = 10; // camera_repeat
const int EE_CAMDEL    = 11; // camera_delay
const int EE_CAMEXP    = 15; // cam_exposure
const int EE_CAMWAIT   = 19; // cam_wait
const int EE_CAMFOC    = 23; // cam_focus


const int EE_M0FLAG    = 27; // flags
const int EE_M0ONP     = 28; // on periods
const int EE_M0RPM     = 30; // rpm
const int EE_M0RATIO   = 34; // ratio

  // note: for each motor, we move the previous defs ahead 12 bytes * motor num

const int EE_MOTOR_SPACE = 12;  
const int EE_POSTMOTOR   = 37 + (EE_MOTOR_SPACE * 2);

/** Check EEPROM Status

 If EEPROM hasn't been stored, or EEPROM version does not
 match our version, it saves our variables to eeprom memory.
 
 Otherwise, it reads stored variables from EEPROM memory
 
 @author C. A. Church
 */
 
void eepromCheck() {
  
  using namespace OMEEPROM;
    
  if( saved() ) {
      if( version() != MEMORY_VERSION )
        eepromWrite();
      else 
        eepromRestore();
  }
  else {
    eepromWrite();
  }
    
}

 /** Write All Variables to EEPROM */
 
void eepromWrite() {
  using namespace OMEEPROM;
 
  version(MEMORY_VERSION);
  
  write(EE_SMS, motion_sms);
  write(EE_ALTDIR, alt_direction);
  write(EE_MAXSHOT, camera_max_shots);
  write(EE_CAMREP, camera_repeat);
  write(EE_CAMDEL, camera_delay);
  write(EE_CAMEXP, camera_exposure);
  write(EE_CAMWAIT, camera_wait);
  write(EE_CAMFOC, camera_focus);
  
    // write data about each motor  
  for(byte i = 0; i < MOTOR_COUNT; i++) {
    write(EE_M0FLAG + (EE_MOTOR_SPACE * i), *((byte*) &motors[i].flags));
    write(EE_M0ONP + (EE_MOTOR_SPACE * i), motors[i].onPeriods);
    write(EE_M0RPM + (EE_MOTOR_SPACE * i), motors[i].rpm);
    write(EE_M0RATIO + (EE_MOTOR_SPACE * i), motors[i].ratio);
  }
  
}


 /** Read all variables from EEPROM */
 
void eepromRestore() {
  using namespace OMEEPROM;
  
  read(EE_SMS, motion_sms);
  read(EE_ALTDIR, alt_direction);
  read(EE_MAXSHOT, camera_max_shots);
  read(EE_CAMREP, camera_repeat);
  read(EE_CAMDEL, camera_delay);
  read(EE_CAMEXP, camera_exposure);
  read(EE_CAMWAIT, camera_wait);
  read(EE_CAMFOC, camera_focus);
  
      // read data about each motor  
  for(byte i = 0; i < MOTOR_COUNT; i++) {
    read(EE_M0FLAG + (EE_MOTOR_SPACE * i), *((byte*) &motors[i].flags));
    read(EE_M0ONP + (EE_MOTOR_SPACE * i), motors[i].onPeriods);
    read(EE_M0RPM + (EE_MOTOR_SPACE * i), motors[i].rpm);
    read(EE_M0RATIO + (EE_MOTOR_SPACE * i), motors[i].ratio);    
  }
  
}


