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
  Structs, Constants, Enums, Etc.
  ========================================
  
*/



  // multiplier for inches to CM
const float CM_CONSTANT = 2.54;

  // milliseconds in a second
const unsigned int SECOND = 1000;

 /*  state transitions
 
  ST_BLOCK - do not allow any action to occur (some event is in process, block the state engine)
  ST_CLEAR - clear to start cycle
  ST_MOVE  - clear to move motor
  ST_RUN   - motor is currently running
  ST_EXP   - clear to expose camera (or not...)
  ST_WAIT  - in camera wait 
  ST_ALTP  - check for alt output post 
  
 */
 
const byte ST_BLOCK = 0;
const byte ST_CLEAR = 1;
const byte  ST_MOVE = 2;
const byte   ST_RUN = 3;
const byte   ST_EXP = 4;
const byte  ST_WAIT = 5;
const byte  ST_ALTP = 6;

/*

  Sensor Data Constants
  
*/

const unsigned int    MAX_CURRENT = 6000;
const float              MAX_TEMP = 90.0;
const unsigned int  SENS_RST_TIME = 30000;
const unsigned int SENS_POLL_TIME = 10000;
const byte      SENS_CURRENT_FLAG = B00000001;
const byte         SENS_TEMP_FLAG = B00000010;
const byte         SENS_VOLT_FLAG = B00000100;

/*

  Save State Constants
  
*/

const byte SS_COUNT = 3;

/*

  VFD Brigthness State Constants
  
*/

const byte VFD_COUNT      = 4;
const byte Brightness_25  = 0x2B;
const byte Brightness_50  = 0x2A;
const byte Brightness_75  = 0x29;
const byte Brightness_100 = 0x28;


/*

  Alt I/O Data
  
*/

  // debounce threshold time
const byte ALT_TRIG_THRESH  = 50;

  // what alt i/o modes do we support?

const byte    ALT_OFF = 0;
const byte  ALT_START = 1;
const byte   ALT_STOP = 2;
const byte ALT_TOGGLE = 3;
const byte ALT_EXTINT = 4;
const byte    ALT_DIR = 5;
const byte  ALT_OUT_B = 6;
const byte  ALT_OUT_A = 7;

const byte ALT_OUT_FLAG_B = B00010000;
const byte ALT_OUT_FLAG_A = B00000001;
const byte  ALT_OUT_ANY_B = B11110000;
const byte  ALT_OUT_ANY_A = B00001111;
const byte     ALT_TRIG_B = 1;
const byte     ALT_TRIG_A = 2;
const byte ALT_BLOCK_NONE = 0;
const byte    ALT_BLOCK_B = ALT_TRIG_B;
const byte    ALT_BLOCK_A = ALT_TRIG_A;



/*


 Camera Settings
 
 
 */
 
const int            CAM_MIN_TRIG = 250;
const float          CAM_MIN_BULB = 0.25;

const unsigned long   CAMWAIT_MAX = 65535;
const unsigned long   CAMWAIT_MIN = 0;
const unsigned long    CAMFOC_MAX = 65535;
const unsigned long    CAMFOC_MIN = 0;
const unsigned long    CAMEXP_MAX = 65535;
const unsigned long    CAMEXP_MIN = 0;

/*


  Motor Data
  
 */
 
 // define necessary constants
 


const byte MOTOR_ENABLE_FLAG = B10000000;
const byte   MOTOR_HIGH_FLAG = B01000000;
const byte    MOTOR_DIR_FLAG = B00100000;
const byte    MOTOR_ROT_FLAG = B00010000;
const byte    MOTOR_UEN_FLAG = B00001000;
const byte   MOTOR_CDIR_FLAG = B00000100;
const byte   MOTOR_RAMP_FLAG = B00000010;

const byte MOTOR_COUNT = 3;

 // these are parameters for our speed fitting function
const double MOTOR_SCA = 4.8756308185438346E+00; 
const double MOTOR_SCB = 4.0945526450071874E+00; 
const double MOTOR_SCC = -1.5602646081740568E+00;

