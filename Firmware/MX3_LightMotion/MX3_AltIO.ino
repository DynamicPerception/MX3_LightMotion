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
byte                alt_block = 0;
byte            alt_direction = FALLING;


 /** Alt I/O Setup 
 */
 
void altSetup() {
  altConnect(0,  alt_inputs[0]);
  altConnect(1,  alt_inputs[1]); 
  altConnect(2,  alt_inputs[2]);
  altConnect(3,  alt_inputs[3]);
  
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
 
  volatile static unsigned long trigLast = millis();
  
  if( millis() - trigLast > ALT_TRIG_THRESH ) {
    
    trigLast = millis();
    
    if( alt_inputs[p_which] == ALT_START) 
        startProgram();
    else if( alt_inputs[p_which] == ALT_STOP)
        stopProgram();
    else if( alt_inputs[p_which] == ALT_TOGGLE) {
        if( running )
          stopProgram();
        else
          startProgram();
    }
    else if( alt_inputs[p_which] == ALT_EXTINT ) {
          // set camera ok to fire
        alt_force_shot = true;
        Engine.state(ST_CLEAR);
    }
    else if( alt_inputs[p_which] == ALT_DIR)
        motorDirFlip();
        
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
  
  alt_inputs[p_which] = p_mode;


    // disable the input?
 
  if( p_mode == ALT_OFF ) {
      detachInterrupt(p_which);
      digitalWrite(ALT_START_PIN + p_which, LOW);
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
      case 0: 
        attachInterrupt(0, altISROne, alt_direction);
        break;
      case 1:
        attachInterrupt(1, altISRTwo, alt_direction);
        break;
      case 2: 
        attachInterrupt(6, altISRThree, alt_direction);
        break;
      case 3:
        attachInterrupt(7, altISRFour, alt_direction);
        break;
    }
  }
  
  else {
    
      // it's an output mode
      
    digitalWrite(ALT_START_PIN + p_which, LOW);
    pinMode(ALT_START_PIN + p_which, OUTPUT);
    
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
          digitalWrite(i, HIGH); 
          altStarted = true;
       }
  }
    
  if( altStarted == true ) {
    MsTimer2::set(adelay, altOutStop);
    MsTimer2::start();
    Engine.state(ST_BLOCK);
  }
  
}

/** Bring Down any Triggered Outputs 

 @author 
 C. A. Church
 */
 
void altOutStop() {
 
   byte flag = alt_block == ALT_BLOCK_B ? ALT_OUT_FLAG_B : ALT_OUT_FLAG_A; 
   
   for(byte i = 0; i < 4; i++) {
       if( alt_out_flags & ( flag << i ) ) {
          digitalWrite(i, LOW); 
       }
  }
  
  if( alt_block == ALT_BLOCK_B ) 
    Engine.state(ST_CLEAR);
  else
    Engine.state(ST_MOVE);
    
}
