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
  Menu Definitions for the UI, placed here to keep
  Clean in the MX3_UI.ino file
  ========================================
  
*/


/*

  UI Data
  
 */
 
  // some macros to make life easier....


#define DCT_SIZE(x)    sizeof(x) / sizeof(uiDisplayCursorTarget*)
#define DCT_PTR(x)     reinterpret_cast<void*>(x)  
  

// ====== Core UI Setup Data (for OMMenuMgr) ==== 
 

const byte LCD_ROWS = 2;
const byte LCD_COLS = 16;

 // button values
 

  // analog button read values
const int BUTSEL_VAL  = 75;   //old values: 40
const int BUTFWD_VAL  = 236;  //old values: 255
const int BUTREV_VAL  = 443;  //old values: 461
const int BUTDEC_VAL  = 636;  //old values: 646
const int BUTINC_VAL  = 831;  //old values: 831
 // threshold range for reading analog values
const byte BUT_THRESH  = 75;  //old values: 30

  // mapping of analog button values for menu
const int BUT_MAP[5][2] = {
                         {BUTFWD_VAL, BUTTON_FORWARD}, 
                         {BUTINC_VAL, BUTTON_INCREASE}, 
                         {BUTDEC_VAL, BUTTON_DECREASE}, 
                         {BUTREV_VAL, BUTTON_BACK}, 
                         {BUTSEL_VAL, BUTTON_SELECT} 
                    };
                            



// ====== Memory Strings Used in the UI ========

const char MX3_VERSTR[]  =  "LightMotion";
const char MX3_SUBSTR[]  =  "v.1.05 Badger   ";
const char MX3_C1STR[]   =  "(c) 2013 Dynamic";
const char MX3_C2STR[]   =  "Perception";
  // run, stop, and ext must be exact same length, pad with spaces
const char STR_RUN[]     =  "On ";
const char STR_STOP[]    =  "Off";
const char STR_EXT[]     =  "Ext";
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
  // en, dis, and ramp must be same length
const char STR_EN[]      =  " On ";
const char STR_DIS[]     =  " Off";
const char STR_RAMP[]    =  " RMP";
const char STR_INIT[]    =  "Input Init";
const char STR_DONE[]    =  "Done!";
const char STR_RES1[]    =  "Reset In   Sec";
const char STR_RES2[]    =  "Enter to Cancel";
const char STR_RES3[]    =  "Memory Reset";
const char STR_RES4[]    =  "Power-Cycle MX3";
const char STR_SAV1[]    =  "Save Memory";
const char STR_SAV2[]    =  "Slot: ";
const char STR_SAV3[]    =  "Memory Saved!";
const char STR_SAV4[]    =  "Press Enter";
const char STR_LOD1[]    =  "Load From Memory";
const char STR_LOD2[]    =  "Slot: ";
const char STR_LOD3[]    =  "Memory Loaded";
const char STR_LOD4[]    =  "Press Enter";
const char STR_SMS[]     =  "SMS ";
const char STR_CONT[]    =  "Cont";
const char STR_WARN1[]   =  "Unavailable     ";
const char STR_WARN2[]   =  "Stop First      ";
const char STR_DOT       =  '.';
const char STR_QUOTE     =  '"';
const char STR_DIV[]     =  "1/";
const char STR_RA        =  '>';
const char STR_UA        =  '^';
const char STR_SPACE     =  ' ';
const char STR_SQUOTE    =  '\'';
const char STR_SEC[]     =  "s ";
const char STR_PRESET[]  =  "Motor Preset";
const char STR_MTEMP[]   =  "Temp. Motor ";
const char STR_MCURR[]   =  "Motor Current";
const char STR_MVOLT[]   =  "Voltage Level";
const char STR_MA[]      =  " mA";
const char STR_V[]       =  " V";
const char STR_AST       =  '*';
const char STR_BRI1[]    =  "VFD Brightness";
const char STR_BRI2[]    =  "%: ";
const char STR_BRI3[]    =  "VFD Changed";
const char STR_BRI4[]    =  "Press Enter";



// ====== UI Constant Data ========

const unsigned int UI_REFRESH_TM     = 333;
const byte         UI_SCREEN_MAIN    = 0;
const byte         UI_SCREEN_CAMERA  = 1;
const byte         UI_SCREEN_MOTOR1  = 2;
const byte         UI_SCREEN_MOTOR2  = 3;
const byte         UI_SCREEN_MOTOR3  = 4;
const unsigned int UI_ERR_REFRESH_TM = 5000;
const unsigned int UI_ERR_BLINK_TM   = 1000;



  // camera shutter speed display divisions
const float UI_CAMDIVS[]  = { 0.001, 0.002, 0.004, 0.008, 0.01, 0.0166667, 0.033333, 0.0666667, 0.125, 0.25,  0.333333, 0.5, 0.75, 1.0, 1.3, 1.6, 2 };
const byte  UI_CAMDIVSIZE = ( sizeof(UI_CAMDIVS) / sizeof(float) ) - 1;

