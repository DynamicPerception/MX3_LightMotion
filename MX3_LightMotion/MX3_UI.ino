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
  User interface functions
  ========================================
  
*/




 // global cursor display data
uiDisplayCursor ui_cursor = { 0, 0, 0 };

boolean ui_refresh = false;

 // cursor targets for each screen

 // main screen
const uiDisplayCursorTarget ui_ct_main1 = { 0, 0, uiToggleRun };
const uiDisplayCursorTarget ui_ct_main2 = { 0, 4, uiAdjustInt };

const uiDisplayCursorTarget* ui_dctl_main[] = { &ui_ct_main1, &ui_ct_main2 };
const uiDisplayCursors ui_dc_main = { DCT_SIZE(ui_dctl_main), DCT_PTR(&ui_dctl_main) };

 // screens with no targets
const uiDisplayCursors ui_dc_none = { 0, 0 };

 // all screen cursors...
 
    // main, camera, m1, m2, m3
const uiDisplayCursors*  ui_dc_list[] = { &ui_dc_main, &ui_dc_none, &ui_dc_none, &ui_dc_none, &ui_dc_none };

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

MENU_LIST    ui_list_cam[]     = { &ui_it_camDelay, &ui_it_camBulb, &ui_it_camExposure, &ui_it_camWait, &ui_it_camFocus, &ui_it_camRepeat };

MENU_ITEM    ui_it_camList     = { {"Camera"}, ITEM_MENU, MENU_SIZE(ui_list_cam), MENU_TARGET(&ui_list_cam) };


  // ======= Motor Menus

  // motor 1 inputs
  
MENU_FLAG    ui_flag_m0_rot    = { 4, (byte*) &motors[0].flags };
MENU_FLAG    ui_flag_m0_flip   = { 5, (byte*) &motors[0].flags };

MENU_VALUE   ui_in_m0_rot      = { TYPE_BFLAG, 0, 0, MENU_TARGET(&ui_flag_m0_rot), EE_M0FLAG };
MENU_VALUE   ui_in_m0_flip     = { TYPE_BFLAG, 0, 0, MENU_TARGET(&ui_flag_m0_flip), EE_M0FLAG };
MENU_VALUE   ui_in_m0_rpm      = { TYPE_FLOAT_100, 500, 0, MENU_TARGET(&motors[0].rpm), EE_M0RPM };
MENU_VALUE   ui_in_m0_ratio    = { TYPE_FLOAT_100, 5000, 0, MENU_TARGET(&motors[0].ratio), EE_M0RATIO };
MENU_VALUE   ui_in_m0_onp      = { TYPE_UINT, 500, 1, MENU_TARGET(&motors[0].onPeriods), EE_M0ONP };

MENU_ITEM    ui_it_m0_rot      = { {"Rotary"}, ITEM_VALUE, 0, MENU_TARGET(&ui_in_m0_rot) };
MENU_ITEM    ui_it_m0_flip     = { {"Invert Dir"}, ITEM_VALUE, 0, MENU_TARGET(&ui_in_m0_flip) };
MENU_ITEM    ui_it_m0_rpm      = { {"RPM"}, ITEM_VALUE, 0, MENU_TARGET(&ui_in_m0_rpm) };
MENU_ITEM    ui_it_m0_ratio    = { {"Ratio"}, ITEM_VALUE, 0, MENU_TARGET(&ui_in_m0_ratio) };
MENU_ITEM    ui_it_m0_onp      = { {"On Periods"}, ITEM_VALUE, 0, MENU_TARGET(&ui_in_m0_onp) };

MENU_LIST    ui_list_m0[]      = { &ui_it_m0_rot, &ui_it_m0_flip, &ui_it_m0_rpm, &ui_it_m0_ratio, &ui_it_m0_onp };
MENU_ITEM    ui_it_m0List      = { {"Axis 1"}, ITEM_MENU, MENU_SIZE(ui_list_m0), MENU_TARGET(&ui_list_m0) };

  // motor 2 inputs
  
MENU_FLAG    ui_flag_m1_rot    = { 4, (byte*) &motors[1].flags };
MENU_FLAG    ui_flag_m1_flip   = { 5, (byte*) &motors[1].flags };

