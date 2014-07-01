

/* 

   MX3 LightMotion Firmware
   
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
  Cursor Interaction Functions
  (Interacting with screen elements on main screens)
  ========================================
  
*/



/*

 ==============
 Main Screen Cursors
 ==============
 
 */
 
 
 /** Toggle Run State
 
 */

void uiCursorToggleRun(byte p_dir) {
  if( running ){
    stopProgram();
  } 
  else if(camera_timer > 0 && !camera_flag) {
    start_time = millis();
    delay_time = start_time;
    camera_flag = true;
  } 
  else if (camera_flag){
    camera_flag = false;
  } 
  else {
	  refreshMotors(true); // Make sure the motors have the correct power settings before starting
	  startProgram();    
  }
    
  eepromWrite();
   
}


/** Adjust Interval Time
*/
void uiCursorAdjustInt(byte p_dir) {
  
  float incr = 0.1;
  
  if( ! p_dir ) {
    if( camera_delay <= 0.1 )
      camera_delay = 0.0;
    else
      camera_delay -= incr;
  }
  else
     camera_delay += incr;
  
  OMEEPROM::write(EE_CAMDEL, camera_delay); 
      
}

/** Adjust SMS setting
*/

void uiCursorAdjustSMS(byte p_dir) {
  
  // can't switch to SMS while running!
  if( running )
    return;
  
  // Switch SMS state
  motion_sms = !motion_sms;

  // Arrays to store the current mode speed. Allows
  // for restoring of last used speed in each state
  static float continuous_speed[3];
  static float SMS_dist[3];

  // Emperically tested PWM settings for each mode
  const int CONTINUOUS_PERIOD = 1200;
  const int SMS_PERIOD = 300;

  // For SMS mode, set the speed to 0.001 in/s, cm/s, etc.
  if(motion_sms) {
	  
	  for (byte i = 0; i < MOTOR_COUNT; i++) {

		  // Save the continuous speeds for later
		  continuous_speed[i] = motors[i].target_speed;

		  // Set PWM period
		  motor_pwm_minperiod = SMS_PERIOD;

		  // Refresh the motor power settings. Do this just once, since refeshMotors() refreshes them all
		  if (i == (MOTOR_COUNT - 1))
			  refreshMotors(true);

		  // Save the new settings
		  OMEEPROM::write(EE_M0FLAG + (EE_MOTOR_SPACE * i), motors[i].flags);
	  }
  }
  // Continuous mode
  else {
	  for(byte i = 0; i < MOTOR_COUNT; i++ ) {
		  
		  // Save the SMS speeds for later
		  SMS_dist[i] = motors[i].target_sms_distance;
		 
		  // Restore the speeds last used in continuous mode
		  motors[i].target_speed = continuous_speed[i];

		  // Refresh the motor power settings. Do this just once, since refeshMotors() refreshes them all
		  if (i == (MOTOR_COUNT - 1))
			  refreshMotors(true);

		  // Set PWM period
		  motor_pwm_minperiod = CONTINUOUS_PERIOD;
		  
		  // Save the new settings
		  OMEEPROM::write(EE_M0FLAG + (EE_MOTOR_SPACE * i), motors[i].flags);
	  }
  }

  // If we're in EZ mode, update all the motor speeds using the EZ adjust value.
  if (ez_mode)
	  EZmodeUpdateAll();


  OMEEPROM::write(EE_SMS, motion_sms);  
}

/** Enter EZ Mode **/

