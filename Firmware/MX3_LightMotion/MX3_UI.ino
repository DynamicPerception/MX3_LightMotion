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
  User interface functions
  ========================================
  
*/


 // Load UI Menus and other useful things

#include "MX3_UI.h"

 // global cursor display data
uiDisplayCursor ui_cursor = { 0, 0, 0 };
boolean        ui_refresh = false;
byte          ui_curMotor = 0;


// time variables
  unsigned int hours = 0;
  unsigned int mins = 0;
  unsigned int secs = 0;
  unsigned long temp_time = 0;

 // initialize menu manager
OMMenuMgr Menu(&ui_it_root);



/** UI Setup

  Set up the LCD, Menu Manager, and display welcome screen
  
  @author
  C. A. Church
  */

void uiMenuSetup() {

  lcd.begin(LCD_COLS, LCD_ROWS);
  
 
  
  
  pinMode(LCD_BKL, OUTPUT);
  uiLCDBackLight(true);
  
  Menu.setDrawHandler(uiDraw);
  Menu.setExitHandler(uiClear);
  Menu.setAnalogButtonPin(BUT_PIN, BUT_MAP, BUT_THRESH);
  Menu.enable(true); 
  VFDBrightness(VFDBright);
  
  lcd.print(MX3_VERSTR);
  lcd.setCursor(0, 1);
  lcd.print(MX3_SUBSTR);
  
  delay(3000);

  lcd.clear();
  lcd.print(MX3_C1STR);
  lcd.setCursor(0,1);
  lcd.print(MX3_C2STR);

  delay(2000); 
  uiClear();
  
}



 /** Check UI Status
 
   Poll Menu, Check for Input
   
   @author
   C. A. Church
   */
  
void uiCheck() {
   
 static unsigned long lastButTm = 0;
 static boolean           lcdOn = true;
 static byte            wasSens = sensor_statFlags;
 static unsigned long lastSenTm = 0;
 static boolean       clearSens = false;

 
 byte                    button = Menu.checkInput();

 
  // check for disabling the LCD backlight
  
 lcd.noCursor();
  
 if( button == BUTTON_NONE ) {
   if((lcdDisable > 0 && millis() - lastButTm > (lcdDisable * SECOND) ) ) {
     uiLCDBackLight(false);
     lcdOn = false;
     if( sensor_enHeater )
     {
        uiClear(); 
        lcd.noDisplay();
        return;
     }
   }
 }
 else {
   lcd.display();
   lastButTm = millis();
   if( lcdOn == false ) {
     uiLCDBackLight(true);
     lcdOn = true;
   }
 }
 
   // show base screen, if desired
   
 if( ! Menu.shown() ) {
   
    // clear any ignore sensor warnings flag, if enough time has passed or
    // stat flags change
    
   if( sensor_statFlags != wasSens || ( clearSens == true && millis() - lastSenTm > SENS_RST_TIME ) ) {
     clearSens = false;
     wasSens   = sensor_statFlags;
     lcdOn     = true;
     Menu.enable(true);
     uiLCDBackLight(true);     
   }
   
     // don't show base screen if there are any sensor errors
   if( sensor_statFlags && clearSens == false ) {
     
     uiSensorErrorScreen();
     
     Menu.enable(false);
     
       // is user has pressed select, make the error go away for SENS_RST_TM
     if( button == BUTTON_SELECT ) {
       clearSens = true;
       lastSenTm = millis();
       lcdOn     = true;
       Menu.enable(true);
       uiLCDBackLight(true);
       return;
     }
       
   } // end if( sensor_stagFlags...
   else {
     uiBaseScreen(button); 
   }
   
 }
     
}



 /** Draw Handler Callback
 
   Draw handler callback for OMMenuMgr
   
   @param p_text
   Char pointer with text to display
   
   @param p_row 
   Row to display at
   
   @param p_col 
   Column to start display at
   
   @param len
   Length of p_text pointer
   
   @author
   C. A. Church
   */
   
void uiDraw(char* p_text, int p_row, int p_col, int len) {
  lcd.setCursor(p_col, p_row);
  
  for( int i = 0; i < len; i++ ) {
    if( p_text[i] < '!' || p_text[i] > '~' )
      lcd.write(' ');
    else  
      lcd.write(p_text[i]);
  }
}

 /** Exit Callback Handler
 
  Exit callback handler for OMMenuMgr
  
  @author
  C. A. Church
  */
  