const byte MOTOR_PRESET_STRLEN = 15;

struct MotorPreset {
  
  char name[MOTOR_PRESET_STRLEN];
  float rpm;
  float ratio;
  byte rotary;
 
};


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
 
  /** Ramp Shots Start */
 unsigned int ramp_start;
 
  /** Ramp Shots End */
 unsigned int ramp_end;  
 
  /** Lead-in/Out */
 unsigned int lead;
 
  /** Shot count that force ramp started at */
 unsigned long forceRampStart;
 
  /** Shot count for ramp starting (when started motor during program run)*/
 unsigned long startShots;
 
   /** Default Constructor */
   
 MotorDefinition() {
   flags = 0;
   restPeriods = 0;
   onTimePeriods = 1.0;
   rpm = 8.13;
   ratio = 3.229;
   ramp_start = 0;
   ramp_end = 0;
   lead = 0;
   speed = 0.01;
   setSpeed = 0.01;
   forceRampStart = 0;
   startShots = 0;
 }
 
};



/* 


  EEPROM Data
  
*/

 // stored memory layout version
 // this number MUST be changed every time the memory layout is changed
const unsigned int MEMORY_VERSION    = 34;


/* Locations of each variable to be stored, note correct spacing
   based on data size */

const int EE_NONE      = 0; // do not store
const int EE_SMS       = 1; // motion_sms
const int EE_MAXSHOT   = EE_SMS     + 1; // camera max shots      2
const int EE_CAMREP    = EE_MAXSHOT + 2; // camera_repeat         4 
const int EE_CAMDEL    = EE_CAMREP  + 1; // camera_delay          5
const int EE_CAMEXP    = EE_CAMDEL  + 4; // cam_exposure          9
const int EE_CAMWAIT   = EE_CAMEXP  + 4; // cam_wait              13
const int EE_CAMFOC    = EE_CAMWAIT + 4; // cam_focus             17 
const int EE_CAMBULB   = EE_CAMFOC  + 4; // bulb mode             21 
const int EE_CAMLOCK   = EE_CAMBULB + 1; // focus lock            22

const int EE_M0FLAG    = EE_CAMLOCK  + 1; // flags
const int EE_M0RPM     = EE_M0FLAG   + 1; // rpm
const int EE_M0RATIO   = EE_M0RPM    + 4; // ratio
const int EE_MORSPEED  = EE_M0RATIO  + 4; // speed
const int EE_M0RAMP    = EE_MORSPEED + 4; // ramping in
const int EE_M0RAMPE   = EE_M0RAMP   + 2; // ramping out
const int EE_M0LEAD    = EE_M0RAMPE  + 2; // lead-in/out

  // note: for each motor, we move the previous defs ahead 21 bytes * motor num

const int EE_MOTOR_SPACE = 21; //21;  
const int EE_POSTMOTOR = EE_M0LEAD + 2 + (EE_MOTOR_SPACE * 2);

const int EE_LCDOFF    = EE_POSTMOTOR + 1; // lcd off time
const int EE_VFDBRI    = EE_LCDOFF    + 1; // VFD brightness
const int EE_ALT1      = EE_VFDBRI    + 1; // alt input 1 mode
const int EE_ALT2      = EE_ALT1      + 1; // alt input 2 mode
const int EE_ALT3      = EE_ALT2      + 1; // alt input 3 mode
const int EE_ALT4      = EE_ALT3      + 1; // alt input 4 mode
const int EE_ALTDIR    = EE_ALT4      + 1; // alt input trigger direction
const int EE_ALTBD     = EE_ALTDIR    + 1; // alt trigger before delay
const int EE_ALTBT     = EE_ALTBD     + 2; // alt trigger before time
const int EE_ALTAD     = EE_ALTBT     + 2; // alt trigger after delay
const int EE_ALTAT     = EE_ALTAD     + 2; // alt trigger after time
const int EE_ALTHL     = EE_ALTAT     + 2; // high or low output trigger
const int EE_PERIOD    = EE_ALTHL     + 1; // minimum period in mS
const int EE_MPRESET   = EE_PERIOD    + 2; // selected presets
const int EE_VOLTH     = EE_MPRESET   + 4; // voltage threshold
const int EE_VOLWARN   = EE_VOLTH     + 4; // voltage warning flag
const int EE_HEATER    = EE_VOLWARN   + 1; // heater on/off flag 
const int EE_METRIC    = EE_HEATER    + 1; // metric display on/off

