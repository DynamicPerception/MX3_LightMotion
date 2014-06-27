
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

 @change log
 Michael Ploof 5/2/14:
 *Added several new move modes and broke all modes out into separate 
  functions for clarity
 
 */
 
void uiMenuManual(byte p_motor) {
  
  lcd.noBlink();
    
    // can't do manual move when running
  if( running ) {
    lcd.home();
    lcd.print(STR_WARN1);
    lcd.setCursor(0, 1);
    lcd.print(STR_WARN2);    
    delay(1000);
    return;
  }
  
  // Execute the manual move based on the type selected (hold or start/stop)
  manualMove(p_motor, mmSetting);

}


/* Manual move with buttons as start / stop toggles

@param p_motor
Motor to move manually

@param p_type
Use continuous or hold type move


@author
Kevin Melotti
*/

void manualMove(byte p_motor, byte p_type) {

	// Save all the motor settings so they can be
	// restored after leaving manual move mode.
	float        wasSpd = motorSpeed(p_motor);
	boolean		 wasDir = motors[p_motor].desiredDirection;
	boolean         run = false;
	byte       wasFlags = motors[p_motor].flags;
	byte       wasMotor = ui_curMotor;
	float           spd = 1.00;

	Menu.enable(false);

	lcd.clear();
	lcd.print(STR_RES2);

	// user-enable motor (necessary if the user had it disabled)
	motors[p_motor].flags |= MOTOR_UEN_FLAG;

	// need to do this for the use of uiCursorChangeMotSpd below
	ui_curMotor = p_motor;

	// Continuous type move
	while (1) {


		byte       button = Menu.checkInput();
		int		   buttonValue = analogRead(BUT_PIN);

		lcd.setCursor(4, 1);
		if (motors[p_motor].flags & MOTOR_ROT_FLAG)
			lcd.print("-");
		else
			lcd.print("L");
		

		lcd.setCursor(6, 1);
		
		if(spd != 1)
			lcd.print(" ");
		
		lcd.print(spd * 100, 0);
		lcd.print("% ");

		lcd.setCursor(11, 1);

		if (motors[p_motor].flags & MOTOR_ROT_FLAG)
			lcd.print("+");
		else
			lcd.print("R");

		if (button == BUTTON_FORWARD || button == BUTTON_BACK) {
			byte dir = (button == BUTTON_FORWARD) ? 1 : 0;
				
			if (p_type == MM_SELECT) {
				// if not already running, run it
				if (!run) {
					motorDir(p_motor, dir);
					motorSpeed(p_motor, spd);
					motorRun(false, p_motor);
					run = true;
					//USBSerial.println("Starting motor on continuous run");
				}
				else {
					// already running, stop the motor
					//USBSerial.println("Stopping motor from continuous run");
					motorStop();
					run = false;
				}
			}
			else if (p_type == MM_HOLD) {
				motorDir(p_motor, dir);
				motorSpeed(p_motor, spd);
				motorRun(false, p_motor);
				run = true;

				//Sets the lower and upper boundaries for the analog button depending on which button is pressed
				int lowerBound = ((button == BUTTON_BACK) ? BUTREV_VAL : BUTFWD_VAL) - BUT_THRESH;
				int upperBound = ((button == BUTTON_BACK) ? BUTREV_VAL : BUTFWD_VAL) + BUT_THRESH;

				//Keeps the motor running while the button is held down      
				while (lowerBound <= buttonValue && buttonValue <= upperBound) {
					buttonValue = analogRead(BUT_PIN);
				}

				//stops the motors after the button is released
				motorStop();
				run = false;

			}
		} // end if( button == BUTTON_FORWARD...
			
		//Adjust the speed of the motor
		if (button == BUTTON_INCREASE || button == BUTTON_DECREASE) {
			float dir = (button == BUTTON_INCREASE) ? 0.25 : -0.25;
			spd += dir;
			if (spd >= 1.00)
				spd = 1.00;
			else if (spd <= 0.25)
				spd = 0.25;
		}

		// enter exits, recover state and exit action
		if (button == BUTTON_SELECT) {
			ui_curMotor = wasMotor;
			motorDir(p_motor, wasDir);
			motorStop();
			motors[p_motor].flags = wasFlags;
			motorSpeed(p_motor, wasSpd);
			Menu.enable(true);
			return;
		}

	}
}