void uiClear() {
  
  lcd.clear();
  lcd.setCursor(0, 0);
}

 /** Set LCD Backlight
 
  Enables or disables the LCD Backlight
  
  @param p_en
  Enable (true) or disable (false) the backlight
  
  @author
  C. A. Church
  */

void uiLCDBackLight(boolean p_en) {
 digitalWrite(LCD_BKL, p_en); 
}

/** Base Interface Display

 Displays the basic interface
 
 @author
 C. A. Church
 */
 
void uiBaseScreen(byte p_button) {

  static byte             screen = UI_SCREEN_MAIN;
  static byte         lastScreen = screen;
  static unsigned long lastUpdTm = 0;
 
  // only let up and down change screens when the cursor isn't on an item
  if( ! ui_cursor.enabled ) {
    if( p_button == BUTTON_INCREASE ) {
		
		lcd.clear();
		
		// If the user tries to increment to a screen higher than allowed, wrap around to the lowest numbered screen
		// Manual mode
		if (!ez_mode && screen == UI_SCREEN_MOTOR3)
			screen = UI_SCREEN_MAIN;
		// EZ mode, extended on
		else if (ez_mode && ez_extended && screen == UI_SCREEN_EZMODE){
			screen = UI_SCREEN_MAIN;
		}
		// EZ mode, extended off
		else if (ez_mode && !ez_extended){
			if (screen == UI_SCREEN_CAMERA)
				screen = UI_SCREEN_EZMODE;
			else if (screen > UI_SCREEN_CAMERA)
				screen = UI_SCREEN_MAIN;
			else
				screen++;
		}
		else
			screen++;
    }
	
    else if( p_button == BUTTON_DECREASE ) {
      
		lcd.clear();

		// If the user tries to decrement to a negative numbered screen, wrap around to the highest numbered screen	
		// Manual mode
		if (!ez_mode && screen == UI_SCREEN_MAIN)
			screen = UI_SCREEN_MOTOR3;
		// EZ mode, extended on
		else if (ez_mode && ez_extended && screen == UI_SCREEN_MAIN){
			screen = UI_SCREEN_MOTOR3;
		}
		// EZ mode, extended off
		else if (ez_mode && !ez_extended) {
			if (ez_mode && screen == UI_SCREEN_MAIN)
				screen = UI_SCREEN_EZMODE;
			else if (ez_mode && screen == UI_SCREEN_EZMODE)
				screen = UI_SCREEN_CAMERA;
			else
				screen--;
		}
		else
			screen--;
    }	
  }
    
  // handle screen navigation
  uiScreenInput(screen, p_button);
    
  // display, or update the screen?
  if( ui_refresh || screen != lastScreen || millis() - lastUpdTm > UI_REFRESH_TM ) {
    
   lcd.noBlink();    //turns the blinking cursor off while refreshing the screen
   lcd.home();

   ui_refresh = false;
   lastScreen = screen;
   lastUpdTm = millis();

   // first page of display
   if (screen == UI_SCREEN_MAIN)
	   screen = uiMainScreen();
   else if (screen == UI_SCREEN_CAMERA)
	   uiCamScreen();
   else if (screen == UI_SCREEN_EZMODE)
	   uiEZModeScreen();
   else {
	   uiMotorScreen(screen);
   }
      
  }
 
   // if cursor input is enabled, re-seek cursor to correct position, enables blinking
  if( ui_cursor.enabled ){
    lcd.setCursor(ui_cursor.col, ui_cursor.row);
    lcd.blink();
  }
}

/** Interval Calculation and Display Used in Main Display and EZ Mode **/

void mainFirstLine(){

	// Determine the minimum camera interval
	float minInt = calcMinInterval();

	// display correct running string
	if (running) {
		if (alt_ext_int)
			lcd.print(STR_EXT);
		else
			lcd.print(STR_RUN);
	}
	else if (camera_flag){
		lcd.print(STR_TIM);
	}
	else if (paused){
		lcd.print(STR_PAUSE);
	}
	else {
		lcd.print(STR_STOP);
	}

	lcd.print(STR_SPACE);

	if (minInt > camera_delay)
	{
		lcd.print(minInt, 1);
		camera_delay = minInt;
	}
	else
		lcd.print(camera_delay, 1);


	lcd.print(STR_SEC);

	lcd.setCursor(10, 0);

	if (motion_sms)
		lcd.print(STR_SMS);
	else
		lcd.print(STR_CONT);
}

