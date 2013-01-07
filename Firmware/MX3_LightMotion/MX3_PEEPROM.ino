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
  write(EE_CAMBULB, camera_bulb);
  write(EE_CAMLOCK, camera_focLock);
  
    // write data about each motor  
  for(byte i = 0; i < MOTOR_COUNT; i++) {
    write(EE_M0FLAG + (EE_MOTOR_SPACE * i), *((byte*) &motors[i].flags));
    write(EE_M0RPM + (EE_MOTOR_SPACE * i), motors[i].rpm);
    write(EE_M0RATIO + (EE_MOTOR_SPACE * i), motors[i].ratio);
    write(EE_M0RAMP + (EE_MOTOR_SPACE * i), motors[i].ramp);
    write(EE_M0LEAD + (EE_MOTOR_SPACE * i), motors[i].lead);
  }
  
  write(EE_LCDOFF, lcdDisable);
  write(EE_ALT1, alt_inputs[0]);
  write(EE_ALT2, alt_inputs[1]);
  write(EE_PERIOD, motor_pwm_minperiod);
  write(EE_MPRESET, *((byte*) motorPresetSelected), sizeof(motorPresetSelected) / sizeof(byte));
  
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
  read(EE_CAMBULB, camera_bulb);
  read(EE_CAMLOCK, camera_focLock);

  
      // read data about each motor  
  for(byte i = 0; i < MOTOR_COUNT; i++) {
    read(EE_M0FLAG + (EE_MOTOR_SPACE * i), *((byte*) &motors[i].flags));
    read(EE_M0RPM + (EE_MOTOR_SPACE * i), motors[i].rpm);
    read(EE_M0RATIO + (EE_MOTOR_SPACE * i), motors[i].ratio);    
    read(EE_M0RAMP + (EE_MOTOR_SPACE * i), motors[i].ramp);
    read(EE_M0LEAD + (EE_MOTOR_SPACE * i), motors[i].lead);
  }
  
  read(EE_LCDOFF, lcdDisable);
  read(EE_ALT1, alt_inputs[0]);
  read(EE_ALT2, alt_inputs[1]);
  read(EE_PERIOD, motor_pwm_minperiod);
  read(EE_MPRESET, *((byte*) motorPresetSelected), sizeof(motorPresetSelected) / sizeof(byte));
}


