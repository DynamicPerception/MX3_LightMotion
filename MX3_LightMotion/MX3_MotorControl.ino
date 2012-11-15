/*


MX3 LightMotion

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
  Motor control functions
  ========================================
  
*/






 // predeclare this function to provide a default in the prototype
 
void run_motors(boolean p_once = false);


 // control variables
boolean motor_runOnce = false;

 // create an array of structures for our three motors

MotorDefinition motors[] = { MotorDefinition(), MotorDefinition(), MotorDefinition() };



/** Set Speed Percent for a Given Motor
 
 Sets speed for a given motor as a percentage of time, per minute, that the motor
 will be on.
 
 @param p_motor
 The motor to set the speed for, 0, 1, or 2
 
 @param p_rel
 The relative ontime, expressed as a value from 0.0 to 1.0
 
 @author 
 C. A. Church
 */
 
void motorSpeed(byte p_motor, float p_rel) {
 
    // periods off between each on period to achieve percentage of time moving per minute
  float offTime = MOTOR_PWM_MAXPERIOD / ((1.0 - p_rel) * MOTOR_PWM_MAXPERIOD);
  motors[p_motor].offPeriods = (unsigned long) offTime;
  motors[p_motor].offError = offTime - ( (unsigned long) offTime );
  
}

/** Get Speed Percent for a Given Motor

 Returns the currently set speed of the motor, in percentage from 0.0 to 1.0
 
 @param p_motor
 The motor to get the speed for, 0, 1, or 2
 
 @return
 A floating point integer, from 0.0 to 1.0
 
 @author
 C. A. Church
 */
  
float motorSpeed(byte p_motor) {
 
 float offTime = (float) motors[p_motor].offPeriods + motors[p_motor].offError;
 return( (offTime * MOTOR_PWM_MAXPERIOD) / MOTOR_PWM_MAXPERIOD );  
 
}

/** Get Speed as Output Ratio for a Given Motor

 Returns the currently set speed for the motor, expressed as the output
 ratio (RPM * Ratio * Speed) for linear, and degrees (RPM * Ratio * Speed * 360).
 
 @param p_motor
 The motor to get the speed for, 0, 1, or 2
 
 @return
 A floating point integer, from 0.0 to maximum possible travel
 */
 
float motorSpeedRatio(byte p_motor) {
  float spdPct = motorSpeed(p_motor);
  float spd = motors[p_motor].rpm * motors[p_motor].ratio * spdPct;
  
  if( motors[p_motor].flags & MOTOR_ROT_FLAG )
    spd *= 360.0;
    
  return( spd );
}

/** Control Motor Direction
  
  @param p_motor
  Motor (0, 1, or 2)
  
  @param p_dir
  Direction (true or false)
  
  We use an efficient method here, that relies upon the motor dir pins 
  to be in sequence, e.g.:
  
  M1A = 7
  M1B = 8
  M2A = 9
  M2B = 10
  ...
  
  In this fashion, we are able to quickly and with little effort
  switch the directions by not having to determine what pins to
  use through a series of if's.
  
  @author
  C. A. Church
  */
  
void motorDir(byte p_motor, boolean p_dir) {
  
    // determine which direction to go based on input and
    // direction flip
  byte mask = motors[p_motor].flags & MOTOR_DIR_FLAG ? LOW : HIGH;
 
    
    // swap mask based on direction, and record what direction
    // the user asked for
  if( p_dir ) {
    mask = !mask;
    motors[p_motor].flags |= MOTOR_CDIR_FLAG;
  }
  else {
    motors[p_motor].flags &= (B11111111 ^ MOTOR_CDIR_FLAG);
  }
    
  digitalWrite(MOTOR_DIR_PINSTART + (p_motor * 2), mask);
  digitalWrite(MOTOR_DIR_PINSTART + 1 + (p_motor * 2), !mask);
}

/** Flip All Motor Directions

 Inverts the direction of all motors.
 
 @author
 C. A. Church
 */
 
void motorDirFlip() {
    // foreach motor, invert its direction
  for(int i = 0; i < MOTOR_COUNT; i++)
    motorDir(i, ! (boolean) (motors[i].flags & MOTOR_CDIR_FLAG) );
}

/** Start All Motors Running

 Causes all user-enabled motors to begin running. If motors are not
 already running, will start required timer1 and attach interrupt.
 
 @param p_once
 Run motors once (i.e. shoot-move-shoot)?  defaults to false
 
 @author 
 C. A. Church
 */
 