void uiCursorEZmode(byte p_dir){

	// Can't switch to EZ mode while running
	if (running)
		return;

	Menu.enable(false);

	// Switch EZ mode state
	ez_mode = !ez_mode;

	static byte enable_states[MOTOR_COUNT];

	for (byte i = 0; i < MOTOR_COUNT; i++) {
		// If entering EZ mode, save the current motor enable states and then enable them
		if (ez_mode) {
			// Save the MOTOR_UEN_FLAG only if the current state is disabled. This makes the XOR logic work when restoring the states below
			enable_states[i] = (motors[i].flags & MOTOR_UEN_FLAG) ? 0 : MOTOR_UEN_FLAG;
			motors[i].flags |= MOTOR_UEN_FLAG;
		}
		// If exiting EZ mode, restore the motor enable states
		else {
			motors[i].flags ^= enable_states[i];
		}
	}

	// If we're entering EZ mode and not exiting it, request setup info
	if (ez_mode) {

		// Prompt the user to camera focal length
		lcd.noBlink();
		lcd.clear();
		lcd.home();
		lcd.print("Set focal length");
		lcd.setCursor(0, 1);
		lcd.print(camera_focal_length);
		lcd.setCursor(3, 1);
		lcd.print("mm");

		// Remain in the prompt until the user hits "select"
		byte button;
		while (button != BUTTON_SELECT) {

			button = Menu.checkInput();
			if (button == BUTTON_INCREASE || button == BUTTON_DECREASE) {

				uiCursorChangeFocalLength(button);
				lcd.setCursor(0, 1);
				lcd.print("   ");
				lcd.setCursor(0, 1);
				lcd.print(camera_focal_length);
			}
		}

		for (byte i = 0; i < MOTOR_COUNT; i++) {
			// Prompt user to set the motor presets
			uiMenuPreset(i);
		}

		for (byte i = 0; i < MOTOR_COUNT; i++) {
			// Reset the ez_adjust value to 1.0
			motors[i].ez_adjust = 1.0;
		}

		

		// Update the motor speeds using the default EZ settings
		EZmodeUpdateAll();
	}

	lcd.clear();

	Menu.enable(true);

	OMEEPROM::write(EE_EZMODE, ez_mode);
}

/*

==============
EZ Mode Screen Cursors
==============

*/

// Change EZ Direction
void uiCursorEZdir0(byte p_dir){
	uiCursorEZdir(0);
}

void uiCursorEZdir1(byte p_dir){
	uiCursorEZdir(1);
}

void uiCursorEZdir2(byte p_dir){
	uiCursorEZdir(2);
}

void uiCursorEZdir(byte p_motor) {
	motorDirFlip(p_motor);
	OMEEPROM::write(EE_M0FLAG + (EE_MOTOR_SPACE * p_motor), motors[p_motor].flags);
}


// Change EZ Adjust Value
void uiCursorEZadjust0(byte p_dir){
	changeEZadjust(0, p_dir);
}

void uiCursorEZadjust1(byte p_dir){
	changeEZadjust(1, p_dir);
}

void uiCursorEZadjust2(byte p_dir){
	changeEZadjust(2, p_dir);
}

void changeEZadjust(byte p_motor, byte p_dir) {

	float increment = p_dir ? EZADJUST_INCREMENT : -EZADJUST_INCREMENT;
	motors[p_motor].ez_adjust += increment;
	if (motors[p_motor].ez_adjust < EZADJUST_MIN) {
		motors[p_motor].ez_adjust = EZADJUST_MIN;

	}
	else if (motors[p_motor].ez_adjust > EZADJUST_MAX) {
		motors[p_motor].ez_adjust = EZADJUST_MAX;
	}

	OMEEPROM::write((EE_EZADJ0 + EE_MOTOR_SPACE_V1_1 * p_motor), motors[p_motor].ez_adjust);
}


/*

 ==============
 Cam Screen Cursors
 ==============
 
 */


/** Interacting with shutter time on Cam Screen
*/

