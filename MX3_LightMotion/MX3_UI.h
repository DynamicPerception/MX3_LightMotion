
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
  Menu Definitions for the UI, placed here to keep
  Clean in the MX3_UI.ino file
  ========================================
  
*/


/*

  UI Data
  
 */
 
  // some macros to make life easier....
 
#define PAD(x)         if(x < 10) lcd.print('0'); lcd.print(x, DEC);
#define PAD3(x)        if(x < 100) lcd.print('0'); PAD(x)
#define DCT_SIZE(x)    sizeof(x) / sizeof(uiDisplayCursorTarget*)
#define DCT_PTR(x)     reinterpret_cast<void*>(x)
 


// ====== Core UI Setup Data (for OMMenuMgr) ==== 
 

const byte LCD_ROWS = 2;
const byte LCD_COLS = 16;

 // button values
 

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
const char MX3_SUBSTR[]  =  "dooF tibbaR";
const char MX3_C1STR[]   =  "(c) 2012 Dynamic";
const char MX3_C2STR[]   =  "Perception";
  // run and stop must be exact same length, pad with spaces
const char STR_RUN[]     =  "On ";
const char STR_STOP[]    =  "Off";
const char STR_TIME[]    =  "Time: ";
const char STR_CAM[]     =  "Cam ";
const char STR_BULB[]    =  "B ";
const char STR_ECAM[]    =  "C ";
const char STR_BUSY[]    =  "Busy";
const char STR_IDLE[]    =  "Idle";
const char STR_MOTOR[]   =  "Motor ";
const char STR_LEFT      =  'L';
const char STR_RIGHT     =  'R';
const char STR_CW        =  '+';
const char STR_CCW       =  '-';
const char STR_BLNK[]    =  "                ";
const char STR_FOC[]     =  "F ";
const char STR_EN[]      =  " On ";
const char STR_DIS[]     =  " Off";
const char STR_INIT[]    =  "Input Init";
const char STR_DONE[]    =  "Done!";
const char STR_RES1[]    =  "Reset In   Sec";
const char STR_RES2[]    =  "Enter to Cancel";
const char STR_RES3[]    =  "Memory Reset";
const char STR_RES4[]    =  "Power-Cycle MX3";
const char STR_DMRK[]    =  "icd";
const char STR_RAMP[]    =  " RMP";

// ====== UI Constant Data ========

const unsigned int UI_REFRESH_TM     = 250;
const byte         UI_SCREEN_MAIN    = 0;
const byte         UI_SCREEN_CAMERA  = 1;
const byte         UI_SCREEN_MOTOR1  = 2;
const byte         UI_SCREEN_MOTOR2  = 3;
const byte         UI_SCREEN_MOTOR3  = 4;



// ====== Cursor (main screen interaction) Data =======

 // function pointer for cursors
typedef void(*uiTargetFunc)(byte);


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


 // cursor targets for each screen

 // main screen
const uiDisplayCursorTarget  ui_ct_main1     = { 0, 0, uiCursorToggleRun };
const uiDisplayCursorTarget  ui_ct_main2     = { 0, 4, uiCursorAdjustInt };
const uiDisplayCursorTarget* ui_dctl_main[]  = { &ui_ct_main1, &ui_ct_main2 };
const uiDisplayCursors       ui_dc_main      = { DCT_SIZE(ui_dctl_main), DCT_PTR(&ui_dctl_main) };

 // camera screen
const uiDisplayCursorTarget  ui_ct_cam1      = { 1, 0, uiCursorChangeCamBulb };
const uiDisplayCursorTarget  ui_ct_cam2      = { 1, 2, uiCursorChangeShutterTime };
const uiDisplayCursorTarget  ui_ct_cam3      = { 1, 11, uiCursorChangeFocusTime };
const uiDisplayCursorTarget* ui_dctl_cam[]   = { &ui_ct_cam1, &ui_ct_cam2, &ui_ct_cam3 };
const uiDisplayCursors       ui_dc_cam       = { DCT_SIZE(ui_dctl_cam), DCT_PTR(&ui_dctl_cam) };

 // motor screens