MENU_VALUE   ui_in_m1_rot      = { TYPE_BFLAG, 0, 0, MENU_TARGET(&ui_flag_m1_rot), EE_M0FLAG + EE_MOTOR_SPACE};
MENU_VALUE   ui_in_m1_flip     = { TYPE_BFLAG, 0, 0, MENU_TARGET(&ui_flag_m1_flip), EE_M0FLAG + EE_MOTOR_SPACE};
MENU_VALUE   ui_in_m1_rpm      = { TYPE_FLOAT_100, 500, 0, MENU_TARGET(&motors[1].rpm), EE_M0RPM + EE_MOTOR_SPACE};
MENU_VALUE   ui_in_m1_ratio    = { TYPE_FLOAT_100, 5000, 0, MENU_TARGET(&motors[1].ratio), EE_M0RATIO + EE_MOTOR_SPACE};
MENU_VALUE   ui_in_m1_onp      = { TYPE_UINT, 500, 1, MENU_TARGET(&motors[1].onPeriods), EE_M0ONP + EE_MOTOR_SPACE };

MENU_ITEM    ui_it_m1_rot      = { {"Rotary"}, ITEM_VALUE, 0, MENU_TARGET(&ui_in_m1_rot) };
MENU_ITEM    ui_it_m1_flip     = { {"Invert Dir"}, ITEM_VALUE, 0, MENU_TARGET(&ui_in_m1_flip) };
MENU_ITEM    ui_it_m1_rpm      = { {"RPM"}, ITEM_VALUE, 0, MENU_TARGET(&ui_in_m1_rpm) };
MENU_ITEM    ui_it_m1_ratio    = { {"Ratio"}, ITEM_VALUE, 0, MENU_TARGET(&ui_in_m1_ratio) };
MENU_ITEM    ui_it_m1_onp      = { {"On Periods"}, ITEM_VALUE, 0, MENU_TARGET(&ui_in_m1_onp) };

MENU_LIST    ui_list_m1[]      = { &ui_it_m1_rot, &ui_it_m1_flip, &ui_it_m1_rpm, &ui_it_m1_ratio, &ui_it_m1_onp };
MENU_ITEM    ui_it_m1List      = { {"Axis 2"}, ITEM_MENU, MENU_SIZE(ui_list_m1), MENU_TARGET(&ui_list_m1) };

  // motor 2 inputs
  
MENU_FLAG    ui_flag_m2_rot    = { 4, (byte*) &motors[2].flags };
MENU_FLAG    ui_flag_m2_flip   = { 5, (byte*) &motors[2].flags };

MENU_VALUE   ui_in_m2_rot      = { TYPE_BFLAG, 0, 0, MENU_TARGET(&ui_flag_m2_rot), EE_M0FLAG + EE_MOTOR_SPACE * 2};
MENU_VALUE   ui_in_m2_flip     = { TYPE_BFLAG, 0, 0, MENU_TARGET(&ui_flag_m2_flip), EE_M0FLAG + EE_MOTOR_SPACE * 2};
MENU_VALUE   ui_in_m2_rpm      = { TYPE_FLOAT_100, 500, 0, MENU_TARGET(&motors[2].rpm), EE_M0RPM + EE_MOTOR_SPACE * 2};
MENU_VALUE   ui_in_m2_ratio    = { TYPE_FLOAT_100, 5000, 0, MENU_TARGET(&motors[2].ratio), EE_M0RATIO + EE_MOTOR_SPACE * 2};
MENU_VALUE   ui_in_m2_onp      = { TYPE_UINT, 500, 1, MENU_TARGET(&motors[2].onPeriods), EE_M0ONP + EE_MOTOR_SPACE * 2 };

MENU_ITEM    ui_it_m2_rot      = { {"Rotary"}, ITEM_VALUE, 0, MENU_TARGET(&ui_in_m2_rot) };
MENU_ITEM    ui_it_m2_flip     = { {"Invert Dir"}, ITEM_VALUE, 0, MENU_TARGET(&ui_in_m2_flip) };
MENU_ITEM    ui_it_m2_rpm      = { {"RPM"}, ITEM_VALUE, 0, MENU_TARGET(&ui_in_m2_rpm) };
MENU_ITEM    ui_it_m2_ratio    = { {"Ratio"}, ITEM_VALUE, 0, MENU_TARGET(&ui_in_m2_ratio) };
MENU_ITEM    ui_it_m2_onp      = { {"On Periods"}, ITEM_VALUE, 0, MENU_TARGET(&ui_in_m2_onp) };

MENU_LIST    ui_list_m2[]      = { &ui_it_m2_rot, &ui_it_m2_flip, &ui_it_m2_rpm, &ui_it_m2_ratio, &ui_it_m2_onp };
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
MENU_ITEM    ui_it_altset   = { {"Init I/O"}, ITEM_ACTION, 0, MENU_TARGET(uiAltInit) };