void motorRun(boolean p_once) {
  
  motor_runOnce = p_once;
  
  for(byte i = 0; i < MOTOR_COUNT; i++ ) 
    motors[i].flags |= MOTOR_ENABLE_FLAG;
    
  motorStartISR();
  
}

/** Start One Motor Running

 Initializes timer, if not already initialized, and sets one motor 
 to be program-enabled, allowing it to run.  
 
 You may call this multiple times to start multiple individual motors.
 
 @author
 C. A. Church
 */
 
void motorRun(boolean p_once, byte p_motor) {
 
 motor_runOnce = p_once;
 
 motors[p_motor].flags |= MOTOR_ENABLE_FLAG;
 
 motorStartISR();

}

/** Stop All Motors Running

 Stops execution of the timer for all motor movement, and sets all
 motors to be program-disabled, allowing a subsequent start one motor
 command to start only that motor.
 
 
 @author 
 C. A. Church
 */
 
void motorStop() {
  motor_running = false;
  Timer1.detachInterrupt();
  
  for(byte i = 0; i < MOTOR_COUNT; i++ ) 
    motors[i].flags &= (B11111111 ^ MOTOR_ENABLE_FLAG);
    
}

/** Start Motor Driving Interrupt Service 

 Starts interrupt, sets data to indicate motors are running
 
 @author
 C. A. Church
 */
 
void motorStartISR() {
  
  if( ! motor_running ) { 
      motor_running = true;
      Timer1.initialize(MOTOR_PWM_MINPERIOD);
      Timer1.attachInterrupt(motorRunISR);
  }
  
}

/** Motor Driving Interrupt Service Routine

 This routine is run via an interrupt on a mS-level basis (timer1, 4, or 2).
 
 We effectively create three independent channels of software PWM, giving us full
 control over the the pulse width and the duty cycle.
 
 This is a simple time-proportioning algorithm, with each channel sharing
 a starting sample edge.
 
 It controls the on/off state of three motors, based on the contents
 of the MotorState structure for each of the three motors.
 
 Each motor has two flags:
 
  - motor enabled
  - motor drive pin currently held high
  
 Additionally, we record two required pieces of information:
 
  - onPeriods: on pulse periods
  - offPeriods: off pulse periods

 We store back into the structure the current period counter.
 
 Motor PWM input pins are REQUIRED to be sequential on the SAME pin register!
 
 @author 
 C. A. Church
 
 */

void motorRunISR() {
  
    // track for run once (SMS) operation
 static byte startCount = 0;
 static byte ranCount   = 0;
 
    // check status of each motor

 
 for(byte i = 0; i <= MOTOR_COUNT; i++) {
   if( motors[i].flags & ( MOTOR_ENABLE_FLAG | MOTOR_UEN_FLAG ) ) {
       // motor is enabled
       
       // determine which direction we go (high or low)
     byte dir = true;
     unsigned long rest = motors[i].offPeriods;
     
     if( motors[i].flags & MOTOR_HIGH_FLAG ) {
       dir = false;
       rest = motors[i].onPeriods;
     }
     
       // if we've exceeded required waiting periods...
       
     if( motors[i].restPeriods >= rest ) {
        
         // correct for accumulated error in off-time, if overflowed
        if( motors[i].error > 1.0 ) {
          motors[i].error -= 1.0;
            // skip, don't take an action here.
          continue;
        }
        
        if( dir ) {
            // going up, enable output pin
          MOTOR_DRV_PREG  |= (1 << (MOTOR_DRV_FMASK + i));
          motors[i].flags |= MOTOR_HIGH_FLAG;
          
          if( motor_runOnce )
            startCount++;
            
        }
        else {
            // going down, disable output pin
          MOTOR_DRV_PREG  &= (B11111111 ^ (1 << (MOTOR_DRV_FMASK + i)));
          motors[i].flags &= (B11111111 ^ MOTOR_HIGH_FLAG);
            // accumulate off-period error for one period
          motors[i].error += motors[i].offError;
          
            // add distance moved...
          motors[i].distance += ( motors[i].flags & MOTOR_CDIR_FLAG ) ? 1 : -1;
          
          if( motor_runOnce )
            ranCount++;
            
        }
        
                 // at the next run, we will have already waited one period...
        motors[i].restPeriods = 1;

     }
     else {
         // not enough periods past, advance
       motors[i].restPeriods++;
       

     }
     
   } // end if motor enabled
 } // end for...
 
   // if we're supposed to run our motors just once,
   // then check to see if we're done, and stop
   // the interrupt
   
 if( motor_runOnce && startCount == ranCount ) {
     startCount = 0;
     ranCount   = 0;
     motorStop();
 }
  
}