void uiCursorChangeShutterTime(byte p_dir) {
 
  unsigned long *ptr = &camera_wait;
  int            mod = p_dir ? 1 : -1;
  int            mem = EE_CAMWAIT;
  
 if( camera_bulb ) {
   ptr = &camera_exposure;
   mem = EE_CAMEXP;
 }

 float          div = (float) *ptr / 1000.0;
   
 if( div >= UI_CAMDIVS[UI_CAMDIVSIZE] ) {
     // we are greater than or equal to the largest div, generally we move in 1 second increments above
     // this range
    if( div >= (UI_CAMDIVS[UI_CAMDIVSIZE] + 1.0) ) {
       div += mod;
    }
    else { 
       if( p_dir )
         div = UI_CAMDIVS[UI_CAMDIVSIZE] + 1.0;
       else 
         div = UI_CAMDIVS[UI_CAMDIVSIZE - 1];
    }
 }
 else {
   
     // we are below the largest div, we locate our current division, and move up or down
     // to the next
     
   byte pos = 0;
   
   for( pos = 0; pos <= UI_CAMDIVSIZE; pos++ )
     if( UI_CAMDIVS[pos] >= div ) 
       break;
   
     // if we're not trying to go below zero index,
     // change to next index
   if( ! ( pos == 0 && ! mod ) ) 
     div = UI_CAMDIVS[pos + mod];
 
 }
 
   // set the floor on bulb exposure times
 if( camera_bulb ) 
   div = div < CAM_MIN_BULB ? CAM_MIN_BULB : div;
   
 
   // set correct wait value
 *ptr = (unsigned long) (1000.0 * div);
 
   // never go below 1ms exposure time
 if( *ptr == 0 )
   *ptr = 1;
   
 OMEEPROM::write(mem, *ptr);
 
   // if already running, re-configure camera
 if( running )
   camSetup();
 
}


 /** Interacting with Bulb setting on Camera Screen */
void uiCursorChangeCamBulb(byte p_dir) {
  
  camera_bulb = !camera_bulb;
  
  OMEEPROM::write(EE_CAMBULB, camera_bulb);
  
  if( running )
    camSetup(); // reinitialize cam
    
}


void uiCursorChangeFocusTime(byte p_dir) {
 
 if( p_dir ) 
  camera_focus += 500;
 else if(camera_focus >= 500)
  camera_focus -= 500;
 else
  camera_focus = 0;
 
 OMEEPROM::write(EE_CAMFOC, camera_focus);
  
 if( running )
    camSetup(); // reinitialize cam
    
}

 //Adjust the camera focal length
void uiCursorChangeFocalLength(byte p_dir) {

	if ((p_dir == BUTTON_INCREASE || p_dir == 1) && camera_focal_length < CAMFL_MAX && camera_focal_length >= CAMFL_MIN)
		camera_focal_length++;
	else if ((p_dir == BUTTON_DECREASE || p_dir == 0) && camera_focal_length <= CAMFL_MAX && camera_focal_length > CAMFL_MIN)
		camera_focal_length--;
	else
		camera_focal_length = camera_focal_length;

	// If for some reason the focal length was set outside the acceptable bounds, reset it to the minimum value
	if (!(camera_focal_length <= CAMFL_MAX && camera_focal_length >= CAMFL_MIN))
		camera_focal_length = CAMFL_MIN;
		

	OMEEPROM::write(EE_CAMFL, camera_focal_length);

}


/*

 ==============
 Motor Screen Cursors
 ==============
 
 */
 
 