/** Main Display Screen */

byte uiMainScreen() {

	// Calculate the minimum interval between shots, print 
	// the on/off option, and print the cont/SMS option 
	mainFirstLine();

	static byte cursor_position;

	// Print whether EZ mode or manual mode is active
	lcd.setCursor(13, 1);
	if (ez_mode)
		lcd.print("EZ ");
	else
		lcd.print("Man");
  
	// Print the elapsed time on the second line of the screen
	lcd.setCursor(0, 1);
  
	if (camera_flag)
	{
	temp_time = camera_timer * 60 - (delay_time - start_time)/1000;
	hours = ( temp_time ) / 3600;
	mins  = ( temp_time - (hours * 3600) ) / 60;
	secs  = ( temp_time - (hours * 3600) - (mins * 60) ) ;
    
	} else {
	hours = run_time / 3600000L;
	mins  = ( run_time - (hours * 3600000) ) / 60000L;
	secs  = ( run_time - (hours * 3600000) - (mins * 60000L) ) / SECOND;
	}
	lcd.print(STR_TIME);
	uiPad(2, hours);
	lcd.print(STR_SQUOTE);
	uiPad(2, mins);
	lcd.print(STR_QUOTE);
	uiPad(2, secs);

	lcd.setCursor(17,0);   //moves cursor off screen, prevents a blinking cursor from showing up at the end

	return UI_SCREEN_MAIN;
  
}

/** EZ Mode Display Screen **/
byte uiEZModeScreen() {
	
	static float ez_adjust_old[3] = { 1.0, 1.0, 1.0};

	// Calculate the minimum interval between shots, print 
	// the on/off option, and print the cont/SMS option 
	mainFirstLine();

	lcd.setCursor(14, 0);
	lcd.print("EZ");

	//USBSerial.println("Refreshing...................");

	// Print the direction and adjust options for the three separate axes
	for (byte i = 0; i < MOTOR_COUNT; i++) {

		/** If the EZ adjust value has changed, computed the new speed / distance setting **/
		if (abs(motors[i].ez_adjust - ez_adjust_old[i]) > 0.05) {
			// Update the motor speed / SMS distance settings
			EZmodeUpdate(i);
		}

		// Check to see if the motor is in rotary mode
		boolean rotary = (motors[i].flags & MOTOR_ROT_FLAG);

		lcd.setCursor((i * 6), 1);
		if (motors[i].desiredDirection == 0) {
			if (rotary)
				lcd.print("-");
			else
				lcd.print("L");
		}
		else {
			if (rotary)
				lcd.print("+");
			else
				lcd.print("R");
		}

		lcd.setCursor((i * 6) + 1, 1);
		lcd.print("  ");
		lcd.setCursor((i * 6) + 1, 1);

		// If the EZ adjust range is too high, indicate it by showing asterisks
		if (motors[i].ez_adjust > EZADJUST_MAX)
			lcd.print("*.*");
		// Otherwise, just show the value
		else
			lcd.print(motors[i].ez_adjust, 1);

		// Save the new EZ adjust and camera delay values for future comparison
		ez_adjust_old[i] = motors[i].ez_adjust;

		// Serial output for debugging
		//USBSerial.print("The EZadjust value is: ");
		//USBSerial.println(motors[i].ez_adjust);
		//USBSerial.print("The set speed is: ");
		//USBSerial.print(motors[i].target_speed);
		//if (rotary && motion_sms)
		//	USBSerial.println("deg/move");
		//else if (rotary && !motion_sms)
		//	USBSerial.println("deg/min");
		//else if (!rotary && motion_sms)
		//	USBSerial.println("inch/move");
		//else if (!rotary && !motion_sms)
		//	USBSerial.println("inch/min");
		//USBSerial.print("The set speed is: ");
		//USBSerial.print(motorSpeed(i)*100);
		//USBSerial.println("%");
		//USBSerial.println("");

	}
	
	lcd.setCursor(17, 0);   //moves cursor off screen, prevents a blinking cursor from showing up at the end

	return UI_SCREEN_EZMODE;
		
}

  
 /** Display Screen for Camera */
 
