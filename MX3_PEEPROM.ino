/*


MX3 LightMotion

See dynamicperception.com for more information


(c) 2008-2013 C.A. Church / Dynamic Perception LLC

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
      {
        eepromWrite();
        eepromWrite_SS0();
        eepromWrite_SS1();
        eepromWrite_SS2();
      }
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
  write(EE_CAMTIMER, camera_timer);
  
  
    // write data about each motor  
  for(byte i = 0; i < MOTOR_COUNT; i++) {
    write(EE_M0FLAG + (EE_MOTOR_SPACE * i), *((byte*) &motors[i].flags));
    write(EE_M0RPM + (EE_MOTOR_SPACE * i), motors[i].rpm);
    write(EE_M0RATIO + (EE_MOTOR_SPACE * i), motors[i].ratio);
    write(EE_M0RAMP + (EE_MOTOR_SPACE * i), motors[i].ramp_start);
    write(EE_M0RAMPE + (EE_MOTOR_SPACE * i), motors[i].ramp_end);
    write(EE_M0LEAD + (EE_MOTOR_SPACE * i), motors[i].lead);
	write(EE_MORSPEED + (EE_MOTOR_SPACE * i), motors[i].speed);
	write(EE_DES_SPEED0 + (EE_MOTOR_SPACE_V1_1 * i), motors[i].target_speed);
	write(EE_DES_SMSDIST0 + (EE_MOTOR_SPACE_V1_1 * i), motors[i].target_sms_distance);
	write(EE_EZADJ0 + (EE_MOTOR_SPACE_V1_1 * i), motors[i].ez_adjust);
	
  }
  
  write(EE_LCDOFF, lcdDisable);
  write(EE_VFDBRI, VFDBright);
  
  write(EE_ALT1, alt_inputs[0]);
  write(EE_ALT2, alt_inputs[1]);
  write(EE_ALT3, alt_inputs[2]);
  write(EE_ALT4, alt_inputs[3]);
  write(EE_ALTDIR, alt_direction);
  write(EE_ALTBD, alt_before_delay);
  write(EE_ALTBT, alt_before_ms);
  write(EE_ALTAD, alt_after_delay);
  write(EE_ALTAT, alt_after_ms);
  write(EE_ALTHL, alt_out_trig);
  
  write(EE_PERIOD, motor_pwm_minperiod);
  write(EE_MPRESET, *((byte*) motorPresetSelected), sizeof(motorPresetSelected) / sizeof(byte));
  write(EE_VOLTH, sensor_minVoltage);
  write(EE_VOLWARN, sensor_enVWarn);
  write(EE_HEATER, sensor_enHeater);
  write(EE_UNITS, units);
  write(EE_INCREMENT, motorIncrement);

  // Is the device set as a MoCoBus node?
  write(EE_NODE, node);
  write(EE_ADDR, dev_addr);

  write(EE_CAMFL, camera_focal_length);

  write(EE_EZMODE, ez_mode);
  write(EE_EZX, ez_extended);

  write(EE_FACTORY_RESET, factory_reset);
  
}


 /** Read all variables from EEPROM */
 