const uiDisplayCursorTarget  ui_ct_mot1      = { 0, 8, uiCursorChangeMotEn };
const uiDisplayCursorTarget  ui_ct_mot2      = { 1, 0, uiCursorChangeMotDir };
const uiDisplayCursorTarget  ui_ct_mot3      = { 1, 2, uiCursorChangeMotSpd };
const uiDisplayCursorTarget  ui_ct_mot4      = { 0, 13, uiCursorChangeMotLead };
const uiDisplayCursorTarget  ui_ct_mot5      = { 1, 13, uiCursorChangeMotRamp };

const uiDisplayCursorTarget* ui_dctl_mot[]   = { &ui_ct_mot1, &ui_ct_mot2, &ui_ct_mot3, &ui_ct_mot4, &ui_ct_mot5 };
const uiDisplayCursors       ui_dc_mot       = { DCT_SIZE(ui_dctl_mot), DCT_PTR(&ui_dctl_mot) };

 // screens with no targets
const uiDisplayCursors ui_dc_none = { 0, 0 };

 // all screen cursors...
 
    // main, camera, m1, m2, m3
const uiDisplayCursors*  ui_dc_list[] = { &ui_dc_main, &ui_dc_cam, &ui_dc_mot, &ui_dc_mot, &ui_dc_mot };


  // camera shutter speed display divisions
const float ui_camDivs[]  = { 0.001, 0.002, 0.004, 0.008, 0.01, 0.0166667, 0.033333, 0.0666667, 0.125, 0.25,  0.333333, 0.5, 0.75, 1.0, 1.3, 1.6, 2 };


// ====== Menu Data =========== 

  // Create a list of states and values for a select input
MENU_SELECT_ITEM   ui_sel_on  = { 1, {"On"} };
MENU_SELECT_ITEM   ui_sel_off = { 0, {"Off"} };
 
 // list of alt modes
 
MENU_SELECT_ITEM  ui_sel_altstart = { ALT_START, {"Start"} };
MENU_SELECT_ITEM  ui_sel_altstop  = { ALT_STOP, {"Stop"} };
MENU_SELECT_ITEM  ui_sel_alttog   = { ALT_TOGGLE, {"Toggle"} };
MENU_SELECT_ITEM  ui_sel_altext   = { ALT_EXTINT, {"Ext. Int"} };
MENU_SELECT_ITEM  ui_sel_altdir   = { ALT_DIR, {"Dir."} };

MENU_SELECT_LIST  ui_sel_list_onoff[] = { &ui_sel_off, &ui_sel_on };
MENU_SELECT_LIST  ui_sel_list_alt[]   = { &ui_sel_off, &ui_sel_altstart, &ui_sel_altstop, &ui_sel_alttog, &ui_sel_altext, &ui_sel_altdir };


  // ===== Camera Menu 

MENU_SELECT  ui_sl_camBulb     = { &camera_bulb, MENU_SELECT_SIZE(ui_sel_list_onoff), MENU_TARGET(&ui_sel_list_onoff) };

MENU_VALUE   ui_in_camMaxShots = { TYPE_UINT, 0, 0, MENU_TARGET(&camera_max_shots), EE_MAXSHOT };
MENU_VALUE   ui_in_camRepeat   = { TYPE_BYTE, 0, 0, MENU_TARGET(&camera_repeat), EE_CAMREP };
MENU_VALUE   ui_in_camBulb     = { TYPE_SELECT, 0, 0, MENU_TARGET(&ui_sl_camBulb), EE_CAMBULB };
MENU_VALUE   ui_in_camExposure = { TYPE_ULONG, 0, 0, MENU_TARGET(&camera_exposure), EE_CAMEXP };
MENU_VALUE   ui_in_camFocus    = { TYPE_ULONG, 0, 0, MENU_TARGET(&camera_focus), EE_CAMFOC };
MENU_VALUE   ui_in_camWait     = { TYPE_ULONG, 0, 0, MENU_TARGET(&camera_wait), EE_CAMWAIT };
MENU_VALUE   ui_in_camDelay    = { TYPE_FLOAT_10, 0, 0, MENU_TARGET(&camera_delay), EE_CAMDEL };