// ====== Menu Data =========== 

  // Create a list of states and values for a select input
MENU_SELECT_ITEM   ui_sel_on  = { 1, {"On"} };
MENU_SELECT_ITEM   ui_sel_off = { 0, {"Off"} };
 
 // list of alt modes
 
MENU_SELECT_ITEM  ui_sel_altstart = { ALT_START,  {"Start"} };
MENU_SELECT_ITEM  ui_sel_altstop  = { ALT_STOP,   {"Stop"} };
MENU_SELECT_ITEM  ui_sel_alttog   = { ALT_TOGGLE, {"Toggle"} };
MENU_SELECT_ITEM  ui_sel_altext   = { ALT_EXTINT, {"Ext. Int"} };
MENU_SELECT_ITEM  ui_sel_altdir   = { ALT_DIR,    {"Dir."} };
MENU_SELECT_ITEM  ui_sel_altoutb  = { ALT_OUT_B,  {"Out B4"} };
MENU_SELECT_ITEM  ui_sel_altouta  = { ALT_OUT_A,  {"Out Aft"} };

MENU_SELECT_ITEM ui_sel_falling = { FALLING, {"Falling"} };
MENU_SELECT_ITEM ui_sel_rising  = { RISING,  {"Rising"} };

MENU_SELECT_ITEM ui_sel_high = { HIGH, {"High"} };
MENU_SELECT_ITEM ui_sel_low  = { LOW,  {"Low"} };

MENU_SELECT_LIST  ui_sel_list_onoff[]  = { &ui_sel_off, &ui_sel_on };
MENU_SELECT_LIST  ui_sel_list_alt[]    = { &ui_sel_off, &ui_sel_altstart, &ui_sel_altstop, &ui_sel_alttog, &ui_sel_altext, &ui_sel_altdir, &ui_sel_altoutb, &ui_sel_altouta };
MENU_SELECT_LIST  ui_sel_list_altOut[] = { &ui_sel_off, &ui_sel_altoutb, &ui_sel_altouta };
MENU_SELECT_LIST  ui_sel_list_dir[]    = { &ui_sel_falling, &ui_sel_rising };
MENU_SELECT_LIST  ui_sel_list_hilo[]    = { &ui_sel_high, &ui_sel_low };

  // ===== Camera Menu 

MENU_SELECT  ui_sl_camBulb     = { &camera_bulb,    MENU_SELECT_SIZE(ui_sel_list_onoff), MENU_TARGET(&ui_sel_list_onoff) };
MENU_SELECT  ui_sl_camLock     = { &camera_focLock, MENU_SELECT_SIZE(ui_sel_list_onoff), MENU_TARGET(&ui_sel_list_onoff) };

MENU_VALUE   ui_in_camMaxShots = { TYPE_UINT,             0,           0, MENU_TARGET(&camera_max_shots), EE_MAXSHOT };
MENU_VALUE   ui_in_camRepeat   = { TYPE_BYTE,             0,           0, MENU_TARGET(&camera_repeat),    EE_CAMREP };
MENU_VALUE   ui_in_camBulb     = { TYPE_SELECT,           0,           0, MENU_TARGET(&ui_sl_camBulb),    EE_CAMBULB };
MENU_VALUE   ui_in_camLock     = { TYPE_SELECT,           0,           0, MENU_TARGET(&ui_sl_camLock),    EE_CAMLOCK };
MENU_VALUE   ui_in_camExposure = { TYPE_ULONG,   CAMEXP_MAX,  CAMEXP_MIN, MENU_TARGET(&camera_exposure),  EE_CAMEXP };
MENU_VALUE   ui_in_camFocus    = { TYPE_ULONG,   CAMFOC_MAX,  CAMFOC_MIN, MENU_TARGET(&camera_focus),     EE_CAMFOC };
MENU_VALUE   ui_in_camWait     = { TYPE_ULONG,  CAMWAIT_MAX, CAMWAIT_MIN, MENU_TARGET(&camera_wait),      EE_CAMWAIT };
MENU_VALUE   ui_in_camDelay    = { TYPE_FLOAT_10,         0,           0, MENU_TARGET(&camera_delay),     EE_CAMDEL };