void uiCursorChangeMotEn(byte p_dir) {
  if (motion_sms)
  {
   while (motor_running == true) //wait until motor stops moving before changing direction
   Engine.checkCycle();
  } 
  
    // if currently running, and a ramp, ramp out
  if( (running == true) && (motors[ui_curMotor].flags & MOTOR_UEN_FLAG) && (motors[ui_curMotor].ramp_end > 0) && ((motors[ui_curMotor].inRamp) == false) ) {
    
    motorForceRamp(ui_curMotor);
  }
  else {
    if( motors[ui_curMotor].flags & MOTOR_UEN_FLAG ) {
        // motor is currently enabled
        
       // make sure motor is properly stopped
      motorStopThis(ui_curMotor);
        
        // disable motor
      motors[ui_curMotor].flags &= (B11111111 ^ MOTOR_UEN_FLAG);
    }
    else {
      //sets the speed to zero to prevent any jump in the motor when the motor is enabled and the program is on
      motors[ui_curMotor].onCycleRatio = 0.0;
      
        // program already running when motor turned on?
      if( running ) {
        
        if( motors[ui_curMotor].ramp_start > 0 ) {
          motors[ui_curMotor].startShots = camera_fired;
           // set speed to zero before starting motor!
          motorSpeed(ui_curMotor, 0, true);
        }
        
        motorRun(motion_sms, ui_curMotor);
      } // end if(running
      
      motors[ui_curMotor].flags |= MOTOR_UEN_FLAG;  
      
    } // end else (motor uen)

    OMEEPROM::write(EE_M0FLAG + (EE_MOTOR_SPACE * ui_curMotor), motors[ui_curMotor].flags);
  } // end else (currently running, ramp set...)
  
}

/** Helper functions for the changing the motor speed/SMS distance from the UI **/

void uiCursorChangeMotSpd_100(byte p_dir) {
	uiCursorChangeMotSpd(p_dir, 100);
}
void uiCursorChangeMotSpd_10(byte p_dir) {
	uiCursorChangeMotSpd(p_dir, 10);
}
void uiCursorChangeMotSpd_1(byte p_dir) {
	uiCursorChangeMotSpd(p_dir, 1);
}
void uiCursorChangeMotSpd_0_1(byte p_dir) {
	uiCursorChangeMotSpd(p_dir, 0.1);
}
void uiCursorChangeMotSpd_0_01(byte p_dir) {
	uiCursorChangeMotSpd(p_dir, 0.01);
}