void uiCamScreen() {
    
  lcd.print(STR_CAM);

  if( Camera.busy() )
    lcd.print(STR_BUSY);
  else
    lcd.print(STR_IDLE);
    
  lcd.setCursor(OM_MENU_COLS - 4, 0);
  uiPad(4, camera_fired);
   
  lcd.setCursor(0, 1);

  unsigned long s_time;
  
  if( camera_bulb ) {
    lcd.print(STR_BULB);
    s_time = camera_exposure;
  }
  else {
    lcd.print(STR_ECAM);
    s_time = camera_wait;
  }
 
 lcd.print("      ");    //Clears old time
 lcd.setCursor(2, 1); 
    
 uiDisplayCamTime(s_time);
    
 lcd.setCursor(9, 1);
 
 lcd.print("FL");
 
 lcd.print("      ");    //Clears old focal length
 if (camera_focal_length >= 100)
	 lcd.setCursor(11, 1);
 else
	 lcd.setCursor(12, 1);
 
 lcd.print(camera_focal_length);
 lcd.print("mm");
 
 lcd.setCursor(17,0);   //moves cursor off screen, prevents a blinking cursor from showing up at the end
 

  
}

/** Display Screen for Motors */

void uiMotorScreen(byte p_motor) {

	// Print "Axis" on LCD
	lcd.print(STR_MOTOR);

	// Motor screen 1 is screen == 2, motor screen 2 is screen == 3, etc.
	// so subtract 1 before displaying axis number
	p_motor -= 1;

	// Print which motor screen is being displayed
	lcd.print(p_motor, DEC);

	// Subtract 1 again so we're working with motors 0, 1, 2 
	// instead of motors 1, 2, 3
	p_motor -= 1;

	// Create a pointer to the address of the motors struct
	MotorDefinition* def = &motors[p_motor];
	// Which motor are we using?
	ui_curMotor = p_motor;

	// Print the current move setting, "LEAD", "RMP", "On", or "Off"  
	if ((def->lead > 0) && (def->flags & MOTOR_UEN_FLAG) && (camera_fired <= (def->lead + def->startShots) || camera_fired > (camera_max_shots - (def->lead + def->startShots))) && running)
		lcd.print(STR_LEAD);
	else if (def->flags & MOTOR_RAMP_FLAG || motors[p_motor].inRamp)
		lcd.print(STR_RAMP);
	else if (def->flags & MOTOR_UEN_FLAG)
		lcd.print(STR_EN);
	else
		lcd.print(STR_DIS);

	// Print the lead-in / -out values
	lcd.setCursor(12, 0);
	lcd.print(STR_RA);
	uiPad(3, def->lead);

	// Move down to the second line or the display
	lcd.setCursor(0, 1);

	// Rotary move
	if (def->flags & MOTOR_ROT_FLAG) {

		// Check current direction and print result
		if (def->flags & MOTOR_CDIR_FLAG)
			lcd.print(STR_CW);
		else
			lcd.print(STR_CCW);
	}
	// Linear move
	else {
		// Check current direction and print result
		if (def->flags & MOTOR_CDIR_FLAG)
			lcd.print(STR_RIGHT);
		else
			lcd.print(STR_LEFT);
	}

	float		spd;					// The speed or SMS distance to display on the LCD
	byte		spdPrec;				// Number of decimal places of precision to use in the SMS distance / continuous speed readout
	static byte old_units = STANDARD;	// Variable to keep track of the units we were using the last time the screen refreshed

	// If we're returning from being in percent mode, update the target speed/distance with that dicated by the last percent/time setting
	// Do this for all the motors, just not the current one
	if (units != old_units && old_units == PERCENT) {
		for (byte i = 0; i < MOTOR_COUNT; i++) {
			if (!motion_sms)
				motors[i].target_speed = motorSpeedCalc(i);
			else
				motors[i].target_sms_distance = motorSpeedCalc(i);
		}
	}

	// Set the speed value to be displayed based on SMS or continuous targets
	if (motion_sms && (units == STANDARD || units == METRIC) && !ez_mode)
		spd = motors[p_motor].target_sms_distance;
	else if (!motion_sms && (units == STANDARD || units == METRIC) && !ez_mode)
		spd = motors[p_motor].target_speed;
	else if (!motion_sms && units == PERCENT && !ez_mode)
		// Multiply by 100 to convert from 0.0-1.0 range to 0-100 range
		spd = motorSpeed(p_motor) * 100.0;
	else if (motion_sms && units == PERCENT && !ez_mode)
		// Multiply by 60 to give move time in seconds
		spd = motorSpeed(p_motor) * SEC_PER_MIN;
	else if (ez_mode)
		spd = motors[p_motor].ez_adjust;

	// If units are metric and we're not in rotary or EZ modes, multiply standard speed by 2.54 to give cm
	if (units == METRIC && !(motors[p_motor].flags & MOTOR_ROT_FLAG) && !ez_mode)
		spd *= CM_CONSTANT;

	// Clear old speed
	lcd.setCursor(1, 1);
	lcd.print("        ");

	// Position the cursor, pad with a 0 if necessary, and set spdPrec
		// EZ Mode
		if (ez_mode) {
			lcd.setCursor(3, 1);
			spdPrec = 1;
		}
		// Manual Mode
		else {
			lcd.setCursor(2, 1);
			// Compare to 9.9999 instead of 10 because of floating point inaccuracy
			if (spd < 9.9999) {
				lcd.print("0");
			}
			else if ((int)spd >= 100) {
				lcd.setCursor(1, 1);
			}
			spdPrec = 2;
		}

	// If in EZ mode and the adjust value has been calculated to be higher than the allowable maximum, indicate with asterisks
	if (ez_mode && motors[p_motor].ez_adjust > EZADJUST_MAX)
		lcd.print("*.*");
	// Print the speed in inches, cm, %, seconds, or EZ units
	else
		lcd.print(spd, spdPrec);

	// Print the appropriate unit (but don't print units when in EZ mode)
	if (!ez_mode && (units == STANDARD || units == METRIC)) {
		if (motors[p_motor].flags & MOTOR_ROT_FLAG)
			lcd.print("deg");
		else if (!(motors[p_motor].flags & MOTOR_ROT_FLAG)){
			if (units == STANDARD)
				lcd.print("in");
			else if (units == METRIC)
				lcd.print("cm");
		}
	}
	else if (!ez_mode && units == PERCENT) {
		if (!motion_sms)
			lcd.print("%");
		else if (motion_sms)
			lcd.print("sec");
	}
	else if (ez_mode)
		lcd.print("ez");
  
	// Print the number of frames used for ramping in and out
	lcd.setCursor(12, 1);
	lcd.print(STR_UA);
  
	if( def->ramp_start == def->ramp_end )
	uiPad(3, def->ramp_start);
	else
	lcd.print(STR_AST);

	old_units = units;
      
}


 /** Screen Input Handler
 
  Handles cursor-based input on screens, using dynamic definition of cursors
  via the uiDisplay* structs.
  
  @param p_screen
  The current screen we're on
  
  @param p_button
  The button which was pressed
  
  @author
  C. A. Church
  */
  
