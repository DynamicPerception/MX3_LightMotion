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

  // milliseconds per minute
const unsigned int MILLIS_PER_MIN = 60000;

  // seconds per minute
const int SEC_PER_MIN = 60;

// minutes per hour
const int MIN_PER_HR = 60;

  // inches per meter
const float INCH_PER_METER = 39.37;

// EZ Mode Constants
const float EZADJUST_MAX = 3.0;			// Max EZ adjust value
const float EZADJUST_MIN = 0.1;			// Min EZ adjust value
const float EZADJUST_INCREMENT = 0.1;	// By how much the EZ adjust value should change per button press

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

  Manual Move Settings
  
*/

const byte		       MM_HOLD = 0;
const byte 			 MM_SELECT = 1;
const byte		  MM_ENDURANCE = 2;


/*

MoCoBus Settings

*/

const boolean	       MOCOBUS_MASTER = false;
const boolean			 MOCOBUS_NODE = true;


/*

  Sensor Data Constants
  
*/

const unsigned int    MAX_CURRENT = 6000;
const float              MAX_TEMP = 90.0;
const unsigned int  SENS_RST_TIME = 30000;
const unsigned int SENS_POLL_TIME = 5000;
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
const int ALT_TRIG_THRESH  = 750;

  // what alt i/o modes do we support?

const byte         ALT_OFF = 0;
const byte       ALT_START = 1;
const byte        ALT_STOP = 2;
const byte      ALT_TOGGLE = 3;
const byte      ALT_EXTINT = 4;
const byte         ALT_DIR = 5;
const byte       ALT_OUT_B = 6;
const byte       ALT_OUT_A = 7;
const byte ALT_STOP_MOTORS = 8;
const byte  ALT_PAUSE_PRGM = 9;

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
const unsigned long	  CAMDELAY_MAX = 65535;
const unsigned long   CAMDELAY_MIN = 0;
const unsigned long    CAMFOC_MAX = 65535;
const unsigned long    CAMFOC_MIN = 0;
const unsigned long    CAMEXP_MAX = 65535;
const unsigned long    CAMEXP_MIN = 0;
const int				CAMFL_MAX = 600;
const int				CAMFL_MIN = 18;

/*


  Motor Data
  
 */
 
 // define necessary constants
 


const byte MOTOR_ENABLE_FLAG = B10000000; // Motor is enabled
const byte   MOTOR_HIGH_FLAG = B01000000; // PWM on
const byte    MOTOR_INVERT_FLAG = B00100000; // Direction invert
const byte    MOTOR_ROT_FLAG = B00010000; // 1 - Rotary, 0 - Linear
const byte    MOTOR_UEN_FLAG = B00001000; // Global user enabled
const byte   MOTOR_CDIR_FLAG = B00000100; // Current direction
const byte   MOTOR_RAMP_FLAG = B00000010; // Ramping

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
 
  /** Stored Speed Value (Motor power setting from 0.0-1.0) */
 float speed;
 
  /** Stored set speed (used when ramping) */
  
 float setSpeed;

 /** The adjust value used in EZ mode */
 float ez_adjust;

 /** EZ mode base move value to be modified by adjust multiplier */
 float	ez_center_val;

   /** Volatile, used by motor_run_isr for overflow */
 // M. Ploof: Count of how many cycles have occured while the motor is doing an SMS move
 volatile unsigned long currentMoveCycles;
 
 /** needed to change the speed of the motor for SMS mode */ 
 int pwmOnCycles;
 
 /** flag to determine if the motor is currently ramping */
 bool inRamp;
 
  /** Time Periods (ratio of on periods to off periods) */
 float onCycleRatio;
 
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
 
 /** pwm step count for speed ramping*/
 int speedSteps;
 
 /** desired speed for the motor to reach */
// float = desiredSpeed;
 
 /** desired direction for the motor to move, 0 or 1 */
 bool desiredDirection;

 /** Which preset is the motor using (0 for none) */
 byte motorPreset;

 /** The target speed in in/min, cm/min, or deg/min */
 float target_speed;

 /** The target SMS distance in in/move, cm/move, or deg/move */
 float target_sms_distance;
 
 
 /** Default Constructor */
   
 MotorDefinition() {
   flags = 0;
   currentMoveCycles = 0;
   onCycleRatio = 1.0;
   rpm = 8.13;
   ratio = 3.229;
   ramp_start = 0;
   ramp_end = 0;
   lead = 0;
   speed = 0.01;
   setSpeed = 0.01;
   ez_adjust = 1.0;
   ez_center_val = 0.2;
   forceRampStart = 0;
   startShots = 0;
   pwmOnCycles = 0;
   inRamp = 0;
   speedSteps = 0;
   desiredDirection = 0;
   motorPreset = 0;
   target_speed = 1.0;
   target_sms_distance = 0.2;
 }
 
};

