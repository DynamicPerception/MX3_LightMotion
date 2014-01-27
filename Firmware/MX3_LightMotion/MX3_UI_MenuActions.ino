
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


void uiMenuAltInit() {
 lcd.noBlink();
 
 altSetup();

 lcd.clear();

 lcd.print(STR_INIT);
 lcd.setCursor(0, 1);
 lcd.print(STR_DONE);
 
 delay(2000); 
}
*/

/** Reset Menu Action

 Gives user a chance to cancel within 9 seconds.  
 
 Once memory is reset, user is informed that they can only power-cycle MX3.
 
 @author
 C. A. Church
 */
 
void uiMenuResetMem() {
 lcd.noBlink();
 
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
     //lcd.noBlink();
     unsigned int dispTime = (9100 - diff) / SECOND;
     lcd.setCursor(9, 0);
     lcd.print(dispTime, DEC);
     //lcd.setCursor(17,0);       //moves cursor off screen, prevents a blinking cursor from showing up at the end
   }
 }
 
 lcd.clear();
 OMEEPROM::saved(false);
 
 lcd.print(STR_RES3);
 lcd.setCursor(0, 1);
 lcd.print(STR_RES4);
 //lcd.setCursor(17,0);   //moves cursor off screen, prevents a blinking cursor from showing up at the end
 
 while(1) 
  ; // block all activity until user power-cycles
}

/** Save State Menu Action

 Save State for EEPROM
 
 @author
 Parker Dillmann
 */
 
void uiMenuSaveMem() {
 lcd.noBlink();
  
 int save_state = 0;
 int menu_check = 1;
 
 lcd.clear();

 lcd.print(STR_SAV1);
 lcd.setCursor(0, 1);
 lcd.print(STR_SAV2);
 
 lcd.print(save_state+1);
 
 Menu.enable(false);
 
 while( menu_check ) 
 {
   byte button = Menu.checkInput();
   
   if( button == BUTTON_BACK ) 
   {
     Menu.enable(true);
     return;
   }
   else if (button == BUTTON_INCREASE) 
   {
     save_state++;
     if(save_state == SS_COUNT)
     {
       save_state = 0;
     } 
     
     lcd.setCursor(6, 1);
     lcd.print(save_state+1);
   }
   else if (button == BUTTON_DECREASE) 
   {
     save_state--;
     if(save_state < 0)
     {
       save_state = SS_COUNT - 1;
     }  
     
     lcd.setCursor(6, 1);
     lcd.print(save_state+1);
   }
   else if (button == BUTTON_SELECT)
   {
     switch (save_state)
     {
       case 0:
       
         eepromWrite_SS0();
         menu_check = 0;
       
         break;
         
       case 1:
       
         eepromWrite_SS1();
         menu_check = 0;
       
         break;
         
       case 2:
       
         eepromWrite_SS2();
         menu_check = 0;
       
         break;
         
       default:
       
         lcd.clear();
         lcd.print("Something Broke!");
         lcd.setCursor(0,1);
         lcd.print("Contact Admin");
         while(1);
     } 
   }
   else
   {
     
   }
 }
 
 lcd.clear();
 
 lcd.print(STR_SAV3);
 lcd.setCursor(0, 1);
 lcd.print(STR_SAV4);
 
 while( 1 ) 
 {
   byte button1 = Menu.checkInput();
   
   if( button1 == BUTTON_SELECT ) 
   {
     Menu.enable(true);
     return;
   }
 }
 
}

/** Load State Menu Action

 Load State for EEPROM
 
 @author
 Parker Dillmann
 */
 
