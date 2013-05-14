
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
  Menu Action Functions
  ========================================
  
*/

/** Initialize Alt Inputs Action

   Must be done after changing input mode.
   
   @author
   C. A. Church
*/

void uiMenuAltInit() {
 
 altSetup();

 lcd.clear();

 lcd.print(STR_INIT);
 lcd.setCursor(0, 1);
 lcd.print(STR_DONE);
 
 delay(2000); 
}


/** Reset Menu Action

 Gives user a chance to cancel within 9 seconds.  
 
 Once memory is reset, user is informed that they can only power-cycle MX3.
 
 @author
 C. A. Church
 */
 
void uiMenuResetMem() {
 
 lcd.clear();

 lcd.print(STR_RES1);
 lcd.setCursor(0, 1);
 lcd.print(STR_RES2);
 
 Menu.enable(false);
 
 unsigned long cancelTm = millis();
 
 while( 1 ) {
   byte button = Menu.checkInput();
   
   if( button == BUTTON_SELECT ) {
     Menu.enable(true);
     return;
   }
   else {
     unsigned long diff = millis() - cancelTm;
     
     if( diff >= 9100 )
       break;
     
     unsigned int dispTime = (9100 - diff) / SECOND;
     lcd.setCursor(9, 0);
     lcd.print(dispTime, DEC);
   }
 }
 
 lcd.clear();
 OMEEPROM::saved(false);
 
 lcd.print(STR_RES3);
 lcd.setCursor(0, 1);
 lcd.print(STR_RES4);
 while(1) 
  ; // block all activity 
}

/** Voltage Value

 Pulls up data from the onboard Sensors 
 
 @author
 Parker Dillmann
 */
 
void uiMenuVoltage() {
 int  update_voltage = 0;
 
 lcd.clear();
 Menu.enable(false);
 
 lcd.print(STR_MVOLT);
 
 while(1)  {
   byte button = Menu.checkInput();
   
   if(button != BUTTON_NONE) {
     Menu.enable(true);
     return;
   }
   else if(update_voltage == 0) {
     lcd.setCursor(0,1);
     lcd.print(STR_BLNK);
     lcd.setCursor(0,1);
     lcd.print( sensorVoltage(), 2 );
     lcd.print(STR_V);
     update_voltage++;
   }
   else {
     update_voltage++;
     if(update_voltage > 1000)
       update_voltage = 0;
     
   } // end else
 } // end while
 
}

/** Current Value

 Pulls up data from the onboard Sensors 
 
 @author
 Parker Dillmann
 */
 
void uiMenuCurrent() {
 int  update_current = 0;
 
 lcd.clear();
 Menu.enable(false);
 
 lcd.print(STR_MCURR);
 
 while(1) {
   byte button = Menu.checkInput();
   
   if(button != BUTTON_NONE) {
     Menu.enable(true);
     return;
   }
   else if(update_current == 0) {
     lcd.setCursor(0,1);
     lcd.print(STR_BLNK);
     lcd.setCursor(0,1);
     lcd.print( sensorCurrent(), DEC );
     lcd.print(STR_MA);
     update_current++;
   }
   else {
     update_current++;
     if(update_current > 1000)
       update_current = 0;
   } // end else
 } // end while
 
}


void uiMenuTemp(byte p_sens) {
  
 int  update_temp = 0;
 
 lcd.clear();
 Menu.enable(false);
 
 lcd.print(STR_MTEMP);
 lcd.print(p_sens + 1, DEC);
 
 while(1)  {
   byte button = Menu.checkInput();
   
   if(button != BUTTON_NONE) {
     Menu.enable(true);
     return;
   }
   else if(update_temp == 0) {
     lcd.setCursor(0,1);
     lcd.print(STR_BLNK);
     lcd.setCursor(0,1);
     
     lcd.print(sensorTemp(0), 2);
     
     lcd.print(STR_ECAM);
     update_temp++;
   }
   else {
     update_temp++;
     if(update_temp > 1000)
       update_temp = 0;
   } // end else
 } // end while
 
}

/** Temp0 Value

 Pulls up data from the onboard Sensors 
 
 @author
 Parker Dillmann
 */
 
void uiMenuTemp0() {
  uiMenuTemp(0);
}

/** Temp1 Value

 Pulls up data from the onboard Sensors 
 
 @author
 Parker Dillmann
 */
 
void uiMenuTemp1() {
  uiMenuTemp(1);
}

/** Temp2 Value

 Pulls up data from the onboard Sensors 
 
 @author
 Parker Dillmann
 */
 
void uiMenuTemp2() {
  uiMenuTemp(2);
}


/** Manual Move Screen

 Perform a manual move of a motor
 
 @author
 C. A. Church
 */
 
