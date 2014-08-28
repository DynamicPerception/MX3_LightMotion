/* 

   MX3 LightMotion Firmware
   
   (c) 2008-2013 C.A. Church / Dynamic Perception LLC
   
   Additional Code by Parker Dillmann
   
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
Library Includes
========================================

*/

#include <TimerOne.h>
#include <Arduino.h>
#include <MsTimer2.h>
#include <EEPROM.h>
#include <LiquidCrystal.h>


#include "OMCamera.h"
#include "OMState.h"
#include "OMEEPROM.h"
#include "OMMenuMgr.h"
//#include "OMAxis.h"

 // Our pin mappings, structs, constants, etc.

#include "MX3_PinMapping.h" 
#include "MX3_Core.h"

// MoCoBus libraries needed to establish the bus object object,
// and provide master and node functionalities
#include "OMMoCoBus.h"
//#include "OMMoCoNode.h"
#include "OMMoCoMaster.h"


/*

========================================
MoCoBus Definitions and Declarations
========================================

*/

// Register and pin definitions for AT90USB1287 chip
// These are used to enable and handle MoCoBus RS-485 communications

// RS-485 driver enable pin
#define OMDE_PIN       28
// The register on which the driver pin is located
#define OMB_DEREG      PORTD
// The port name for the driver pin
#define DOMB_DEPFLAG   PORTD4
// Receieve buffer register
#define OMB_SRDREG     UDR1
// Data register empty flag
#define OMB_SRRFLAG    UDRE1
// USART constrol and status register
#define OMB_SRSREG     UCSR1A
// Transmit complete flag
#define OMB_SRTXFLAG   TXC1


// Setting device IDs
char devId[] = "MX3Ctrl!";
int devVer = 1;
byte dev_addr = 0;
boolean node = false;

// Setting the device address (2 is default node address)
byte devAddr = 2;

// initialize node / master object
//OMMoCoNode Node = OMMoCoNode(Serial, devAddr, devVer, devId);
OMMoCoMaster Master = OMMoCoMaster(Serial);





/*

========================================
MX3 Declarations
========================================

*/

// predefine this function to declare the default argument
void stopProgram(boolean force_clear = true);

// Should the device load with factory default settings?
boolean			  factory_reset = true;


// Prep Control Variables that must be defined early
boolean				ez_mode = false;
boolean			ez_extended = false;
boolean          motion_sms = false;
boolean             running = false;
boolean				 paused = false;
boolean       motor_running = false;
byte             lcdDisable = 30;
byte               VFDBright = 3;        // 0 = 25%, 1 = 50%, 2 = 75%, 3 = 100%
unsigned long    check_time = 0;
unsigned long      run_time = 0;
unsigned long     camera_tm = 0;
float     sensor_minVoltage = 10.5;
boolean      sensor_enVWarn = true;
boolean     sensor_enHeater = false;
byte              alt_block = 0;

enum unit_types { STANDARD, METRIC, PERCENT, EZ };
byte         units = STANDARD;

//variables for timer
unsigned long       start_time = 0;
unsigned long     camera_timer = 0;
unsigned long      delay_time  = 0;
bool               camera_flag = false;

float correction_table[100];
const float STATIC_CORRECTION = 1.05;
const float DEFAULT_LIN_PCT = 0.25; // Default percent of max speed to use for linear motors
const float DEFAULT_ROT_PCT = 0.10; // Default percent of max speed to use for rotary motors


 // initialize core objects
 
 
OMCamera     Camera = OMCamera(SHUTTER_PIN, FOCUS_PIN);
    // there are 8 possible states in 
    // our engine (0-7)
OMState      Engine = OMState(7);



 // initialize LCD object
LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);


unsigned long time = millis();


MotorDefinition motors[] = { MotorDefinition(), MotorDefinition(), MotorDefinition() };


// PWM Cycle count used for ghetto timer in battery test mode in MX3_UI_MenuActions.ino
long cycle_count = 0;


