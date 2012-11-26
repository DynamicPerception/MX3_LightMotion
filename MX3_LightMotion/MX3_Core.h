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
  Structs, Constants, Enums, Etc.
  ========================================
  
*/




const unsigned int SECOND  = 1000;

 /*  state transitions
 
  ST_BLOCK - do not allow any action to occur (some event is in process, block the state engine)
  ST_CLEAR - clear to start cycle
  ST_MOVE  - clear to move motor
  ST_RUN   - motor is currently running
  ST_EXP   - clear to expose camera (or not...)
  ST_WAIT  - in camera wait 
  
 */
 
const byte ST_BLOCK  = 0;
const byte ST_CLEAR  = 1;
const byte ST_MOVE   = 2;
const byte ST_RUN    = 3;
const byte ST_EXP    = 4;
const byte ST_WAIT   = 5;




/*

  Alt I/O Data
  
*/

  // debounce threshold time
const byte ALT_TRIG_THRESH  = 100;

  // what alt i/o modes do we support?

const byte ALT_OFF       = 0;
const byte ALT_START     = 1;
const byte ALT_STOP      = 2;
const byte ALT_TOGGLE    = 3;
const byte ALT_EXTINT    = 4;
const byte ALT_DIR       = 5;


/*


 Camera Settings
 
 
 */
 
const int CAM_MIN_TRIG    = 120;
const float CAM_MIN_BULB  = 0.1;

/*


  Motor Data
  
 */
 
 // define necessary constants
 


const byte MOTOR_ENABLE_FLAG  = B10000000;
const byte MOTOR_HIGH_FLAG    = B01000000;
const byte MOTOR_DIR_FLAG     = B00100000;
const byte MOTOR_ROT_FLAG     = B00010000;
const byte MOTOR_UEN_FLAG     = B00001000;
const byte MOTOR_CDIR_FLAG    = B00000100;
const byte MOTOR_RAMP_FLAG    = B00000010;

const byte MOTOR_COUNT        = 3;




/** Motor Definition Structure

  Defining Parameters for a motor
  
  */

struct MotorDefinition {
    /** Status flags
     (7) B0 = enabled for current move cycle (internally-controlled)
     (6) B1 = currently high pulse
     (5) B2 = direction flip 
     (4) B3 = type (rot = 1 / linear = 0)
     (3) B4 = user globally enabled (user-controlled)
     (2) B5 = current direction
     (1) B6 = force ramp out now
     (0) B7 = 
     
     Must be volatile to support B1 modulation in ISR
     */
 volatile byte flags;

   /** Volatile, used by motor_run_isr for error overflow */
 volatile float offError;
 volatile float onError;
 
  /** Stored Speed Value */
 float speed;
 
  /** Stored set speed (used when ramping) */
  
 float setSpeed;
 
   /** Volatile, used by motor_run_isr for overflow */
 volatile unsigned long restPeriods;
 
  /** Time Periods (on periods for every off period) */
 float onTimePeriods;
 
 
   /** max output shaft RPM of motor */
 float rpm;
 
   /** Output ratio
   
     Determines how many units are moved per rotation.  E.g., if units are
     "inches", and a drive pulley has a pitch circumference of 3.2 inches, 3.2 would be
     input.  However, if units are "degrees" (rotational), and the motor output shaft connects to a 
     1:1 right-angle gearbox, the ratio would be 1.0; whereas a 15:1 gearbox would use 15.0 as ratio.
    */
    
 float ratio;
 
  /** Distance traveled (in on pulses) */
 unsigned long distance;
 
  /** Ramp Shots */
 byte ramp;
 
  /** Lead-in/Out */
  
 byte lead;
 
  /** Shot count that force ramp started at */
 byte forceRampStart;
 
  /** Shot count for rampt starting (when started motor during program run)*/
 unsigned long startShots;
 
   /** Default Constructor */
   
 MotorDefinition() {
   flags = 0;
   restPeriods = 0;
   onTimePeriods = 1.0;
   rpm = 1.0;
   ratio = 1.0;
   distance = 0;
   ramp = 0;
   lead = 0;
   speed = 1.0;
   setSpeed = 1.0;
   forceRampStart = 0;
   startShots = 0;
 }
 
};



/* 


  EEPROM Data
  
*/

 // stored memory layout version
const unsigned int MEMORY_VERSION    = 10;


/* Locations of each variable to be stored, note correct spacing
   based on data size */

const int EE_NONE      = 0; // do not store
const int EE_SMS       = 1; // motion_sms
const int EE_ALTDIR    = EE_SMS     + 1; // alt_direction

const int EE_MAXSHOT   = EE_ALTDIR  + 1; // camera max shots
const int EE_CAMREP    = EE_MAXSHOT + 2; // camera_repeat
const int EE_CAMDEL    = EE_CAMREP  + 1; // camera_delay
const int EE_CAMEXP    = EE_CAMDEL  + 4; // cam_exposure
const int EE_CAMWAIT   = EE_CAMEXP  + 4; // cam_wait
const int EE_CAMFOC    = EE_CAMWAIT + 4; // cam_focus
const int EE_CAMBULB   = EE_CAMFOC  + 4; // bulb mode

const int EE_M0FLAG    = EE_CAMBULB + 1; // flags
const int EE_M0RPM     = EE_M0FLAG  + 1; // rpm
const int EE_M0RATIO   = EE_M0RPM   + 4; // ratio
const int EE_M0RAMP    = EE_M0RATIO + 4; // ramping
const int EE_M0LEAD    = EE_M0RAMP  + 1; // lead-in/out

  // note: for each motor, we move the previous defs ahead 12 bytes * motor num

const int EE_MOTOR_SPACE = 13;  
const int EE_POSTMOTOR   = EE_M0LEAD + 1 + (EE_MOTOR_SPACE * 2);

const int EE_LCDOFF    = EE_POSTMOTOR + 1; // lcd off time
const int EE_METRIC    = EE_LCDOFF + 1; // metric display
const int EE_ALT1      = EE_METRIC + 1; // alt input 1 mode
const int EE_ALT2      = EE_ALT1   + 1; // alt input 2 mode
const int EE_PERIOD    = EE_ALT2   + 1; // minimum period in mS