void uiMenuLoadMem() {
 lcd.noBlink();
  
 int save_state = 0;
 int menu_check = 1;
 
 lcd.clear();

 lcd.print(STR_LOD1);
 lcd.setCursor(0, 1);
 lcd.print(STR_LOD2);
 
 lcd.print(save_state+1);
 
 Menu.enable(false);
 
 while( menu_check ) 
 {
   byte button = Menu.checkInput();
   
   if( button == BUTTON_BACK ) 
   {
     Menu.enable(true);
     return;
   }
   else if (button == BUTTON_INCREASE) 
   {
     save_state++;
     if(save_state == SS_COUNT)
     {
       save_state = 0;
     } 
     
     lcd.setCursor(6, 1);
     lcd.print(save_state+1);
   }
   else if (button == BUTTON_DECREASE) 
   {
     save_state--;
     if(save_state < 0)
     {
       save_state = SS_COUNT - 1;
     }  
     
     lcd.setCursor(6, 1);
     lcd.print(save_state+1);
   }
   else if (button == BUTTON_SELECT)
   {
     switch (save_state)
     {
       case 0:
       
         eepromRestore_SS0();
         menu_check = 0;
       
         break;
         
       case 1:
       
         eepromRestore_SS1();
         menu_check = 0;
       
         break;
         
       case 2:
       
         eepromRestore_SS2();
         menu_check = 0;
       
         break;
         
       default:
       
         lcd.clear();
         lcd.print("Something Broke!");
         lcd.setCursor(0,1);
         lcd.print("Contact Admin");
         while(1);
     } 
   }
   else
   {
     
   }
 }
 
 lcd.clear();
 
 lcd.print(STR_LOD3);
 lcd.setCursor(0, 1);
 lcd.print(STR_LOD4);
 
 while( 1 ) 
 {
   byte button1 = Menu.checkInput();
   
   if( button1 == BUTTON_SELECT ) 
   {
     Menu.enable(true);
     return;
   }
 }
 
}


/** Change VFD Brightness Menu

 Changes the brightness level of the VFD screen
 
 @author
 Kevin Melotti
 */
 
void uiVFDBrightness() {
 lcd.noBlink();
  
 byte brightness_state = VFDBright;
 int menu_check = 1;
 
 lcd.clear();

 lcd.print(STR_BRI1);
 lcd.setCursor(0, 1);
 lcd.print(STR_BRI2);
 
 lcd.print((brightness_state+1)*25);
 
 Menu.enable(false);
 
 while( menu_check ) 
 {
   byte button = Menu.checkInput();
   
   if( button == BUTTON_BACK ) 
   {
     VFDBrightness(VFDBright);
     Menu.enable(true);
     return;
   }
   else if (button == BUTTON_INCREASE) 
   {
     brightness_state++;
     if(brightness_state == VFD_COUNT)
     {
       brightness_state = 0;
     } 
     
     lcd.setCursor(3, 1);
     lcd.print("   ");
     lcd.setCursor(3, 1);
     lcd.print((brightness_state+1)*25);
   }
   else if (button == BUTTON_DECREASE) 
   {
     brightness_state--;
     if(brightness_state > VFD_COUNT)
     {
       brightness_state = VFD_COUNT - 1;
     }  
  
     lcd.setCursor(3, 1);
     lcd.print("   ");
     lcd.setCursor(3, 1);
     lcd.print((brightness_state+1)*25);
   }
   else if (button == BUTTON_SELECT)
   {
     VFDBright = brightness_state;
     menu_check = 0;
   }
   VFDBrightness(brightness_state);
 }
 
 OMEEPROM::write(EE_VFDBRI, VFDBright);
 
 lcd.clear();
 
 lcd.print(STR_BRI3);
 lcd.setCursor(0, 1);
 lcd.print(STR_BRI4);
 
 while( 1 ) 
 {
   byte button1 = Menu.checkInput();
   
   if( button1 == BUTTON_SELECT ) 
   {
     Menu.enable(true);
     return;
   }
 }
 
}

/** Voltage Value

 Pulls up data from the onboard Sensors 
 
 @author
 Parker Dillmann
 */
 
