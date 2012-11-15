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


 // Load UI Menus

#include "MX3_UI.h"

 // global cursor display data
uiDisplayCursor ui_cursor = { 0, 0, 0 };

boolean ui_refresh = false;

 // cursor targets for each screen

 // main screen
const uiDisplayCursorTarget  ui_ct_main1     = { 0, 0, uiToggleRun };
const uiDisplayCursorTarget  ui_ct_main2     = { 0, 4, uiAdjustInt };
const uiDisplayCursorTarget* ui_dctl_main[]  = { &ui_ct_main1, &ui_ct_main2 };
const uiDisplayCursors       ui_dc_main      = { DCT_SIZE(ui_dctl_main), DCT_PTR(&ui_dctl_main) };

 // camera screen
const uiDisplayCursorTarget  ui_ct_cam1      = { 1, 2, uiChangeShutterTime };
const uiDisplayCursorTarget* ui_dctl_cam[]   = { &ui_ct_cam1 };
const uiDisplayCursors       ui_dc_cam       = { DCT_SIZE(ui_dctl_cam), DCT_PTR(&ui_dctl_cam) };

 // screens with no targets
const uiDisplayCursors ui_dc_none = { 0, 0 };

 // all screen cursors...
 
    // main, camera, m1, m2, m3
const uiDisplayCursors*  ui_dc_list[] = { &ui_dc_main, &ui_dc_cam, &ui_dc_none, &ui_dc_none, &ui_dc_none };


  // camera shutter speed display divisions
const float ui_camDivs[]  = { 0.001, 0.002, 0.004, 0.008, 0.01, 0.0166667, 0.033333, 0.0666667, 0.125, 0.25,  0.333333, 0.5, 0.75, 1.0, 1.3, 1.6, 2 };

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

  float minInt = 0.0;
  
    // minimum interval calculation
  if( camera_bulb )
    minInt += CAM_MIN_TRIG;
  else
    minInt += camera_exposure;
    
  minInt += camera_wait;
  minInt += camera_focus;
  
  minInt = minInt / 1000.0;
  
  if( running )
    lcd.print(STR_RUN);
  else
    lcd.print(STR_STOP);
    
  lcd.print(' ');
  
  if( minInt > camera_delay )
    lcd.print(minInt, 1);
  else
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
    
   
   //lcd.print(s_time);
   
  if( s_time >= 250 ) {
    
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

/** Interacting with shutter time on Cam Screen
*/

void uiChangeShutterTime(byte p_dir) {
 
  unsigned long *ptr = &camera_wait;
  byte      scaleLen = (sizeof(ui_camDivs) / sizeof(float)) - 1;
  float          div = (float) *ptr / 1000.0;
  int            mod = 1;
  int            mem = EE_CAMWAIT;
  
 if( camera_bulb ) {
   ptr = &camera_exposure;
   mem = EE_CAMEXP;
 }

 if( p_dir != 1 )
   mod = -1;
   
 if( div >= ui_camDivs[scaleLen] ) {
     // we are greater than or equal to the largest div, generally we must in 1 second increments above
     // this range
    if( div >= (ui_camDivs[scaleLen] + 1.0) ) {
       div += mod;
    }
    else { 
       if( p_dir )
         div = ui_camDivs[scaleLen] + 1.0;
       else 
         div = ui_camDivs[scaleLen - 1];
    }
 }
 else {
     // we are below the largest div, we move around in divs here
   byte pos = 0;
   for( pos = 0; pos <= scaleLen; pos++ ) {
     if( ui_camDivs[pos] >= div ) {
       break;
     }
   }
   
   //div = ui_camDivs[pos + mod];
   
   if( pos == scaleLen && mod )
     div = ui_camDivs[scaleLen] + 1.0;
   else if( pos == 0 && ! mod) 
     ; // do nothing!
   else
     div = ui_camDivs[pos + mod];
 
 }
 
   // set correct wait value
 *ptr = (unsigned long) (1000.0 * div);
 
 if( *ptr == 0 )
   *ptr = 1;
   
   // write new value to eeprom
 OMEEPROM::write(mem, *ptr);
 
   // if already running, re-configure camera
 if( running )
   camSetup();
  
   // overwrite exposure display area...
 lcd.print(STR_BLNK); 
 
}
