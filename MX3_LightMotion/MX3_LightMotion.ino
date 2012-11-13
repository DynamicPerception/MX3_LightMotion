/* 

 MX3 LightMotion Firmware
 
 Copyright (c) 2012 Dynamic Perception LLC
 
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


const char MX3_VERSTR[]  =  "MX3 v. 0.0.1";
const char MX3_SUBSTR[]  =  "Rabbit Food";
const char MX3_C1STR[]   =  "(c) 2012 Dynamic";
const char MX3_C2STR[]   =  "Perception";

 /*  state transitions
 
  ST_BLOCK - do not allow any action to occur (some event is in process, block the state engine)
  ST_CLEAR - clear to start cycle
  ST_MOVE  - clear to move motor
  ST_RUN   - motor is currently running
  ST_EXP   - clear to expose camera (or not...)
  ST_WAIT  - in camera wait 
  
 */
 
const byte ST_BLOCK  = 0;
const byte ST_CLEAR  = 1;
const byte ST_MOVE   = 2;
const byte ST_RUN    = 3;
const byte ST_EXP    = 4;
const byte ST_WAIT   = 5;


  
const byte SHUTTER_PIN = 22;
const byte FOCUS_PIN   = 21;

  // predefine this function to declare the default argument
void stopProgram(boolean force_clear = true);


  // Prep Control Variables

boolean       motion_sms       = false;
boolean       running          = false;
boolean       motor_running    = false;

 // initialize core objects
 
 
OMCamera     Camera = OMCamera();
    // there are 6 possible states in 
    // our engine (0-5)
OMState      Engine = OMState(6);






void setup() {

    // manage restoring EEPROM memory
 eepromCheck();
 
    // initalize state engine
 cycleSetup();
 Engine.state(ST_BLOCK);
 
   // setup camera defaults
 camSetup();
 
   // setup Menu
   
 uiMenuSetup();
  
}

void loop() {
  
   uiCheck();
  
      // if our program is currently running...
      
   if( running ) {
       // check current engine state and handle appropriately
     Engine.checkCycle();
   }

 
}



void pauseProgram() {
     // pause program
  Camera.stop();
  running = false;
}

void stopProgram(boolean force_clear) {
              
   // stop/clear program
   
  if( force_clear == true ) {

  }

  running = false;
  
    // Force a stop on the camera
  Camera.stop();
    // Force block on state engine
  Engine.state(ST_BLOCK);
  
}

void startProgram() {
  
     // start program
  running = true;
  
  Engine.state(ST_CLEAR); 
                    
}





