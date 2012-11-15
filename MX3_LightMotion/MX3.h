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


  
const byte SHUTTER_PIN = 22;
const byte FOCUS_PIN   = 21;


/*

  Alt I/O Data
  
*/

  // debounce threshold time
const byte ALT_TRIG_THRESH  = 100;
  // first alt i/o pin (digital #)
const byte ALT_START_PIN    = 2;

  // what alt i/o modes do we support?

const byte ALT_OFF       = 0;
const byte ALT_START     = 1;
const byte ALT_STOP      = 2;
const byte ALT_TOGGLE    = 3;
const byte ALT_EXTINT    = 4;
const byte ALT_DIR       = 5;


/*


  Motor Data
  
 */
 
 // define necessary constants
 
#define MOTOR_DRV_PREG      PINB
#define MOTOR_DRV_FMASK     PINB1
#define MOTOR_DIR_PINSTART  7

const byte MOTOR_ENABLE_FLAG  = B10000000;
const byte MOTOR_HIGH_FLAG    = B01000000;
const byte MOTOR_DIR_FLAG     =  B00100000;
const byte MOTOR_ROT_FLAG     = B00010000;
const byte MOTOR_UEN_FLAG     = B00001000;
const byte MOTOR_CDIR_FLAG    = B00000100;

const byte MOTOR_COUNT        = 3;

  // minimum pwm timing period is 100 uS
const float MOTOR_PWM_MINPERIOD  = 100.0;
  // maximum number of periods per minute
const float MOTOR_PWM_MAXPERIOD  = ( 60000000.0 / MOTOR_PWM_MINPERIOD );


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
     (1) B6 = 
     (0) B7 = 
     
     Must be volatile to support B1 modulation in ISR
     */
 volatile byte flags;
 
   /** On Timer Periods (minimum pulse width) */
 unsigned int onPeriods;
 
   /** Off Timer Periods (duty cycle, effectively) */
 unsigned long offPeriods;
 
   /** Error per off period from expected speed */
 float offError;
 
   /** Volatile, used by motor_run_isr for error overflow */
 volatile float error;
 
   /** Volatile, used by motor_run_isr for overflow */
 volatile unsigned int restPeriods;
 
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
 
   /** Default Constructor */
   
 MotorDefinition() {
   flags = 0;
   onPeriods = 0;
   offPeriods = 0;
   restPeriods = 0;
   rpm = 0.0;
   offError = 0.0;
   ratio = 1.0;
   distance = 0;
   ramp = 0;
 }
 
};



/* 


  EEPROM Data
  
*/

 // stored memory layout version
const unsigned int MEMORY_VERSION    = 4;


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
const int EE_CAMBULB   = EE_CAMFOC  + 4; // bulb

const int EE_M0FLAG    = EE_CAMBULB + 1; // flags
const int EE_M0ONP     = EE_M0FLAG  + 1; // on periods
const int EE_M0RPM     = EE_M0ONP   + 2; // rpm
const int EE_M0RATIO   = EE_M0RPM   + 4; // ratio

  // note: for each motor, we move the previous defs ahead 12 bytes * motor num

const int EE_MOTOR_SPACE = 12;  
const int EE_POSTMOTOR   = EE_M0RATIO + 4 + (EE_MOTOR_SPACE * 2);

const int EE_LCDOFF    = EE_POSTMOTOR + 1; // lcd off time
const int EE_METRIC    = EE_LCDOFF + 1; // metric display
const int EE_ALT1      = EE_METRIC + 1; // alt input 1 mode
const int EE_ALT2      = EE_ALT1   + 1; // alt input 2 mode


/*

  UI Data
  
 */
 
#define PAD(x)         if(x < 10) lcd.print('0'); lcd.print(x, DEC);
#define DCT_SIZE(x)    sizeof(x) / sizeof(uiDisplayCursorTarget*)
#define DCT_PTR(x)     reinterpret_cast<void*>(x)


typedef void(*uiTargetFunc)(byte);

 // ui setup
 
    // lcd pins
const byte LCD_RS  = 7;
const byte LCD_EN  = 6;
const byte LCD_D4  = 2;
const byte LCD_D5  = 3;
const byte LCD_D6  = 4;
const byte LCD_D7  = 5;
const byte LCD_BKL = 19;

const byte LCD_ROWS = 2;
const byte LCD_COLS = 16;

 // button values
 
 // which input is our button
const byte BUT_PIN = 23;

  // analog button read values
const int BUTSEL_VAL  = 70;
const int BUTFWD_VAL  = 250;
const int BUTREV_VAL  = 450;
const int BUTDEC_VAL  = 655;
const int BUTINC_VAL  = 830;

const byte BUT_THRESH  = 60;

  // mapping of analog button values for menu
const int BUT_MAP[5][2] = {
                         {BUTFWD_VAL, BUTTON_FORWARD}, 
                         {BUTINC_VAL, BUTTON_INCREASE}, 
                         {BUTDEC_VAL, BUTTON_DECREASE}, 
                         {BUTREV_VAL, BUTTON_BACK}, 
                         {BUTSEL_VAL, BUTTON_SELECT} 
                    };
                            



// ====== Memory Strings Used in the UI ========

const char MX3_VERSTR[]  =  "MX3 v. 0.0.1";
const char MX3_SUBSTR[]  =  "Rabbit Food";
const char MX3_C1STR[]   =  "(c) 2012 Dynamic";
const char MX3_C2STR[]   =  "Perception";
  // run and stop must be exact same length, pad with spaces
const char STR_RUN[]     =  "On ";
const char STR_STOP[]    =  "Off";
const char STR_TIME[]    =  "Time: ";
const char STR_CAM[]     =  "Cam ";
const char STR_BULB[]    =  "B ";
const char STR_ECAM[]    =  "* ";
const char STR_BUSY[]    =  "Busy";
const char STR_IDLE[]    =  "Idle";
const char STR_MOTOR[]   =  "Motor ";
const char STR_LEFT      =  'L';
const char STR_RIGHT     =  'R';
const char STR_CW[]      =  "CW";
const char STR_CCW[]     =  "CCW";


// ====== UI Control Vars ========

const unsigned int UI_REFRESH_TM     = 500;
const byte         UI_SCREEN_MAIN    = 0;
const byte         UI_SCREEN_CAMERA  = 1;
const byte         UI_SCREEN_MOTOR1  = 2;
const byte         UI_SCREEN_MOTOR2  = 3;
const byte         UI_SCREEN_MOTOR3  = 4;


 /** Display Cursor
 
   For cursor activity on screens, this struct defines, and should be used
   or one global variable, the position of the cursor on-screen, and whether
   it is enabled
   */
   
struct uiDisplayCursor {
  byte enabled;
  byte row;
  byte col;
  
};

/** Cursor Target

 The user may cursor around main screens for activities.  These activities are defined
 by cursor targets, which indicate the row and column of the target, and what
 activity to call (callback) when interacted with (up or down key)
 */
 
struct uiDisplayCursorTarget {
 byte row;
 byte col;
 uiTargetFunc func; 
};

 /** Screen Cursor Targets
 
  For every screen, we must define what targets are available on that screen
  */
  
struct uiDisplayCursors {
  byte count;
    /** A void pointer which should resolve to a uiDisplayCursorTarget**, and the length of that
        list must be equivalent to count */
  void* targets;
};

