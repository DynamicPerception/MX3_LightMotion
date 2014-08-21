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
  Camera control functions
  ========================================
  
*/

 

 // necessary camera control variables

unsigned int    camera_max_shots = 0;
unsigned long       camera_fired = 0;
byte            camera_mirror_up = 0;
unsigned long  camera_exposure = 120;	// Wait time in ms
unsigned long      camera_wait = 100;	// Wait time in ms
unsigned long       camera_focus = 0;
int			camera_focal_length = 50;
byte                 camera_bulb = 0;
byte              camera_focLock = 1;
float             camera_delay = 1.0;	// Interval time in seconds


/** Initialize Camera

 @author 
 C. A. Church
 */
 
void camSetup() {
  
  if( camera_bulb ) {
    Camera.exposeTime(camera_exposure);
    Camera.waitTime(camera_wait);
  }
  else {
    Camera.exposeTime(camera_exposure); //CAM_MIN_TRIG);
    Camera.waitTime(camera_wait);
  }
  
  Camera.focusTime(camera_focus);
  Camera.exposureFocus(camera_focLock);
  
   // attach status callback hanlder
  Camera.setHandler(camCallBack);
 
}

/** Clear Camera

 To be called when stopProgram is executed
 
 @author
 C. A. Church
 */
 
void camClear() {
  camera_fired = 0;
    // Force a stop on the camera
  Camera.stop();
}

/** Camera Expose State Handler

  @author
  C. A. Church
  */
  
void camExpose() {
    // state to block must happen before call to expose()
  Engine.state(ST_BLOCK); // block further activity until exposure is done
  Camera.expose();
}

/** Camera Wait State Handler

 @author
 C. A. Church
 */
 

void camWait() {
    // state to block must happen before call to wait()
  Engine.state(ST_BLOCK); // block further activity until post delay is done
  Camera.wait();
}


/** Camera Callback Handler

  This callback is called whenever an asynchronous camera activity begins or ends, and 
  is able to take some actions as different states are reported out of the
  camera object.
  
  We only care about when focus/exposure/wait activities complete, so that's what we look for..
  
  @author
  C. A. Church
  */
  
void camCallBack(byte code) {
    

  //
  // Do NOT attempt to use delay() here, or call another camera action directly, as this
  // function is called in an interrupt and can daisy-chain under certain configurations,
  // which can result in unexpected behavior
  
  
  if( code == OM_CAM_FFIN ) {
      // focus done
    Engine.state(ST_EXP);
  }
  else if( code == OM_CAM_EFIN ) {
      // exposure done
    camera_fired++;
    Engine.state(ST_WAIT);
  }
  else if( code == OM_CAM_WFIN ) {
      // wait done
      
      // we may have exposure repeat cycle to manage
      // after the post-exposure delay
    checkMirrorUp();
  }
  

}

 /** Camera Mirror Up Move Checking
 
  Checks to see if the camera must fire another exposure because of the mirror up setting is enabled
  
  @author
  C. A. Church

  @modified
  M. Ploof
  */
  
void checkMirrorUp() {
  
    static boolean mirror_up_done = false;
    
		// if we don't have the mirror up function enabled,
		// then go ahead and clear for alt out post shot check
	if (camera_mirror_up == 0) {
		Engine.state(ST_ALTP);
		return;
    }
	else if (mirror_up_done == true) {
        // reset the mirror up move var
		mirror_up_done = false;
		// clear for check post-exposure alt output trigger
		Engine.state(ST_ALTP);
		return;
    }
   
	// counteract the exposure incrementing due to the mirror trigger
	camera_fired--; 
    // trigger another exposure
	mirror_up_done = true;
    Engine.state(ST_EXP);
}