MENU_ITEM    ui_it_camMaxShot  = { {"Max Shots"}, ITEM_VALUE, 0, MENU_TARGET(&ui_in_camMaxShots) };
MENU_ITEM    ui_it_camRepeat   = { {"Repeat Shots"}, ITEM_VALUE, 0, MENU_TARGET(&ui_in_camRepeat) };
MENU_ITEM    ui_it_camBulb     = { {"Bulb Exposure"}, ITEM_VALUE, 0, MENU_TARGET(&ui_in_camBulb) };
MENU_ITEM    ui_it_camExposure = { {"Exp. Time  mS"}, ITEM_VALUE, 0, MENU_TARGET(&ui_in_camExposure) };
MENU_ITEM    ui_it_camFocus    = { {"Focus      mS"}, ITEM_VALUE, 0, MENU_TARGET(&ui_in_camFocus) };
MENU_ITEM    ui_it_camWait     = { {"Exp Delay  mS"}, ITEM_VALUE, 0, MENU_TARGET(&ui_in_camWait) };
MENU_ITEM    ui_it_camDelay    = { {"Interval   Sec"}, ITEM_VALUE, 0, MENU_TARGET(&ui_in_camDelay) };

MENU_LIST    ui_list_cam[]     = { &ui_it_camDelay, &ui_it_camMaxShot, &ui_it_camBulb, &ui_it_camExposure, &ui_it_camWait, &ui_it_camFocus, &ui_it_camRepeat };

MENU_ITEM    ui_it_camList     = { {"Camera"}, ITEM_MENU, MENU_SIZE(ui_list_cam), MENU_TARGET(&ui_list_cam) };


  // ======= Motor Menus

  // motor 1 inputs
  
MENU_FLAG    ui_flag_m0_rot    = { 4, (byte*) &motors[0].flags };
MENU_FLAG    ui_flag_m0_flip   = { 5, (byte*) &motors[0].flags };

MENU_VALUE   ui_in_m0_rot      = { TYPE_BFLAG, 0, 0, MENU_TARGET(&ui_flag_m0_rot), EE_M0FLAG };
MENU_VALUE   ui_in_m0_flip     = { TYPE_BFLAG, 0, 0, MENU_TARGET(&ui_flag_m0_flip), EE_M0FLAG };
MENU_VALUE   ui_in_m0_rpm      = { TYPE_FLOAT_100, 500, 0, MENU_TARGET(&motors[0].rpm), EE_M0RPM };
MENU_VALUE   ui_in_m0_ratio    = { TYPE_FLOAT_100, 5000, 0, MENU_TARGET(&motors[0].ratio), EE_M0RATIO };

MENU_ITEM    ui_it_m0_rot      = { {"Rotary"}, ITEM_VALUE, 0, MENU_TARGET(&ui_in_m0_rot) };
MENU_ITEM    ui_it_m0_flip     = { {"Invert Dir"}, ITEM_VALUE, 0, MENU_TARGET(&ui_in_m0_flip) };
MENU_ITEM    ui_it_m0_rpm      = { {"RPM"}, ITEM_VALUE, 0, MENU_TARGET(&ui_in_m0_rpm) };
MENU_ITEM    ui_it_m0_ratio    = { {"Ratio"}, ITEM_VALUE, 0, MENU_TARGET(&ui_in_m0_ratio) };

MENU_LIST    ui_list_m0[]      = { &ui_it_m0_rot, &ui_it_m0_flip, &ui_it_m0_rpm, &ui_it_m0_ratio };
MENU_ITEM    ui_it_m0List      = { {"Axis 1"}, ITEM_MENU, MENU_SIZE(ui_list_m0), MENU_TARGET(&ui_list_m0) };

  // motor 2 inputs
  
MENU_FLAG    ui_flag_m1_rot    = { 4, (byte*) &motors[1].flags };
MENU_FLAG    ui_flag_m1_flip   = { 5, (byte*) &motors[1].flags };