//SAVE STATE 0

const int EE_NONE_SS0      = 0;                   // do not store
const int EE_SMS_SS0       = EE_METRIC      + 1;  // motion_sms
const int EE_MAXSHOT_SS0   = EE_SMS_SS0     + 1;  // camera max shots
const int EE_CAMREP_SS0    = EE_MAXSHOT_SS0 + 2;  // camera_repeat
const int EE_CAMDEL_SS0    = EE_CAMREP_SS0  + 1;  // camera_delay
const int EE_CAMEXP_SS0    = EE_CAMDEL_SS0  + 4;  // cam_exposure
const int EE_CAMWAIT_SS0   = EE_CAMEXP_SS0  + 4;  // cam_wait
const int EE_CAMFOC_SS0    = EE_CAMWAIT_SS0 + 4;  // cam_focus
const int EE_CAMBULB_SS0   = EE_CAMFOC_SS0  + 4;  // bulb mode
const int EE_CAMLOCK_SS0   = EE_CAMBULB_SS0 + 1;  // focus lock

const int EE_M0FLAG_SS0    = EE_CAMLOCK_SS0 + 1;  // flags
const int EE_M0RPM_SS0     = EE_M0FLAG_SS0  + 1;  // rpm
const int EE_M0RATIO_SS0   = EE_M0RPM_SS0   + 4;  // ratio
const int EE_MORSPEED_SS0  = EE_M0RATIO_SS0 + 4;  // speed
const int EE_M0RAMP_SS0    = EE_MORSPEED_SS0 + 4; // ramping in
const int EE_M0RAMPE_SS0   = EE_M0RAMP_SS0  + 2;  // ramping out
const int EE_M0LEAD_SS0    = EE_M0RAMPE_SS0 + 2;  // lead-in/out

  // note: for each motor, we move the previous defs ahead 21 bytes * motor num

const int EE_MOTOR_SPACE_SS0 = 21;  
const int EE_POSTMOTOR_SS0 = EE_M0LEAD_SS0 + 2 + (EE_MOTOR_SPACE_SS0 * 2);

const int EE_LCDOFF_SS0    = EE_POSTMOTOR_SS0 + 1; // lcd off time
const int EE_ALT1_SS0      = EE_LCDOFF_SS0    + 1; // alt input 1 mode
const int EE_ALT2_SS0      = EE_ALT1_SS0      + 1; // alt input 2 mode
const int EE_ALT3_SS0      = EE_ALT2_SS0      + 1; // alt input 3 mode
const int EE_ALT4_SS0      = EE_ALT3_SS0      + 1; // alt input 4 mode
const int EE_ALTDIR_SS0    = EE_ALT4_SS0      + 1; // alt input trigger direction
const int EE_ALTBD_SS0     = EE_ALTDIR_SS0    + 1; // alt trigger before delay
const int EE_ALTBT_SS0     = EE_ALTBD_SS0     + 2; // alt trigger before time
const int EE_ALTAD_SS0     = EE_ALTBT_SS0     + 2; // alt trigger after delay
const int EE_ALTAT_SS0     = EE_ALTAD_SS0     + 2; // alt trigger after time
const int EE_ALTHL_SS0     = EE_ALTAT_SS0     + 2; // high or low output trigger
const int EE_PERIOD_SS0    = EE_ALTHL_SS0     + 1; // minimum period in mS
const int EE_MPRESET_SS0   = EE_PERIOD_SS0    + 2; // selected presets
const int EE_VOLTH_SS0     = EE_MPRESET_SS0   + 4; // voltage threshold
const int EE_VOLWARN_SS0   = EE_VOLTH_SS0     + 4; // voltage warning flag
const int EE_HEATER_SS0    = EE_VOLWARN_SS0   + 1; // heater on/off flag 
const int EE_METRIC_SS0    = EE_HEATER_SS0    + 1; // metric display on/off

