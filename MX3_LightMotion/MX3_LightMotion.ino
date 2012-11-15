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



#include <TimerOne.h>
#include <Arduino.h>
#include <MsTimer2.h>
#include <EEPROM.h>
#include <LiquidCrystal.h>


#include "OMCamera.h"
#include "OMState.h"
#include "OMEEPROM.h"
#include "OMMenuMgr.h"

 // Our structs, constants, etc.
 
#include "MX3.h"


  // predefine this function to declare the default argument
void stopProgram(boolean force_clear = true);


  // Prep Control Variables

boolean       motion_sms       = false;
boolean       running          = false;
boolean       motor_running    = false;
boolean       metric_ui        = false;

byte          lcdDisable       = 30;

unsigned long run_time         = 0;
unsigned long check_time       = 0;

 // initialize core objects
 
 
OMCamera     Camera = OMCamera(SHUTTER_PIN, FOCUS_PIN);
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
 
   // Setup Alt I/O
 altSetup();
  
}

void loop() {
   
   uiCheck();
  
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
  Camera.stop();
  running = false;
}

void stopProgram(boolean force_clear) {
              
   // stop/clear program
   
  //  if( force_clear == true ) 

  running      = false;
  run_time     = 0;
  
  camClear();
  
    // Force block on state engine
  Engine.state(ST_BLOCK);
  
}

void startProgram() {
  
     // start program
  running = true;
  
  Engine.state(ST_CLEAR); 
  
  check_time = millis();
                    
}