/* 


  EEPROM Data
  
*/

 // stored memory layout version
 // this number MUST be changed every time the memory layout is changed
const unsigned int MEMORY_VERSION    = 41;


/* Locations of each variable to be stored, note correct spacing
   based on data size */

const int EE_NONE      = 0; // do not store
const int EE_SMS       = 1; // motion_sms
const int EE_MMSETTING = EE_SMS       + 1; // manual move settings  
const int EE_MAXSHOT   = EE_MMSETTING + 1; // camera max shots      2
const int EE_CAMREP    = EE_MAXSHOT   + 2; // camera_repeat         4 
const int EE_CAMDEL    = EE_CAMREP    + 1; // camera_delay          5
const int EE_CAMEXP    = EE_CAMDEL    + 4; // cam_exposure          9
const int EE_CAMWAIT   = EE_CAMEXP    + 4; // cam_wait              13
const int EE_CAMFOC    = EE_CAMWAIT   + 4; // cam_focus             17 
const int EE_CAMBULB   = EE_CAMFOC    + 4; // bulb mode             21 
const int EE_CAMLOCK   = EE_CAMBULB   + 1; // focus lock            22
const int EE_CAMTIMER  = EE_CAMLOCK   + 1; // focus lock            24

const int EE_M0FLAG    = EE_CAMTIMER + 4; // flags          1
const int EE_M0RPM     = EE_M0FLAG   + 1; // rpm            4
const int EE_M0RATIO   = EE_M0RPM    + 4; // ratio          4
const int EE_MORSPEED  = EE_M0RATIO  + 4; // speed          4
const int EE_M0RAMP    = EE_MORSPEED + 4; // ramping in     2 
const int EE_M0RAMPE   = EE_M0RAMP   + 2; // ramping out    2
const int EE_M0LEAD    = EE_M0RAMPE  + 2; // lead-in/out    2

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
const int EE_UNITS     = EE_HEATER    + 1; // unit type
const int EE_INCREMENT = EE_UNITS	  + 1; // motor speed increment

//SAVE STATE 0

const int EE_NONE_SS0      = 0;                   // do not store
const int EE_SMS_SS0       = EE_INCREMENT     + 4;  // motion_sms
const int EE_MAXSHOT_SS0   = EE_SMS_SS0       + 1;  // camera max shots
const int EE_CAMREP_SS0    = EE_MAXSHOT_SS0   + 2;  // camera_repeat
const int EE_CAMDEL_SS0    = EE_CAMREP_SS0    + 1;  // camera_delay
const int EE_CAMEXP_SS0    = EE_CAMDEL_SS0    + 4;  // cam_exposure
const int EE_CAMWAIT_SS0   = EE_CAMEXP_SS0    + 4;  // cam_wait
const int EE_CAMFOC_SS0    = EE_CAMWAIT_SS0   + 4;  // cam_focus
const int EE_CAMBULB_SS0   = EE_CAMFOC_SS0    + 4;  // bulb mode
const int EE_CAMLOCK_SS0   = EE_CAMBULB_SS0   + 1;  // focus lock
const int EE_CAMTIMER_SS0  = EE_CAMLOCK_SS0   + 1; // focus lock                    

const int EE_M0FLAG_SS0    = EE_CAMTIMER_SS0  + 4;  // flags
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
const int EE_UNITS_SS0     = EE_HEATER_SS0    + 1; // unit type
const int EE_INCREMENT_SS0 = EE_UNITS_SS0     + 1; // motor speed increment


//SAVE STATE 1

