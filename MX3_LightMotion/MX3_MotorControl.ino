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


/** Set up and Initialize Motors

 @author
 C. A. Church
 */
 
void motorSetup() {
 
  for(byte i =0; i < (MOTOR_COUNT * 2); i++ ) {
    pinMode(MOTOR_DIR_PINSTART + i, OUTPUT);
      // set to 10% speed

  }
  
  for(byte i = 0; i < MOTOR_COUNT; i++ ) {
    motorDir(i, 0);
    motorSpeed(i, 0.1);
  }
  
  pinMode(MOTOR_PWM_0, OUTPUT);
  pinMode(MOTOR_PWM_1, OUTPUT);
  pinMode(MOTOR_PWM_2, OUTPUT);
  
  
}

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
  
  if( p_rel >= 1.0 ) {
      motors[p_motor].onTimePeriods = MOTOR_PWM_MAXPERIOD;
  }
  else {
      float offTime       = (1.0 - p_rel) * MOTOR_PWM_MAXPERIOD;
      float onTime        = MOTOR_PWM_MAXPERIOD - offTime;
      float onTimePeriods = onTime / offTime;
      motors[p_motor].onTimePeriods = onTimePeriods;
  }

  motors[p_motor].speed         = p_rel;

}

/** Get Speed Percent for a Given Motor

 Returns the currently set speed of the motor, in percentage from 0.0 to 1.0
 
 @param p_motor
 The motor to get the speed for, 0, 1, or 2
 
 @return
 A floating point number, from 0.0 to 1.0
 
 @author
 C. A. Church
 */
  
float motorSpeed(byte p_motor) {
  return( motors[p_motor].speed );
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

/** Set Speed as Output Ratio for a Given Motor

 Sets the current speed for the motor, expressed as the output
 ratio (RPM * Ratio * Speed) for linear, and degrees (RPM * Ratio * Speed * 360).
 
 @param p_motor
 The motor to get the speed for, 0, 1, or 2
 
 @param p_ratio
 A floating point integer, from 0.0 to maximum possible travel
 */
 
void motorSpeedRatio(byte p_motor, float p_ratio) {
  float spd = motors[p_motor].rpm * motors[p_motor].ratio;
  
  if( motors[p_motor].flags & MOTOR_ROT_FLAG )
      spd *= 360.0;
     
  spd = p_ratio > spd ? 1.0 : p_ratio / spd;
  
  motorSpeed(p_motor, spd);
}


/** Get Maximum Possible Speed as Output Ratio for a Given Motor

 Gets the current max speed for the motor, expressed as the output
 ratio (RPM * Ratio * Speed) for linear, and degrees (RPM * Ratio * Speed * 360).
 
 @param p_motor
 The motor to get the speed for, 0, 1, or 2
 
 @return p_ratio
 A floating point integer
 */
 
float motorMaxSpeedRatio(byte p_motor) {
  float spd = motors[p_motor].rpm * motors[p_motor].ratio;
  
  if( motors[p_motor].flags & MOTOR_ROT_FLAG )
      spd *= 360.0;
      
  return spd;
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
    motorDirFlip(i);
}

/** Flip Motor Direction

 Inverts the direction of a single motor.
 
 @param p_motor
 Motor to flip direction
 
 @author
 C. A. Church
 */
 
void motorDirFlip(byte p_motor) {
  motorDir(p_motor, ! (boolean) (motors[p_motor].flags & MOTOR_CDIR_FLAG) );
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
  
  if( motor_running )
    return;
    
    // enable each motor, if it does not have a lead-in/out
  for(byte i = 0; i < MOTOR_COUNT; i++ ) 
    if( ! motors[i].lead )
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
  
    // force stop on all motors
  for(byte i = 0; i < MOTOR_COUNT; i++ ) {
    motors[i].flags &= (B11111111 ^ MOTOR_ENABLE_FLAG);
    MOTOR_DRV_PREG  &= (B11111111 ^ (1 << (MOTOR_DRV_FMASK + i)));
  }
  
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
 volatile static byte startCount = 0;
 volatile static byte ranCount   = 0;
 
    // check status of each motor


 
 for(byte i = 0; i <= MOTOR_COUNT; i++) {
   if( motors[i].flags & MOTOR_ENABLE_FLAG && motors[i].flags & MOTOR_UEN_FLAG ) {
       // motor is enabled
       
   
     if( ! (motors[i].flags & MOTOR_HIGH_FLAG) ) {
           // motor is currently not moving
           
          
       bool goHigh = false;
              
       if( motors[i].onTimePeriods >= 1.0 ) {
           // we are 1 off period for every X on periods
          goHigh = true;
       }
       else {
         motors[i].offError += motors[i].onTimePeriods;
       }


       if( motors[i].offError >= 1.0 ) {
           // we are off more often than on, and we've been off long enough
         goHigh = true;
         motors[i].offError -= 1.0;
       }
       
       if( goHigh ) {
               // going up, enable output pin

          MOTOR_DRV_PREG  |= (1 << (MOTOR_DRV_FMASK + i));
          motors[i].flags |= MOTOR_HIGH_FLAG;                  
                   
          if( motor_runOnce )
            startCount++;
       }

       
     }
     else {
       
                // motor is currently moving
                
         boolean goLow = false;
         
         if( motors[i].onTimePeriods > 0 ) {
             // we are on more often than off
             
           if( motors[i].restPeriods >= motors[i].onTimePeriods ) {
               // have gone enough periods.
             if( motors[i].onError >= 1.0 ) {
                 // but wait!  We've accumulated enough on error to wait one more cycle
               motors[i].onError -= 1.0;
               continue;
             }
             else {
                 // not enough on error to wait one more cycle
               goLow = true;
             }
           } // end if( ... restPeriods
         } // end if onTimePeriods
         else {
           goLow = true;
         }
         
         if( goLow ) {
           
           // going down, disable output pin
            MOTOR_DRV_PREG  &= (B11111111 ^ (1 << (MOTOR_DRV_FMASK + i)));
            motors[i].flags &= (B11111111 ^ MOTOR_HIGH_FLAG);
              // accumulate off-period error for one period
            motors[i].restPeriods = 0;
            motors[i].onError += motors[i].onTimePeriods - (unsigned long) motors[i].onTimePeriods;
         }
         else {
           motors[i].restPeriods++;
         }
             
       
     } //end else (motor currently high)
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


/** Set Motor Enable Based on Lead-in
 
   Sets motor program-enable flag based on the lead-in amount for
   the motor, and the current shot count
   
   @author
   C. A. Church
   */
   
void motorCheckLead() {
     for( byte i = 0; i < MOTOR_COUNT; i++ ) {
      if( motors[i].lead > 0 && ( camera_fired < motors[i].lead || camera_fired > (camera_max_shots - motors[i].lead) ) )
        motors[i].flags &= (B11111111 ^ MOTOR_ENABLE_FLAG);
      else if( motors[i].lead > 0 && ( camera_fired > motors[i].lead || camera_fired < (camera_max_shots - motors[i].lead) ) )
        motors[i].flags |= MOTOR_ENABLE_FLAG;
    } 
}

