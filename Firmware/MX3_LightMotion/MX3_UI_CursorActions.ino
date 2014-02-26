

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
  } else if(camera_timer > 0 && !camera_flag) {
    start_time = millis();
    delay_time = start_time;
    camera_flag = true;
  } else if (camera_flag){
    camera_flag = false;
  } else {
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
    
  motion_sms = !motion_sms;
  
  if(motion_sms)
  {
    for(byte i = 0; i < MOTOR_COUNT; i++ ) 
    {
      motorDir(i, 0);
      motorSpeed(i, 0.001);
      OMEEPROM::write(EE_M0FLAG + (EE_MOTOR_SPACE * i), motors[i].flags);
      OMEEPROM::write(EE_MORSPEED + (EE_MOTOR_SPACE * i), motors[i].speed);
    }
  }
  else
  {
    for(byte i = 0; i < MOTOR_COUNT; i++ ) 
    {
      motorDir(i, 0);
      motorSpeed(i, 0.01);
      OMEEPROM::write(EE_M0FLAG + (EE_MOTOR_SPACE * i), motors[i].flags);
      OMEEPROM::write(EE_MORSPEED + (EE_MOTOR_SPACE * i), motors[i].speed);
    }
  }
  
  OMEEPROM::write(EE_SMS, motion_sms);
    
  
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


void uiCursorChangeMotSpd(byte p_dir) {

 float   curSpd = motorSpeed(ui_curMotor);
 float   mod    = .00025;
 
 if(motion_sms)
 {
   mod = .00025;
   curSpd += p_dir ? mod : mod * -1.0;
   curSpd = curSpd > .1 ? .1 : curSpd < mod ? mod : curSpd;
 }
 else
 {
   curSpd += p_dir ? motorIncrement : motorIncrement * -1.0;
   curSpd = curSpd > 1.0 ? 1.0 : curSpd < motorIncrement ? motorIncrement : curSpd;
 }

 motorSpeed(ui_curMotor, curSpd);  
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

