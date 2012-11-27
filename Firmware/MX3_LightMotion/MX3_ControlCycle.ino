/*

LightMotion

See dynamicperception.com for more information


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
  Main Timing Cycle functions - initialize StateEngine (OMState)
  ========================================
  
*/




float          camera_delay     = 1.0;


/** Initialize State Engine

  @author
  C. A. Church
  */

void cycleSetup() {

 Engine.setHandler(ST_CLEAR, cycleCamera);
 Engine.setHandler(ST_MOVE, cycleClearToMove);
 Engine.setHandler(ST_RUN, cycleCheckMotor);
 Engine.setHandler(ST_EXP, camExpose);
 Engine.setHandler(ST_WAIT, camWait);
}



/** Check Camera Cycle Callback Handler

 Checks for a camera ready to fire cycle (on interval), and triggers the chain of
 camera actions.  
 
 Side effects:
 
   - Stops program if maximum shots configured and exceeded
   - Skips camera cycle and goes to motor move if camera disabled
   - Does nothing if camera currently busy
   
 @author 
 C. A. Church 
 */
   
void cycleCamera() {

    // stop program if max shots exceeded    
  if( camera_max_shots > 0  && camera_fired >= camera_max_shots ) {
           // stop program running w/o clearing variables
      stopProgram(false);
      return;
  }
  

    // if enough time has passed, and we're ok to take an exposure
    // note: for slaves, we only get here by a master signal, so we don't check interval timing
  
  if( alt_force_shot == true || ( millis() - camera_tm ) >= (camera_delay * SECOND)  ) {

      // trigger focus, if needed, which will set off the chain of
      // callback executions that will walk us through the complete exposure cycle.
      // -- if no focus is configured, nothing will happen but trigger
      // the callback that will trigger exposing the camera immediately
    
    if( ! Camera.busy() ) {
        // only execute cycle if the camera is not currently busy
        
      Engine.state(ST_BLOCK);
      alt_force_shot = false;
      camera_tm = millis();  
      Camera.focus();
    } 
    
  }
  
}

/** Move Motors Callback Handler

  Executes any required move
  
  @author
  C. A. Church
  */
 
void cycleClearToMove() {
    
    // disables the motor if a lead-in or out is required
   motorCheckLead();
   
    // checks ramp settings for each motor
   motorCheckRamp();
        
         // ok to run motors, if needed
   motorRun(motion_sms);
   
        // trigger motor status check on next engine cycle
   Engine.state(ST_RUN);
}


/** Check Motor Status Callback Handler

 This callback handler handles the ST_RUN state.  
 
 If continuous motion is requested, sets back to clear to fire state, no matter if motors
 are running.
 
 If interleaved (SMS) motion is requested, blocks fire state until all movement is complete.
 
 @author
 C. A. Church
 */
 
void cycleCheckMotor() {
         // still running
     
     // do not block on continuous motion of any sort

    if( motion_sms && motor_running )
      return;
  
    // no longer running, or in continuous mode ok to fire camera

      Engine.state(ST_CLEAR);        
}