/** Change the target speed or SMS distance for the current motor

	@param p_dir
	Whether the button press was an increase or decrease

	@param p_mod
	// By how much the speed value should be changed

	@author
	M. Ploof
*/
void uiCursorChangeMotSpd(byte p_dir, float p_mod) {

	float		curSpd;												// Value used to set motorSpeedCalc() for continuous moves or motorSpeed() for SMS moves	

	// Percent mode
	if (units == PERCENT && !ez_mode) {
		if (motion_sms){

			// Convert from power setting to seconds
			curSpd = motors[ui_curMotor].speed * SEC_PER_MIN;
			// Add or subtract the necessary seconds
			curSpd += p_dir ? p_mod : p_mod * -1.0;
			// Convert back to power setting 0.0-1.0
			curSpd /= SEC_PER_MIN;

		}
		else {
			curSpd = motors[ui_curMotor].speed;
			// Divide the modifier by 100 since the user sees a value 0-100, but the power setting is really 0.0-1.0
			p_mod /= 100;
			curSpd += p_dir ? p_mod : p_mod * -1.0;
			curSpd = curSpd > 1.0 ? 1.0 : curSpd < 0.0 ? 0.0 : curSpd;
		}

		motorSpeed(ui_curMotor, curSpd);

	}
	// Standard or metric mode
	else if ((units == STANDARD || units == METRIC) && !ez_mode) {
		// SMS speed change
		if (motion_sms) {
			// Adjusting up or down?
			motors[ui_curMotor].target_sms_distance += p_dir ? p_mod : p_mod * -1.0;
			// If we try to set the distance to less than zero, set it to zero
			motors[ui_curMotor].target_sms_distance = motors[ui_curMotor].target_sms_distance > 0 ? motors[ui_curMotor].target_sms_distance : 0;
			// Divide the distance by the max speed for this motor to determine what fraction of a minute it should stay on to complete the SMS move
			curSpd = motors[ui_curMotor].target_sms_distance / (motorMaxSpeed(ui_curMotor));

			//USBSerial.println("SMS change");
			//USBSerial.print("Setting speed to: ");
			//USBSerial.println(curSpd, 5);

			/*
			// Set the motor power perecent directly
			motorSpeed(ui_curMotor, curSpd);
			*/

			motorSpeedCalc(ui_curMotor, motors[ui_curMotor].target_sms_distance);

			// Save new SMS distance
			OMEEPROM::write((EE_DES_SMSDIST0 + EE_MOTOR_SPACE_V1_1 * ui_curMotor), motors[ui_curMotor].target_sms_distance);
		}

		// Continuous speed change
		else {
			//mod = motorIncrement;
			motors[ui_curMotor].target_speed += p_dir ? p_mod : p_mod * -1.0;
			// If the user attempts to set the motor below 0, set it to 0. If the user attempts to set the motor higher than the maximum possible speed for the battery voltage, set it to the max speed
			motors[ui_curMotor].target_speed = motors[ui_curMotor].target_speed < 0 ? 0 : motors[ui_curMotor].target_speed > motorMaxSpeedCompensated(ui_curMotor) ? motorMaxSpeedCompensated(ui_curMotor) : motors[ui_curMotor].target_speed;
			curSpd = motors[ui_curMotor].target_speed;
			
			//USBSerial.println("Continuous change");
			//USBSerial.print("Setting speed to: ");
			//USBSerial.println(curSpd, 4);

			// Set the motor power percent via the speed calculation function
			motorSpeedCalc(ui_curMotor, curSpd);

			// Save new continuous speed
			OMEEPROM::write((EE_DES_SPEED0 + EE_MOTOR_SPACE_V1_1 * ui_curMotor), motors[ui_curMotor].target_speed);
		}
	}
	// EZ mode
	else if (ez_mode) {
		// Adjust the value
		motors[ui_curMotor].ez_adjust += p_dir ? EZADJUST_INCREMENT : EZADJUST_INCREMENT * -1.0;
		// Make sure it's within the acceptable bounds
		motors[ui_curMotor].ez_adjust = motors[ui_curMotor].ez_adjust < EZADJUST_MIN ? EZADJUST_MIN : motors[ui_curMotor].ez_adjust > EZADJUST_MAX ? EZADJUST_MAX : motors[ui_curMotor].ez_adjust;
		// Update the real speed
		EZmodeUpdate(ui_curMotor);
		// Save the EZ adjuste value to EEPROM
		OMEEPROM::write((EE_EZADJ0 + EE_MOTOR_SPACE_V1_1*ui_curMotor), motors[ui_curMotor].ez_adjust);
	}
	
	OMEEPROM::write(EE_MORSPEED + (EE_MOTOR_SPACE * ui_curMotor), motors[ui_curMotor].speed);

}

void uiCursorChangeMotDir(byte p_dir) {
 motorDirFlip(ui_curMotor);
 OMEEPROM::write(EE_M0FLAG + (EE_MOTOR_SPACE * ui_curMotor), motors[ui_curMotor].flags);
}


void uiCursorChangeMotLead(byte p_dir) {
  motors[ui_curMotor].lead += p_dir ? 1 : -1;
  if (motors[ui_curMotor].lead >= 655)
    motors[ui_curMotor].lead = 0;
  OMEEPROM::write(EE_M0LEAD + (EE_MOTOR_SPACE * ui_curMotor), motors[ui_curMotor].lead);
}

void uiCursorChangeMotRamp(byte p_dir) {
  
    // don't allow ramp change during ramp execution, that would be bad.
  if( motors[ui_curMotor].inRamp ) 
    return;
 
   // set both ramps the same when changed here
  motors[ui_curMotor].ramp_start += p_dir ? 1 : -1;
  if (motors[ui_curMotor].ramp_start >= 655)
    motors[ui_curMotor].ramp_start = 0;
  motors[ui_curMotor].ramp_end    = motors[ui_curMotor].ramp_start;
}