MENU_VALUE   ui_in_m1_rot      = { TYPE_BFLAG, 0, 0, MENU_TARGET(&ui_flag_m1_rot), EE_M0FLAG + EE_MOTOR_SPACE};
MENU_VALUE   ui_in_m1_flip     = { TYPE_BFLAG, 0, 0, MENU_TARGET(&ui_flag_m1_flip), EE_M0FLAG + EE_MOTOR_SPACE};
MENU_VALUE   ui_in_m1_rpm      = { TYPE_FLOAT_100, 500, 0, MENU_TARGET(&motors[1].rpm), EE_M0RPM + EE_MOTOR_SPACE};
MENU_VALUE   ui_in_m1_ratio    = { TYPE_FLOAT_100, 5000, 0, MENU_TARGET(&motors[1].ratio), EE_M0RATIO + EE_MOTOR_SPACE};

MENU_ITEM    ui_it_m1_rot      = { {"Rotary"}, ITEM_VALUE, 0, MENU_TARGET(&ui_in_m1_rot) };
MENU_ITEM    ui_it_m1_flip     = { {"Invert Dir"}, ITEM_VALUE, 0, MENU_TARGET(&ui_in_m1_flip) };
MENU_ITEM    ui_it_m1_rpm      = { {"RPM"}, ITEM_VALUE, 0, MENU_TARGET(&ui_in_m1_rpm) };
MENU_ITEM    ui_it_m1_ratio    = { {"Ratio"}, ITEM_VALUE, 0, MENU_TARGET(&ui_in_m1_ratio) };

MENU_LIST    ui_list_m1[]      = { &ui_it_m1_rot, &ui_it_m1_flip, &ui_it_m1_rpm, &ui_it_m1_ratio };
MENU_ITEM    ui_it_m1List      = { {"Axis 2"}, ITEM_MENU, MENU_SIZE(ui_list_m1), MENU_TARGET(&ui_list_m1) };

  // motor 2 inputs
  
MENU_FLAG    ui_flag_m2_rot    = { 4, (byte*) &motors[2].flags };
MENU_FLAG    ui_flag_m2_flip   = { 5, (byte*) &motors[2].flags };

MENU_VALUE   ui_in_m2_rot      = { TYPE_BFLAG, 0, 0, MENU_TARGET(&ui_flag_m2_rot), EE_M0FLAG + EE_MOTOR_SPACE * 2};
MENU_VALUE   ui_in_m2_flip     = { TYPE_BFLAG, 0, 0, MENU_TARGET(&ui_flag_m2_flip), EE_M0FLAG + EE_MOTOR_SPACE * 2};
MENU_VALUE   ui_in_m2_rpm      = { TYPE_FLOAT_100, 500, 0, MENU_TARGET(&motors[2].rpm), EE_M0RPM + EE_MOTOR_SPACE * 2};
MENU_VALUE   ui_in_m2_ratio    = { TYPE_FLOAT_100, 5000, 0, MENU_TARGET(&motors[2].ratio), EE_M0RATIO + EE_MOTOR_SPACE * 2};

MENU_ITEM    ui_it_m2_rot      = { {"Rotary"}, ITEM_VALUE, 0, MENU_TARGET(&ui_in_m2_rot) };
MENU_ITEM    ui_it_m2_flip     = { {"Invert Dir"}, ITEM_VALUE, 0, MENU_TARGET(&ui_in_m2_flip) };
MENU_ITEM    ui_it_m2_rpm      = { {"RPM"}, ITEM_VALUE, 0, MENU_TARGET(&ui_in_m2_rpm) };
MENU_ITEM    ui_it_m2_ratio    = { {"Ratio"}, ITEM_VALUE, 0, MENU_TARGET(&ui_in_m2_ratio) };

MENU_LIST    ui_list_m2[]      = { &ui_it_m2_rot, &ui_it_m2_flip, &ui_it_m2_rpm, &ui_it_m2_ratio };
MENU_ITEM    ui_it_m2List      = { {"Axis 3"}, ITEM_MENU, MENU_SIZE(ui_list_m2), MENU_TARGET(&ui_list_m2) };


 // Create motors sub-menu
MENU_LIST    ui_list_motors[]  = { &ui_it_m0List, &ui_it_m1List, &ui_it_m2List };
MENU_ITEM    ui_it_motors      = { {"Motors"}, ITEM_MENU, MENU_SIZE(ui_list_motors), MENU_TARGET(&ui_list_motors) };


 // ===== Alt Input Options
 