void eepromRestore() {

  using namespace OMEEPROM;
  
  read(EE_SMS, motion_sms);
  read(EE_MMSETTING, mmSetting);  
  read(EE_ALTDIR, alt_direction);
  read(EE_MAXSHOT, camera_max_shots);
  read(EE_CAMREP, camera_repeat);
  read(EE_CAMDEL, camera_delay);
  read(EE_CAMEXP, camera_exposure);
  read(EE_CAMWAIT, camera_wait);
  read(EE_CAMFOC, camera_focus);
  read(EE_CAMBULB, camera_bulb);
  read(EE_CAMLOCK, camera_focLock);  
  read(EE_CAMTIMER, camera_timer);  
  
      // read data about each motor  
  for(byte i = 0; i < MOTOR_COUNT; i++) {
    read(EE_M0FLAG + (EE_MOTOR_SPACE * i), *((byte*) &motors[i].flags));
    read(EE_M0RPM + (EE_MOTOR_SPACE * i), motors[i].rpm);
    read(EE_M0RATIO + (EE_MOTOR_SPACE * i), motors[i].ratio);    
    read(EE_M0RAMP + (EE_MOTOR_SPACE * i), motors[i].ramp_start);
    read(EE_M0RAMPE + (EE_MOTOR_SPACE * i), motors[i].ramp_end);
    read(EE_M0LEAD + (EE_MOTOR_SPACE * i), motors[i].lead);
    read(EE_MORSPEED + (EE_MOTOR_SPACE * i), motors[i].speed);
	read(EE_DES_SPEED0 + (EE_MOTOR_SPACE_V1_1 * i), motors[i].target_speed);
	read(EE_DES_SMSDIST0 + (EE_MOTOR_SPACE_V1_1 * i), motors[i].target_sms_distance);
	read(EE_EZADJ0 + (EE_MOTOR_SPACE_V1_1 * i), motors[i].ez_adjust);
  }
  
  read(EE_LCDOFF, lcdDisable);
  read(EE_VFDBRI, VFDBright);
  
  read(EE_ALT1, alt_inputs[0]);
  read(EE_ALT2, alt_inputs[1]);
  read(EE_ALT3, alt_inputs[2]);
  read(EE_ALT4, alt_inputs[3]);
  read(EE_ALTDIR, alt_direction);
  read(EE_ALTBD, alt_before_delay);
  read(EE_ALTBT, alt_before_ms);
  read(EE_ALTAD, alt_after_delay);
  read(EE_ALTAT, alt_after_ms);
  read(EE_ALTHL, alt_out_trig);
  
  read(EE_PERIOD, motor_pwm_minperiod);
  read(EE_MPRESET, *((byte*) motorPresetSelected), sizeof(motorPresetSelected) / sizeof(byte));
  read(EE_VOLTH, sensor_minVoltage);
  read(EE_VOLWARN, sensor_enVWarn);
  read(EE_HEATER, sensor_enHeater);
  read(EE_UNITS, units);
  read(EE_INCREMENT, motorIncrement);

  // Is the device set as a MoCoBus node?
  read(EE_NODE, node);
  read(EE_ADDR, dev_addr);
  
  read(EE_CAMFL, camera_focal_length);

  read(EE_EZMODE, ez_mode);
  read(EE_EZX, ez_extended);

  read(EE_FACTORY_RESET, factory_reset);
  
}

/** Write All Variables to Save State 0 */
 
void eepromWrite_SS0() {
 
  using namespace OMEEPROM;
    
  version(MEMORY_VERSION);
  
  write(EE_SMS_SS0, motion_sms);
  write(EE_ALTDIR_SS0, alt_direction);
  write(EE_MAXSHOT_SS0, camera_max_shots);
  write(EE_CAMREP_SS0, camera_repeat);
  write(EE_CAMDEL_SS0, camera_delay);
  write(EE_CAMEXP_SS0, camera_exposure);
  write(EE_CAMWAIT_SS0, camera_wait);
  write(EE_CAMFOC_SS0, camera_focus);
  write(EE_CAMBULB_SS0, camera_bulb);
  write(EE_CAMLOCK_SS0, camera_focLock);
  write(EE_CAMTIMER_SS0, camera_timer);
  
  
    // write data about each motor  
  for(byte i = 0; i < MOTOR_COUNT; i++) {
    write(EE_M0FLAG_SS0 + (EE_MOTOR_SPACE_SS0 * i), *((byte*) &motors[i].flags));
    write(EE_M0RPM_SS0 + (EE_MOTOR_SPACE_SS0 * i), motors[i].rpm);
    write(EE_M0RATIO_SS0 + (EE_MOTOR_SPACE_SS0 * i), motors[i].ratio);
    write(EE_M0RAMP_SS0 + (EE_MOTOR_SPACE_SS0 * i), motors[i].ramp_start);
    write(EE_M0RAMPE_SS0 + (EE_MOTOR_SPACE_SS0 * i), motors[i].ramp_end);
    write(EE_M0LEAD_SS0 + (EE_MOTOR_SPACE_SS0 * i), motors[i].lead);
    write(EE_MORSPEED_SS0 + (EE_MOTOR_SPACE_SS0 * i), motors[i].speed);
	write(EE_DES_SPEED0_SS0 + (EE_MOTOR_SPACE_V1_1_SS0 * i), motors[i].target_speed);
	write(EE_DES_SMSDIST0_SS0 + (EE_MOTOR_SPACE_V1_1_SS0 * i), motors[i].target_sms_distance);
	write(EE_EZADJ0_SS0 + (EE_MOTOR_SPACE_V1_1_SS0 * i), motors[i].ez_adjust);
  }
  
  write(EE_LCDOFF_SS0, lcdDisable);
  
  write(EE_ALT1_SS0, alt_inputs[0]);
  write(EE_ALT2_SS0, alt_inputs[1]);
  write(EE_ALT3_SS0, alt_inputs[2]);
  write(EE_ALT4_SS0, alt_inputs[3]);
  write(EE_ALTDIR_SS0, alt_direction);
  write(EE_ALTBD_SS0, alt_before_delay);
  write(EE_ALTBT_SS0, alt_before_ms);
  write(EE_ALTAD_SS0, alt_after_delay);
  write(EE_ALTAT_SS0, alt_after_ms);
  write(EE_ALTHL_SS0, alt_out_trig);
  
  write(EE_PERIOD_SS0, motor_pwm_minperiod);
  write(EE_MPRESET_SS0, *((byte*) motorPresetSelected), sizeof(motorPresetSelected) / sizeof(byte));
  write(EE_VOLTH_SS0, sensor_minVoltage);
  write(EE_VOLWARN_SS0, sensor_enVWarn);
  write(EE_HEATER_SS0, sensor_enHeater);
  write(EE_UNITS_SS0, units);
  write(EE_INCREMENT_SS0, motorIncrement);

  // Is the device set as a MoCoBus node?
  write(EE_NODE_SS0, node);
  write(EE_ADDR_SS0, dev_addr);

  write(EE_CAMFL_SS0, camera_focal_length);

  write(EE_EZMODE_SS0, ez_mode);
  write(EE_EZX_SS0, ez_extended);

  write(EE_FACTORY_RESET_SS0, factory_reset);
  
}

