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

/*

  ========================================
  Sensor Functions
  ========================================
  
*/


byte sensor_statFlags = 0;


/** Set up and Initialize Sensors

@author
PDillmann

 */
void sensorSetup() {
   pinMode(CUR_SENSOR,     INPUT);
   pinMode(VOL_SENSOR,     INPUT);
   pinMode(TEMP_SENSOR_0,  INPUT);
   pinMode(TEMP_SENSOR_1,  INPUT);
   pinMode(TEMP_SENSOR_2,  INPUT);
   pinMode(LCD_HEATER_12V, OUTPUT);
   pinMode(LCD_HEATER_24V, OUTPUT);
   
   digitalWrite(LCD_HEATER_12V, HIGH);
   digitalWrite(LCD_HEATER_24V, HIGH);
}

/** Get the current for all motors (combined)

  @return
  Amerpage, in milliAmps
  
  @author
  PDillmann
 */
unsigned int sensorCurrent() {
  
  int current = analogRead(CUR_SENSOR);
  
  current = ((float) current * 16.292) - 16;
     
  current = current <= 0.0 ? 0.1 : current;
  
  return current;
}

/** Get Input Power Voltage

    @return
    Voltage, in fractional volts

   @author
   PDillmann
 */
float sensorVoltage() {


  int voltage = analogRead(VOL_SENSOR);
  
  voltage = voltage <= 0 ? 1 : voltage;
  
  float outVolts = ((float) voltage * 29.307) / 1000.0;
  
  return outVolts;
}

/** Get the temperature for a motor controller

    @param p_sensor
    The sensor to read
    
    @return
    Temperature, in Celsius
    
   @author
   PDillmann
 */
float sensorTemp(byte p_sensor) {
  
  int temp = analogRead(TEMP_SENSOR_0 - p_sensor);
 
   // deal with negative values
  temp = temp < 0 ? 1 : temp;
  
  float outTemp = ((float) temp - 82.0) / 4.0;
 
 return outTemp;
}

/** Poll Sensors

 @author
 PDillmann
 */
 
void sensorPoll() {
  
    // check input voltage
   float volts = sensorVoltage();
      
   if( sensor_enVWarn ) {
    
      // ignore voltage warning if running on USB only
     if(volts > 5.5 && volts < sensor_minVoltage) {
       sensor_statFlags |= SENS_VOLT_FLAG; 
     }
     else {
       sensor_statFlags &= ~SENS_VOLT_FLAG;
     }
   }
   else {
     sensor_statFlags &= ~SENS_VOLT_FLAG;
   }
   
     // check total motor current draw

   if(sensorCurrent() > MAX_CURRENT) {
     sensor_statFlags |= SENS_CURRENT_FLAG; 
     motorStop(); // force a motor stop
   }
   else {
     sensor_statFlags &= ~SENS_CURRENT_FLAG;
   }
   
     // for each sensor, check temperature

   if(sensorTempMax() > MAX_TEMP) {
         sensor_statFlags |= SENS_TEMP_FLAG; 
         motorStop(); // force a motor stop
    }
    else {
         sensor_statFlags &= ~SENS_TEMP_FLAG;
    }
    
   if( sensor_enHeater ) {
  
      //check voltage to see to use LCD_HEATER_24V or LCD_HEATER_12V. Uses p-channel mosfets, low is active.
      //turn off heater before switching power sources.  
      
     if (volts > 13.5) {
       digitalWrite(LCD_HEATER_12V, HIGH);
       digitalWrite(LCD_HEATER_24V, LOW);
     }
     else {
       digitalWrite(LCD_HEATER_24V, HIGH);
       digitalWrite(LCD_HEATER_12V, LOW);
     }
   }
   else {  //make sure heater is off 
     digitalWrite(LCD_HEATER_24V, HIGH);
     digitalWrite(LCD_HEATER_12V, HIGH);
   }
}

/** Maximum Temperature Across All Temp Sensors

  @return 
  Maximum temperature (in celsius) read from any temp sensor
  
  @author
  C. A. Church
  */
  
float sensorTempMax() {
  
  float maxTemp = 0.0;
  
  for( byte i = 0; i < 3; i++ ) {
    float temp = sensorTemp(i);
    maxTemp = temp > maxTemp ? temp : maxTemp;
  }
  
  return maxTemp;
}


