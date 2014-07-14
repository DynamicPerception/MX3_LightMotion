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
  Alt I/O Handlers
  ========================================
  
*/


byte             alt_inputs[] = { ALT_OFF, ALT_OFF, ALT_OFF, ALT_OFF };
byte            alt_out_flags = 0;
unsigned int alt_before_delay = 100;
unsigned int  alt_after_delay = 100;
unsigned int    alt_before_ms = 100;
unsigned int     alt_after_ms = 100;
boolean        alt_force_shot = false;
boolean           alt_ext_int = false;
byte            alt_direction = FALLING;
byte             alt_out_trig = HIGH;

//Old version of alt i/o variables to compare them to see if they changed while program is running
byte             alt_inputs_old[] = { ALT_OFF, ALT_OFF, ALT_OFF, ALT_OFF };
unsigned int alt_before_delay_old = alt_before_delay;
unsigned int  alt_after_delay_old = alt_after_delay;
unsigned int    alt_before_ms_old = alt_before_ms;
unsigned int     alt_after_ms_old = alt_after_ms;
boolean        alt_force_shot_old = alt_force_shot;
boolean           alt_ext_int_old = alt_ext_int;
byte            alt_direction_old = alt_direction;
byte             alt_out_trig_old = alt_out_trig;

//Pins for the I/O
const byte           AuxARing = 0;
const byte            AuxATip = 1;
const byte            AuxBTip = 2;
const byte           AuxBRing = 3;


//timer for the I/O interrupt
unsigned long trigLast = millis();



 /** Alt I/O Setup 
 */
 
void altSetup() {
  altConnect(AuxATip ,  alt_inputs[0]);
  altConnect(AuxARing,  alt_inputs[1]); 
  altConnect(AuxBTip ,  alt_inputs[2]);
  altConnect(AuxBRing,  alt_inputs[3]);
  
    // check if any inputs are set to ext intervalometer
  boolean doExt = false;
  
  for( byte i = 0; i <= 3; i++ )
    if( alt_inputs[i] == ALT_EXTINT )
      doExt = true;
      
  alt_ext_int = doExt;
  
}

/** Handler For Alt I/O Action Trigger

 Given a particular Alt I/O line being triggered in input mode, this function
 debounces and executes any required action for an Alt I/O line.
 
 @param p_which
 The I/O line that triggered, 0 or 1.
 
 @author
 C. A. Church
 */
 
void altHandler(byte p_which) {
  
	// Debounce the trigger signal to avoid rapid-fire function execution
	if((millis() - trigLast) >= ALT_TRIG_THRESH ) {
    
		// Reset the debounce time counter
		trigLast = millis();
		
		/** Check to see with alt state is active and execute the appropriate function **/
		
		// "Start"
		if( alt_inputs[p_which] == ALT_START) 
			startProgram();
		
		// "Stop Motors"
		else if( alt_inputs[p_which] == ALT_STOP_MOTORS && running == true){
			for(byte i = 0; i < MOTOR_COUNT; i++ ){
			motorStopThis(i);
			// disable motor
			motors[i].flags &= ~MOTOR_UEN_FLAG;
			motor_running = false;
			OMEEPROM::write(EE_M0FLAG + (EE_MOTOR_SPACE * i), motors[i].flags);
			}
		}
		
		// "Stop"
		else if( alt_inputs[p_which] == ALT_STOP)
			stopProgram(false);
		
		// "Toggle"
		else if( alt_inputs[p_which] == ALT_TOGGLE) {
			if( running )
				stopProgram(false);
			else
				startProgram();
		}

		// "Ext. Interval"
		else if( alt_inputs[p_which] == ALT_EXTINT ) {
				// set camera ok to fire
			alt_force_shot = true;
				// do not clear the state, as we may be in the middle of a move
				// when a trigger is received! (or some other activity, for that matter)
			// Engine.state(ST_CLEAR);
		}

		// "Direction Flip"
		else if (alt_inputs[p_which] == ALT_DIR)
			motorDirFlip();

		// "Pause Program"
		else if (alt_inputs[p_which] == ALT_PAUSE_PRGM){
			if (running)
				pauseProgram();
			else
				startProgram();
		}
        
	} //end if millis...
}

/** Handler for ISR One */
      
void altISROne() {
  altHandler(0);
}

/** Handler for ISR Two */
void altISRTwo() {
  altHandler(1);
}

/** Handler for ISR One */
      
void altISRThree() {
  altHandler(2);
}

/** Handler for ISR Two */
void altISRFour() {
  altHandler(3);
}

/** Connect (or Disconnect) an Alt I/O Line

 This function attches or detaches the required interrupt
 given an I/O line and a mode.
 
 @param p_which
 Which I/O, 0 or 1
 
 @param p_mode
 A valid altMode 
 
 @author
 C. A. Church
 */