MENU_ITEM    ui_it_camMaxShot  = { {"Max Shots"},      ITEM_VALUE, 0, MENU_TARGET(&ui_in_camMaxShots) };
MENU_ITEM    ui_it_camRepeat   = { {"Repeat Shots"},   ITEM_VALUE, 0, MENU_TARGET(&ui_in_camRepeat) };
MENU_ITEM    ui_it_camBulb     = { {"Bulb Exposure"},  ITEM_VALUE, 0, MENU_TARGET(&ui_in_camBulb) };
MENU_ITEM    ui_it_camExposure = { {"Exp. Time   mS"}, ITEM_VALUE, 0, MENU_TARGET(&ui_in_camExposure) };
MENU_ITEM    ui_it_camFocus    = { {"Focus       mS"}, ITEM_VALUE, 0, MENU_TARGET(&ui_in_camFocus) };
MENU_ITEM    ui_it_camWait     = { {"Exp Delay   mS"}, ITEM_VALUE, 0, MENU_TARGET(&ui_in_camWait) };
MENU_ITEM    ui_it_camDelay    = { {"Interval   Sec"}, ITEM_VALUE, 0, MENU_TARGET(&ui_in_camDelay) };
MENU_ITEM    ui_it_camLock     = { {"Focus Lock"},     ITEM_VALUE, 0, MENU_TARGET(&ui_in_camLock) };

MENU_LIST    ui_list_cam[]     = { &ui_it_camDelay, &ui_it_camMaxShot, &ui_it_camBulb, &ui_it_camExposure, &ui_it_camWait, &ui_it_camFocus, &ui_it_camRepeat, &ui_it_camLock };

MENU_ITEM    ui_it_camList     = { {"Camera"}, ITEM_MENU, MENU_SIZE(ui_list_cam), MENU_TARGET(&ui_list_cam) };


  // ======= Motor Menus

  // motor 1 inputs
  
MENU_FLAG    ui_flag_m0_rot    = { 4, (byte*) &motors[0].flags };
MENU_FLAG    ui_flag_m0_flip   = { 5, (byte*) &motors[0].flags };

MENU_VALUE   ui_in_m0_rot      = { TYPE_BFLAG,      0,    0, MENU_TARGET(&ui_flag_m0_rot),  EE_M0FLAG };
MENU_VALUE   ui_in_m0_flip     = { TYPE_BFLAG,      0,    0, MENU_TARGET(&ui_flag_m0_flip), EE_M0FLAG };
MENU_VALUE   ui_in_m0_rpm      = { TYPE_FLOAT_100,  1000, 0, MENU_TARGET(&motors[0].rpm),   EE_M0RPM };
MENU_VALUE   ui_in_m0_ratio    = { TYPE_FLOAT_1000, 5000, 0, MENU_TARGET(&motors[0].ratio), EE_M0RATIO };

MENU_ITEM    ui_it_m0_pres     = { {"Motor Preset"}, ITEM_ACTION, 0, MENU_TARGET(uiMenuPresetOne) };
MENU_ITEM    ui_it_m0_man      = { {"Manual Move"},  ITEM_ACTION, 0, MENU_TARGET(uiMenuManualOne) };
MENU_ITEM    ui_it_m0_rot      = { {"Rotary"},       ITEM_VALUE,  0, MENU_TARGET(&ui_in_m0_rot) };
MENU_ITEM    ui_it_m0_flip     = { {"Invert Dir"},   ITEM_VALUE,  0, MENU_TARGET(&ui_in_m0_flip) };
MENU_ITEM    ui_it_m0_rpm      = { {"RPM"},          ITEM_VALUE,  0, MENU_TARGET(&ui_in_m0_rpm) };
MENU_ITEM    ui_it_m0_ratio    = { {"Ratio"},        ITEM_VALUE,  0, MENU_TARGET(&ui_in_m0_ratio) };

MENU_LIST    ui_list_m0[]      = { &ui_it_m0_pres, &ui_it_m0_man, &ui_it_m0_rot, &ui_it_m0_flip, &ui_it_m0_rpm, &ui_it_m0_ratio };
MENU_ITEM    ui_it_m0List      = { {"Axis 1"}, ITEM_MENU, MENU_SIZE(ui_list_m0), MENU_TARGET(&ui_list_m0) };

  // motor 2 inputs
  
MENU_FLAG    ui_flag_m1_rot    = { 4, (byte*) &motors[1].flags };
MENU_FLAG    ui_flag_m1_flip   = { 5, (byte*) &motors[1].flags };

MENU_VALUE   ui_in_m1_rot      = { TYPE_BFLAG,      0,    0, MENU_TARGET(&ui_flag_m1_rot),  EE_M0FLAG  + EE_MOTOR_SPACE };
MENU_VALUE   ui_in_m1_flip     = { TYPE_BFLAG,      0,    0, MENU_TARGET(&ui_flag_m1_flip), EE_M0FLAG  + EE_MOTOR_SPACE };
MENU_VALUE   ui_in_m1_rpm      = { TYPE_FLOAT_100,  1000, 0, MENU_TARGET(&motors[1].rpm),   EE_M0RPM   + EE_MOTOR_SPACE };
MENU_VALUE   ui_in_m1_ratio    = { TYPE_FLOAT_1000, 5000, 0, MENU_TARGET(&motors[1].ratio), EE_M0RATIO + EE_MOTOR_SPACE };