void uiScreenInput(byte p_screen, byte p_button) {

  static byte wasScreen = UI_SCREEN_MAIN;

    byte action = 0;
	static byte uiPos = 0;
  

    // reset cursor if screen has changed
  if( p_screen != wasScreen ) {
        uiPos = 0;
        wasScreen = p_screen;
        ui_cursor.enabled = 0;
          // in case a new menu root was set as a special menu
          // for a cursor item
          //check what screen it's on to determine the root menu from that screen
        if (p_screen == UI_SCREEN_MOTOR1){
          Menu.setRoot(&ui_it_m0List);
        }
        else if (p_screen == UI_SCREEN_MOTOR2){
          Menu.setRoot(&ui_it_m1List);
        }
        else if (p_screen == UI_SCREEN_MOTOR3) {
          Menu.setRoot(&ui_it_m2List);
        }
        else {
          Menu.setRoot(&ui_it_root);
        }
  }


   // handle activity based on button pressed
   if( p_button == BUTTON_FORWARD ) {
      uiPos++;
   }
   else if( p_button == BUTTON_BACK ) {
      uiPos--;
   }
   else if( p_button == BUTTON_INCREASE ) {
      action = 2;
   }
   else if( p_button == BUTTON_DECREASE ) {
      action = 1;
   }

   // If EZ mode is active, only let the user change the 0.1 place of the EZ adjust value
   if (ez_mode && (p_screen == UI_SCREEN_MOTOR1 || p_screen == UI_SCREEN_MOTOR2 || p_screen == UI_SCREEN_MOTOR3)){
	   if (uiPos == 3 && p_button == BUTTON_FORWARD)
		   uiPos += 2;
	   else if (uiPos == 6 && p_button == BUTTON_FORWARD)
		   uiPos++;
	   else if (uiPos == 6 && p_button == BUTTON_BACK)
		   uiPos--;
	   else if (uiPos == 4 && p_button == BUTTON_BACK)
		   uiPos -= 2;
   }
  
    // get cursor
  const uiDisplayCursors* thsCursor = ui_dc_list[p_screen];
  
    // don't try to access a cursor further out than known for screen
  if( uiPos > thsCursor->count )
    uiPos = 0;
    
    // no cursor selected, clear cursor and bail
  if( uiPos == 0 ) {
    ui_cursor.enabled = 0;
    lcd.noBlink();
    if (p_screen == UI_SCREEN_MOTOR1){
      Menu.setRoot(&ui_it_m0List);
    }
    else if (p_screen == UI_SCREEN_MOTOR2){
      Menu.setRoot(&ui_it_m1List);
    }
    else if (p_screen == UI_SCREEN_MOTOR3) {
      Menu.setRoot(&ui_it_m2List);
    }
    else {
      Menu.setRoot(&ui_it_root);
    }
    return;
  }


    // set cursor enabled, blink, and note position of cursor
  ui_cursor.enabled = 1;
  //lcd.blink();
   
  uiDisplayCursorTarget** targets = reinterpret_cast<uiDisplayCursorTarget**>(thsCursor->targets);
  
  uiDisplayCursorTarget* target = targets[uiPos - 1];
  ui_cursor.row = target->row;
  ui_cursor.col = target->col;
  
 
    // set new root menu for enter being pressed...
  if( target->mnu != 0 )
    Menu.setRoot(target->mnu);
  else {
    if (p_screen == UI_SCREEN_MOTOR1){
      Menu.setRoot(&ui_it_m0List);
    }
    else if (p_screen == UI_SCREEN_MOTOR2){
      Menu.setRoot(&ui_it_m1List);
    }
    else if (p_screen == UI_SCREEN_MOTOR3) {
      Menu.setRoot(&ui_it_m2List);
    }
    else {
      Menu.setRoot(&ui_it_root);
    }
  }
  
  
  if( action ) {
    uiTargetFunc func = target->func;
    func(action - 1);
    ui_refresh = true;
  }
      
}



