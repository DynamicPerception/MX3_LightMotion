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
 Engine.setHandler(ST_ALTP, cycleCheckAltPost);
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
  
    // trigger any outputs that need to go before the exposure
  if( alt_out_flags & ALT_OUT_ANY_B && ( millis() - camera_tm ) >= ( (camera_delay * (float) SECOND) - alt_before_delay ) && ! alt_block ) {
      alt_block = ALT_BLOCK_B;
      altOutStart(ALT_TRIG_B);
      return;
  }
    
    
    // NOTE: You cannot combine External Interval and Out Before (how would we know when the external signal would be
    // coming, and trigger the output at the right time before, eh?)
    
    
    // if enough time has passed, and we're ok to take an exposure
    // note: for slaves, we only get here by a master signal, so we don't check interval timing
  
  if( alt_force_shot == true || ( millis() - camera_tm ) >= (camera_delay * (float) SECOND)  ) {
    
        // only execute cycle if the camera is not currently busy

    if( ! Camera.busy() ) 
        cycleTriggerCamera();
    
  }
  
}

void cycleTriggerCamera() {
 
  Engine.state(ST_BLOCK);
  
    // we must clear this here, elsewise out alt output may just trigger over and over
  alt_block      = ALT_BLOCK_NONE;
  
  alt_force_shot = false;
  camera_tm      = millis();  
  
      // trigger focus, if needed, which will set off the chain of
      // callback executions that will walk us through the complete exposure cycle.
      // -- if no focus is configured, nothing will happen but trigger
      // the callback that will trigger exposing the camera immediately

  Camera.focus(); 
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
     // (if we're in SMS, and the motor is running, do not
     // change state engine state)

    if( motion_sms && motor_running )
      return;
  
    // no longer running, or in continuous mode ok to fire camera

      Engine.state(ST_CLEAR);        
}

/** Check Alt Output Post Trigger

 This callback handler handle the ST_ALTP state.
 
 If one or more I/Os are configured as post-exposure outputs, this state
 will cause a run delay for the specified output delay time, and then
 trigger the outputs to fire.
 
 @author
 C. A. Church
 */

void cycleCheckAltPost() {
  
  static unsigned long alt_tm = millis();
  
  if( alt_out_flags & ALT_OUT_ANY_A && ! alt_block ) {
    alt_block = ALT_BLOCK_A;
    alt_tm = millis();
  }
  else if( alt_out_flags & ALT_OUT_ANY_A && ( millis() - alt_tm ) > alt_after_delay ) {
    alt_block = ALT_BLOCK_NONE;
    altOutStart(ALT_TRIG_A);
  }
  
}