/** Write All Variables to Save State 1 */
 
void eepromWrite_SS1() {
 
  using namespace OMEEPROM;
    
  version(MEMORY_VERSION);
  
  write(EE_SMS_SS1, motion_sms);
  write(EE_ALTDIR_SS1, alt_direction);
  write(EE_MAXSHOT_SS1, camera_max_shots);
  write(EE_CAMREP_SS1, camera_repeat);
  write(EE_CAMDEL_SS1, camera_delay);
  write(EE_CAMEXP_SS1, camera_exposure);
  write(EE_CAMWAIT_SS1, camera_wait);
  write(EE_CAMFOC_SS1, camera_focus);
  write(EE_CAMBULB_SS1, camera_bulb);
  write(EE_CAMLOCK_SS1, camera_focLock);
  write(EE_CAMTIMER_SS1, camera_timer);
  
    // write data about each motor  
  for(byte i = 0; i < MOTOR_COUNT; i++) {
    write(EE_M0FLAG_SS1 + (EE_MOTOR_SPACE_SS1 * i), *((byte*) &motors[i].flags));
    write(EE_M0RPM_SS1 + (EE_MOTOR_SPACE_SS1 * i), motors[i].rpm);
    write(EE_M0RATIO_SS1 + (EE_MOTOR_SPACE_SS1 * i), motors[i].ratio);
    write(EE_M0RAMP_SS1 + (EE_MOTOR_SPACE_SS1 * i), motors[i].ramp_start);
    write(EE_M0RAMPE_SS1 + (EE_MOTOR_SPACE_SS1 * i), motors[i].ramp_end);
    write(EE_M0LEAD_SS1 + (EE_MOTOR_SPACE_SS1 * i), motors[i].lead);
    write(EE_MORSPEED_SS1 + (EE_MOTOR_SPACE_SS1 * i), motors[i].speed);
	write(EE_DES_SPEED0_SS1 + (EE_MOTOR_SPACE_V1_1_SS1 * i), motors[i].target_speed);
	write(EE_DES_SMSDIST0_SS1 + (EE_MOTOR_SPACE_V1_1_SS1 * i), motors[i].target_sms_distance);
	write(EE_EZADJ0_SS1 + (EE_MOTOR_SPACE_V1_1_SS1 * i), motors[i].ez_adjust);
  }
  
  write(EE_LCDOFF_SS1, lcdDisable);
  
  write(EE_ALT1_SS1, alt_inputs[0]);
  write(EE_ALT2_SS1, alt_inputs[1]);
  write(EE_ALT3_SS1, alt_inputs[2]);
  write(EE_ALT4_SS1, alt_inputs[3]);
  write(EE_ALTDIR_SS1, alt_direction);
  write(EE_ALTBD_SS1, alt_before_delay);
  write(EE_ALTBT_SS1, alt_before_ms);
  write(EE_ALTAD_SS1, alt_after_delay);
  write(EE_ALTAT_SS1, alt_after_ms);
  write(EE_ALTHL_SS1, alt_out_trig);
  
  write(EE_PERIOD_SS1, motor_pwm_minperiod);
  write(EE_MPRESET_SS1, *((byte*) motorPresetSelected), sizeof(motorPresetSelected) / sizeof(byte));
  write(EE_VOLTH_SS1, sensor_minVoltage);
  write(EE_VOLWARN_SS1, sensor_enVWarn);
  write(EE_HEATER_SS1, sensor_enHeater);
  write(EE_UNITS_SS1, units);
  write(EE_INCREMENT_SS1, motorIncrement);

  // Is the device set as a MoCoBus node?
  write(EE_NODE_SS1, node);
  write(EE_ADDR_SS1, dev_addr);

  write(EE_CAMFL_SS1, camera_focal_length);

  write(EE_EZMODE_SS1, ez_mode);
  write(EE_EZX_SS1, ez_extended);

  write(EE_FACTORY_RESET_SS1, factory_reset);
  
}