void setup() {

/*

========================================
MoCoBus setup and debugging features
========================================

*/

  // Start USBSerial to enable debugging through serial terminal

  USBSerial.begin(57600);
  
  //lcd.print("Waiting...");
  // while( ! USBSerial ) {
  //   delay(10); // do nothing
  //  }
  //  
  // delay(20);
   
   USBSerial.println("Communication established");

  // Start serial communications to enable MoCoBus  
 Serial.begin(OM_SER_BPS);

 // Set the enable pin on the rs-485 converter low (enable receieve (high enables transmit))
 digitalWrite(28, LOW);

 // Set default MoCoBus command handler functions
 //Node.setHandler(commandHandler);
 //Node.setBCastHandler(bCastHandler);


 /*

 ========================================
 MX3 setup
 ========================================

 */
 
 // manage restoring EEPROM memory
 eepromCheck();
 
 // initalize state engine
 cycleSetup();
 Engine.state(ST_BLOCK);
 
 // setup camera defaults
 camSetup();
 
 // setup motor
 motorSetup();
 
 // setup Menu
 uiMenuSetup();
 
 // Setup Alt I/O
 altSetup();
 
 // Setup Sensors
 sensorSetup();

 // Initialize correction lookup table for correcting rotational SMS moves
 // Values for moves > 0.20 degrees are based on an empirical 4th order polynomial
 // Values for moves <= 0.20 degress are basedon a power function fit
 for (byte i = 1; i <= 100; i++){
	 float degrees = (float)i / 100.0;
	 if (i <= 20)
		 correction_table[i] = 0.902349032 * pow(degrees, 0.6473069713) / degrees;
	 else {
		 correction_table[i] =
			 4.1828928087055699e+000 * pow(degrees, 0)
			 + -1.5948465805487203e+001 * pow(degrees, 1)
			 + 3.3103884510080256e+001 * pow(degrees, 2)
			 + -3.1003023885246094e+001 * pow(degrees, 3)
			 + 1.0822107570207381e+001 * pow(degrees, 4);
	 }
 }

 // Check whether the device just had the bootloader installed
 // If it has, set the motors to 8.13rpm dolly, 3.07rpm pan/tilt, and 3.07rpm pan/tilt, respectively
 if (factory_reset) {

	 // Set rotary flags
	 motors[0].flags &= ~MOTOR_ROT_FLAG;
	 motors[1].flags |= MOTOR_ROT_FLAG;
	 motors[2].flags |= MOTOR_ROT_FLAG;

	 // Set motor RPM speeds
	 motors[0].rpm = 8.13;
	 motors[1].rpm = 3.07;
	 motors[2].rpm = 3.07;

	 // Set dolly / rotary stage gear ratios
	 motors[0].ratio = 3.346;
	 motors[1].ratio = 0.306;
	 motors[2].ratio = 0.306;

	 // Set preset value
	 motors[0].motorPreset = 6;
	 motors[1].motorPreset = 7;
	 motors[2].motorPreset = 7;

	 // Disable factory_reset variable
	 factory_reset= false;

	 // Save EEPROM settings
	 
	 eepromWrite();

 }

 lcd.clear();

 // Refresh the motor power based on the initial battery setting
 refreshMotors(true);

 jumpSetup();
 


//   
//    USBSerial.write("Connected");
//    time = millis();

  
}

void loop() {
//  
//  if (millis() - time >= 500){
//    for(byte i = 0; i < MOTOR_COUNT; i++ ) {
//      
//      USBSerial.print("Flag Check for motor ");
//      USBSerial.print(i);
//      USBSerial.print(" is ");
//      USBSerial.print(((motors[i].flags & MOTOR_CDIR_FLAG == 0)^(motors[i].flags & MOTOR_INVERT_FLAG == 0)));
//          // set to 10% speed as default
//      USBSerial.print(" flag & cdir ");
//      USBSerial.print(motors[i].flags & MOTOR_CDIR_FLAG);
//      USBSerial.print(" flag & dir ");
//      USBSerial.println(motors[i].flags & MOTOR_INVERT_FLAG);
//
//    }
//    
//    time = millis();
//  }

	// If the node flag is set, immediately enter the node mode
//	if (node) {
//		nodeMode();
//	}
  
  static unsigned long  sensor_tm = 0;
   
       // check sensors
   if( millis() - sensor_tm > SENS_POLL_TIME ) {
     sensorPoll();
     sensor_tm = millis();
   }
   
   if (camera_flag) {
     delay_time = millis();
     if((delay_time-start_time)/1000 >= (camera_timer*60)){
       startProgram();
       camera_flag = false;
     }       
   }

       // handle UI interaction/updates
   uiCheck();

   
   if (!altArraysCompare()){
     altSetup();
   }
  
      // if our program is currently running...
      
   if( running ) {
     unsigned long time = millis();
     
     run_time += time - check_time;
     check_time = time;
     
       // check current engine state and handle appropriately
     Engine.checkCycle();


   } 

   // Refresh the motor power if the wait time has been exceeded
   refreshMotors(false);
}



void pauseProgram() {
     // pause program
     
  if( alt_block )
    altOutStop();

  Camera.stop();
  running = false;
  paused = true;

  if (!motion_sms)
	  motorStop(false);
}

void stopProgram(boolean force_clear) {
              
   // stop/clear program
   
  //  if( force_clear == true ) 

  running  = false;
  run_time = 0;
  
  altOutStop();

  motorStop(false);

  if (force_clear)
	  Camera.stop();
  else
	  camClear();
  
    // Force block on state engine
  Engine.state(ST_BLOCK);
  
}

void startProgram() {

	// If resuming from a paused state, unpause and don't reset lead-in / ramping counters
	if (paused){
		paused = false;
	}

	else {
		camClear();
		for (byte i = 0; i < MOTOR_COUNT; i++) {
			motors[i].startShots = 0;
			motors[i].onCycleRatio = 0;
		}
	}
  
		// start program
	running = true;
  
	Engine.state(ST_CLEAR); 
  
	check_time = millis();
	camera_tm  = millis();
                    
}