MENU_ITEM    ui_it_m1_pres     = { {"Motor Preset"}, ITEM_ACTION, 0, MENU_TARGET(uiMenuPresetTwo) };
MENU_ITEM    ui_it_m1_man      = { {"Manual Move"},  ITEM_ACTION, 0, MENU_TARGET(uiMenuManualTwo) };
MENU_ITEM    ui_it_m1_rot      = { {"Rotary"},       ITEM_VALUE,  0, MENU_TARGET(&ui_in_m1_rot) };
MENU_ITEM    ui_it_m1_flip     = { {"Invert Dir"},   ITEM_VALUE,  0, MENU_TARGET(&ui_in_m1_flip) };
MENU_ITEM    ui_it_m1_rpm      = { {"RPM"},          ITEM_VALUE,  0, MENU_TARGET(&ui_in_m1_rpm) };
MENU_ITEM    ui_it_m1_ratio    = { {"Ratio"},        ITEM_VALUE,  0, MENU_TARGET(&ui_in_m1_ratio) };

MENU_LIST    ui_list_m1[]      = { &ui_it_m1_pres, &ui_it_m1_man, &ui_it_m1_rot, &ui_it_m1_flip, &ui_it_m1_rpm, &ui_it_m1_ratio };
MENU_ITEM    ui_it_m1List      = { {"Axis 2"}, ITEM_MENU, MENU_SIZE(ui_list_m1), MENU_TARGET(&ui_list_m1) };

  // motor 3 inputs
  
MENU_FLAG    ui_flag_m2_rot    = { 4, (byte*) &motors[2].flags };
MENU_FLAG    ui_flag_m2_flip   = { 5, (byte*) &motors[2].flags };

MENU_VALUE   ui_in_m2_rot      = { TYPE_BFLAG,      0,    0, MENU_TARGET(&ui_flag_m2_rot),  EE_M0FLAG  + EE_MOTOR_SPACE * 2 };
MENU_VALUE   ui_in_m2_flip     = { TYPE_BFLAG,      0,    0, MENU_TARGET(&ui_flag_m2_flip), EE_M0FLAG  + EE_MOTOR_SPACE * 2 };
MENU_VALUE   ui_in_m2_rpm      = { TYPE_FLOAT_100,  1000, 0, MENU_TARGET(&motors[2].rpm),   EE_M0RPM   + EE_MOTOR_SPACE * 2 };
MENU_VALUE   ui_in_m2_ratio    = { TYPE_FLOAT_1000, 5000, 0, MENU_TARGET(&motors[2].ratio), EE_M0RATIO + EE_MOTOR_SPACE * 2 };

MENU_ITEM    ui_it_m2_pres     = { {"Motor Preset"}, ITEM_ACTION, 0, MENU_TARGET(uiMenuPresetThree) };
MENU_ITEM    ui_it_m2_man      = { {"Manual Move"},  ITEM_ACTION, 0, MENU_TARGET(uiMenuManualThree) };
MENU_ITEM    ui_it_m2_rot      = { {"Rotary"},       ITEM_VALUE,  0, MENU_TARGET(&ui_in_m2_rot) };
MENU_ITEM    ui_it_m2_flip     = { {"Invert Dir"},   ITEM_VALUE,  0, MENU_TARGET(&ui_in_m2_flip) };
MENU_ITEM    ui_it_m2_rpm      = { {"RPM"},          ITEM_VALUE,  0, MENU_TARGET(&ui_in_m2_rpm) };
MENU_ITEM    ui_it_m2_ratio    = { {"Ratio"},        ITEM_VALUE,  0, MENU_TARGET(&ui_in_m2_ratio) };

MENU_LIST    ui_list_m2[]      = { &ui_it_m2_pres, &ui_it_m2_man, &ui_it_m2_rot, &ui_it_m2_flip, &ui_it_m2_rpm, &ui_it_m2_ratio };
MENU_ITEM    ui_it_m2List      = { {"Axis 3"}, ITEM_MENU, MENU_SIZE(ui_list_m2), MENU_TARGET(&ui_list_m2) };


 // Create motors sub-menu
MENU_LIST    ui_list_motors[]  = { &ui_it_m0List, &ui_it_m1List, &ui_it_m2List };
MENU_ITEM    ui_it_motors      = { {"Motors"}, ITEM_MENU, MENU_SIZE(ui_list_motors), MENU_TARGET(&ui_list_motors) };


 // ===== Alt Input Options
 
