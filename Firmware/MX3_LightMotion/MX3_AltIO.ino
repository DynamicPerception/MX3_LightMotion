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


byte alt_inputs[] = { ALT_OFF, ALT_OFF };


boolean alt_force_shot = false;
int      alt_direction = FALLING;


 /** Alt I/O Setup 
 */
 
void altSetup() {
  altConnect(0,  alt_inputs[0]);
  altConnect(1,  alt_inputs[1]); 
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
  
    // set pin as input
  pinMode(ALT_START_PIN + p_which, INPUT);
    // enable pull-up resistor
  digitalWrite(ALT_START_PIN + p_which, HIGH);
  
  if( p_which )
    attachInterrupt(1, altISRTwo, alt_direction);
  else 
    attachInterrupt(0, altISROne, alt_direction);
 
  
}    
  