/** Write All Variables to Save State 2 */
 
void eepromWrite_SS2() {
 
  using namespace OMEEPROM;
    
  version(MEMORY_VERSION);
  
  write(EE_SMS_SS2, motion_sms);
  write(EE_ALTDIR_SS2, alt_direction);
  write(EE_MAXSHOT_SS2, camera_max_shots);
  write(EE_CAMREP_SS2, camera_repeat);
  write(EE_CAMDEL_SS2, camera_delay);
  write(EE_CAMEXP_SS2, camera_exposure);
  write(EE_CAMWAIT_SS2, camera_wait);
  write(EE_CAMFOC_SS2, camera_focus);
  write(EE_CAMBULB_SS2, camera_bulb);
  write(EE_CAMLOCK_SS2, camera_focLock);
  write(EE_CAMTIMER_SS2, camera_timer);
  
    // write data about each motor  
  for(byte i = 0; i < MOTOR_COUNT; i++) {
    write(EE_M0FLAG_SS2 + (EE_MOTOR_SPACE_SS2 * i), *((byte*) &motors[i].flags));
    write(EE_M0RPM_SS2 + (EE_MOTOR_SPACE_SS2 * i), motors[i].rpm);
    write(EE_M0RATIO_SS2 + (EE_MOTOR_SPACE_SS2 * i), motors[i].ratio);
    write(EE_M0RAMP_SS2 + (EE_MOTOR_SPACE_SS2 * i), motors[i].ramp_start);
    write(EE_M0RAMPE_SS2 + (EE_MOTOR_SPACE_SS2 * i), motors[i].ramp_end);
    write(EE_M0LEAD_SS2 + (EE_MOTOR_SPACE_SS2 * i), motors[i].lead);
    write(EE_MORSPEED_SS2 + (EE_MOTOR_SPACE_SS2 * i), motors[i].speed);
	write(EE_DES_SPEED0_SS2 + (EE_MOTOR_SPACE_V1_1_SS2 * i), motors[i].target_speed);
	write(EE_DES_SMSDIST0_SS2 + (EE_MOTOR_SPACE_V1_1_SS2 * i), motors[i].target_sms_distance);
	write(EE_EZADJ0_SS2 + (EE_MOTOR_SPACE_V1_1_SS2 * i), motors[i].ez_adjust);
  }
  
  write(EE_LCDOFF_SS2, lcdDisable);
  
  write(EE_ALT1_SS2, alt_inputs[0]);
  write(EE_ALT2_SS2, alt_inputs[1]);
  write(EE_ALT3_SS2, alt_inputs[2]);
  write(EE_ALT4_SS2, alt_inputs[3]);
  write(EE_ALTDIR_SS2, alt_direction);
  write(EE_ALTBD_SS2, alt_before_delay);
  write(EE_ALTBT_SS2, alt_before_ms);
  write(EE_ALTAD_SS2, alt_after_delay);
  write(EE_ALTAT_SS2, alt_after_ms);
  write(EE_ALTHL_SS2, alt_out_trig);
  
  write(EE_PERIOD_SS2, motor_pwm_minperiod);
  write(EE_MPRESET_SS2, *((byte*) motorPresetSelected), sizeof(motorPresetSelected) / sizeof(byte));
  write(EE_VOLTH_SS2, sensor_minVoltage);
  write(EE_VOLWARN_SS2, sensor_enVWarn);
  write(EE_HEATER_SS2, sensor_enHeater);
  write(EE_UNITS_SS2, units);
  write(EE_INCREMENT_SS2, motorIncrement);

  // Is the device set as a MoCoBus node?
  write(EE_NODE_SS2, node);
  write(EE_ADDR_SS2, dev_addr);

  write(EE_CAMFL_SS2, camera_focal_length);

  write(EE_EZMODE_SS2, ez_mode);
  write(EE_EZX_SS2, ez_extended);

  write(EE_FACTORY_RESET_SS2, factory_reset);
  
}