///* Battery endurance testing mode
//
//This mode runs all three motors at full speed, alternating
//every 5 or 10 seconds. The direction change interval is 
//selected by the user in any of the motor axis menus.
//
//@author
//Michael Ploof
//*/
//
//void batteryTestMode(byte p_motor) {
//
//	float        wasSpd[MOTOR_COUNT];
//	byte       wasFlags[MOTOR_COUNT];
//	for (byte i = 0; i < MOTOR_COUNT; i++) {
//		wasSpd[i] = motorSpeed(i);
//		wasFlags[i] = motors[i].flags;
//	}
//	boolean         run = false;
//	byte       wasMotor = ui_curMotor;
//	float           spd = 1.00;
//
//	Menu.enable(false);
//
//	const char BATT[] = "Batt. Endurance!";
//	const char FAST[] = "5s Interval     ";
//	const char SLOW[] = "10s Interval    ";
//	long interval = 10; // Motor switching interval in seconds
//	long cycle_max = interval * 10000 / 6;
//	cycle_count = 0;
//	byte dir = 0;
//
//	lcd.clear();
//	lcd.print(BATT);
//
//	// user-enable motor
//	for (byte i = 0; i < MOTOR_COUNT; i++)
//	{
//		motors[i].flags |= MOTOR_UEN_FLAG;
//	}
//
//	while (1)
//	{
//		byte button = Menu.checkInput();
//
//		if (!run) {
//			// Prompt user to select test speed
//			lcd.setCursor(0, 1);
//			lcd.print("L-5s : R-10s");
//
//		}
//		// If the test has been started, continue it and reverse directions at set interval times
//		else {
//
//			USBSerial.println("Entering run if");
//			// Run the motor test until the select button is pressed
//			while (run) {
//				button = Menu.checkInput();
//
//				USBSerial.print("Current cycle count: ");
//				USBSerial.println(cycle_count);
//				USBSerial.print("Current button press: ");
//				USBSerial.println(button);
//				// When time exceeds interval length, based on PWM count, switch the motor directions
//				if (cycle_count > cycle_max) {
//					USBSerial.println("Direction switching!");
//					motorStop();
//					motorDirFlip();
//					cycle_count = 0;
//					for (byte i = 0; i < MOTOR_COUNT; i++) {
//						motorRun(false, i);
//					}
//					// Display current motor direction
//					if (dir == 0) {
//						dir = 1;
//						lcd.setCursor(16, 1);
//						lcd.print("F");
//						USBSerial.println("Motor going forward");
//					}
//					else {
//						dir = 0;
//						lcd.setCursor(16, 1);
//						lcd.print("R");
//						USBSerial.println("Motor going backward");
//					}
//				}
//				if (button == BUTTON_SELECT) {
//					USBSerial.println("Stopping motors");
//					motorStop();
//					USBSerial.println("Setting run flag false");
//					run = false;
//				}
//			}
//			button = BUTTON_NONE;
//			delay(100);
//		}
//
//		// Button press is forward or back
//		if (button == BUTTON_FORWARD || button == BUTTON_BACK) {
//
//			// If the motors aren't running when a button is pressed, start them
//			if (!run)	{
//
//				// Draw fast / slow screens
//				lcd.setCursor(0, 1);
//				if (button == BUTTON_FORWARD) {
//					interval = 10;
//					cycle_max = interval * 10000 / 6;
//					lcd.print(SLOW);
//					USBSerial.print("Setting long cycle_max: ");
//					USBSerial.println(cycle_max);
//				}
//				else if (button == BUTTON_BACK) {
//					interval = 5;
//					cycle_max = interval * 10000 / 6;
//					lcd.print(FAST);
//					USBSerial.print("Setting short cycle_max");
//					USBSerial.println(cycle_max);
//				}
//
//				// Start motor 0
//				/*motorDir(0, 0);
//				motorSpeed(0, 1.0);
//				motorRun(false, 0);
//				USBSerial.println("Starting motor 1");
//				run = true;
//				*/
//				// 	Start all three motors
//
//				for (byte i = 0; i < MOTOR_COUNT; i++) {
//					motorDir(i, 0);
//					motorSpeed(i, 1.0);
//					motorRun(false, i);
//					USBSerial.println("Starting motor");
//					run = true;
//				}
//
//				USBSerial.println("Ending motor setting");
//			}
//		}
//
//		// Button press is select, increase, or decrease
//		else {
//
//			if (button == BUTTON_INCREASE || button == BUTTON_DECREASE) {
//				// Do nothing
//
//			}
//			else if (button == BUTTON_SELECT) {
//				// recover state and exit action
//				ui_curMotor = wasMotor;
//				motorStop();
//				for (byte i; i < MOTOR_COUNT; i++) {
//					motors[i].flags = wasFlags[i];
//					motorSpeed(i, wasSpd[i]);
//				}
//				Menu.enable(true);
//				return;
//			}
//		}
//	}
//}