//SAVE STATE 1

const int EE_NONE_SS1      = 0;                   // do not store
const int EE_SMS_SS1       = EE_METRIC_SS0  + 1;  // motion_sms
const int EE_MAXSHOT_SS1   = EE_SMS_SS1     + 1;  // camera max shots
const int EE_CAMREP_SS1    = EE_MAXSHOT_SS1 + 2;  // camera_repeat
const int EE_CAMDEL_SS1    = EE_CAMREP_SS1  + 1;  // camera_delay
const int EE_CAMEXP_SS1    = EE_CAMDEL_SS1  + 4;  // cam_exposure
const int EE_CAMWAIT_SS1   = EE_CAMEXP_SS1  + 4;  // cam_wait
const int EE_CAMFOC_SS1    = EE_CAMWAIT_SS1 + 4;  // cam_focus
const int EE_CAMBULB_SS1   = EE_CAMFOC_SS1  + 4;  // bulb mode
const int EE_CAMLOCK_SS1   = EE_CAMBULB_SS1 + 1;  // focus lock

const int EE_M0FLAG_SS1    = EE_CAMLOCK_SS1 + 1;  // flags
const int EE_M0RPM_SS1     = EE_M0FLAG_SS1  + 1;  // rpm
const int EE_M0RATIO_SS1   = EE_M0RPM_SS1   + 4;  // ratio
const int EE_MORSPEED_SS1  = EE_M0RATIO_SS1 + 4;  // speed
const int EE_M0RAMP_SS1    = EE_MORSPEED_SS1 + 4; // ramping in
const int EE_M0RAMPE_SS1   = EE_M0RAMP_SS1  + 2;  // ramping out
const int EE_M0LEAD_SS1    = EE_M0RAMPE_SS1 + 2;  // lead-in/out

  // note: for each motor, we move the previous defs ahead 21 bytes * motor num

const int EE_MOTOR_SPACE_SS1 = 21;  
const int EE_POSTMOTOR_SS1 = EE_M0LEAD_SS1 + 2 + (EE_MOTOR_SPACE_SS1 * 2);

const int EE_LCDOFF_SS1    = EE_POSTMOTOR_SS1 + 1; // lcd off time
const int EE_ALT1_SS1      = EE_LCDOFF_SS1    + 1; // alt input 1 mode
const int EE_ALT2_SS1      = EE_ALT1_SS1      + 1; // alt input 2 mode
const int EE_ALT3_SS1      = EE_ALT2_SS1      + 1; // alt input 3 mode
const int EE_ALT4_SS1      = EE_ALT3_SS1      + 1; // alt input 4 mode
const int EE_ALTDIR_SS1    = EE_ALT4_SS1      + 1; // alt input trigger direction
const int EE_ALTBD_SS1     = EE_ALTDIR_SS1    + 1; // alt trigger before delay
const int EE_ALTBT_SS1     = EE_ALTBD_SS1     + 2; // alt trigger before time
const int EE_ALTAD_SS1     = EE_ALTBT_SS1     + 2; // alt trigger after delay
const int EE_ALTAT_SS1     = EE_ALTAD_SS1     + 2; // alt trigger after time
const int EE_ALTHL_SS1     = EE_ALTAT_SS1     + 2; // high or low output trigger
const int EE_PERIOD_SS1    = EE_ALTHL_SS1     + 1; // minimum period in mS
const int EE_MPRESET_SS1   = EE_PERIOD_SS1    + 2; // selected presets
const int EE_VOLTH_SS1     = EE_MPRESET_SS1   + 4; // voltage threshold
const int EE_VOLWARN_SS1   = EE_VOLTH_SS1     + 4; // voltage warning flag
const int EE_HEATER_SS1    = EE_VOLWARN_SS1   + 1; // heater on/off flag 
const int EE_METRIC_SS1    = EE_HEATER_SS1    + 1; // metric display on/off