/** Restore All Variables from Save State 0 */

void eepromRestore_SS0() {

  using namespace OMEEPROM;
  
  read(EE_SMS_SS0, motion_sms);
  read(EE_ALTDIR_SS0, alt_direction);
  read(EE_MAXSHOT_SS0, camera_max_shots);
  read(EE_CAMREP_SS0, camera_repeat);
  read(EE_CAMDEL_SS0, camera_delay);
  read(EE_CAMEXP_SS0, camera_exposure);
  read(EE_CAMWAIT_SS0, camera_wait);
  read(EE_CAMFOC_SS0, camera_focus);
  read(EE_CAMBULB_SS0, camera_bulb);
  read(EE_CAMLOCK_SS0, camera_focLock);
  read(EE_CAMTIMER_SS0, camera_timer);

  
      // read data about each motor  
  for(byte i = 0; i < MOTOR_COUNT; i++) {
    read(EE_M0FLAG_SS0 + (EE_MOTOR_SPACE_SS0 * i), *((byte*) &motors[i].flags));
    read(EE_M0RPM_SS0 + (EE_MOTOR_SPACE_SS0 * i), motors[i].rpm);
    read(EE_M0RATIO_SS0 + (EE_MOTOR_SPACE_SS0 * i), motors[i].ratio);    
    read(EE_M0RAMP_SS0 + (EE_MOTOR_SPACE_SS0 * i), motors[i].ramp_start);
    read(EE_M0RAMPE_SS0 + (EE_MOTOR_SPACE_SS0 * i), motors[i].ramp_end);
    read(EE_M0LEAD_SS0 + (EE_MOTOR_SPACE_SS0 * i), motors[i].lead);
    read(EE_MORSPEED_SS0 + (EE_MOTOR_SPACE_SS0 * i), motors[i].speed);
	read(EE_DES_SPEED0_SS0 + (EE_MOTOR_SPACE_V1_1_SS0 * i), motors[i].target_speed);
	read(EE_DES_SMSDIST0_SS0 + (EE_MOTOR_SPACE_V1_1_SS0 * i), motors[i].target_sms_distance);
	read(EE_EZADJ0_SS0 + (EE_MOTOR_SPACE_V1_1_SS0 * i), motors[i].ez_adjust);
  }
  
  read(EE_LCDOFF_SS0, lcdDisable);

  read(EE_ALT1_SS0, alt_inputs[0]);
  read(EE_ALT2_SS0, alt_inputs[1]);
  read(EE_ALT3_SS0, alt_inputs[2]);
  read(EE_ALT4_SS0, alt_inputs[3]);
  read(EE_ALTDIR_SS0, alt_direction);
  read(EE_ALTBD_SS0, alt_before_delay);
  read(EE_ALTBT_SS0, alt_before_ms);
  read(EE_ALTAD_SS0, alt_after_delay);
  read(EE_ALTAT_SS0, alt_after_ms);
  read(EE_ALTHL_SS0, alt_out_trig);
  
  read(EE_PERIOD_SS0, motor_pwm_minperiod);
  read(EE_MPRESET_SS0, *((byte*) motorPresetSelected), sizeof(motorPresetSelected) / sizeof(byte));
  read(EE_VOLTH_SS0, sensor_minVoltage);
  read(EE_VOLWARN_SS0, sensor_enVWarn);
  read(EE_HEATER_SS0, sensor_enHeater);
  read(EE_UNITS_SS0, units);
  read(EE_INCREMENT_SS0, motorIncrement);

  // Is the device set as a MoCoBus node?
  read(EE_NODE_SS0, node);
  read(EE_ADDR_SS0, dev_addr);

  read(EE_CAMFL_SS0, camera_focal_length);

  read(EE_EZMODE_SS0, ez_mode);
  read(EE_EZX_SS0, ez_extended);

  read(EE_FACTORY_RESET_SS0, factory_reset);
   
}