/* MoCoBus Node Mode

The MX3 becomes a node on the MoCoBus and will listen for 
commands from a master device. It will stay in this mode until
the user presses the select button to exit. This mode is entered
by selecting the "MoCoBus Node" setting in the manual movement
type selection screen, then acessing any of the three motor axis menus.

@author
Michael Ploof
*/

void nodeMode() {
/*
	float        wasSpd[MOTOR_COUNT];
	byte       wasFlags[MOTOR_COUNT];
	for (byte i = 0; i < MOTOR_COUNT; i++) {
		wasSpd[i] = motorSpeed(i);
		wasFlags[i] = motors[i].flags;
	}
	boolean         run = false;
	byte       wasMotor = ui_curMotor;
	float           spd = 1.00;

	Menu.enable(false);

	// Set node flag as true
	node = true;	

	// LCD messages
	const char MOCOBUS_NODE_0[] = "MoCoBus Node    ";
	const char MOCOBUS_NODE_1[] = "Back to exit	   ";
	const char MOCOBUS_NODE_2[] = "Exit node mode? ";
	const char MOCOBUS_NODE_3[] = "Back to confirm ";

	lcd.clear();
	lcd.print(MOCOBUS_NODE_0);
	lcd.setCursor(0,1);
	lcd.print(MOCOBUS_NODE_1);

	// clear any stale commands left on the serial bus
	flushSerial();

	// user-enable motor
	for (byte i = 0; i < MOTOR_COUNT; i++) {
		motors[i].flags |= MOTOR_UEN_FLAG;
	}

	// Put controller into node mode until back button is pressed
	while (1)
	{
		// Look for a button press and exit on back
		byte button = Menu.checkInput();
		if (button == BUTTON_BACK) {

			// Confirm exit of node mode
			lcd.clear();
			lcd.print(MOCOBUS_NODE_2);
			lcd.setCursor(0, 1);
			lcd.print(MOCOBUS_NODE_3);
			button = BUTTON_NONE;
			
			// Check the current time and set the menu time-out to 9 seconds
			long time = millis();
			long elapsed_time = 0;
			const int wait_time = 9;

			// Wait 5 seconds for user to confirm node exit before returning to node screen
			while (elapsed_time < wait_time) {


				// Calculate the remaining time till time-out and display it
				elapsed_time = round((millis() - time) / SECOND);
				byte disp_time = 9 - (millis() - time);

				lcd.noBlink();
				lcd.setCursor(0, 16);
				lcd.print(disp_time);

				// Check button input
				button == Menu.checkInput();

				// If back button is pressed, exit node mode
				if (button == BUTTON_BACK) {

					// recover state and exit action
					ui_curMotor = wasMotor;
					motorStop();
					for (byte i; i < MOTOR_COUNT; i++) {
						motors[i].flags = wasFlags[i];
						motorSpeed(i, wasSpd[i]);
					}
					Menu.enable(true);
					run = false;
					return;
				}
			}			
		}

		// See if there is an incoming command
		if (Serial.available()) {
			
			// If there is a command available, the assigned handler will run.
			// For this version of the firmware, the handlers for commands to 
			// the MX3 when in node mode are found in MX3_MoCoBusHandlers.ino
			Node.check();
			flushSerial();

		}
	}
	*/
}