MENU_SELECT  ui_sl_alt1     = { &alt_inputs[1], MENU_SELECT_SIZE(ui_sel_list_alt),    MENU_TARGET(&ui_sel_list_alt) };
MENU_SELECT  ui_sl_alt2     = { &alt_inputs[0], MENU_SELECT_SIZE(ui_sel_list_alt),    MENU_TARGET(&ui_sel_list_alt) };
MENU_SELECT  ui_sl_alt3     = { &alt_inputs[2], MENU_SELECT_SIZE(ui_sel_list_alt),    MENU_TARGET(&ui_sel_list_alt) };
MENU_SELECT  ui_sl_alt4     = { &alt_inputs[3], MENU_SELECT_SIZE(ui_sel_list_altOut), MENU_TARGET(&ui_sel_list_altOut) };
MENU_SELECT  ui_sl_altdir   = { &alt_direction, MENU_SELECT_SIZE(ui_sel_list_dir),    MENU_TARGET(&ui_sel_list_dir) };
MENU_SELECT  ui_sl_althl    = { &alt_out_trig,  MENU_SELECT_SIZE(ui_sel_list_hilo),   MENU_TARGET(&ui_sel_list_hilo) };

MENU_VALUE   ui_in_alt1     = { TYPE_SELECT, 0, 0, MENU_TARGET(&ui_sl_alt1),       EE_ALT1 };
MENU_VALUE   ui_in_alt2     = { TYPE_SELECT, 0, 0, MENU_TARGET(&ui_sl_alt2),       EE_ALT2 };
MENU_VALUE   ui_in_alt3     = { TYPE_SELECT, 0, 0, MENU_TARGET(&ui_sl_alt3),       EE_ALT3 };
MENU_VALUE   ui_in_alt4     = { TYPE_SELECT, 0, 0, MENU_TARGET(&ui_sl_alt4),       EE_ALT4 };
MENU_VALUE   ui_in_altdir   = { TYPE_SELECT, 0, 0, MENU_TARGET(&ui_sl_altdir),     EE_ALTDIR };
MENU_VALUE   ui_in_althl    = { TYPE_SELECT, 0, 0, MENU_TARGET(&ui_sl_althl),      EE_ALTHL };
MENU_VALUE   ui_in_altbd    = { TYPE_UINT,   0, 0, MENU_TARGET(&alt_before_delay), EE_ALTBD };
MENU_VALUE   ui_in_altbt    = { TYPE_UINT,   0, 0, MENU_TARGET(&alt_before_ms),    EE_ALTBT };
MENU_VALUE   ui_in_altad    = { TYPE_UINT,   0, 0, MENU_TARGET(&alt_after_delay),  EE_ALTAD };
MENU_VALUE   ui_in_altat    = { TYPE_UINT,   0, 0, MENU_TARGET(&alt_after_ms),     EE_ALTAT };

MENU_ITEM    ui_it_alt1     = { {"I/O #1 Mode"},     ITEM_VALUE,  0, MENU_TARGET(&ui_in_alt1) };
MENU_ITEM    ui_it_alt2     = { {"I/O #2 Mode"},     ITEM_VALUE,  0, MENU_TARGET(&ui_in_alt2) };
MENU_ITEM    ui_it_alt3     = { {"I/O #3 Mode"},     ITEM_VALUE,  0, MENU_TARGET(&ui_in_alt3) };
MENU_ITEM    ui_it_alt4     = { {"I/O #4 Mode"},     ITEM_VALUE,  0, MENU_TARGET(&ui_in_alt4) };
MENU_ITEM    ui_it_altbd    = { {"Out B4 Time"},     ITEM_VALUE,  0, MENU_TARGET(&ui_in_altbd) };
MENU_ITEM    ui_it_altbt    = { {"Out B4 Trig mS"},  ITEM_VALUE,  0, MENU_TARGET(&ui_in_altbt) };
MENU_ITEM    ui_it_altad    = { {"Out Aft Time"},    ITEM_VALUE,  0, MENU_TARGET(&ui_in_altad) };
MENU_ITEM    ui_it_altat    = { {"Out Aft Trig mS"}, ITEM_VALUE,  0, MENU_TARGET(&ui_in_altat) };
MENU_ITEM    ui_it_altdir   = { {"In Trigger"},      ITEM_VALUE,  0, MENU_TARGET(&ui_in_altdir) };
MENU_ITEM    ui_it_althl    = { {"Out Level"},       ITEM_VALUE,  0, MENU_TARGET(&ui_in_althl) };
MENU_ITEM    ui_it_altset   = { {"Init I/O"},        ITEM_ACTION, 0, MENU_TARGET(uiMenuAltInit) };

MENU_LIST    ui_list_alt[]  = { &ui_it_altset, &ui_it_alt1, &ui_it_alt2, &ui_it_alt3, &ui_it_alt4, &ui_it_altdir, &ui_it_altbd, &ui_it_altbt, &ui_it_altad, &ui_it_altat, &ui_it_althl };
MENU_ITEM    ui_it_alt      = { {"Aux I/O"}, ITEM_MENU, MENU_SIZE(ui_list_alt), MENU_TARGET(&ui_list_alt) };

 // ===== Sensor Options

