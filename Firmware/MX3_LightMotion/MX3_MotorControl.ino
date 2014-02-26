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
  Motor control functions
  ========================================
  
*/



 // predeclare these functions to provide a default in the prototype

void motorStop(boolean p_once = false);
void motorSpeed(byte p_motor, float p_rel, boolean p_ramp = false);
void motorSpeedRatio(byte p_motor, float p_ratio, boolean p_ramp = false);
void run_motors(boolean p_once = false);


 // control variables
 
   // minimum pwm timing period is 100 uS
unsigned int motor_pwm_minperiod  = 600;
  // maximum number of periods per minute
float motor_pwm_maxperiod  = ( 60000000.0 / (float) motor_pwm_minperiod );

int motor_dir_delay = 500;

boolean motor_flushSMS = false;

//Manaual move setting - 0 for hold to move, 1 for select to stop
byte mmSetting = 0;

//increment for the motor speed in continous mode 
float motorIncrement = 0.01;

//SMS on period ration - determines the speed of the motor in SMS mode, periodsOn/periodsOff
float smsOnPeriodRatio = 4;

 // create an array of structures for our three motors

//MotorDefinition motors[] = { MotorDefinition(), MotorDefinition(), MotorDefinition() };

 // Motor Presets
 
MotorPreset motorPresets[] = { 
                                {{"Custom"}, 1.0, 1.0, 0},
                                {{"Dolly 1.56"}, 1.56, 3.229, 0},
                                {{"Pan/Tilt 1.56"}, 1.56, 10.0, 1},
                                {{"Dolly 8.23"}, 8.23, 3.229, 0},
                                {{"Pan/Tilt 8.23"}, 8.23, 10.0, 1},
                                {{"Dolly 21.58"}, 21.58, 3.229, 0},
                                {{"Pan/Tilt 21.58"}, 21.58, 10.0, 1},
                                {{"Dolly 42.13"}, 42.13, 3.229, 0},
                                {{"Pan/Tilt 42.13"}, 42.13, 10.0, 1}
};
                                
byte motorPresetSelected[] = { 0, 0, 0 };

/** Set up and Initialize Motors

 @author
 C. A. Church
 */
 
void motorSetup() {
 
  for(byte i = 0; i < (MOTOR_COUNT * 3); i++ ) {
    pinMode(MOTOR_DIR_PINSTART + i, OUTPUT);
  }
  
  for(byte i = 0; i < MOTOR_COUNT; i++ ) {
          // set to 10% speed as default
    motorSpeed(i, motors[i].speed);
  }
 
   digitalWrite(MOTOR_INH_0, HIGH);
   digitalWrite(MOTOR_INH_1, HIGH);
   digitalWrite(MOTOR_INH_2, HIGH);
   
  
}

/** Set Speed Percent for a Given Motor
 
 Sets speed for a given motor as a percentage of time, per minute, that the motor
 will be on.
 
 @param p_motor
 The motor to set the speed for, 0, 1, or 2
 
 @param p_rel
 The relative ontime, expressed as a value from 0.0 to 1.0
 
 @param p_ramp
 (Optional, default = false) Whether or not this speed change is part of a ramp cycle
 
 @author 
 C. A. Church
 */
 
