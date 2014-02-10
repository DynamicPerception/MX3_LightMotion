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


#include <TimerOne.h>
#include <Arduino.h>
#include <MsTimer2.h>
#include <EEPROM.h>
#include <LiquidCrystal.h>


#include "OMCamera.h"
#include "OMState.h"
#include "OMEEPROM.h"
#include "OMMenuMgr.h"

 // Our pin mappings, structs, constants, etc.

#include "MX3_PinMapping.h" 
#include "MX3_Core.h"




  // predefine this function to declare the default argument
void stopProgram(boolean force_clear = true);


  // Prep Control Variables that must be defined early

boolean          motion_sms = false;
boolean             running = false;
boolean       motor_running = false;
boolean           metric_ui = false;
byte             lcdDisable = 30;
byte               VFDBright = 3;        // 0 = 25%, 1 = 50%, 2 = 75%, 3 = 100%
unsigned long    check_time = 0;
unsigned long      run_time = 0;
unsigned long     camera_tm = 0;
float     sensor_minVoltage = 10.5;
boolean      sensor_enVWarn = true;
boolean     sensor_enHeater = false;
byte              alt_block = 0;
boolean         disp_metric = 0;



 // initialize core objects
 
 
OMCamera     Camera = OMCamera(SHUTTER_PIN, FOCUS_PIN);
    // there are 8 possible states in 
    // our engine (0-7)
OMState      Engine = OMState(7);



 // initialize LCD object
LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

//byte nodeAddr 	= 2;

// OMMoCoMaster myBus = OMMoCoMaster(Serial);




void setup() {
  
  
 // Serial.begin(OM_SER_BPS);

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
 
 jumpSetup();
  
}

void loop() {
  
  static unsigned long  sensor_tm = 0;
   
       // check sensors
   if( millis() - sensor_tm > SENS_POLL_TIME ) {
     sensorPoll();
     sensor_tm = millis();
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
   
 
}



void pauseProgram() {
     // pause program
     
  if( alt_block )
    altOutStop();
    
  Camera.stop();
  running = false;
}

void stopProgram(boolean force_clear) {
              
   // stop/clear program
   
  //  if( force_clear == true ) 

  running  = false;
  run_time = 0;
  
  altOutStop();

  motorStop(false);
   
  camClear();
  
    // Force block on state engine
  Engine.state(ST_BLOCK);
  
}

void startProgram() {
  
     // start program
  running = true;
  
  Engine.state(ST_CLEAR); 
  
  check_time = millis();
  camera_tm  = millis();
                    
}