MENU_LIST    ui_list_alt[]  = { &ui_it_altset, &ui_it_alt1, &ui_it_alt2 };
MENU_ITEM    ui_it_alt      = { {"Alt I/O"}, ITEM_MENU, MENU_SIZE(ui_list_alt), MENU_TARGET(&ui_list_alt) };

 // ===== Global Options

MENU_SELECT  ui_sl_glLCD       = { &motion_sms, MENU_SELECT_SIZE(ui_sel_list_onoff), MENU_TARGET(&ui_sel_list_onoff) };
MENU_SELECT  ui_sl_glMet       = { &metric_ui, MENU_SELECT_SIZE(ui_sel_list_onoff), MENU_TARGET(&ui_sel_list_onoff) };

MENU_VALUE   ui_in_glLCD       = { TYPE_BYTE, 0, 0, MENU_TARGET(&lcdDisable), EE_LCDOFF };   
MENU_VALUE   ui_in_glSMS       = { TYPE_SELECT, 0, 0, MENU_TARGET(&ui_sl_glLCD), EE_SMS };
MENU_VALUE   ui_in_glMet       = { TYPE_SELECT, 0, 0, MENU_TARGET(&ui_sl_glMet), EE_METRIC };

MENU_ITEM    ui_it_glSMS       = { {"SMS Motion"}, ITEM_VALUE, 0, MENU_TARGET(&ui_in_glSMS) };
MENU_ITEM    ui_it_glMet       = { {"Metric Display"}, ITEM_VALUE, 0, MENU_TARGET(&ui_in_glMet) };
MENU_ITEM    ui_it_glLCD       = { {"LCD AutoOff Sec"}, ITEM_VALUE, 0, MENU_TARGET(&ui_in_glLCD) };

MENU_LIST    ui_list_gl[]      = { &ui_it_glSMS, &ui_it_glMet, &ui_it_glLCD, &ui_it_alt };
MENU_ITEM    ui_it_glList      = { {"Settings"}, ITEM_MENU, MENU_SIZE(ui_list_gl), MENU_TARGET(&ui_list_gl) };

 // ===== Main Menu
 
 // Create top-level menu
MENU_LIST    ui_list_top[]     = { &ui_it_camList, &ui_it_motors, &ui_it_glList };

                  // Root item is always created last, so we can add all other items to it
MENU_ITEM    ui_it_root        = { {"Root"},        ITEM_MENU,   MENU_SIZE(ui_list_top),    MENU_TARGET(&ui_list_top) };


 // initialize LCD object
LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

 // initialize menu manager
OMMenuMgr Menu(&ui_it_root);




/** UI Setup

  Set up the LCD, Menu Manager, and display welcome screen
  
  @author
  C. A. Church
  */

void uiMenuSetup() {

  lcd.begin(LCD_COLS, LCD_ROWS);
  
  pinMode(LCD_BKL, OUTPUT);
  uiLCDBackLight(true);
  
  Menu.setDrawHandler(uiDraw);
  Menu.setExitHandler(uiClear);
  Menu.setAnalogButtonPin(BUT_PIN, BUT_MAP, BUT_THRESH);
  Menu.enable(true); 
  
  lcd.print(MX3_VERSTR);
  lcd.setCursor(0, 1);
  lcd.print(MX3_SUBSTR);
  
  delay(3000);

  lcd.clear();
  lcd.print(MX3_C1STR);
  lcd.setCursor(0,1);
  lcd.print(MX3_C2STR);

  delay(2000);
  uiClear();
  
}



 /** Check UI Status
 
   Poll Menu, Check for Input
   
   @author
   C. A. Church
   */
  
void uiCheck() {
 static unsigned long lastButTm = 0;
 static boolean       lcdOn     = true;
 
 byte button = Menu.checkInput();
 
  // check for disabling the LCD backlight
  
 if( button == BUTTON_NONE ) {
   if(lcdOn == true && millis() - lastButTm > (lcdDisable * 1000) ) {
     uiLCDBackLight(false);
     lcdOn = false;
   }
 }
 else {
   lastButTm = millis();
   if( lcdOn == false ) {
     uiLCDBackLight(true);
     lcdOn = true;
   }
 }
 
   // show base screen, if desired
   
 if( ! Menu.shown() ) 
   uiBaseScreen(button);

   
}



 /** Draw Handler Callback
 
   Draw handler callback for OMMenuMgr
   
   @author
   C. A. Church
   */
   