void uiMenuVoltage() {
 lcd.noBlink();
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
 lcd.noBlink();
 
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

/** Temperature Value

 Pulls up data from the onboard Sensors 
 
 @author
 Parker Dillmann
 */

void uiMenuTemp(byte p_sens) {
 
 lcd.noBlink();
  
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

 Perform a manual move of a motor, gives the user limit speed resolution
 and requires them to hold the button down to move the motor. To go "right"
 the user must press the right button and to go "left" the user must hit
 the left button.
 
 @author
 Kevin Melotti
 */
 
void uiMenuManual(byte p_motor) {
  
  lcd.noBlink();
  float        wasSpd = motorSpeed(p_motor);
  boolean         run = false;
  byte       wasFlags = motors[p_motor].flags;
  byte       wasMotor = ui_curMotor;
  float           spd = 1.00;
  
  
    // can't do manual move when running
  if( running ) {
    lcd.home();
    lcd.print(STR_WARN1);
    lcd.setCursor(0, 1);
    lcd.print(STR_WARN2);    
    delay(2000);
    return;
  }
  
  //Check to see if the user wants to move the motor by holding the button down or not
  if (mmSetting == MM_HOLD){
    
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
         
  
      int buttonValue = analogRead(BUT_PIN); 
    
      byte button = Menu.checkInput();
      
      lcd.setCursor(0, 1);
      lcd.print(spd*100, 0); 
      lcd.print("% ");
      
      //Moves the motor based on which button is being pressed        
      if (button == BUTTON_FORWARD || button == BUTTON_BACK){
        
        byte dir = (button == BUTTON_BACK) ? 0 : 1;
        motorDir(p_motor, dir);
        motorSpeed(p_motor, spd);
        motorRun(false, p_motor);
        run = true;
        
        //Sets the lower and upper boundaries for the analog button depending on which button is pressed
        int lowerBound = ((button == BUTTON_BACK) ? BUTREV_VAL : BUTFWD_VAL) - BUT_THRESH;
        int upperBound = ((button == BUTTON_BACK) ? BUTREV_VAL : BUTFWD_VAL) + BUT_THRESH;
        
        //Keeps the motor running while the button is held down      
        while( lowerBound <= buttonValue && buttonValue <= upperBound) {    
          buttonValue = analogRead(BUT_PIN);
        } 
        
        //stops the motors after the button is released
        motorStop();
        run = false;
        
      }// end if( button == BUTTON_FORWARD...
      
      //Adjust the speed of the motor
      if( button == BUTTON_INCREASE || button == BUTTON_DECREASE ) {
        float dir = (button == BUTTON_INCREASE) ? 0.25 : -0.25;
        spd += dir;
        if (spd >= 1.00)
          spd = 1.00;
        else if (spd <= 0.25)
          spd = 0.25;
      }
      
      // enter exits, recover state and exit action
      if( button == BUTTON_SELECT ) {
        ui_curMotor = wasMotor;
        motorStop();
        motors[p_motor].flags = wasFlags;
        motorSpeed(p_motor, wasSpd);
        Menu.enable(true);
        return;      
      }    
      
    } // end while
  } else { 
  
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
      lcd.print(spd*100, 0); 
      lcd.print("% "); 
      
      if( button == BUTTON_FORWARD || button == BUTTON_BACK ) {
          // if not already running, run it
        if( ! run ) {
          byte dir = (button == BUTTON_FORWARD) ? 1 : 0;
          motorDir(p_motor, dir);
          motorSpeed(p_motor, spd);
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
          float dir = (button == BUTTON_INCREASE) ? 0.25 : -0.25;
          spd += dir;
          if (spd >= 1.00)
            spd = 1.00;
          else if (spd <= 0.25)
            spd = 0.25;
          motorSpeed(p_motor, spd);
        }
        else if( button == BUTTON_SELECT ) {
            // enter exits, recover state and exit action
          ui_curMotor = wasMotor;
          motorStop();
          motors[p_motor].flags = wasFlags;
          motorSpeed(p_motor, wasSpd);
          Menu.enable(true);
          return;
        }
      }
    }  
  }
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


/* Select Preset for Given Motor

 Displays a select list, and behaves like a normal menu select,
 to choose a specific preset for a motor and apply the changes.
 
 @param p_motor
 The motor # to set the preset for
 
 @author
 C. A. Church
 */
 
void uiMenuPreset(byte p_motor) {
 lcd.noBlink();
 
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
       motDef->flags &= ~(MOTOR_ROT_FLAG);
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

