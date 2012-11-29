
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


/** Manual Move Screen

 Perform a manual move of a motor
 
 @author
 C. A. Church
 */
 
void uiMenuManual(byte p_motor) {

  byte   spdPrec = (motorMaxSpeedRatio(p_motor) / 100 < 0.1) ? 2 : 1;
  float      spd = motorSpeedRatio(p_motor);
  boolean    run = false;
  byte  wasFlags = motors[p_motor].flags;
  byte wasButton = BUTTON_NONE;
  byte  wasMotor = ui_curMotor;
  
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
  lcd.setCursor(0, 1);
  lcd.print(spd, spdPrec); 
 
   // user-enable motor
  motors[p_motor].flags |= MOTOR_UEN_FLAG;
  
  ui_curMotor = p_motor;
  
  while( 1 ) {
    byte button = Menu.checkInput();
    
    if( button == BUTTON_FORWARD || button == BUTTON_BACK ) {
        // change direction needed?
      if( button != wasButton ) {
        byte dir = button == BUTTON_FORWARD ? 1 : 0;
        motorDir(p_motor, dir);
      }
        // if not already running, run it
      if( ! run ) {
        motorRun(false, p_motor);
        run = true;
      }
    }
    else {
        // not forward or back, we only move when forward or
        // back are held!
      motorStop(false);
      run = false;
      
      if( button == BUTTON_INCREASE || button == BUTTON_DECREASE ) {
        byte dir = button == BUTTON_INCREASE ? 1 : 0;
          // use existing cursor code to change speed
        uiCursorChangeMotSpd(dir);
          // display new speed
        spd = motorSpeedRatio(p_motor);
        lcd.setCursor(0, 1);
        lcd.print(STR_BLNK);
        lcd.print(spd, spdPrec); 
      }
      else if( button == BUTTON_SELECT ) {
          // enter exits, recover state and exit action
        ui_curMotor = wasMotor;
        motors[p_motor].flags = wasFlags;
        Menu.enable(true);
        return;
      }
    }
    
    wasButton = button;
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