/** Display a common notation camera time

 Shows time on screen using common notation, e.g. 1/1000, 1.5", etc.
 
 @author
 C. A. Church
 */

void uiDisplayCamTime(unsigned long p_time) {
 
  if( p_time == 0 ){
    lcd.print(0, DEC);
    lcd.print("     ");
  }else if( p_time >= 300 ) {
    unsigned int s = p_time / SECOND;
    unsigned int p = (p_time - (s * SECOND)) / 100;
    
    lcd.print(s, DEC);
    
    if( p > 0 ) {
      lcd.print(STR_DOT);
      lcd.print(p, DEC);
    }
    
    lcd.print(STR_QUOTE);
  }
  else {
    unsigned int s = SECOND / p_time;
    lcd.print(STR_DIV);
    lcd.print(s, DEC);
  }
  
}


/** Pad Displayed Numnbers

 Displays a number on-screen, with specified length of
 zero padding.
 
 @param p_count
 Total digits to display
 
 @param p_val
 Value to display
 
 @author
 C. A. Church
 */
 
void uiPad(byte p_count, unsigned long p_val) {
    for(byte i = p_count; i > 1; i--) {
        // this is less flash intensive than pow()
      byte x = 1;
      unsigned long z = 1;
      while( x < i) { 
        z *= 10;
        x++;
      }
      
      if( p_val <  z)
        lcd.print('0');
      else
        break;
    }
    
    lcd.print(p_val, DEC);
  }


/** Display Sensor Error Warnings

@author
C. A. Church
*/

void uiSensorErrorScreen() {
  
  boolean           refresh = false;
  static boolean   ledState = true;
  static byte      wasSense = 0;
  static unsigned long uiTm = millis();
  static unsigned long blTm = millis();
  
  
  if( wasSense != sensor_statFlags || millis() - uiTm > UI_ERR_REFRESH_TM ) {
    wasSense = sensor_statFlags;
    refresh  = true;
    uiTm     = millis();
  }
  
  
  if( refresh ) {
    lcd.clear();
    lcd.setCursor(0,0);
    
    if( wasSense & SENS_TEMP_FLAG ) {
      lcd.print(STR_MTEMP);
      lcd.setCursor(0,1);
      lcd.print( sensorTempMax(), 2 );
    }
    else if( wasSense & SENS_CURRENT_FLAG ) {
      lcd.print(STR_MCURR);
      lcd.setCursor(0,1);
      lcd.print( sensorCurrent() );
      lcd.print(STR_MA);
    }
    else if( wasSense & SENS_VOLT_FLAG ) {
      lcd.print(STR_MVOLT);
      lcd.setCursor(0,1);
      lcd.print( sensorVoltage(), 2 );
      lcd.print(STR_V);
    }
   
   } // end if refresh 
    
      // flash LCD backlight
   if( millis() - blTm > UI_ERR_BLINK_TM ) {
     ledState = ! ledState;
     uiLCDBackLight(ledState);
     blTm = millis();
   }
     
  
}