MENU_SELECT  ui_sl_alt1     = { &alt_inputs[0], MENU_SELECT_SIZE(ui_sel_list_alt), MENU_TARGET(&ui_sel_list_alt) };
MENU_SELECT  ui_sl_alt2     = { &alt_inputs[1], MENU_SELECT_SIZE(ui_sel_list_alt), MENU_TARGET(&ui_sel_list_alt) };

MENU_VALUE   ui_in_alt1     = { TYPE_SELECT, 0, 0, MENU_TARGET(&ui_sl_alt1), EE_ALT1 };
MENU_VALUE   ui_in_alt2     = { TYPE_SELECT, 0, 0, MENU_TARGET(&ui_sl_alt2), EE_ALT2 };

MENU_ITEM    ui_it_alt1     = { {"I/O #1 Mode"}, ITEM_VALUE, 0, MENU_TARGET(&ui_in_alt1) };
MENU_ITEM    ui_it_alt2     = { {"I/O #2 Mode"}, ITEM_VALUE, 0, MENU_TARGET(&ui_in_alt1) };
MENU_ITEM    ui_it_altset   = { {"Init I/O"}, ITEM_ACTION, 0, MENU_TARGET(uiMenuAltInit) };

MENU_LIST    ui_list_alt[]  = { &ui_it_altset, &ui_it_alt1, &ui_it_alt2 };
MENU_ITEM    ui_it_alt      = { {"Alt I/O"}, ITEM_MENU, MENU_SIZE(ui_list_alt), MENU_TARGET(&ui_list_alt) };

 // ===== Global Options

MENU_SELECT  ui_sl_glLCD       = { &motion_sms, MENU_SELECT_SIZE(ui_sel_list_onoff), MENU_TARGET(&ui_sel_list_onoff) };
MENU_SELECT  ui_sl_glMet       = { &metric_ui, MENU_SELECT_SIZE(ui_sel_list_onoff), MENU_TARGET(&ui_sel_list_onoff) };

MENU_VALUE   ui_in_glLCD       = { TYPE_BYTE, 0, 0, MENU_TARGET(&lcdDisable), EE_LCDOFF };   
MENU_VALUE   ui_in_glSMS       = { TYPE_SELECT, 0, 0, MENU_TARGET(&ui_sl_glLCD), EE_SMS };
MENU_VALUE   ui_in_glMet       = { TYPE_SELECT, 0, 0, MENU_TARGET(&ui_sl_glMet), EE_METRIC };
MENU_VALUE   ui_in_glPer       = { TYPE_UINT, 1000, 50, MENU_TARGET(&motor_pwm_minperiod), EE_PERIOD };   

MENU_ITEM    ui_it_glSMS       = { {"SMS Motion"}, ITEM_VALUE, 0, MENU_TARGET(&ui_in_glSMS) };
MENU_ITEM    ui_it_glMet       = { {"Metric Display"}, ITEM_VALUE, 0, MENU_TARGET(&ui_in_glMet) };
MENU_ITEM    ui_it_glLCD       = { {"LCD AutoOff Sec"}, ITEM_VALUE, 0, MENU_TARGET(&ui_in_glLCD) };
MENU_ITEM    ui_it_glPer       = { {"Motor Timing"}, ITEM_VALUE, 0, MENU_TARGET(&ui_in_glPer) };
MENU_ITEM    ui_it_glMem       = { {"Reset Memory"}, ITEM_ACTION, 0, MENU_TARGET(uiMenuResetMem) };


MENU_LIST    ui_list_gl[]      = { &ui_it_glSMS, &ui_it_glMet, &ui_it_glLCD, &ui_it_alt, &ui_it_glPer, &ui_it_glMem };
MENU_ITEM    ui_it_glList      = { {"Settings"}, ITEM_MENU, MENU_SIZE(ui_list_gl), MENU_TARGET(&ui_list_gl) };

 // ===== Main Menu
 
 // Create top-level menu
MENU_LIST    ui_list_top[]     = { &ui_it_camList, &ui_it_motors, &ui_it_glList };

                  // Root item is always created last, so we can add all other items to it
MENU_ITEM    ui_it_root        = { {"Root"},        ITEM_MENU,   MENU_SIZE(ui_list_top),    MENU_TARGET(&ui_list_top) };


