
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
  Menu Action Functions
  ========================================
  
*/

/** Initialize Alt Inputs Action

   Must be done after changing input mode.
   
   @author
   C. A. Church
*/

void uiMenuAltInit() {
 
 altSetup();

 lcd.clear();

 lcd.print(STR_INIT);
 lcd.setCursor(0, 1);
 lcd.print(STR_DONE);
 
 delay(2000); 
}


/** Reset Menu Action

 Gives user a chance to cancel within 9 seconds.  
 
 Once memory is reset, user is informed that they can only power-cycle MX3.
 
 @author
 C. A. Church
 */
 
void uiMenuResetMem() {
 
 lcd.clear();

 lcd.print(STR_RES1);
 lcd.setCursor(0, 1);
 lcd.print(STR_RES2);
 
 Menu.enable(false);
 
 unsigned long cancelTm = millis();
 
 while( 1 ) {
   byte button = Menu.checkInput();
   
   if( button == BUTTON_SELECT ) {
     Menu.enable(true);
     return;
   }
   else {
     unsigned long diff = millis() - cancelTm;
     
     if( diff >= 9100 )
       break;
     
     unsigned int dispTime = (9100 - diff) / SECOND;
     lcd.setCursor(9, 0);
     lcd.print(dispTime, DEC);
   }
 }
 
 lcd.clear();
 OMEEPROM::saved(false);
 
 lcd.print(STR_RES3);
 lcd.setCursor(0, 1);
 lcd.print(STR_RES4);
 while(1) 
  ; // block all activity 
}
