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



 // ui setup
 
    // lcd pins
const byte LCD_RS  = 7;
const byte LCD_EN  = 6;
const byte LCD_D4  = 2;
const byte LCD_D5  = 3;
const byte LCD_D6  = 4;
const byte LCD_D7  = 5;


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
                            


// ====== Test Menu =========== 



  // Create a list of states and values for a select input
MENU_SELECT_ITEM   ui_sel_on  = { 1, {"On"} };
MENU_SELECT_ITEM   ui_sel_off = { 0, {"Off"} };

MENU_SELECT_ITEM  ui_sel_left  = { 0, {"Left"} };
MENU_SELECT_ITEM  ui_sel_right = { 1, {"Right"} };


MENU_SELECT_LIST  ui_sel_list_onoff[] = { &ui_sel_off, &ui_sel_on };
MENU_SELECT_LIST  ui_sel_list_dir[]   = { &ui_sel_left, &ui_sel_right };



  // ===== Camera Menu 
  
MENU_VALUE   ui_in_camMaxShots = { TYPE_UINT, 0, 0, MENU_TARGET(&camera_max_shots), EE_MAXSHOT };
MENU_VALUE   ui_in_camRepeat   = { TYPE_BYTE, 0, 0, MENU_TARGET(&camera_repeat), EE_CAMREP };
MENU_VALUE   ui_in_camExposure = { TYPE_ULONG, 0, 0, MENU_TARGET(&camera_exposure), EE_CAMEXP };
MENU_VALUE   ui_in_camFocus    = { TYPE_ULONG, 0, 0, MENU_TARGET(&camera_focus), EE_CAMFOC };
MENU_VALUE   ui_in_camWait     = { TYPE_ULONG, 0, 0, MENU_TARGET(&camera_wait), EE_CAMWAIT };
MENU_VALUE   ui_in_camDelay    = { TYPE_ULONG, 0, 0, MENU_TARGET(&camera_delay), EE_CAMDEL };

MENU_ITEM    ui_it_camMaxShot  = { {"Max Shots"}, ITEM_VALUE, 0, MENU_TARGET(&ui_in_camMaxShots) };
MENU_ITEM    ui_it_camRepeat   = { {"Repeat Shots"}, ITEM_VALUE, 0, MENU_TARGET(&ui_in_camRepeat) };
MENU_ITEM    ui_it_camExposure = { {"Exp. Time (ms)"}, ITEM_VALUE, 0, MENU_TARGET(&ui_in_camExposure) };
MENU_ITEM    ui_it_camFocus    = { {"Focus (ms)"}, ITEM_VALUE, 0, MENU_TARGET(&ui_in_camFocus) };
MENU_ITEM    ui_it_camWait     = { {"Exp Delay (ms)"}, ITEM_VALUE, 0, MENU_TARGET(&ui_in_camWait) };
MENU_ITEM    ui_it_camDelay    = { {"Interval"}, ITEM_VALUE, 0, MENU_TARGET(&ui_in_camDelay) };


MENU_LIST    ui_list_cam[]     = { &ui_it_camDelay, &ui_it_camExposure, &ui_it_camWait, &ui_it_camFocus, &ui_it_camRepeat };

MENU_ITEM    ui_it_camList     = { {"Camera"}, ITEM_MENU, MENU_SIZE(ui_list_cam), MENU_TARGET(&ui_list_cam) };


  // ======= Motor Menus

  // motor 1 inputs
  
MENU_FLAG    ui_flag_m0_rot    = { 4, (byte*) &motors[0].flags };
MENU_FLAG    ui_flag_m0_flip   = { 5, (byte*) &motors[0].flags };

MENU_VALUE   ui_in_m0_rot      = { TYPE_BFLAG, 0, 0, MENU_TARGET(&ui_flag_m0_rot), EE_M0FLAG };
MENU_VALUE   ui_in_m0_flip     = { TYPE_BFLAG, 0, 0, MENU_TARGET(&ui_flag_m0_flip), EE_M0FLAG };
MENU_VALUE   ui_in_m0_rpm      = { TYPE_FLOAT_100, 0, 0, MENU_TARGET(&motors[0].rpm), EE_M0RPM };
MENU_VALUE   ui_in_m0_ratio    = { TYPE_FLOAT_100, 0, 0, MENU_TARGET(&motors[0].ratio), EE_M0RATIO };

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
MENU_VALUE   ui_in_m1_rpm      = { TYPE_FLOAT_100, 0, 0, MENU_TARGET(&motors[1].rpm), EE_M0RPM + EE_MOTOR_SPACE};
MENU_VALUE   ui_in_m1_ratio    = { TYPE_FLOAT_100, 0, 0, MENU_TARGET(&motors[1].ratio), EE_M0RATIO + EE_MOTOR_SPACE};

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
MENU_VALUE   ui_in_m2_rpm      = { TYPE_FLOAT_100, 0, 0, MENU_TARGET(&motors[2].rpm), EE_M0RPM + EE_MOTOR_SPACE * 2};
MENU_VALUE   ui_in_m2_ratio    = { TYPE_FLOAT_100, 0, 0, MENU_TARGET(&motors[2].ratio), EE_M0RATIO + EE_MOTOR_SPACE * 2};

MENU_ITEM    ui_it_m2_rot      = { {"Rotary"}, ITEM_VALUE, 0, MENU_TARGET(&ui_in_m2_rot) };
MENU_ITEM    ui_it_m2_flip     = { {"Invert Dir"}, ITEM_VALUE, 0, MENU_TARGET(&ui_in_m2_flip) };
MENU_ITEM    ui_it_m2_rpm      = { {"RPM"}, ITEM_VALUE, 0, MENU_TARGET(&ui_in_m2_rpm) };
MENU_ITEM    ui_it_m2_ratio    = { {"Ratio"}, ITEM_VALUE, 0, MENU_TARGET(&ui_in_m2_ratio) };

MENU_LIST    ui_list_m2[]      = { &ui_it_m2_rot, &ui_it_m2_flip, &ui_it_m2_rpm, &ui_it_m2_ratio };
MENU_ITEM    ui_it_m2List      = { {"Axis 3"}, ITEM_MENU, MENU_SIZE(ui_list_m2), MENU_TARGET(&ui_list_m2) };


 // Create motors sub-menu
MENU_LIST    ui_list_motors[]  = { &ui_it_m0List, &ui_it_m1List, &ui_it_m2List };
MENU_ITEM    ui_it_motors      = { {"Motors"}, ITEM_MENU, MENU_SIZE(ui_list_motors), MENU_TARGET(&ui_list_motors) };


 // ===== Main Menu
 
 // Create top-level menu
MENU_LIST    ui_list_top[]     = { &ui_it_camList, &ui_it_motors };
                  // Root item is always created last, so we can add all other items to it
MENU_ITEM    ui_it_root     = { {"Root"},        ITEM_MENU,   MENU_SIZE(ui_list_top),    MENU_TARGET(&ui_list_top) };


 // initialize LCD object
LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

OMMenuMgr Menu(&ui_it_root);



void uiMenuSetup() {

  lcd.begin(LCD_COLS, LCD_ROWS);
  
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

void uiCheck() {
 Menu.checkInput();
 
}

void testAction() {

 digitalWrite(5, HIGH);  
}


void uiDraw(char* p_text, int p_row, int p_col, int len) {
  lcd.setCursor(p_col, p_row);
  
  for( int i = 0; i < len; i++ ) {
    if( p_text[i] < '!' || p_text[i] > '~' )
      lcd.write(' ');
    else  
      lcd.write(p_text[i]);
  }
}


void uiClear() {
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Enter for Menu");
}