MENU_SELECT  ui_sl_volen     = { &sensor_enVWarn, MENU_SELECT_SIZE(ui_sel_list_onoff), MENU_TARGET(&ui_sel_list_onoff) };

MENU_VALUE   ui_in_volth    = { TYPE_FLOAT_10, 24.0, 7.0, MENU_TARGET(&sensor_minVoltage), EE_VOLTH };
MENU_VALUE   ui_in_volen    = { TYPE_SELECT,   0,    0,   MENU_TARGET(&ui_sl_volen),       EE_VOLWARN };


MENU_ITEM    ui_it_vol      = { {"Voltage Level"},  ITEM_ACTION, 0, MENU_TARGET(uiMenuVoltage) };
MENU_ITEM    ui_it_cur      = { {"Motor Current"},  ITEM_ACTION, 0, MENU_TARGET(uiMenuCurrent) };
MENU_ITEM    ui_it_temp0    = { {"Temp Motor 1"},   ITEM_ACTION, 0, MENU_TARGET(uiMenuTemp0) };
MENU_ITEM    ui_it_temp1    = { {"Temp Motor 2"},   ITEM_ACTION, 0, MENU_TARGET(uiMenuTemp1) };
MENU_ITEM    ui_it_temp2    = { {"Temp Motor 3"},   ITEM_ACTION, 0, MENU_TARGET(uiMenuTemp2) };
MENU_ITEM    ui_it_volth    = { {"Voltage Thresh"}, ITEM_VALUE,  0, MENU_TARGET(&ui_in_volth) };
MENU_ITEM    ui_it_volen    = { {"Voltage Warn"},   ITEM_VALUE,  0, MENU_TARGET(&ui_in_volen) };

MENU_LIST    ui_list_sen[]  = { &ui_it_vol, &ui_it_cur, &ui_it_temp0, &ui_it_temp1, &ui_it_temp2, &ui_it_volen, &ui_it_volth };
MENU_ITEM    ui_it_sen      = { {"Sensors"}, ITEM_MENU, MENU_SIZE(ui_list_sen), MENU_TARGET(&ui_list_sen) };

 // ===== Global Options

MENU_SELECT  ui_sl_glLCD       = { &motion_sms,      MENU_SELECT_SIZE(ui_sel_list_onoff), MENU_TARGET(&ui_sel_list_onoff) };
MENU_SELECT  ui_sl_glHet       = { &sensor_enHeater, MENU_SELECT_SIZE(ui_sel_list_onoff), MENU_TARGET(&ui_sel_list_onoff) };
MENU_SELECT  ui_sl_glMet       = { &disp_metric,     MENU_SELECT_SIZE(ui_sel_list_onoff), MENU_TARGET(&ui_sel_list_onoff) };

MENU_VALUE   ui_in_glLCD       = { TYPE_BYTE,   0,    0,  MENU_TARGET(&lcdDisable),          EE_LCDOFF };     
MENU_VALUE   ui_in_glPer       = { TYPE_UINT,   5000, 50, MENU_TARGET(&motor_pwm_minperiod), EE_PERIOD };   
MENU_VALUE   ui_in_glHet       = { TYPE_SELECT, 0,    0,  MENU_TARGET(&ui_sl_glHet),         EE_HEATER };
MENU_VALUE   ui_in_glMet       = { TYPE_SELECT, 0,    0,  MENU_TARGET(&ui_sl_glMet),         EE_METRIC };

MENU_ITEM    ui_it_glMet       = { {"Metric Display"},  ITEM_VALUE,  0, MENU_TARGET(&ui_in_glMet) };
MENU_ITEM    ui_it_glLCD       = { {"LCD AutoOff Sec"}, ITEM_VALUE,  0, MENU_TARGET(&ui_in_glLCD) };
MENU_ITEM    ui_it_glPer       = { {"Motor Timing"},    ITEM_VALUE,  0, MENU_TARGET(&ui_in_glPer) };
MENU_ITEM    ui_it_glVFD       = { {"VFD Brightness"},  ITEM_ACTION, 0, MENU_TARGET(uiVFDBrightness) };
MENU_ITEM    ui_it_glSav       = { {"Save Memory"},     ITEM_ACTION, 0, MENU_TARGET(uiMenuSaveMem) };
MENU_ITEM    ui_it_glLod       = { {"Load Memory"},     ITEM_ACTION, 0, MENU_TARGET(uiMenuLoadMem) };
MENU_ITEM    ui_it_glMem       = { {"Reset Memory"},    ITEM_ACTION, 0, MENU_TARGET(uiMenuResetMem) };
MENU_ITEM    ui_it_glHet       = { {"Cold Mode"},       ITEM_VALUE,  0, MENU_TARGET(&ui_in_glHet) };
MENU_ITEM    ui_it_glJmp       = { {"Jump!"},           ITEM_ACTION, 0, MENU_TARGET(uiJump) };