void uiDraw(char* p_text, int p_row, int p_col, int len) {
  lcd.setCursor(p_col, p_row);
  
  for( int i = 0; i < len; i++ ) {
    if( p_text[i] < '!' || p_text[i] > '~' )
      lcd.write(' ');
    else  
      lcd.write(p_text[i]);
  }
}

 /** Exit Callback Handler
 
  Exit callback handler for OMMenuMgr
  
  @author
  C. A. Church
  */
  
void uiClear() {
  
  lcd.clear();
  lcd.setCursor(0, 0);
}

 /** Set LCD Backlight
 
  Enables or disables the LCD Backlight
  
  @param p_en
  Enable (true) or disable (false) the backlight
  
  @author
  C. A. Church
  */

void uiLCDBackLight(boolean p_en) {
 digitalWrite(LCD_BKL, p_en); 
}

/** Base Interface Display

 Displays the basic interface
 
 @author
 C. A. Church
 */
 
void uiBaseScreen(byte p_button) {
  
  static byte             screen = UI_SCREEN_MAIN;
  static byte         lastScreen = screen;
  static unsigned long lastUpdTm = 0;
 
 
     // only let up and down change screens when the cursor isn't on an item
  
  if( ! ui_cursor.enabled ) {
    if( p_button == BUTTON_INCREASE ) {
      lcd.clear();
      screen = screen > UI_SCREEN_MOTOR3 ? UI_SCREEN_MAIN : screen + 1;
    }
    else if( p_button == BUTTON_DECREASE ) {
      lcd.clear();
      screen = screen == UI_SCREEN_MAIN ? UI_SCREEN_MOTOR3 : screen - 1;
    }
      
    if( screen > UI_SCREEN_MOTOR3 )
      screen = UI_SCREEN_MAIN;
  }
  
  
      // handle screen navigation
    
  uiScreenInput(screen, p_button);
    
      // display, or update the screen?
  if( ui_refresh || screen != lastScreen || millis() - lastUpdTm > UI_REFRESH_TM ) {
    
   ui_refresh = false;
   lastScreen = screen;
   lastUpdTm = millis();
   
   lcd.setCursor(0,0);
 
      // first page of display
    if( screen == UI_SCREEN_MAIN )
      uiMainScreen(p_button);
    else if( screen == UI_SCREEN_CAMERA )
      uiCamScreen(p_button);
    else if( screen == UI_SCREEN_MOTOR1 || screen == UI_SCREEN_MOTOR2 || screen == UI_SCREEN_MOTOR3 )
      uiMotorScreen(p_button, screen);
      
  }
 
   // if cursor input is enabled, re-seek cursor to correct position
  if( ui_cursor.enabled )
    lcd.setCursor(ui_cursor.col, ui_cursor.row);
    
}

/** Main Display Screen */

byte uiMainScreen(byte p_button) {

  if( running )
    lcd.print(STR_RUN);
  else
    lcd.print(STR_STOP);
    
  lcd.print(' ');
  lcd.print(camera_delay, 1);
  lcd.print("s ");
  
  
  uiShowShotsFired();
  
  lcd.setCursor(0, 1);
  
  unsigned int hours = run_time / 3600000L;
  unsigned int mins  = ( run_time - (hours * 3600000) ) / 60000L;
  unsigned int secs  = ( run_time - (hours * 3600000) - (mins * 60000L) ) / 1000;
  
  lcd.print(STR_TIME);
  PAD(hours);
  lcd.print('\'');
  PAD(mins);
  lcd.print('"');
  PAD(secs);
  

  return UI_SCREEN_MAIN;
  
}

 /** Display Screen for Camera */
 
void uiCamScreen(byte p_button) {
 
  lcd.print(STR_CAM);

  if( Camera.busy() )
    lcd.print(STR_BUSY);
  else
    lcd.print(STR_IDLE);
    
  uiShowShotsFired();
   
  lcd.setCursor(0, 1);
  
  unsigned long s_time;
  
  if( camera_bulb ) {
    lcd.print(STR_BULB);
    s_time = camera_exposure;
  }
  else {
    lcd.print(STR_ECAM);
    s_time = camera_wait;
  }
    
  if( s_time >= 1000 ) {
    
    unsigned int s = s_time / 1000;
    unsigned int p = (s_time - (s * 1000)) / 100;
    lcd.print(s, DEC);
    
    if( p > 0 ) {
      lcd.print('.');
      lcd.print(p, DEC);
    }
    
    lcd.print('"');
  }
  else {
    unsigned int s = 1000 / s_time;
    lcd.print("1/");
    lcd.print(s, DEC);
  }
  
    

  
}

