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


/* Locations of each variable to be stored, note correct spacing
   based on data size */

const int EE_SMS       = 1; // motion_sms
const int EE_ALTDIR    = 2; // alt_direction
const int EE_MAXSHOT   = 4; // camera max shots
const int EE_CAMTM     = 6; // camera_tm
const int EE_CAMREP    = 10; // camera_repeat
const int EE_CAMDEL    = 11; // camera_delay
const int EE_CAMEXP    = 15; // cam_exposure
const int EE_CAMWAIT   = 19; // cam_wait
const int EE_CAMFOC    = 23; // cam_focus


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
  write(EE_CAMTM, camera_tm);
  write(EE_CAMREP, camera_repeat);
  write(EE_CAMDEL, camera_delay);
  write(EE_CAMEXP, camera_exposure);
  write(EE_CAMWAIT, camera_wait);
  write(EE_CAMFOC, camera_focus);
  
  
}


 /** Read all variables from EEPROM */
 
void eepromRestore() {
  using namespace OMEEPROM;
  
  read(EE_SMS, motion_sms);
  read(EE_ALTDIR, alt_direction);
  read(EE_MAXSHOT, camera_max_shots);
  read(EE_CAMTM, camera_tm);
  read(EE_CAMREP, camera_repeat);
  read(EE_CAMDEL, camera_delay);
  read(EE_CAMEXP, camera_exposure);
  read(EE_CAMWAIT, camera_wait);
  read(EE_CAMFOC, camera_focus);
  
}