const int EE_NONE_SS1      = 0;                   // do not store
const int EE_SMS_SS1       = EE_INCREMENT_SS0 + 4;  // motion_sms
const int EE_MAXSHOT_SS1   = EE_SMS_SS1       + 1;  // camera max shots
const int EE_CAMREP_SS1    = EE_MAXSHOT_SS1   + 2;  // camera_repeat
const int EE_CAMDEL_SS1    = EE_CAMREP_SS1    + 1;  // camera_delay
const int EE_CAMEXP_SS1    = EE_CAMDEL_SS1    + 4;  // cam_exposure
const int EE_CAMWAIT_SS1   = EE_CAMEXP_SS1    + 4;  // cam_wait
const int EE_CAMFOC_SS1    = EE_CAMWAIT_SS1   + 4;  // cam_focus
const int EE_CAMBULB_SS1   = EE_CAMFOC_SS1    + 4;  // bulb mode
const int EE_CAMLOCK_SS1   = EE_CAMBULB_SS1   + 1;  // focus lock
const int EE_CAMTIMER_SS1  = EE_CAMLOCK_SS1   + 1; // focus lock            

const int EE_M0FLAG_SS1    = EE_CAMTIMER_SS1 + 4;  // flags
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
const int EE_UNITS_SS1    = EE_HEATER_SS1     + 1; // unit type on/off
const int EE_INCREMENT_SS1 = EE_UNITS_SS1     + 1; // motor speed increment


//SAVE STATE 2

const int EE_NONE_SS2      = 0;                   // do not store
const int EE_SMS_SS2       = EE_INCREMENT_SS1 + 4;  // motion_sms
const int EE_MAXSHOT_SS2   = EE_SMS_SS2       + 1;  // camera max shots
const int EE_CAMREP_SS2    = EE_MAXSHOT_SS2   + 2;  // camera_repeat
const int EE_CAMDEL_SS2    = EE_CAMREP_SS2    + 1;  // camera_delay
const int EE_CAMEXP_SS2    = EE_CAMDEL_SS2    + 4;  // cam_exposure
const int EE_CAMWAIT_SS2   = EE_CAMEXP_SS2    + 4;  // cam_wait
const int EE_CAMFOC_SS2    = EE_CAMWAIT_SS2   + 4;  // cam_focus
const int EE_CAMBULB_SS2   = EE_CAMFOC_SS2    + 4;  // bulb mode
const int EE_CAMLOCK_SS2   = EE_CAMBULB_SS2   + 1;  // focus lock
const int EE_CAMTIMER_SS2  = EE_CAMLOCK_SS2   + 1; // focus lock            


const int EE_M0FLAG_SS2    = EE_CAMTIMER_SS2 + 4;  // flags
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
const int EE_UNITS_SS2     = EE_HEATER_SS2     + 1; // unit type on/off
const int EE_INCREMENT_SS2 = EE_UNITS_SS2     + 1; // motor speed increment


// EEPROM Block starting at address 500 reserved for values added in and after Vesion 1.1 of the MX3 firmware.
// Each group of settins is allocated 500 bytes of memory, but this can be changed by modifying the start values
// of the memory blocks for each of the save state groups.

/** Default Save State **/

// MoCoBus Settings 
const int EE_V1_1_MAIN_START = 500;						// Address at which to start storing the default save state settings
const int EE_NODE			= EE_V1_1_MAIN_START;		// is the device a node on the MoCoBus? (byte)
const int EE_ADDR			= EE_NODE + 1;				// The device's address on the bus (byte)

// Camera Settings
const int EE_CAMFL			= EE_ADDR + 1;				// Camera focal length (mm) (int)
	
// Motor Settings
const int EE_MOTOR_SPACE_V1_1 = 12;						// How many addresses to move ahead for each motor when restoring or saving V1.1 and newer motor values
const int EE_DES_SPEED0 = EE_CAMFL + 2;					// Target speed for motor 0 (float)
const int EE_DES_SMSDIST0 = EE_DES_SPEED0 + 4;			// Target SMS distance for motor0 (float)
const int EE_EZADJ0 = EE_DES_SMSDIST0 + 4;				// Adjust value for EZ mode (float)

// EZ Mode Settings
const int EE_EZMODE = EE_EZADJ0 + (EE_MOTOR_SPACE_V1_1 * MOTOR_COUNT);	// Whether EZ mode is active (boolean)
const int EE_EZX = EE_EZMODE + 1;										// Whether the EZ extended / EZ manual mode is enabled (boolean)


/** User Save State 0**/