/* MoCoBus Master Mode

The MX3 becomes a master on the MoCoBus and can issue commands to other
devices on the bus.

@author
Michael Ploof
*/

void masterMode() {
	/*
	float        wasSpd[MOTOR_COUNT];
	byte       wasFlags[MOTOR_COUNT];
	for (byte i = 0; i < MOTOR_COUNT; i++) {
		wasSpd[i] = motorSpeed(i);
		wasFlags[i] = motors[i].flags;
	}
	boolean         run = false;
	byte       wasMotor = ui_curMotor;
	float           spd = 1.00;

	Menu.enable(false);

	// Setting the device address (0 is master address)
	byte devAddr = 0;

	// initialize device as master
	//OMMoCoMaster Master = OMMoCoMaster(Serial);

	const char MOCOBUS_MASTER[] = "MoCoBus Master  ";

	lcd.clear();
	lcd.print(MOCOBUS_MASTER);

	// clear any stale commands left on the serial bus
	flushSerial();

	// user-enable motor
	for (byte i = 0; i < MOTOR_COUNT; i++) {
		motors[i].flags |= MOTOR_UEN_FLAG;
	}

	// Put controller into master mode until... er, I'll figure that out later
	while (1)
	{
		// Look for a button press and exit on select
		byte button = Menu.checkInput();
		if (button == BUTTON_SELECT) {
			// enter exits, recover state and exit action
			ui_curMotor = wasMotor;
			motorStop();
			for (byte i; i < MOTOR_COUNT; i++) {
				motors[i].flags = wasFlags[i];
				motorSpeed(i, wasSpd[i]);
			}
			Menu.enable(true);
			run = false;
			return;
		}

		// See if there is an incoming command
		if (Serial.available()) {
			// If there is a command available, the assigned handler will run
			Node.check();
			flushSerial();
		}
	}
	*/
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


/* CHeck Preset for Given Motor

Returns the preset number for a given motor

@param p_motor
The motor # to check the preset for

@author
M. Ploof
*/

byte presetCheck(byte p_motor) {

	byte totalPresets = sizeof(motorPresets) / sizeof(MotorPreset);
	boolean presetFound;

	// Determine whether the the ratio, RPM, or rotary settings have been changed to non-preset values
	for (byte i = 1; i < totalPresets; i++) {

		boolean rpm_check = motors[p_motor].rpm == motorPresets[i].rpm;
		boolean ratio_check = motors[p_motor].ratio == motorPresets[i].ratio;
		boolean rot_check = (motors[p_motor].flags & MOTOR_ROT_FLAG) == (MOTOR_ROT_FLAG & (motorPresets[i].rotary << 4));


		/*USBSerial.print("Checking matching on preset ");
		USBSerial.println(i);
		USBSerial.print("RPM check: ");
		USBSerial.println(rpm_check);
		USBSerial.print("Reference value: ");
		USBSerial.println(motorPresets[i].rpm);
		USBSerial.print("Comparison value ");
		USBSerial.println(motors[p_motor].rpm);
		USBSerial.print("Ratio check: ");
		USBSerial.println(ratio_check);
		USBSerial.print("Reference value: ");
		USBSerial.println(motorPresets[i].ratio);
		USBSerial.print("Comparison value ");
		USBSerial.println(motors[p_motor].ratio);
		USBSerial.print("Rotation check: ");
		USBSerial.println(rot_check);
		USBSerial.print("Reference value: ");
		USBSerial.println(MOTOR_ROT_FLAG & (motorPresets[i].rotary << 4), BIN);
		USBSerial.print("Comparison value ");
		USBSerial.println(motors[p_motor].flags & MOTOR_ROT_FLAG, BIN);*/

		if (rpm_check == true && ratio_check == true && rot_check == true) {

			//USBSerial.println("This is a matching preset <--------------------------------------------<");
			motors[p_motor].motorPreset = i;
			presetFound = true;
			break;

		}
		else {
			//USBSerial.println("Settings do not match this preset");
			presetFound = false;
		}

		//USBSerial.println("");

	}

	return(presetFound);
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

	// Don't let the user change the preset if that motor is currently running

	lcd.noBlink();

	if ((motors[p_motor].flags & MOTOR_UEN_FLAG) && running) {
		lcd.home();
		lcd.print("Stop motor to   ");
		lcd.setCursor(0, 1);
		lcd.print("change preset   ");
		delay(1000);
		return;
	}

	// disable menu key processing
	Menu.enable(false);
 
	byte totalPresets = sizeof(motorPresets) / sizeof(MotorPreset);
	byte    curPreset = motorPresetSelected[p_motor];
	byte lowest_preset;

	// If EZ mode is active, disallow custom presets
	if (ez_mode)
		lowest_preset = 1;
	else
		lowest_preset = 0;

	// See if the motor settings match a preset. If they don't, display preset 0 ("Custom") when not in EZ mode to the lowest speed motor preset;
	if (!presetCheck(p_motor) && !ez_mode)
		curPreset = lowest_preset;
	if (!presetCheck(p_motor) && ez_mode)
		curPreset = totalPresets-1;
	
	MotorPreset thsPreset = motorPresets[curPreset];

 
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print(STR_PRESET);
	lcd.setCursor(13, 0);
	lcd.print(p_motor + 1);
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
		// It is not a rotary setting turn off the flag
		if (!thsPreset.rotary)
			motDef->flags &= ~(MOTOR_ROT_FLAG);
		// If it is, turn it on
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
		curPreset = (curPreset == totalPresets - 1) ? lowest_preset : curPreset + 1;
	else if (button == BUTTON_DECREASE)
		curPreset = curPreset == lowest_preset ? totalPresets - 1 : curPreset - 1;     

	thsPreset = motorPresets[curPreset];

   
	lcd.setCursor(0,1);
	lcd.print(STR_BLNK);
	lcd.setCursor(0,1);
	USBSerial.println(thsPreset.name);
	lcd.print(thsPreset.name);

	motors[p_motor].motorPreset = curPreset;
	}

	refreshMotors(true);
    
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

/* MoCoBus Mode

If the MX3 is set as the master, this menu will
populate with devices on the MoCoBus bus

@author
Michael Ploof
*/

//*****************************************************************************************************++++++++++++++++++++++++++++++++===================================!!!!!!!!!!!!!
// Temporary function to test communication between MX3 and NMX
void uiMenuManualMocobus() {

	Menu.enable(false);
	
	// Set the message strings and then print them
	const char MESSAGE_0[] = "Searching for   ";
	const char MESSAGE_1[] = "MoCoBus nodes   ";
	const char MESSAGE_2[] = "List of nodes   ";
	const char MESSAGE_3[] = "would go here   ";
	
	lcd.clear();
	lcd.print(MESSAGE_0);
	lcd.setCursor(0, 1);
	lcd.print(MESSAGE_1);

	byte address_array[255];
	byte address_index = 0;
	int button;

	// Cycle through all the possible node addresses and listen for responses
	for (byte i = 2; i < 30; i++) {

		// Return to higher level menu if the back button is pressed
		button = Menu.checkInput();
		if (button == BUTTON_BACK) {
			Menu.enable(true);
			return;
		}

		char dat_array_0[1] = { 2 };

		// Send command to address 3 and listen for a response
		if (Master.command((uint8_t)i, (uint8_t)1, (char*)dat_array_0, (uint8_t) sizeof(dat_array_0)) != -1) {

			outputReponse(i, Master.responseType(), Master.responseData());

			address_array[address_index] = i;
			address_index++;
		}
		else {
			USBSerial.print("Nothing at address ");
			USBSerial.println(i);
		}
	}

	lcd.clear();
	lcd.print(MESSAGE_2);
	lcd.setCursor(0, 1);
	lcd.print(MESSAGE_3);

	byte address = 3;

	// Set microsteps to 1
	char dat_array_1[2] = { 0x11, 0x01 };
	uint8_t len = sizeof(dat_array_1);
	Master.command((uint8_t)address, (uint8_t)2, (char*)dat_array_1, (uint8_t) sizeof(dat_array_1));
		
	//USBSerial.print("Output of char dat_array_1: ");
	//for (byte i = 0; i < sizeof(dat_array_1); i++) {

	//	USBSerial.print(dat_array_1[i]);

	//}
	//USBSerial.println("");

	// Run the motor at above address
	motorEnable(address);

	// Continuous mode enable
	continuousMode(address, true);

	// Steps per second
	stepsPerSecond(address, 200); //<-----------------------------------------------------------------------------Change the steps parameter to the actual variable used

	// Motor move (simple)
	byte node_address = 3;
	byte packet_code = 2;
	motorMove(node_address, packet_code);

	while (button != BUTTON_BACK) {
		button = Menu.checkInput();
	}

	button = BUTTON_NONE;

}

/****************** NMX MoCoBus Command Functions ******************/
void motorEnable(byte p_address) {

	// Motor enable
	byte _packet_code = 3;
	char _data[2] = { 0x02, 0x05 };
	Master.command((uint8_t)p_address, (uint8_t)_packet_code, (char*)_data, (uint8_t) sizeof(_data));

}

void continuousMode(byte p_address, boolean p_continuous) {

	// Continuous mode enable
	byte _packet_code = 3;
	if (p_continuous == true) {
		char _data[3] = { 0x02, 0x0b, 0x01 };
		Master.command((uint8_t)p_address, (uint8_t)_packet_code, (char*)_data, (uint8_t) sizeof(_data));
	}
	else if(p_continuous == false) {
		char _data[3] = { 0x02, 0x0b, 0x00 };
		Master.command((uint8_t)p_address, (uint8_t)_packet_code, (char*)_data, (uint8_t) sizeof(_data));
	}

}

void stepsPerSecond(byte p_address, float p_steps) {
	// Set steps per second
	byte _packet_code = 3;
	char _data[6] = { 0x02, 0x09, 0x42, 0xA9, 0x80, 0x00 };
	Master.command((uint8_t)p_address, (uint8_t)_packet_code, (char*)_data, (uint8_t) sizeof(_data));
}

void motorMove(uint8_t p_address, uint8_t p_packet_code) {

	char command_array[6] = { 0x06, 0x01, 0x00, 0x00, 0x00, 0x00 };
	Master.command(p_address, p_packet_code, (char*)command_array, (uint8_t) sizeof(command_array));

}

void outputReponse(byte i, int response_type, char* dat) {

//	USBSerial.print("A node was detected at address ");
//	USBSerial.println(i);
//
//	USBSerial.print("Reponse type is: ");
//	USBSerial.println(response_type);
//
//	USBSerial.print("Reponse length: ");
//	USBSerial.println(Master.responseLen());
//
//	USBSerial.print("Data is: ");
//	USBSerial.println(dat);
//	USBSerial.println("");
}