MENU_LIST    ui_list_gl[]      = { &ui_it_alt, &ui_it_sen, &ui_it_glMet, &ui_it_glLCD, &ui_it_glPer,  &ui_it_glHet, &ui_it_glVFD, &ui_it_glSav, &ui_it_glLod, &ui_it_glMem, &ui_it_glJmp };
MENU_ITEM    ui_it_glList      = { {"Settings"}, ITEM_MENU, MENU_SIZE(ui_list_gl), MENU_TARGET(&ui_list_gl) };

 // ===== Main Menu
 
 // Create top-level menu
MENU_LIST    ui_list_top[]     = { &ui_it_camList, &ui_it_motors, &ui_it_glList };

                  // Root item is always created last, so we can add all other items to it
MENU_ITEM    ui_it_root        = { {"Root"},        ITEM_MENU,   MENU_SIZE(ui_list_top),    MENU_TARGET(&ui_list_top) };


// ======= Special Menu for Asymmetric Ramping for Motors


MENU_VALUE   ui_in_cmot0_ru = { TYPE_UINT,   0, 0, MENU_TARGET(&motors[0].ramp_start), EE_M0RAMP  };
MENU_VALUE   ui_in_cmot0_rd = { TYPE_UINT,   0, 0, MENU_TARGET(&motors[0].ramp_end),   EE_M0RAMPE };

MENU_ITEM    ui_it_cmot0_ru = { {"Ramp In"},  ITEM_VALUE,  0, MENU_TARGET(&ui_in_cmot0_ru) };
MENU_ITEM    ui_it_cmot0_rd = { {"Ramp Out"}, ITEM_VALUE,  0, MENU_TARGET(&ui_in_cmot0_rd) };

MENU_LIST    ui_list_cmot0[] = { &ui_it_cmot0_ru, &ui_it_cmot0_rd };
MENU_ITEM    ui_it_cmot0      = { {"Ramping"}, ITEM_MENU, MENU_SIZE(ui_list_cmot0), MENU_TARGET(&ui_list_cmot0) };

MENU_VALUE   ui_in_cmot1_ru = { TYPE_UINT,   0, 0, MENU_TARGET(&motors[0].ramp_start), EE_M0RAMP  + EE_MOTOR_SPACE };
MENU_VALUE   ui_in_cmot1_rd = { TYPE_UINT,   0, 0, MENU_TARGET(&motors[0].ramp_end),   EE_M0RAMPE + EE_MOTOR_SPACE };

MENU_ITEM    ui_it_cmot1_ru = { {"Ramp In"},  ITEM_VALUE,  0, MENU_TARGET(&ui_in_cmot1_ru) };
MENU_ITEM    ui_it_cmot1_rd = { {"Ramp Out"}, ITEM_VALUE,  0, MENU_TARGET(&ui_in_cmot1_rd) };

MENU_LIST    ui_list_cmot1[] = { &ui_it_cmot1_ru, &ui_it_cmot1_rd };
MENU_ITEM    ui_it_cmot1     = { {"Ramping"}, ITEM_MENU, MENU_SIZE(ui_list_cmot1), MENU_TARGET(&ui_list_cmot1) };

MENU_VALUE   ui_in_cmot2_ru = { TYPE_UINT,   0, 0, MENU_TARGET(&motors[0].ramp_start), EE_M0RAMP  + EE_MOTOR_SPACE * 2  };
MENU_VALUE   ui_in_cmot2_rd = { TYPE_UINT,   0, 0, MENU_TARGET(&motors[0].ramp_end),   EE_M0RAMPE + EE_MOTOR_SPACE };

MENU_ITEM    ui_it_cmot2_ru = { {"Ramp In"},  ITEM_VALUE,  0, MENU_TARGET(&ui_in_cmot2_ru) };
MENU_ITEM    ui_it_cmot2_rd = { {"Ramp Out"}, ITEM_VALUE,  0, MENU_TARGET(&ui_in_cmot2_rd) };

MENU_LIST    ui_list_cmot2[] = { &ui_it_cmot2_ru, &ui_it_cmot2_rd };
MENU_ITEM    ui_it_cmot2     = { {"Ramping"}, ITEM_MENU, MENU_SIZE(ui_list_cmot2), MENU_TARGET(&ui_list_cmot2) };

// ====== Cursor (main screen interaction) Data =======

 /** function pointer for cursors
 
  Note that the byte argument will be used to inform you whether or not the cursor value was increased (1) or
  decreased (0)
  
  */
typedef void(*uiTargetFunc)(byte);


 /** Display Cursor
 
   For cursor activity on screens, this struct defines, and should be used
   for one global variable, the position of the cursor on-screen, and whether
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
 activity to call (callback) when interacted with (up or down key).  The final element
 is a pointer to a MENU_ITEM, which will cause the enter button to draw up that menu
 when the cursor is over it.  Providing 0 as mnu will prevent a specialized menu from
 being activated on that item.
 */
 