void altConnect(byte p_which, byte p_mode) {
  
  //alt_inputs[p_which] = p_mode;


    if( p_which == AuxARing) {
        detachInterrupt(0);//, altISRTwo, alt_direction);
    } else if ( p_which == AuxATip) {
        detachInterrupt(1);//, altISROne, alt_direction);
    } else if ( p_which == AuxBTip) {
        detachInterrupt(6);//, altISRThree, alt_direction);
    } else {
        detachInterrupt(7);//, altISRFour, alt_direction);
    }

    // disable the input?
 
  if( p_mode == ALT_OFF ) {
    pinMode(ALT_START_PIN + p_which, INPUT);
    //digitalWrite(ALT_START_PIN + p_which, ! alt_out_trig);
    alt_out_flags &= ~( ALT_OUT_FLAG_A << p_which );
    alt_out_flags &= ~( ALT_OUT_FLAG_B << p_which );
    return;
  }
  
    // only attach interrupts for input modes
  if( p_mode != ALT_OUT_B && p_mode != ALT_OUT_A ) {
      // set pin as input
    pinMode(ALT_START_PIN + p_which, INPUT);
      // enable pull-up resistor
    digitalWrite(ALT_START_PIN + p_which, HIGH);
  
      // regarding 6 and 7 below - don't ask me.. ask who ever did that wierd order in WInterrupts.c
    switch( p_which ) {
      case AuxARing: 
        attachInterrupt(0, altISRTwo, alt_direction);
        break;
      case AuxATip:
        attachInterrupt(1, altISROne, alt_direction);
        break;
      case AuxBTip: 
        attachInterrupt(6, altISRThree, alt_direction);
        break;
      case AuxBRing:
        attachInterrupt(7, altISRFour, alt_direction);
        break;
    }
  } else {
    
      // it's an output mode
    
      // note that alt 3 is on a different register..
    byte aPin = p_which <= 2 ? ALT_START_PIN + p_which : ALT_PIN_3;
    
    pinMode(aPin, OUTPUT);
    digitalWrite(aPin, ! alt_out_trig);
    
    if( p_mode == ALT_OUT_B ) {
      alt_out_flags |= ( ALT_OUT_FLAG_B << p_which );
      alt_out_flags &= ~( ALT_OUT_FLAG_A << p_which );
    }
    else {
      alt_out_flags &= ~( ALT_OUT_FLAG_B << p_which );
      alt_out_flags |= ( ALT_OUT_FLAG_A << p_which );
    }
    
  }
 
  
}    
  
 
/** Trigger Outputs

 Triggers all configured outputs for the given mode, for the given length
 of time.  Blocks state engine while output is triggered, and registers callback
 to bring output down.
 
 @param p_mode
 Either ALT_TRIG_A or ALT_TRIG_B, specifying after or before types
 
 @author
 C. A. Church
 */
 
void altOutStart(byte p_mode) {
  
  boolean altStarted = false;
  
  byte           flag = p_mode == ALT_TRIG_B ? ALT_OUT_FLAG_B : ALT_OUT_FLAG_A;
  unsigned int adelay = p_mode == ALT_TRIG_B ? alt_before_ms  : alt_after_ms;
  
  for(byte i = 0; i < 4; i++) {
       if( alt_out_flags & ( flag << i ) ) {
         
           // note that alt 3 is on a different register..
          if( i <= 2 )
            digitalWrite(ALT_START_PIN + i, alt_out_trig); 
          else
            digitalWrite(ALT_PIN_3, alt_out_trig);
            
          altStarted = true;
       }
  }
    
  if( altStarted ) {
    MsTimer2::set(adelay, altOutStop);
    MsTimer2::start();
    Engine.state(ST_BLOCK);
  }
  else if( p_mode == ALT_TRIG_B ) {
    Engine.state(ST_CLEAR);
  }
  else {
      // ok to move
    Engine.state(ST_MOVE);
  }
  
}

/** Bring Down any Triggered Outputs 

 @author 
 C. A. Church
 */
 
void altOutStop() {
 
  MsTimer2::stop();
  
  byte flag = alt_block == ALT_BLOCK_B ? ALT_OUT_FLAG_B : ALT_OUT_FLAG_A; 
   
  for(byte i = 0; i < 4; i++) {
       if( alt_out_flags & ( flag << i ) ) {
           // note that alt 3 is on a different register..
          if( i <= 2 )
            digitalWrite(ALT_START_PIN + i, ! alt_out_trig); 
          else
            digitalWrite(ALT_PIN_3, ! alt_out_trig);
       }
  }
  
  // set correct state to either clear to fire, or clear to move
  
  if( alt_block == ALT_BLOCK_B )
  {
    Engine.state(ST_CLEAR);
  }
  else
  {
    Engine.state(ST_MOVE);
  }
    
}

/** Compares the Alt states each cycle to see if they've changed.

If they've changed it updates the old array to match the new array and returns false.

 @author 
 Kevin Melotti
 */


bool altArraysCompare()
{
  bool equal = true;
  for (int i = 0; i < sizeof(alt_inputs); i++)
  {
   if (alt_inputs[i] != alt_inputs_old[i]){
    equal = false;
    alt_inputs_old[i] = alt_inputs[i]; 
   }
  }
  
  if (alt_before_delay != alt_before_delay_old){
    equal = false;
    alt_before_delay_old = alt_before_delay;
  }
  if (alt_after_delay != alt_after_delay_old){
    equal = false;
    alt_after_delay_old = alt_after_delay;
  }
  if (alt_before_ms != alt_before_ms_old){
    equal = false;
    alt_before_ms_old = alt_before_ms;
  }
  if (alt_after_ms != alt_after_ms_old){
    equal = false;
    alt_after_ms_old = alt_after_ms;
  }
  if (alt_force_shot != alt_force_shot_old){
    equal = false;
    alt_force_shot_old = alt_force_shot;
  }
  if (alt_ext_int != alt_ext_int_old){
    equal = false;
    alt_ext_int_old = alt_ext_int;
  }
  if (alt_direction != alt_direction_old){
    equal = false;
    alt_direction_old = alt_direction;
  }
  if (alt_out_trig != alt_out_trig_old){
    equal = false;
    alt_out_trig_old = alt_out_trig;
  }
  
  return equal;
}