/** Restore All Variables from Save State 1 */

void eepromRestore_SS1() {

  using namespace OMEEPROM;
  
  read(EE_SMS_SS1, motion_sms);
  read(EE_ALTDIR_SS1, alt_direction);
  read(EE_MAXSHOT_SS1, camera_max_shots);
  read(EE_CAMREP_SS1, camera_repeat);
  read(EE_CAMDEL_SS1, camera_delay);
  read(EE_CAMEXP_SS1, camera_exposure);
  read(EE_CAMWAIT_SS1, camera_wait);
  read(EE_CAMFOC_SS1, camera_focus);
  read(EE_CAMBULB_SS1, camera_bulb);
  read(EE_CAMLOCK_SS1, camera_focLock);
  read(EE_CAMTIMER_SS1, camera_timer);

  
      // read data about each motor  
  for(byte i = 0; i < MOTOR_COUNT; i++) {
    read(EE_M0FLAG_SS1 + (EE_MOTOR_SPACE_SS1 * i), *((byte*) &motors[i].flags));
    read(EE_M0RPM_SS1 + (EE_MOTOR_SPACE_SS1 * i), motors[i].rpm);
    read(EE_M0RATIO_SS1 + (EE_MOTOR_SPACE_SS1 * i), motors[i].ratio);    
    read(EE_M0RAMP_SS1 + (EE_MOTOR_SPACE_SS1 * i), motors[i].ramp_start);
    read(EE_M0RAMPE_SS1 + (EE_MOTOR_SPACE_SS1 * i), motors[i].ramp_end);
    read(EE_M0LEAD_SS1 + (EE_MOTOR_SPACE_SS1 * i), motors[i].lead);
    read(EE_MORSPEED_SS1 + (EE_MOTOR_SPACE_SS1 * i), motors[i].speed);
	read(EE_DES_SPEED0_SS1 + (EE_MOTOR_SPACE_V1_1_SS1 * i), motors[i].target_speed);
	read(EE_DES_SMSDIST0_SS1 + (EE_MOTOR_SPACE_V1_1_SS1 * i), motors[i].target_sms_distance);
	read(EE_EZADJ0_SS1 + (EE_MOTOR_SPACE_V1_1_SS1 * i), motors[i].ez_adjust);
  }
  
  read(EE_LCDOFF_SS1, lcdDisable);

  read(EE_ALT1_SS1, alt_inputs[0]);
  read(EE_ALT2_SS1, alt_inputs[1]);
  read(EE_ALT3_SS1, alt_inputs[2]);
  read(EE_ALT4_SS1, alt_inputs[3]);
  read(EE_ALTDIR_SS1, alt_direction);
  read(EE_ALTBD_SS1, alt_before_delay);
  read(EE_ALTBT_SS1, alt_before_ms);
  read(EE_ALTAD_SS1, alt_after_delay);
  read(EE_ALTAT_SS1, alt_after_ms);
  read(EE_ALTHL_SS1, alt_out_trig);
  
  read(EE_PERIOD_SS1, motor_pwm_minperiod);
  read(EE_MPRESET_SS1, *((byte*) motorPresetSelected), sizeof(motorPresetSelected) / sizeof(byte));
  read(EE_VOLTH_SS1, sensor_minVoltage);
  read(EE_VOLWARN_SS1, sensor_enVWarn);
  read(EE_HEATER_SS1, sensor_enHeater);
  read(EE_UNITS_SS1, units);
  read(EE_INCREMENT_SS1, motorIncrement);

  // Is the device set as a MoCoBus node?
  read(EE_NODE_SS1, node);
  read(EE_ADDR_SS1, dev_addr);

  read(EE_CAMFL_SS1, camera_focal_length);

  read(EE_EZMODE_SS1, ez_mode);
  read(EE_EZX_SS1, ez_extended);

  read(EE_FACTORY_RESET_SS1, factory_reset);
  
}