// MoCoBus Settings 
const int EE_V1_1_SS0_START = 1000;								// Address at which to start storing user saved settings 0
const int EE_NODE_SS0 = EE_INCREMENT_SS0 + 4;					// is the device a node on the MoCoBus?
const int EE_ADDR_SS0 = EE_NODE_SS0 + 1;						// The device's address on the bus (byte)

// Camera Settings
const int EE_CAMFL_SS0 = EE_ADDR_SS0 + 1;						// Camera focal length (mm) (int)

// Motor Settings
const int EE_MOTOR_SPACE_V1_1_SS0 = 12;							// How many addresses to move ahead for each motor when restoring or saving V1.1 and newer motor values
const int EE_DES_SPEED0_SS0 = EE_CAMFL + 2;						// Target speed for motor 0 (float)
const int EE_DES_SMSDIST0_SS0 = EE_DES_SPEED0 + 4;				// Target SMS distance for motor0 (float)
const int EE_EZADJ0_SS0 = EE_DES_SMSDIST0_SS0 + 4;				// Adjust value for EZ mode (float)

// EZ Mode Settings
const int EE_EZMODE_SS0 = EE_EZADJ0_SS0 + (EE_MOTOR_SPACE_V1_1_SS0 * MOTOR_COUNT);	// Whether EZ mode is active (boolean)
const int EE_EZX_SS0 = EE_EZMODE_SS0 + 1;											// Whether the EZ extended / EZ manual mode is enabled (boolean)


/** User Save State 1**/

// MoCoBus Settings 
const int EE_V1_1_SS1_START = 1500;								// Address at which to start storing user saved settings 1
const int EE_NODE_SS1 = EE_INCREMENT_SS1 + 4;					// is the device a node on the MoCoBus?
const int EE_ADDR_SS1 = EE_NODE_SS1 + 1;						// The device's address on the bus (byte)

// Camera Settings
const int EE_CAMFL_SS1 = EE_ADDR_SS1 + 1;						// Camera focal length (mm) (int)

// Motor Settings
const int EE_MOTOR_SPACE_V1_1_SS1 = 12;							// How many addresses to move ahead for each motor when restoring or saving V1.1 and newer motor values
const int EE_DES_SPEED0_SS1 = EE_CAMFL + 2;						// Target speed for motor 0 (float)
const int EE_DES_SMSDIST0_SS1 = EE_DES_SPEED0 + 4;				// Target SMS distance for motor0 (float)
const int EE_EZADJ0_SS1 = EE_DES_SMSDIST0_SS1 + 4;				// Adjust value for EZ mode (float)

// EZ Mode Settings
const int EE_EZMODE_SS1 = EE_EZADJ0_SS1 + (EE_MOTOR_SPACE_V1_1_SS1 * MOTOR_COUNT);	// Whether EZ mode is active (boolean)
const int EE_EZX_SS1 = EE_EZMODE_SS1 + 1;											// Whether the EZ extended / EZ manual mode is enabled (boolean)


/** User Save State 2**/

// MoCoBus Settings 
const int EE_V1_1_SS2_START = 2000;								// Address at which to start storing user saved settings 2
const int EE_NODE_SS2 = EE_INCREMENT_SS2 + 4;					// is the device a node on the MoCoBus?
const int EE_ADDR_SS2 = EE_NODE_SS2 + 1;						// The device's address on the bus (byte)

// Camera Settings
const int EE_CAMFL_SS2 = EE_ADDR_SS2 + 1;						// Camera focal length (mm) (int)

// Motor Settings
const int EE_MOTOR_SPACE_V1_1_SS2 = 12;							// How many addresses to move ahead for each motor when restoring or saving V1.1 and newer motor values
const int EE_DES_SPEED0_SS2 = EE_CAMFL + 2;						// Target speed for motor 0 (float)
const int EE_DES_SMSDIST0_SS2 = EE_DES_SPEED0 + 4;				// Target SMS distance for motor0 (float)
const int EE_EZADJ0_SS2 = EE_DES_SMSDIST0_SS2 + 4;				// Adjust value for EZ mode (float)

// EZ Mode Settings
const int EE_EZMODE_SS2 = EE_EZADJ0_SS2 + (EE_MOTOR_SPACE_V1_1_SS2* MOTOR_COUNT);	// Whether EZ mode is active (boolean)
const int EE_EZX_SS2 = EE_EZMODE_SS2 + 1;											// Whether the EZ extended / EZ manual mode is enabled (boolean)