//SAVE STATE 2

const int EE_NONE_SS2      = 0;                   // do not store
const int EE_SMS_SS2       = EE_METRIC_SS1  + 1;  // motion_sms
const int EE_MAXSHOT_SS2   = EE_SMS_SS2     + 1;  // camera max shots
const int EE_CAMREP_SS2    = EE_MAXSHOT_SS2 + 2;  // camera_repeat
const int EE_CAMDEL_SS2    = EE_CAMREP_SS2  + 1;  // camera_delay
const int EE_CAMEXP_SS2    = EE_CAMDEL_SS2  + 4;  // cam_exposure
const int EE_CAMWAIT_SS2   = EE_CAMEXP_SS2  + 4;  // cam_wait
const int EE_CAMFOC_SS2    = EE_CAMWAIT_SS2 + 4;  // cam_focus
const int EE_CAMBULB_SS2   = EE_CAMFOC_SS2  + 4;  // bulb mode
const int EE_CAMLOCK_SS2   = EE_CAMBULB_SS2 + 1;  // focus lock

const int EE_M0FLAG_SS2    = EE_CAMLOCK_SS2 + 1;  // flags
const int EE_M0RPM_SS2     = EE_M0FLAG_SS2  + 1;  // rpm
const int EE_M0RATIO_SS2   = EE_M0RPM_SS2   + 4;  // ratio
const int EE_MORSPEED_SS2  = EE_M0RATIO_SS2 + 4;  // speed
const int EE_M0RAMP_SS2    = EE_MORSPEED_SS2 + 4; // ramping in
const int EE_M0RAMPE_SS2   = EE_M0RAMP_SS2  + 2;  // ramping out
const int EE_M0LEAD_SS2    = EE_M0RAMPE_SS2 + 2;  // lead-in/out

  // note: for each motor, we move the previous defs ahead 21 bytes * motor num

const int EE_MOTOR_SPACE_SS2 = 21;  
const int EE_POSTMOTOR_SS2 = EE_M0LEAD_SS2 + 2 + (EE_MOTOR_SPACE_SS2 * 2);

const int EE_LCDOFF_SS2    = EE_POSTMOTOR_SS2 + 1; // lcd off time
const int EE_ALT1_SS2      = EE_LCDOFF_SS2    + 1; // alt input 1 mode
const int EE_ALT2_SS2      = EE_ALT1_SS2      + 1; // alt input 2 mode
const int EE_ALT3_SS2      = EE_ALT2_SS2      + 1; // alt input 3 mode
const int EE_ALT4_SS2      = EE_ALT3_SS2      + 1; // alt input 4 mode
const int EE_ALTDIR_SS2    = EE_ALT4_SS2      + 1; // alt input trigger direction
const int EE_ALTBD_SS2     = EE_ALTDIR_SS2    + 1; // alt trigger before delay
const int EE_ALTBT_SS2     = EE_ALTBD_SS2     + 2; // alt trigger before time
const int EE_ALTAD_SS2     = EE_ALTBT_SS2     + 2; // alt trigger after delay
const int EE_ALTAT_SS2     = EE_ALTAD_SS2     + 2; // alt trigger after time
const int EE_ALTHL_SS2     = EE_ALTAT_SS2     + 2; // high or low output trigger
const int EE_PERIOD_SS2    = EE_ALTHL_SS2     + 1; // minimum period in mS
const int EE_MPRESET_SS2   = EE_PERIOD_SS2    + 2; // selected presets
const int EE_VOLTH_SS2     = EE_MPRESET_SS2   + 4; // voltage threshold
const int EE_VOLWARN_SS2   = EE_VOLTH_SS2     + 4; // voltage warning flag
const int EE_HEATER_SS2    = EE_VOLWARN_SS2   + 1; // heater on/off flag 
const int EE_METRIC_SS2    = EE_HEATER_SS2    + 1; // metric display on/off