/** Restore All Variables from Save State 2 */

void eepromRestore_SS2() {

  using namespace OMEEPROM;
  
  read(EE_SMS_SS2, motion_sms);
  read(EE_ALTDIR_SS2, alt_direction);
  read(EE_MAXSHOT_SS2, camera_max_shots);
  read(EE_CAMREP_SS2, camera_repeat);
  read(EE_CAMDEL_SS2, camera_delay);
  read(EE_CAMEXP_SS2, camera_exposure);
  read(EE_CAMWAIT_SS2, camera_wait);
  read(EE_CAMFOC_SS2, camera_focus);
  read(EE_CAMBULB_SS2, camera_bulb);
  read(EE_CAMLOCK_SS2, camera_focLock);
  read(EE_CAMTIMER_SS2, camera_timer);

  
      // read data about each motor  
  for(byte i = 0; i < MOTOR_COUNT; i++) {
    read(EE_M0FLAG_SS2 + (EE_MOTOR_SPACE_SS2 * i), *((byte*) &motors[i].flags));
    read(EE_M0RPM_SS2 + (EE_MOTOR_SPACE_SS2 * i), motors[i].rpm);
    read(EE_M0RATIO_SS2 + (EE_MOTOR_SPACE_SS2 * i), motors[i].ratio);    
    read(EE_M0RAMP_SS2 + (EE_MOTOR_SPACE_SS2 * i), motors[i].ramp_start);
    read(EE_M0RAMPE_SS2 + (EE_MOTOR_SPACE_SS2 * i), motors[i].ramp_end);
    read(EE_M0LEAD_SS2 + (EE_MOTOR_SPACE_SS2 * i), motors[i].lead);
    read(EE_MORSPEED_SS2 + (EE_MOTOR_SPACE_SS2 * i), motors[i].speed);
	read(EE_DES_SPEED0_SS2 + (EE_MOTOR_SPACE_V1_1_SS2 * i), motors[i].target_speed);
	read(EE_DES_SMSDIST0_SS2 + (EE_MOTOR_SPACE_V1_1_SS2 * i), motors[i].target_sms_distance);
	read(EE_EZADJ0_SS2 + (EE_MOTOR_SPACE_V1_1_SS2 * i), motors[i].ez_adjust);
  }
  
  read(EE_LCDOFF_SS2, lcdDisable);

  read(EE_ALT1_SS2, alt_inputs[0]);
  read(EE_ALT2_SS2, alt_inputs[1]);
  read(EE_ALT3_SS2, alt_inputs[2]);
  read(EE_ALT4_SS2, alt_inputs[3]);
  read(EE_ALTDIR_SS2, alt_direction);
  read(EE_ALTBD_SS2, alt_before_delay);
  read(EE_ALTBT_SS2, alt_before_ms);
  read(EE_ALTAD_SS2, alt_after_delay);
  read(EE_ALTAT_SS2, alt_after_ms);
  read(EE_ALTHL_SS2, alt_out_trig);
  
  read(EE_PERIOD_SS2, motor_pwm_minperiod);
  read(EE_MPRESET_SS2, *((byte*) motorPresetSelected), sizeof(motorPresetSelected) / sizeof(byte));
  read(EE_VOLTH_SS2, sensor_minVoltage);
  read(EE_VOLWARN_SS2, sensor_enVWarn);
  read(EE_HEATER_SS2, sensor_enHeater);
  read(EE_UNITS_SS2, units);
  read(EE_INCREMENT_SS2, motorIncrement);

  // Is the device set as a MoCoBus node?
  read(EE_NODE_SS2, node);
  read(EE_ADDR_SS2, dev_addr);

  read(EE_CAMFL_SS2, camera_focal_length);

  read(EE_EZMODE_SS2, ez_mode);
  read(EE_EZX_SS2, ez_extended);

  read(EE_FACTORY_RESET_SS2, factory_reset);
  
}


