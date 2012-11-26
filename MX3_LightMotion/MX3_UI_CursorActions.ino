

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
  if( running )
    stopProgram();
  else
    startProgram();
   
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




/*

 ==============
 Cam Screen Cursors
 ==============
 
 */


/** Interacting with shutter time on Cam Screen
*/

void uiCursorChangeShutterTime(byte p_dir) {
 
  unsigned long *ptr = &camera_wait;
  byte      scaleLen = (sizeof(ui_camDivs) / sizeof(float)) - 1;
  int            mod = 1;
  int            mem = EE_CAMWAIT;
  
 if( camera_bulb ) {
   ptr = &camera_exposure;
   mem = EE_CAMEXP;
 }

 float          div = (float) *ptr / 1000.0;

 if( p_dir != 1 )
   mod = -1;
   
 if( div >= ui_camDivs[scaleLen] ) {
     // we are greater than or equal to the largest div, generally we move in 1 second increments above
     // this range
    if( div >= (ui_camDivs[scaleLen] + 1.0) ) {
       div += mod;
    }
    else { 
       if( p_dir )
         div = ui_camDivs[scaleLen] + 1.0;
       else 
         div = ui_camDivs[scaleLen - 1];
    }
 }
 else {
     // we are below the largest div, we move around in divs here
   byte pos = 0;
   for( pos = 0; pos <= scaleLen; pos++ ) {
     if( ui_camDivs[pos] >= div ) {
       break;
     }
   }
   
   //div = ui_camDivs[pos + mod];
   
   if( pos == scaleLen && mod )
     div = ui_camDivs[scaleLen] + 1.0;
   else if( pos == 0 && ! mod) 
     ; // do nothing!
   else
     div = ui_camDivs[pos + mod];
 
 }
 
   // set the floor on bulb exposure times
 if( camera_bulb ) 
   div = div < CAM_MIN_BULB ? CAM_MIN_BULB : div;
   
 
   // set correct wait value
 *ptr = (unsigned long) (1000.0 * div);
 
 if( *ptr == 0 )
   *ptr = 1;
   
   // write new value to eeprom
 OMEEPROM::write(mem, *ptr);
 
   // if already running, re-configure camera
 if( running )
   camSetup();
  
   // overwrite exposure display area...
 lcd.print(STR_BLNK); 
 
}


 /** Interacting with Bulb setting on Camera Screen */
void uiCursorChangeCamBulb(byte p_dir) {
  
  camera_bulb = !camera_bulb;
  
  if( running )
    camSetup(); // reinitialize cam
    
  OMEEPROM::write(EE_CAMBULB, camera_bulb);
  
     // overwrite exposure display area...
  lcd.print(STR_BLNK); 
}


void uiCursorChangeFocusTime(byte p_dir) {
 
 if( p_dir ) 
  camera_focus += 500;
 else
  camera_focus -= 500;
 
 OMEEPROM::write(EE_CAMFOC, camera_focus);
  
 if( running )
    camSetup(); // reinitialize cam
    
     // overwrite exposure display area...
 lcd.print(STR_BLNK); 
  
}


/*

 ==============
 Motor Screen Cursors
 ==============
 
 */
 
 
void uiCursorChangeMotEn(byte p_dir) {
  
    // if currently running, and a ramp is set, ramp out
  if( running == true && motors[ui_curMotor].flags & MOTOR_UEN_FLAG && motors[ui_curMotor].ramp > 0) {
    motorForceRamp(ui_curMotor);
  }
  else {  
    if( motors[ui_curMotor].flags & MOTOR_UEN_FLAG )
      motors[ui_curMotor].flags &= (B11111111 ^ MOTOR_UEN_FLAG);
    else {
      
        // program already running when motor turned on?
      if( running ) {
         motors[ui_curMotor].startShots = camera_fired;
           // ramp down to zero before starting motor!
         motorSpeed(ui_curMotor, 0, true);
      }
      
      motors[ui_curMotor].flags |= MOTOR_UEN_FLAG;  
 
       
    }

    OMEEPROM::write(EE_M0FLAG + (EE_MOTOR_SPACE * ui_curMotor), motors[ui_curMotor].flags);
  }
  
}


void uiCursorChangeMotSpd(byte p_dir) {
 
  
 lcd.setCursor(0, 1);
 lcd.print(STR_BLNK);
 
 float maxSpeed = motorMaxSpeedRatio(ui_curMotor);
 
 float curSpd = motorSpeedRatio(ui_curMotor);
 
 float mod = 0.1;
 
 if( maxSpeed / 100.0 < 0.1 )
   mod = 0.01;
   
 if( p_dir ) {
   
   if( curSpd >= maxSpeed ) 
     curSpd = maxSpeed;
   else
     curSpd += mod;
 }
 else {
   if( curSpd <= mod )
      curSpd = mod;
   else     
     curSpd -= mod;
 }
 

 motorSpeedRatio(ui_curMotor, curSpd);
  
}

void uiCursorChangeMotDir(byte p_dir) {
 
 motorDirFlip(ui_curMotor);
 
}


void uiCursorChangeMotLead(byte p_dir) {
  
  if( p_dir )
    motors[ui_curMotor].lead += 1;
  else
    motors[ui_curMotor].lead -= 1;
}

void uiCursorChangeMotRamp(byte p_dir) {
  
  if( p_dir )
    motors[ui_curMotor].ramp += 1;
  else
    motors[ui_curMotor].ramp -= 1;
}

