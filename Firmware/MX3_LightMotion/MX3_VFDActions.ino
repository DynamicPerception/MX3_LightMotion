/* 

   MX3 LightMotion Firmware
   
   (c) 2008-2013 Kevin Melotti / Dynamic Perception LLC
   
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
  VFD Display Action Menu
  ========================================
  
*/



/** VFD Brightness Level

 Changes the brightness of the VFD  
 
 @author
 Kevin Melotti
 */

void VFDBrightness(byte brightness_state)
{
  switch (brightness_state)
  {
    case 0:
      lcd.send(Brightness_25, LOW);
      break;
    case 1:
      lcd.send(Brightness_50, LOW);
      break;
    case 2:
      lcd.send(Brightness_75, LOW);
      break;
    case 3:
      lcd.send(Brightness_100, LOW);
      break;
    default:
      lcd.send(Brightness_100, LOW);
  }       
}

