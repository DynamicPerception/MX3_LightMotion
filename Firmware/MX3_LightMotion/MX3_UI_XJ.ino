

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
  "JUMP!" Easter Egg
  
  Original code by Parker Dillmann, with modifications by C. A. Church
  ========================================
  
*/

byte jump_obstacle[8] = {
  B00000,
  B00000,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
};

byte jump_player[8] = {
  B00100,
  B01010,
  B00100,
  B11111,
  B00100,
  B01010,
  B01010,
};



void jumpSetup() {

  lcd.createChar(2, jump_obstacle);
  lcd.createChar(3, jump_player);

}

void uiJump() {
  
 int    score = 0;
 byte  obsPos = 0;
 byte playPos = 1;
 byte shownUp = 0;
 
 unsigned long lastTm = millis();
 
 lcd.clear();
 Menu.enable(false);
 
 
 while(1)  {
   
   byte button = Menu.checkInput();
   
   if(button == BUTTON_SELECT) {
     Menu.enable(true);
     return;
   }

  

   if( button == BUTTON_INCREASE ) {
       // can only jump before the stone gets to you
     if( obsPos > 3 ) {
       playPos = 0;
       shownUp = 3;
     }
   }
   

  

   if( millis() - lastTm < 100 )
     continue;
     
   lastTm = millis();
   
   if( obsPos == 3 && playPos == 0 ) {
     score++;
   }
   else if( obsPos == 3 && playPos == 1 )  {
     
     lcd.clear();
     lcd.print("GAMEOVER MAN!");
     lcd.setCursor(0,1);
     lcd.print("Score: ");
     lcd.print(score);
     delay(1000);
     
     score  = 0;
     obsPos = 15;
     
     continue;
   }
    

   lcd.clear();
   lcd.setCursor(12,0);
   lcd.print(score);
   
   lcd.setCursor(obsPos, 1);
   lcd.write(2);
   lcd.setCursor(3, playPos);
   lcd.write(3);
   
   if( playPos == 0 && shownUp == 0) {
     playPos = 1;
   }
   else if( playPos == 0 ) {
     shownUp--;
   }
     
   if( obsPos == 0 )
     obsPos = 15;
   else
     obsPos--;
  
  

 }
  
}