// Constants for uiEstimate() argument
const byte MOD_DIST = 0;
const byte MOD_SHOTS = 1;

// Helper functions for calling uiEstimate from menus
void uiDistEstimate0(){
	uiEstimate(0, MOD_DIST);
}

void uiDistEstimate1(){
	uiEstimate(1, MOD_DIST);
}

void uiDistEstimate2(){
	uiEstimate(2, MOD_DIST);
}

void uiShotsEstimate0(){
	uiEstimate(0, MOD_SHOTS);
}

void uiShotsEstimate1(){
	uiEstimate(1, MOD_SHOTS);
}

void uiShotsEstimate2(){
	uiEstimate(2, MOD_SHOTS);
}

/** Time / Shots Estimate

This ui screen displays the estimated distance per hour and the
estimated shots per hour. The estimated distance or shots may be
modified in order to calculate a new speed for the motor or camera interval.

@param p_motor
The motor to get the speed for, 0, 1, or 2

@p_which
Can be either MOD_DIST or MOD_SHOTS. MOD_DIST will allow the user to change
the estimate distance and MOD_SHOTS will allow the user to change the estimated shots

@author
M. Ploof
*/


void uiEstimate(byte p_motor, byte p_which) {

	Menu.enable(false);
	lcd.noBlink();

	long int dist_est;	// Estimate of the distance/hr based on currnt speed / SMS settings --> Can by modified by user to alter speed setting
	int shots_est;	// Estimate of shots/hr
	byte button;

	shots_est = (SEC_PER_MIN / camera_delay) * MIN_PER_HR;

	if (!motion_sms)
		dist_est = motors[p_motor].target_speed * MIN_PER_HR; // Target speed is in inches, cm, or deg/min, so multiply by 60 to get the total per hour
	else
		dist_est = motors[p_motor].target_sms_distance * shots_est;

	lcd.clear();
	lcd.home();
	lcd.print("Dist/hr:");
	// Handle it if the number is too larget for the display
	if (dist_est > 9999)
		lcd.print(">9999");
	else{
		lcd.print(" ");
		lcd.print(dist_est);
	}

	if (!(motors[p_motor].flags & MOTOR_ROT_FLAG)) {
		if (units == STANDARD || units == PERCENT)
			lcd.print("in");
		else if (units == METRIC)
			lcd.print("cm");
	}
	else
		lcd.print("deg");

	lcd.setCursor(0, 1);
	lcd.print("Shots/hr: ");
	lcd.print(shots_est);

	while (1) {

		button = Menu.checkInput();

		float minInt = calcMinInterval();	// Minimum interval 
		float interval;						// Inverval calculated from modified shots/hr estimate

		// Modifying the distance per hour
		if (p_which == MOD_DIST) {
			// Don't allow blind modification of the estimate value when it's too large to display
			if (dist_est <= 9999 && (button == BUTTON_INCREASE || button == BUTTON_DECREASE)) {
				if (button == BUTTON_INCREASE && (dist_est < 9999))
					dist_est++;
				else if (button == BUTTON_DECREASE && (dist_est > 1))
					dist_est--;
				// Clear the old distance estimate, then print the new one
				lcd.setCursor(8, 0);
				lcd.print("        ");
				lcd.setCursor(8, 0);
				// Handle it if the number is too larget for the display
				if (dist_est > 9999)
					lcd.print(">9999");
				else{
					lcd.print(" ");
					lcd.print(dist_est);
				}

				// Print the appropriate unit again
				if (!(motors[p_motor].flags & MOTOR_ROT_FLAG)) {
					if (units == STANDARD || units == PERCENT)
						lcd.print("in");
					else if (units == METRIC)
						lcd.print("cm");
				}
				else
					lcd.print("deg");
			}
			// If the user presses the select button, convert the new distance estimate back into the targets speed / SMS move distance and the new EZ adjust value
			else if (button == BUTTON_SELECT) {
				if (!motion_sms){
					motors[p_motor].target_speed = (float)dist_est / MIN_PER_HR;
					motors[p_motor].ez_adjust = motors[p_motor].target_speed / motors[p_motor].ez_center_val;
					
					// Save new continuous speed
					OMEEPROM::write((EE_DES_SPEED0 + EE_MOTOR_SPACE_V1_1 * p_motor), motors[p_motor].target_speed);
				}
				else {
					motors[p_motor].target_sms_distance = (float)dist_est / shots_est;
					motors[p_motor].ez_adjust = motors[p_motor].target_sms_distance / motors[p_motor].ez_center_val;
					
					// Save new SMS distance
					OMEEPROM::write((EE_DES_SMSDIST0 + EE_MOTOR_SPACE_V1_1 * p_motor), motors[p_motor].target_sms_distance);
				}

				// Save new EZ adjust value
				OMEEPROM::write((EE_EZADJ0 + EE_MOTOR_SPACE_V1_1 * p_motor), motors[p_motor].ez_adjust);

				// Break out of the while loop
				break;
			}
		}

		// Modifying the shots per hour
		else if (p_which == MOD_SHOTS) {

			if (button == BUTTON_INCREASE || button == BUTTON_DECREASE) {
				if (button == BUTTON_INCREASE)
					shots_est++;
				else if (button == BUTTON_DECREASE && (shots_est > 1))
					shots_est--;

				interval = (float) SEC_PER_HR / shots_est;

				// If the new calculated interval is less than allowed, set it to the minimum and bounce the shots_est down to the previous value
				if (interval < minInt) {
					interval = minInt;
					shots_est--;
				}

				// Clear the old distance estimate, then print the new one
				lcd.setCursor(9, 1);
				lcd.print("       ");
				lcd.setCursor(10, 1);
				lcd.print(shots_est);
			}

			// If the user presses the select button, convert the new shots estimate back into the new interval
			else if (button == BUTTON_SELECT) {

				// Save the calculated interval
				camera_delay = interval;
				OMEEPROM::write(EE_CAMDEL, camera_delay);

				// Break out of the while loop
				break;
			}
		}

		// If the user presses the back button, bail from the while loop, but don't save the changes made
		if (button == BUTTON_BACK) {
			break;
		}
	}
	Menu.enable(true);
	return;
}