void motorSpeed(byte p_motor, float p_rel, boolean p_ramp) {
 
    // periods off between each on period to achieve percentage of time moving per minute
  
  motor_pwm_maxperiod  = ( 60000000.0 / (float) motor_pwm_minperiod );
  
  if( p_rel <= 0.0 ) {
      motors[p_motor].onTimePeriods = 0.0;
  }
  else if( p_rel >= 1.0 ) {
      motors[p_motor].onTimePeriods = motor_pwm_maxperiod;
  }
  else {
      float offTime       = (1.0 - p_rel) * motor_pwm_maxperiod;
      float onTime        = motor_pwm_maxperiod - offTime;
      float onTimePeriods = onTime / offTime;
      motors[p_motor].onTimePeriods = onTimePeriods;
  }

  motors[p_motor].speed         = p_rel;
  
  if( ! p_ramp ) 
    motors[p_motor].setSpeed    = p_rel;
    

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
 
  @author 
 C. A. Church
 */
 
float motorSpeedRatio(byte p_motor) {
  float spdPct = motorSpeed(p_motor);
  
    // simple equation 25 - we need to deal with the fact
    // that the DC motors are not linear.  We use a curve-fit function
    // to extrapolate percentage speed drive to actual movement ratio

  double weighted = MOTOR_SCA / ( MOTOR_SCB + pow(spdPct, MOTOR_SCC) );
  
  float spd = motors[p_motor].rpm;

  
    // handle rotary different than linear
  if( motors[p_motor].flags & MOTOR_ROT_FLAG )
    spd = ( spd * 360.0 / motors[p_motor].ratio ) * weighted;
  else
    spd = spd * motors[p_motor].ratio * weighted;
 
  return( spd );
}

/** Set Speed as Output Ratio for a Given Motor

 Sets the current speed for the motor, expressed as the output
 ratio (RPM * Ratio * Speed) for linear, and degrees (RPM * Ratio * Speed * 360) for
 rotary.
 
 @param p_motor
 The motor to get the speed for, 0, 1, or 2
 
 @param p_ratio
 A floating point integer, from 0.0 to maximum possible travel
 
 @param p_ramp
 (Optional, default = false) Whether or not this speed change is part of a ramp cycle
 
  @author 
 C. A. Church
 */
 
void motorSpeedRatio(byte p_motor, float p_ratio, boolean p_ramp) {
  float spd = motors[p_motor].rpm * motors[p_motor].ratio;
  
  if( motors[p_motor].flags & MOTOR_ROT_FLAG )
      spd *= 360.0;
     
  spd = p_ratio > spd ? 1.0 : p_ratio / spd;
  
  motorSpeed(p_motor, spd, p_ramp);
}


/** Get Maximum Possible Speed as Output Ratio for a Given Motor

 Gets the current max speed for the motor, expressed as the output
 ratio (RPM * Ratio * Speed) for linear, and degrees (RPM * Ratio * Speed * 360).
 
 @param p_motor
 The motor to get the speed for, 0, 1, or 2
 
 @return p_ratio
 A floating point integer
 
  @author 
 C. A. Church
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
  
  @author
  C. A. Church
  */
  
void motorDir(byte p_motor, boolean p_dir) {
  
   bool flag_on = false;

  //disable enable flag while switching directions
  if(motors[p_motor].flags & MOTOR_ENABLE_FLAG){
    flag_on = true;
    motors[p_motor].flags &= ~MOTOR_ENABLE_FLAG;
  }
  
  
  //Turns off the motor diver so that it won't trip the battery when switching directions
  if (p_motor == 0)
    MOTOR_DRV_PREG &= ~(B00000001 << 2);
  else if (p_motor == 1)
    MOTOR_DRV_PREG &= ~(B00000001 << 5);
  else 
    MOTOR_DRV_PREG2 &= ~(B00000001);             
  
  //turns off driver
  MOTOR_DRV_PREG  &= ~(B00000011 << (p_motor*3));
    
  //Set direction  
  if (p_dir)
    motors[p_motor].flags |= (MOTOR_CDIR_FLAG); 
  else
    motors[p_motor].flags &= ~(MOTOR_CDIR_FLAG);  
        
  //delays motor to prevent battery tripping
  delayMicroseconds(65534);
  delayMicroseconds(65534);
  
  //Turns the motor driver back on
  if (p_motor == 0)
    MOTOR_DRV_PREG |= (B00000001 << 2);
  else if (p_motor == 1)
    MOTOR_DRV_PREG |= (B00000001 << 5);
  else 
    MOTOR_DRV_PREG2 |= (B00000001); 
  
  //tursn the flag back on if it was on  
  if(flag_on){
    motors[p_motor].flags |= MOTOR_ENABLE_FLAG;
  }
  
}


/** Flip All Motor Directions

 Inverts the direction of all motors.
 
 @author
 C. A. Church
 */
 
void motorDirFlip() {
    // foreach motor, invert its direction
    
  for(int i = 0; i < MOTOR_COUNT; i++)
  {
    motorDirFlip(i);
    delay(20);
  }
    
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
 
  if( motor_running )
    return;
    
    // (program)-enable each motor, if it does not have a lead-in/out
  for(byte i = 0; i < MOTOR_COUNT; i++ ) 
    if( ! motors[i].lead )
      motors[i].flags |= MOTOR_ENABLE_FLAG;
    
  motorStartISR(p_once);
  
}

/** Start One Motor Running

 Initializes timer, if not already initialized, and sets one motor 
 to be program-enabled, allowing it to run.  
 
 You may call this multiple times to start multiple individual motors.
 
 @author
 C. A. Church
 */
 
void motorRun(boolean p_once, byte p_motor) {
 
 motors[p_motor].flags |= MOTOR_ENABLE_FLAG;
 
 if( ! running )
   motorStartISR(p_once);

}

/** Stop All Motors Running

 Stops execution of the timer for all motor movement, and sets all
 motors to be program-disabled, allowing a subsequent start one motor
 command to start only that motor.
 
 
 @author 
 C. A. Church
 */
 
void motorStop(boolean p_once) {
  motor_running = false;
  Timer1.detachInterrupt();
  
  
  if( ! p_once ) {
      // force stop on all motors (only do this UI inputs or continuous motion)
      // SMS ISR routine should not perform this activity
    for(byte i = 0; i < MOTOR_COUNT; i++ )
      motorStopThis(i);
  }
  
}


/** Stop this Motor

 Performs the activities for stopping one motor.
 
 */
 
void motorStopThis(byte p_motor) {
         // disable motor, set high flag to low, and disable force ramp flag
     motors[p_motor].flags &= (~( MOTOR_ENABLE_FLAG | MOTOR_HIGH_FLAG | MOTOR_RAMP_FLAG ) );
        // change output pin state
     MOTOR_DRV_PREG  &= (~(B00000011 << (p_motor*3)));
        // if speed != setspeed, set new speed back!
     motorSpeed(p_motor, motors[p_motor].setSpeed);
        // get rid of forced ramp
     motors[p_motor].forceRampStart = 0; 
         // clear ramp flag
     motors[p_motor].inRamp = 0;
}

/** Start Motor Driving Interrupt Service 

 Starts interrupt, sets data to indicate motors are running
 
 @param p_once
   Whether shooting continuous (false) or shootmoveshoot (true)
 @author
 C. A. Church
 */
 
void motorStartISR(boolean p_once) {
  
  motor_pwm_maxperiod  = ( 60000000.0 / (float) motor_pwm_minperiod );
  
  if( ! motor_running ) { 
      motor_running = true;
      Timer1.initialize(motor_pwm_minperiod);
      
      if( p_once ) {
        motor_flushSMS = true;
        Timer1.attachInterrupt(motorRunISRSMS);
      }
      else
        Timer1.attachInterrupt(motorRunISR);
  }
  
}


/** Motor Driving Interrupt Service Routine (SMS)

   This routine moves each motor for the time period specified:
   
     motor_pwm_maxperiod * motor[each].speed
     
   It will stop the motors once each motor that must move completes the move.
   
   @author
   C. A. Church
*/

void motorRunISRSMS() {
  
  static byte moveCnt;
  static byte moved;
   
    // we've got to be careful when stopping during a move
  if( motor_flushSMS ) {   //1
    moveCnt = 0;      //2
    moved   = 0;       //2
    motor_flushSMS = false;  //2
  }
  
  
  
  for( byte i = 0; i < MOTOR_COUNT; i++ ) {  //5
    if( motors[i].flags & MOTOR_ENABLE_FLAG && motors[i].flags & MOTOR_UEN_FLAG ) {   //6
       // motor is enabled
       if( !(motors[i].flags & MOTOR_HIGH_FLAG) && motors[i].speed > 0.0) {      //8
         // motor is not currently moving
         if( motors[i].flags & MOTOR_DIR_FLAG ){
           MOTOR_DRV_PREG  |= (B00000001 << (i*3 + (!((motors[i].flags & MOTOR_CDIR_FLAG) >> 2))));
         } else {
           MOTOR_DRV_PREG  |= (B00000001 << (i*3 + (((motors[i].flags & MOTOR_CDIR_FLAG) >> 2)))); 
         }
         //pin register - sets the appropriate pin to drive the motor for each motor
         motors[i].flags |= MOTOR_HIGH_FLAG; 
         //sets flag to high to indicate moving
         motors[i].restPeriods = 1;
         motors[i].smsOnPeriods = 1;
         moveCnt++;
         //add to movement count
       }
       else {     
         if( motors[i].restPeriods >= (motor_pwm_maxperiod * motors[i].speed)) {
           //motor is moving and the rest periods are greater or equal to the distance (in terms of rest periods)
           moved++;
           // going down, disable output pin
           motors[i].flags &= (~( MOTOR_ENABLE_FLAG | MOTOR_HIGH_FLAG ) );
           MOTOR_DRV_PREG  &= ~(B00000011 << (i*3));
            // MOTOR_DRV_PREG  &= ~(B00000001 << (i*3 + (((motors[i].flags & MOTOR_CDIR_FLAG) >> 2))));
           
         } else {  
           //check to see if the motor is on an off period
           if (motors[i].smsOnPeriods < smsOnPeriodRatio){             
             //sees if the invert dir flag is on
             if( motors[i].flags & MOTOR_DIR_FLAG ){
               MOTOR_DRV_PREG  |= (B00000001 << (i*3 + (!((motors[i].flags & MOTOR_CDIR_FLAG) >> 2))));
               MOTOR_DRV_PREG  &= ~(B00000001 << (i*3 + (((motors[i].flags & MOTOR_CDIR_FLAG) >> 2))));
                                                         //!((motors[i].flags & MOTOR_CDIR_FLAG) >> 2)
             } else {
               MOTOR_DRV_PREG  |= (B00000001 << (i*3 + (((motors[i].flags & MOTOR_CDIR_FLAG) >> 2))));
               MOTOR_DRV_PREG  &= ~(B00000001 << (i*3 + (!((motors[i].flags & MOTOR_CDIR_FLAG) >> 2))));
             } 
             motors[i].smsOnPeriods++;
           } 
           
           else {             
             //sees if the invert dir flag is on
             MOTOR_DRV_PREG  &= ~(B00000011 << (i*3));
             motors[i].smsOnPeriods = 0;
           }
           motors[i].restPeriods++;       
         }
       }
    }
    else {
     MOTOR_DRV_PREG  &= ~(B00000011 << (i*3));
    }
  }
 
  
    // moved all motors the required distance?
  if( moved == moveCnt ) {
    moveCnt = 0;
    moved   = 0;
    motorStop(true);
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
 
 Each motor has three flags:
 
  - motor (programatically) enabled
  - motor (user) enabled
  - motor drive pin currently held high
 
 Motor PWM input pins are REQUIRED to be sequential on the SAME port register!
 
 @author 
 C. A. Church
 
 */

void motorRunISR() {
  
    // check status of each motor

 for(byte i = 0; i < MOTOR_COUNT; i++) {
   if( motors[i].flags & MOTOR_ENABLE_FLAG && motors[i].flags & MOTOR_UEN_FLAG ) {
       // motor is enabled
       
     if( ! (motors[i].flags & MOTOR_HIGH_FLAG) && motors[i].speed > 0.0) {
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
          if( motors[i].flags & MOTOR_DIR_FLAG ){
            MOTOR_DRV_PREG  |= (B00000001 << (i*3 + (!((motors[i].flags & MOTOR_CDIR_FLAG) >> 2))));            
          } else { 
            MOTOR_DRV_PREG  |= (B00000001 << (i*3 + (((motors[i].flags & MOTOR_CDIR_FLAG) >> 2))));            
          }
          motors[i].flags |= MOTOR_HIGH_FLAG;                  
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
            MOTOR_DRV_PREG  &= ~(B00000011 << (i*3));
            motors[i].flags &= ~(MOTOR_HIGH_FLAG);
              // accumulate off-period error for one period
            motors[i].restPeriods = 0;
            motors[i].onError += motors[i].onTimePeriods - (unsigned long) motors[i].onTimePeriods;
         }
         else {
           motors[i].restPeriods++;
           if( motors[i].flags & MOTOR_DIR_FLAG ){
             MOTOR_DRV_PREG  |= (B00000001 << (i*3 + (!((motors[i].flags & MOTOR_CDIR_FLAG) >> 2))));
             MOTOR_DRV_PREG  &= ~(B00000001 << (i*3 + (((motors[i].flags & MOTOR_CDIR_FLAG) >> 2))));
             
           } else { 
             MOTOR_DRV_PREG  |= (B00000001 << (i*3 + (((motors[i].flags & MOTOR_CDIR_FLAG) >> 2))));
             MOTOR_DRV_PREG  &= ~(B00000001 << (i*3 + (!((motors[i].flags & MOTOR_CDIR_FLAG) >> 2))));
           }
           
         }
             
     } //end else (motor currently high)
     
   } // end if motor enabled
   else {
     MOTOR_DRV_PREG  &= ~(B00000011 << (i*3));
   }
     
 } // end for...
 
}


/** Set Motor Enable Based on Lead-in
 
   Sets motor program-enable flag based on the lead-in amount for
   the motor, and the current shot count
   
   @author
   C. A. Church
   */
   
void motorCheckLead() {
  
     for( byte i = 0; i < MOTOR_COUNT; i++ ) {
//       USBSerial.print(" startShots is ");            
//       USBSerial.print(motors[i].startShots);
//       USBSerial.print(" .lead is ");  
//       USBSerial.print(motors[i].lead);
//       USBSerial.print(" .ramp_start is ");  
//       USBSerial.println(motors[i].ramp_start);
       
      if( motors[i].lead > 0 && ( camera_fired <= (motors[i].lead + motors[i].startShots) || camera_fired >= (camera_max_shots - (motors[i].lead + motors[i].startShots) ) ) )
        motors[i].flags &= ~(MOTOR_ENABLE_FLAG);
      else if( motors[i].lead > 0 && ( camera_fired > (motors[i].lead + motors[i].startShots) || camera_fired < (camera_max_shots - (motors[i].lead + motors[i].startShots)) ) )
        motors[i].flags |= MOTOR_ENABLE_FLAG;
        
    } 
}


/** Set Motor Speed Based on Ramp

  Controls linear ramping for each motor
  
  @author
  C. A. Church
  */
  
void motorCheckRamp() {
  
      for( byte i = 0; i < MOTOR_COUNT; i++ ) {
//        
//        USBSerial.print("camera shots = ");
//        USBSerial.print(camera_fired);
//        USBSerial.print(" flag is ");  
//        USBSerial.print(motors[i].flags,BIN); 
//        USBSerial.print("      "); 

        //check to see if motor is user enabled
        if(motors[i].flags & MOTOR_UEN_FLAG ) {
          
          if( motors[i].ramp_start > 0 || motors[i].ramp_end > 0 ) {
            
              // do not apply ramp if still in lead-in, or in lead-out
            if( motors[i].lead > 0 && ( camera_fired <= (motors[i].lead + motors[i].startShots) || camera_fired > (camera_max_shots - (motors[i].lead + motors[i].startShots)) ) )
              continue;
             
            unsigned int rampEnd = motors[i].lead  + motors[i].ramp_start;
            unsigned int rampEndF = motors[i].lead + motors[i].ramp_end;
            float diff; 
            
            
              // if we started during run time, set new ramp start point, in shots
            if( motors[i].startShots > 0 )
              rampEnd = rampEnd + motors[i].startShots;
              
            if( motors[i].flags & MOTOR_RAMP_FLAG ) {
                // force ramp down?
              diff = (float) (motors[i].ramp_end - (camera_fired - motors[i].forceRampStart)) / (float) motors[i].ramp_end;
              
              if( diff <= 0.0 ) {
                  // disable motor (user wanted the motor off when ramp completed...)
                  motors[i].flags &= ~(MOTOR_RAMP_FLAG | MOTOR_UEN_FLAG);
                  motors[i].inRamp = 0;
                  motorSpeed(i, motors[i].setSpeed);
                  continue;
              }
                // set motor currently ramping flag
              motors[i].inRamp = 1;
            }
            else if( camera_fired == rampEnd ) {
              diff = 1.0;
                // reset startshots, just in case they were set - since our lead-out/ramp-out is
                // doesn't care about that part.
              motors[i].startShots = 0;
                // disable motor currently ramping flag
              motors[i].inRamp = 0;
            }
            else if( camera_fired < rampEnd ) {
              
              diff = (float) (camera_fired - motors[i].startShots - motors[i].lead) / (float) motors[i].ramp_start;
                 // set motor currently ramping flag
              motors[i].inRamp = 1;
            }
            else if( camera_fired > (camera_max_shots - rampEndF) ) {
              diff = (float) (camera_max_shots - camera_fired - motors[i].lead)  / (float) motors[i].ramp_end;
                 // set motor currently ramping flag
              motors[i].inRamp = 1;
            }
            else {
              continue;
            }

            diff = motors[i].setSpeed * diff;
//            USBSerial.print(" startShots is ");            
//            USBSerial.print(motors[i].startShots);
//            USBSerial.print(" .lead is ");  
//            USBSerial.print(motors[i].lead);
//            USBSerial.print(" .ramp_start is ");  
//            USBSerial.print(motors[i].ramp_start);           
//            USBSerial.print(" the difference for motor speed is ");
//            USBSerial.println(diff);

            motorSpeed(i, diff, true);
          } // end if motors[i].ramp  
        } // end if motors[i].flags
      } // end for loop
}

/** Force Down Ramp of Motor

 Forces a motor to start a down ramp now.
 
 @param p_motor
 The motor to force to ramp down (0-2)
 
 @author
 C. A. Church
 */
 
void motorForceRamp(byte p_motor) {
  motors[p_motor].flags |= MOTOR_RAMP_FLAG;
  motors[p_motor].forceRampStart = camera_fired;
}