void uiMenuManual(byte p_motor) {

 // byte   spdPrec = (motorMaxSpeedRatio(p_motor) / 100 < 0.1) ? 2 : 1;
  float        wasSpd = motorSpeedRatio(p_motor);
  boolean         run = false;
  byte       wasFlags = motors[p_motor].flags;
  //byte      wasButton = BUTTON_NONE;
  byte       wasMotor = ui_curMotor;
  float           spd = wasSpd;
  //unsigned long butTm = millis();
  
    // can't do manual move when running
  if( running ) {
    lcd.print(STR_WARN1);
    lcd.setCursor(0, 1);
    lcd.print(STR_WARN2);    
    delay(2000);
    return;
  }
  
    // disable menu processing so we can watch
    // user input
  Menu.enable(false);

  lcd.clear();
  lcd.print(STR_RES2);
 
   // user-enable motor (necessary if the user had it disabled)
  motors[p_motor].flags |= MOTOR_UEN_FLAG;
  
  // need to do this for the use of uiCursorChangeMotSpd below
  ui_curMotor = p_motor;
  
  while( 1 ) {
       
     
    byte       button = Menu.checkInput();
    
    lcd.setCursor(0, 1);
    lcd.print(spd, 2); 
    
    if( button == BUTTON_FORWARD || button == BUTTON_BACK ) {
        // if not already running, run it
      if( ! run ) {
        byte dir = (button == BUTTON_FORWARD) ? 1 : 0;
        motorDir(p_motor, dir);
        motorRun(false, p_motor);
        run = true;
      }
      else {
          // already running, stop the motor
        motorStop();
        run = false;
      }
    } // end if( button == BUTTON_FORWARD...
    else {
        // not forward or back
      
      if( button == BUTTON_INCREASE || button == BUTTON_DECREASE ) {
        byte dir = (button == BUTTON_INCREASE) ? 1 : 0;
          // use existing cursor code to change speed
        uiCursorChangeMotSpd(dir);
          // display new speed
        spd = motorSpeedRatio(p_motor);
        lcd.setCursor(0, 1);
        lcd.print(STR_BLNK);
      }
      else if( button == BUTTON_SELECT ) {
          // enter exits, recover state and exit action
        ui_curMotor = wasMotor;
        motorStop();
        motors[p_motor].flags = wasFlags;
        motorSpeedRatio(p_motor, wasSpd);
        Menu.enable(true);
        return;
      }
    }
    
  } // end while
      
}

void uiMenuManualOne() {
 uiMenuManual(0); 
}

 void uiMenuManualTwo() {
 uiMenuManual(1); 
}

void uiMenuManualThree() {
 uiMenuManual(2); 
}


void uiMenuPreset(byte p_motor) {
 
    // disable menu key processing
 Menu.enable(false);
 
 byte totalPresets = sizeof(motorPresets) / sizeof(MotorPreset);
 byte    curPreset = motorPresetSelected[p_motor];
 
 MotorPreset thsPreset = motorPresets[curPreset];
 

 lcd.clear();
 lcd.setCursor(0, 0);
 lcd.print(STR_PRESET);
 lcd.setCursor(0,1);
 lcd.print(thsPreset.name);
 
   // process user input to act like a normal menu option
 while(1) {
   byte button = Menu.checkInput();
   
   if( button == BUTTON_NONE ) 
     continue;
   else if( button == BUTTON_BACK ) {
       // user has canceled, change nothing
     Menu.enable(true);
     return;
   }
   else if( button == BUTTON_SELECT || button == BUTTON_FORWARD ) {
       // user has chosen to save values
     motorPresetSelected[p_motor] = curPreset;
     
       // get a pointer to the definition of this motor
     MotorDefinition* motDef = &motors[p_motor];
     
      // set new preset values
      
     if( ! thsPreset.rotary )
       motDef->flags &= (B1111111 ^ MOTOR_ROT_FLAG);
     else
       motDef->flags |= MOTOR_ROT_FLAG;
       
     motDef->rpm   = thsPreset.rpm;
     motDef->ratio = thsPreset.ratio;
     motorPresetSelected[p_motor] = curPreset;
     
       // save new values to eeprom
     OMEEPROM::write(EE_M0FLAG + EE_MOTOR_SPACE * p_motor, motDef->flags);
     OMEEPROM::write(EE_M0RPM + EE_MOTOR_SPACE * p_motor, motDef->rpm);
     OMEEPROM::write(EE_M0RATIO + EE_MOTOR_SPACE * p_motor, motDef->ratio);
     OMEEPROM::write(EE_MPRESET, *((byte*)motorPresetSelected), sizeof(motorPresetSelected) / sizeof(byte));
     
     Menu.enable(true);
     return;
   }
   else if( button == BUTTON_INCREASE ) 
     curPreset = (curPreset == totalPresets - 1) ? 0 : curPreset + 1;
   else if( button == BUTTON_DECREASE )
     curPreset = curPreset == 0 ? totalPresets - 1 : curPreset - 1;
     
     
   thsPreset = motorPresets[curPreset];
   
   lcd.setCursor(0,1);
   lcd.print(STR_BLNK);
   lcd.setCursor(0,1);
   lcd.print(thsPreset.name);
 }
    
}

void uiMenuPresetOne() {
  uiMenuPreset(0);
}

void uiMenuPresetTwo() {
  uiMenuPreset(1);
}

void uiMenuPresetThree() {
  uiMenuPreset(2);
}