/** Display Screen for Motors */

void uiMotorScreen(byte p_button, byte p_motor) {

  lcd.print(STR_MOTOR);
  
  p_motor -= 1;
  
  lcd.print(p_motor, DEC);
  
  p_motor -= 1;
  
  lcd.setCursor(0, 1);
  
  if( motors[p_motor].flags & MOTOR_ROT_FLAG ) {
    if( motors[p_motor].flags & MOTOR_CDIR_FLAG )
       lcd.print(STR_CW);
    else
       lcd.print(STR_CCW); 
  }
  else {
    if( motors[p_motor].flags & MOTOR_CDIR_FLAG )
       lcd.print(STR_RIGHT);
    else
       lcd.print(STR_LEFT); 
  }  
  
  lcd.print(' ');
  
  float spd = motorSpeedRatio(p_motor);
  
  lcd.print(spd, 2);
  
}

/** Display Shots Fired On-Screen
*/

void uiShowShotsFired() {
  
  byte bkMove = 1;
  
  lcd.setCursor(OM_MENU_COLS - 4, 0);
  lcd.print("    ");
  
  if( camera_fired >= 1000 )
    bkMove = 4;
  else if( camera_fired >= 100 )
    bkMove = 3;
  else if( camera_fired >= 10 )
    bkMove = 2;
    
  lcd.setCursor( OM_MENU_COLS - bkMove, 0);
  lcd.print(camera_fired, DEC);
  
}

 /** Screen Input Handler
 
  Handles cursor-based input on screens, using dynamic allocation
  via the uiDisplay* structs.
  
  @param p_screen
  The current screen we're on
  
  @param p_button
  The button which was pressed
  
  @author
  C. A. Church
  */
  
void uiScreenInput(byte p_screen, byte p_button) {

  static byte wasScreen = UI_SCREEN_MAIN;
  static byte uiPos     = 0;

  byte action = 0;

    // reset cursor if screen has changed
  if( p_screen != wasScreen ) {
        uiPos = 0;
    wasScreen = p_screen;
    ui_cursor.enabled = 0;
  }


   // handle activity based on button pressed
   if( p_button == BUTTON_FORWARD ) {
      uiPos++;
   }
   else if( p_button == BUTTON_BACK ) {
      uiPos--;
   }
   else if( p_button == BUTTON_INCREASE ) {
      action = 2;
   }
   else if( p_button == BUTTON_DECREASE ) {
      action = 1;
   }
  
    // get cursor
  const uiDisplayCursors* thsCursor = ui_dc_list[p_screen];
  
    // don't try to access a cursor further out than known for screen
  if( uiPos > thsCursor->count )
    uiPos = 0;
    
    // no cursor selected, clear cursor and bail
  if( uiPos == 0 ) {
    ui_cursor.enabled = 0;
    lcd.noBlink();
    return;
  }


    // set cursor enabled, blink, and note position of cursor
  ui_cursor.enabled = 1; 
  lcd.blink();

  uiDisplayCursorTarget** targets = reinterpret_cast<uiDisplayCursorTarget**>(thsCursor->targets);
  
  uiDisplayCursorTarget* target = targets[uiPos - 1];
  ui_cursor.row = target->row;
  ui_cursor.col = target->col;
  
  if( action ) {
    uiTargetFunc func = target->func;
    func(action - 1);
    ui_refresh = true;
  }
      
}

 /** Toggle Run State
 
 */

void uiToggleRun(byte p_dir) {
  if( running )
    stopProgram();
  else
    startProgram();
   
}

/** Adjust Interval Time
*/
void uiAdjustInt(byte p_dir) {
  
  float incr = 0.1;
  
  if( ! p_dir ) {
    if( camera_delay <= 0.1 )
      camera_delay = 0.0;
    else
      camera_delay -= incr;
  }
  else
     camera_delay += incr;
  
  OMEEPROM::write(EE_CAMDEL, camera_delay); 
      
}

/** Initialize Alt Inputs 
*/

void uiAltInit() {
 
 altSetup();

 lcd.clear();

 lcd.print("Input Init");
 lcd.setCursor(0, 1);
 lcd.print("Done!");
 
 delay(2000); 
}