struct uiDisplayCursorTarget {
    /** The screen row where the cursor is placed */
 byte row;
   /** The screen column where the cursor is placed */
 byte col;
   /** The callback to execute */
 uiTargetFunc func;
   /** Menu item pointer to load special menu, or 0 (null-pointer) for no menu */
 MENU_ITEM* mnu;
};

 /** Screen Cursor Targets
 
  For every screen, we must define what targets are available on that screen
  */
  
struct uiDisplayCursors {
    /** Count items in target list */
  byte count;
    /** A void pointer which should resolve to a uiDisplayCursorTarget**, and the length of that
        list must be equivalent to count */
  void* targets;
};


 // cursor targets for each screen

 // main screen
const uiDisplayCursorTarget  ui_ct_main1     = { 0, 0,  uiCursorToggleRun, 0 };
const uiDisplayCursorTarget  ui_ct_main2     = { 0, 4,  uiCursorAdjustInt, 0 };
const uiDisplayCursorTarget  ui_ct_main3     = { 0, 12, uiCursorAdjustSMS, 0 };

const uiDisplayCursorTarget* ui_dctl_main[]  = { &ui_ct_main1, &ui_ct_main2, &ui_ct_main3 };
const uiDisplayCursors       ui_dc_main      = { DCT_SIZE(ui_dctl_main), DCT_PTR(&ui_dctl_main) };

 // camera screen
const uiDisplayCursorTarget  ui_ct_cam1      = { 1, 0,  uiCursorChangeCamBulb,     &ui_it_camList };
const uiDisplayCursorTarget  ui_ct_cam2      = { 1, 2,  uiCursorChangeShutterTime, &ui_it_camList };
const uiDisplayCursorTarget  ui_ct_cam3      = { 1, 11, uiCursorChangeFocusTime,   &ui_it_camList };

const uiDisplayCursorTarget* ui_dctl_cam[]   = { &ui_ct_cam1, &ui_ct_cam2, &ui_ct_cam3 };
const uiDisplayCursors       ui_dc_cam       = { DCT_SIZE(ui_dctl_cam), DCT_PTR(&ui_dctl_cam) };

 // motor screens
const uiDisplayCursorTarget  ui_ct_mot1      = { 0, 8,  uiCursorChangeMotEn,   0 };
const uiDisplayCursorTarget  ui_ct_mot2      = { 1, 0,  uiCursorChangeMotDir,  0 };
const uiDisplayCursorTarget  ui_ct_mot3      = { 1, 2,  uiCursorChangeMotSpd,  0 };
const uiDisplayCursorTarget  ui_ct_mot4      = { 0, 13, uiCursorChangeMotLead, 0 };
const uiDisplayCursorTarget  ui_ct_mot5_0    = { 1, 13, uiCursorChangeMotRamp, &ui_it_cmot0 };
const uiDisplayCursorTarget  ui_ct_mot5_1    = { 1, 13, uiCursorChangeMotRamp, &ui_it_cmot1 };
const uiDisplayCursorTarget  ui_ct_mot5_2    = { 1, 13, uiCursorChangeMotRamp, &ui_it_cmot2 };

const uiDisplayCursorTarget* ui_dctl_mot0[]   = { &ui_ct_mot1, &ui_ct_mot2, &ui_ct_mot3, &ui_ct_mot4, &ui_ct_mot5_0 };
const uiDisplayCursorTarget* ui_dctl_mot1[]   = { &ui_ct_mot1, &ui_ct_mot2, &ui_ct_mot3, &ui_ct_mot4, &ui_ct_mot5_1 };
const uiDisplayCursorTarget* ui_dctl_mot2[]   = { &ui_ct_mot1, &ui_ct_mot2, &ui_ct_mot3, &ui_ct_mot4, &ui_ct_mot5_2 };

const uiDisplayCursors       ui_dc_mot0       = { DCT_SIZE(ui_dctl_mot0), DCT_PTR(&ui_dctl_mot0) };
const uiDisplayCursors       ui_dc_mot1       = { DCT_SIZE(ui_dctl_mot0), DCT_PTR(&ui_dctl_mot1) };
const uiDisplayCursors       ui_dc_mot2       = { DCT_SIZE(ui_dctl_mot0), DCT_PTR(&ui_dctl_mot2) };

 // screens with no targets
const uiDisplayCursors ui_dc_none = { 0, 0 };

 // all screen cursors...
 
    // main, camera, m1, m2, m3 
const uiDisplayCursors*  ui_dc_list[] = { &ui_dc_main, &ui_dc_cam, &ui_dc_mot0, &ui_dc_mot1, &ui_dc_mot2 };


