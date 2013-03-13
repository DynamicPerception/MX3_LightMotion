/* 

   MX3 LightMotion Firmware
   
   (c) 2013 Parker Dillmann / Dynamic Perception LLC
   
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

/** Set up and Initialize Sensors

 */
void sensorSetup()
{
   pinMode(CUR_SENSOR, INPUT);
   pinMode(VOL_SENSOR, INPUT);
   pinMode(TEMP_SENSOR_0, INPUT);
   pinMode(TEMP_SENSOR_1, INPUT);
   pinMode(TEMP_SENSOR_2, INPUT);
}

/** Get the current for the motors

    Returns amperage in mA.

 */
int getCurrent()
{
  int current;
  
  current = analogRead(CUR_SENSOR);
  
  current = (current * 16.292) - 16;
     
  return current;
}

/** Get the voltage for the motors

    Returns voltage in mV.

 */
int getVoltage()
{
  int voltage;

  voltage = analogRead(VOL_SENSOR);
  
  voltage = voltage * 29.307;
  
  return voltage;
}

/** Get the temperature for a motor controller

    Returns temperature in C.

 */
int getTemp(byte sensor)
{
  int temp;
 
  temp = analogRead(TEMP_SENSOR_0 - sensor);
 
  temp = (temp - 82) / 4;
 
 return temp;
}

void poleSensors()
{
   voltage = getVoltage(); 
   if(voltage < voltage_th)
   {
     low_voltage = 1; 
   }
   else
   {
     low_voltage = 0;
   }
 
   current = getCurrent();
   if(current > max_current)
   {
     over_current = 1; 
     motorStop();
   }
   else
   {
     over_current = 0;
   }
   for(int i = 0; i < 3; i++)
   {
      temp[i] = getTemp(i); 
      if(temp[i] > max_temp)
      {
         over_temp = 1; 
         motorStop();
      }
      else
      {
         over_temp = 0;
      }
   }
}