/** Calculate the Minimum Camera Interval

Determins the minimum camera interval, accounting for camera settings, motor moves, etc.


*/

float calcMinInterval() {
	USBSerial.println("Setting min interval");
	float minInt = 0.0;
	USBSerial.println(minInt);
	
	// minimum interval calculation
	//if( camera_bulb )
	minInt += camera_exposure;
	USBSerial.println(minInt);
	//else
	//  minInt += CAM_MIN_TRIG;

	minInt += camera_wait;
	USBSerial.println(minInt);
	minInt += camera_focus;
	USBSerial.println(minInt);

	if (alt_out_flags & ALT_OUT_ANY_B){ //checks to see if any aux i/o are on before the camera shoots
		minInt += alt_before_ms;
		minInt += alt_before_delay;
	}

	if (alt_out_flags & ALT_OUT_ANY_A) { //checks to see if any aux i/o are on after the camera shoots
		minInt += alt_after_ms;
		minInt += alt_after_delay;
	}

	if (motion_sms)
	{
		byte motorEnabled = 0;
		byte longestMotor = 0;
		for (byte i = 0; i < MOTOR_COUNT; i++)
		{
			if (motors[i].flags & MOTOR_UEN_FLAG) {
				if (motors[i].speed > motors[longestMotor].speed) {    //determine the longest running enabled motor
					longestMotor = i;
				}
				motorEnabled = 1;
			}
		}

		USBSerial.println(motor_pwm_maxperiod);
		USBSerial.println(motors[longestMotor].speed);
		USBSerial.println(motor_pwm_minperiod);

		minInt += motorEnabled * motor_pwm_maxperiod * motors[longestMotor].speed * motor_pwm_minperiod / 1000.0;  //adds time required by longest running motor

	}

	minInt = minInt / 1000.0; // Convert minInt from milliseconds to seconds
	USBSerial.println(minInt);

	return minInt;
}
