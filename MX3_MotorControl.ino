/*


MX3 LightMotion

See dynamicperception.com for more information


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
  Motor control functions
  ========================================
  
*/



 // predeclare these functions to provide a default in the prototype

void motorStop(boolean p_once = false);
void motorSpeed(byte p_motor, float p_rel, boolean p_ramp = false);
void motorSpeedCalc(byte p_motor, float p_ratio, boolean p_ramp = false);
void run_motors(boolean p_once = false);


 // control variables
 
   // minimum pwm timing period is 100 uS
unsigned int motor_pwm_minperiod = 600;

  // maximum number of periods per minute
float motor_pwm_maxperiod  = ( 60000000.0 / (float) motor_pwm_minperiod );

int motor_dir_delay = 500;

boolean motor_flushSMS = false;

//Manaual move setting - 0 for hold to move, 1 for select to stop
byte mmSetting = 0;

//increment for the motor speed in continous mode 
float motorIncrement = 0.01;

//SMS on period ration - determines the speed of the motor in SMS mode, periodsOn/periodsOff
float smsOnPeriodRatio = 4;

 // create an array of structures for our three motors

//MotorDefinition motors[] = { MotorDefinition(), MotorDefinition(), MotorDefinition() };

 // Motor Presets = {{"Preset Name"}, motor speed in RPM, ratio (pulley pitch dia or rotary stage gear ratio), rotary? 0 or 1}
 
MotorPreset motorPresets[] = { 
                                {{"Custom"}, 1.0, 1.0, 0},
								{ { "Pan/Tilt 42.13" }, 40.75, 0.306, 1 },
								{ { "Dolly 42.13" }, 40.75, 3.346, 0 },
								{ { "Pan/Tilt 21.58" }, 20.89, 0.306, 1 },
								{ { "Dolly 21.58" }, 20.89, 3.346, 0 },
								{ { "Pan/Tilt 8.13" }, 8.13, 0.306, 1 },
								{ { "Dolly 8.13" }, 8.13, 3.346, 0 },
								{ { "Pan/Tilt 3.07" }, 3.07, 0.306, 1 },
								{ { "Dolly 3.07" }, 3.07, 3.346, 0 },
								{ { "Pan/Tilt 1.56" }, 1.12, 0.306, 1 },
								{ { "Dolly 1.56" }, 1.12, 3.346, 0 }						
};
                                
byte motorPresetSelected[] = { 0, 0, 0 };

// Constants for the motor speed linear equation
const float FIT_A = 3.5152097902;
const float FIT_B = -1.4196969697;
const float FIT_C = 71.2;

/** Set up and Initialize Motors

 @author
 C. A. Church
 */
 
void motorSetup() {
 
  for(byte i = 0; i < (MOTOR_COUNT * 3); i++ ) {
    pinMode(MOTOR_DIR_PINSTART + i, OUTPUT);
  }
  
  for(byte i = 0; i < MOTOR_COUNT; i++ ) {
          // set to 10% speed as default
    motorSpeed(i, motors[i].speed);
    motors[i].desiredDirection = (motors[i].flags & MOTOR_CDIR_FLAG ? 1 : 0);
  }
 
   digitalWrite(MOTOR_INH_0, HIGH);
   digitalWrite(MOTOR_INH_1, HIGH);
   digitalWrite(MOTOR_INH_2, HIGH);
   
  
}

/** Set Target Speed for a Given Motor

Sets target speed in real units

@param p_motor
The motor to set the speed for, 0, 1, or 2

@param p_speed
The target motor speed in inches/min or deg/min

@author
M. Ploof
*/

void setTargetSpeed(byte p_motor, float p_speed) {

	motors[p_motor].target_speed = p_speed;

	// Save the new target speed to EEPROM
	OMEEPROM::write(EE_DES_SPEED0 + EE_MOTOR_SPACE_V1_1 * p_motor, motors[p_motor].target_speed);

}

/** Set Target SMS Distance for a Given Motor

Sets target SMS distance in real units

@param p_motor
The motor to set the speed for, 0, 1, or 2

@param p_speed
The target motor SMS distance in inches/move or deg/move

@author
M. Ploof
*/

void setTargetDist(byte p_motor, float p_dist) {

	motors[p_motor].target_sms_distance = p_dist;

	// Save the new target speed to EEPROM
	OMEEPROM::write(EE_DES_SMSDIST0 + EE_MOTOR_SPACE_V1_1 * p_motor, motors[p_motor].target_sms_distance);

}


/** Set Speed Percent for a Given Motor
 
 Sets speed for a given motor as a percentage of time, per minute, that the motor
 will be on.
 
 @param p_motor
 The motor to set the speed for, 0, 1, or 2
 
 @param p_rel
 The relative ontime, expressed as a value from 0.0 to 1.0
 
 @param p_ramp
 (Optional, default = false) Whether or not this speed change is part of a ramp cycle
 
 @author 
 C. A. Church
 */
 
void motorSpeed(byte p_motor, float p_rel, boolean p_ramp) {
 
    // periods off between each on period to achieve percentage of time moving per minute
  
//  motor_pwm_maxperiod  = ( 60000000.0 / (float) motor_pwm_minperiod );
//  
//  if( p_rel <= 0.0 ) {
//      motors[p_motor].onCycleRatio = 0.0;
//  }
//  else if( p_rel >= 1.0 ) {
//      motors[p_motor].onCycleRatio = motor_pwm_maxperiod;
//  }
//  else {
//      float offTime       = (1.0 - p_rel) * motor_pwm_maxperiod;
//      float onTime        = motor_pwm_maxperiod - offTime;
//      float onCycleRatio = onTime / offTime;
//      motors[p_motor].onCycleRatio = onCycleRatio;
//  }

	USBSerial.print("Requested power for motor");
	USBSerial.print(p_motor);
	USBSerial.print(": ");
	USBSerial.println(p_rel);

	// Don't allow the speed to be set below 10% or above 100% of full power if we're in continuous mode and EZ mode
	if (p_rel < 0.05 && !motion_sms && ez_mode) {
		lcd.clear();
		lcd.home();
		lcd.print("Mot ");
		lcd.print(p_motor + 1);
		lcd.print(" Spd Low");
		lcd.setCursor(0, 1);
		lcd.print("Use SMS Mode");
		delay(1000);
		lcd.clear();
	}
	if (p_rel > 1.0 && !motion_sms && ez_mode) {
		lcd.clear();
		lcd.home();
		lcd.print("Mot ");
		lcd.print(p_motor + 1);
		lcd.print(" Max Speed");
		delay(1000);
		lcd.clear();
	}

	// Don't let the motor power go below 10% in continuous EZ mode
	if (ez_mode && !motion_sms && p_rel < 0.05) {
		p_rel = 0.05;
		motors[p_motor].speed = p_rel;
		setTargetSpeed(p_motor, motorSpeedCalc(p_motor));
		motors[p_motor].ez_adjust = motors[p_motor].target_speed / motors[p_motor].ez_center_val;
		return;

	}
	// Don't let the motor power go above 100% at any time
	else if (ez_mode && !motion_sms && p_rel > 1.0) {
		p_rel = 0.95;
		motors[p_motor].speed = p_rel;
		setTargetSpeed(p_motor, motorSpeedCalc(p_motor));
		motors[p_motor].ez_adjust = motors[p_motor].target_speed / motors[p_motor].ez_center_val;
		return;

	}



	// Assign the new value to the motor's speed setting
	motors[p_motor].speed         = p_rel;

	//USBSerial.print("This motor's speed is now set to : ");
	//USBSerial.println(p_rel);
	//USBSerial.println("");
	//
	//if (motion_sms) {
	//	USBSerial.print("The number of cycles the SMS mode should move: ");
	//	USBSerial.println(motor_pwm_maxperiod * motors[p_motor].speed);
	//	USBSerial.println("");
	//}
  
	if( ! p_ramp ) 
	motors[p_motor].setSpeed    = p_rel;
    
}

/** Get Speed Percent for a Given Motor

 Returns the currently set speed of the motor, in percentage from 0.0 to 1.0
 
 @param p_motor
 The motor to get the speed for, 0, 1, or 2
 
 @return
 A floating point number, from 0.0 to 1.0
 
 @author
 C. A. Church
 */
  
float motorSpeed(byte p_motor) {
  return( motors[p_motor].speed );
}


/** Coefficients and variables for use with the RMS Voltage Calculation

	Used in pwmRMS(), fitFunction(), and fitFunctionPrime()
	
**/

const float RMS_FIT_0 = -0.00109590955038006;
const float RMS_FIT_1 = 0.235512472783738;
const float RMS_FIT_2 = 0.235512472783738;

const float RMS_FIT_3 = 0.0000114671955623049;
const float RMS_FIT_4 = -0.00178655403946091;
const float RMS_FIT_5 = 0.129178170451785;
const float RMS_FIT_6 = 5.70717895443223;
const float RMS_FIT_7 = 1.017;

const float HIGH_THRESH = 9.2; // Voltage above which the high initial estimate for Newton's method will be used
const float REFERENCE_VOLTAGE = 12.15; // The supply voltage at which the motor calibration curves were determined
const float REFERENCE_VOLTAGE_NOMINAL = 12.0; // The voltage for which the motor max RPM ratings are established
const float MOTOR_MAX_RPM = 3000.0; // The max no-load speed of the motors without any gearing reduction

/** Get RMS Voltage for  Given Power Setting or Power Setting for Given Voltage

 Returns the root mean square voltage output for a particular power percentage or vice versa.
 Use this with motor speed fitting curve to estimate actual motor speed, or for assigning power
 percentage based on a desired speed/distance setting. The voltage output maps to percent speed 
 value in a discontinuous fasion. A 2nd degree polynomial is used to fit the values 51% and below, 
 and a 3rd order polynomial fits the values above 51%.

 @param p_value
 See p_find_percent description

 @param p_find_percent
 If this is false, the p_value should be the motor power between 0.0-1.0. The function will return 
 the RMS voltage produced at that setting. If this parameter is true, the p_value should be 
 a voltage value and the percent power setting required to produce that voltage will be returned.

 @author
 M. Ploof
 */

float pwmRMS(float p_value, boolean p_find_percent) {

	// If we're looking for the voltage from a power setting
	if (!p_find_percent) {
		p_value *= 100; // Multiply by 100 to convert from decimal to true percentage 0-100
		float rms; // The RMS voltage of the PWM output

		//USBSerial.print("The input percentage is: ");
		//USBSerial.println(p_value);

		// Check and correct for out of bounds requests
		if (p_value < 0) {

			p_value = 0;
		}
		else if (p_value > 100) {

			p_value = 100;
		}

		// Calculate voltage
		if (p_value == 0) {

			rms = 0;
		}
		else if (p_value > 0 && p_value <= 51) {

			rms = RMS_FIT_0 * pow(p_value, 2) + RMS_FIT_1 * p_value + RMS_FIT_2;
		}
		else if (p_value > 51 && p_value <= 100) {

			rms = (RMS_FIT_3 * pow(p_value, 3) + RMS_FIT_4 * pow(p_value, 2) + RMS_FIT_5 * p_value + RMS_FIT_6) / RMS_FIT_7;
		}

		//USBSerial.print("The pre-battery correction RMS value is: ");
		//USBSerial.println(rms);

		// Correct for current battery voltage
		rms *= (averageVoltage() / REFERENCE_VOLTAGE);

		return(rms);
	}


	// If we're looking for the power percent to produce a desired voltage
	else {

		const int INTERATIONS = 3; // Number of times to run Newton's method for estimating power percentage. Typically converges after 3 iterations.
		const int ESTIMATE_LOW = 25; // Starting power percentage guess for Newton's method for voltage <= HIGH_THRESH
		const int ESTIMATE_HIGH = 75; // Starting power percentage guess for Newton's method for voltage > HIGH_THRESH
		float power_percent; // Estimated power percentage

		// Set the initial guess for Newton's method based on a voltage theshold at ~51% power
		if (p_value >= HIGH_THRESH)
			power_percent = ESTIMATE_HIGH;
		else
			power_percent = ESTIMATE_LOW;

		// Run Newton's method
		for (byte i = 0; i < INTERATIONS; i++) {

			power_percent = power_percent - (rmsCalc(power_percent, p_value) / rmsCalcPrime(power_percent, p_value));
		}

		power_percent /= 100; // Divide by 100 to convert from 0-100% to 0.0-1.0 range

		// Return the result of the Newton's method calculation
		return power_percent;
	}
}

/** RMS Voltage Calculation Function

  @param p_power_percent
  Estimate of motor power between 0 - 100. 
  
  Returns the voltage for that power setting.

  @author
  M. Ploof

*/

float rmsCalc(float p_power_percent, float desired_voltage) {

	float rms_voltage;

	// If the voltage is above the threshold value (i.e. need to use the 3rd order equation fit)
	if (desired_voltage > HIGH_THRESH) {
		// Because Newton's method will solve power percent for the root of the equation, desired_voltage is subtracted at the end to set the equation == 0
		rms_voltage = RMS_FIT_3 * pow(p_power_percent, 3) + RMS_FIT_4 * pow(p_power_percent, 2) + RMS_FIT_5 * p_power_percent + RMS_FIT_6 - desired_voltage;
	}
	else {
		rms_voltage = RMS_FIT_0 * pow(p_power_percent, 2) + RMS_FIT_1 * p_power_percent + RMS_FIT_2 - desired_voltage;
		
	}

	return rms_voltage;
	
}

/** RMS Voltage Claculation Function Derivative

  @param p_power_percent
  Estimate of motor power between 0 - 100. 
  
  Returns the derivative value for that power setting.

  @author
  M. Ploof

*/

float rmsCalcPrime(float p_power_percent, float desired_voltage) {

	float rms_voltage_prime;

	if (desired_voltage > HIGH_THRESH) {
		rms_voltage_prime = 3 * RMS_FIT_3 * pow(p_power_percent, 2) + 2 * RMS_FIT_4 * p_power_percent + RMS_FIT_5;;
	}
	else {
		rms_voltage_prime = RMS_FIT_0 * p_power_percent + RMS_FIT_1;
	}

	return rms_voltage_prime;
}

/** Refresh the motor power settings to compensate for changing battery voltage

	@param p_force_refresh
	If true, then don't wait for the wait time to be exceeded before refreshing

	@author
	M. Ploof

*/

void refreshMotors(boolean force_refresh) {
	
	const int WAIT = 2000;						// How long to wait in milliseconds between refreshing motor power
	static long time_at_refresh = millis();		// Variable to keep track of how long it's been since the motor power was last refreshed
	long temp_time;
	static int fl_old = 0;
	static boolean fl_changed = false;
	static int delay_old = 0;
	static boolean delay_changed = false;


	// If the current target speed is above the max possible speed upon refresh, update it to the new max speed
	for (byte i = 0; i < MOTOR_COUNT; i++) {
		float speed_check = motors[i].target_speed < 0 ? 0 : motors[i].target_speed > motorMaxSpeedCompensated(i) ? motorMaxSpeedCompensated(i) : motors[i].target_speed;
		setTargetSpeed(i, speed_check);
	}

	if (millis() - time_at_refresh > WAIT && (ez_mode || (running && units != PERCENT)) || force_refresh) {
		temp_time = micros();

		// Check to see if the focal length or the camera delay has changed
		fl_changed = !(fl_old == camera_focal_length);
		delay_changed = !(delay_old == camera_delay);
		
		for (byte i = 0; i < MOTOR_COUNT; i++) {

			// Don't modify the speed if the motor is currently ramping
			if (motors[i].inRamp){
				USBSerial.print("Motor ");
				USBSerial.print(i);
				USBSerial.print(" ramp state: ");
				USBSerial.println(motors[i].inRamp);
				continue;
			}
			// If running in continuous manual mode
			else if (!ez_mode && !motion_sms) {

				motorSpeedCalc(i, motors[i].target_speed);

			}
			// If running in SMS manual mode
			else if (!ez_mode && motion_sms) {
				motorSpeedCalc(i, motors[i].target_sms_distance);
			}
			// If running in EZ mode and the focal length has changed since the last check, run EZmodeUpdate() to update the center value
			else if (ez_mode && (fl_changed || delay_changed)) {
				EZmodeUpdate(i);
			}
			// If the focal length hasn't changed, update normally for cont. and SMS modes
			else if (ez_mode && !motion_sms && !(fl_changed || delay_changed)) {
				motorSpeedCalc(i,motors[i].target_speed);
			}
			else if (ez_mode && motion_sms && !(fl_changed || delay_changed)) {
				motorSpeedCalc(i, motors[i].target_sms_distance);
			}
		}

		// Save the current focal length and camera delay value for future reference
		fl_old = camera_focal_length;
		delay_old = camera_delay;

		//USBSerial.print("Refresh time: ");
		//USBSerial.println(micros() - temp_time);
		time_at_refresh = millis();
	}
}

/** Refresh the motor power settings based on EZ mode adjust values for all motors

@author
M. Ploof

*/

void EZmodeUpdateAll() {

	for (byte i = 0; i < MOTOR_COUNT; i++) {
		EZmodeUpdate(i);
	}
}

/** Refresh the motor power settings based on EZ mode adjust values

@param p_force_refresh
If true, then don't wait for the wait time to be exceeded before refreshing

@author
M. Ploof

*/

void EZmodeUpdate(byte p_motor){
	// Compute the percent speed based on the EZ mode adjust value
	byte gain;			// Adjusts slope of EZ mode speed function

	// Set the EZ mode base move value based upon the current preset setting.
	// Remember, rotary values are deg/move and deg/min, and linear are inch/move and inch/minute.

	const float REF_DISTANCE = 3.048;			// Reference distance in meters for center value calculation
	const float MOVE_PROPORTION = 2.31E-3;		// Proportion of the frame the camera should move during each linear SMS shot as a baseline
	const float ROT_MOVE_PROPORTION = 3.03E-3;  // Proportion of the angle of view the camera should move during each rotational SMS shot as a baseline
	const float SENSOR_WIDTH = 36;				// Width of the camera sensor in mm

	// Calculate the center value to be used based on the focal length
	// If running in rotary mode
	if ((motors[p_motor].flags & MOTOR_ROT_FLAG)){
		float angle_of_view = 2 * atan(SENSOR_WIDTH / (2 * camera_focal_length)) * RAD_TO_DEG;
		motors[p_motor].ez_center_val = angle_of_view * ROT_MOVE_PROPORTION;
		//USBSerial.print("The rotational SMS center value is: ");
		//USBSerial.print(motors[p_motor].ez_center_val, 5);
		//USBSerial.println(" deg/move");
	}
	// if running in linear mode
	else if (!(motors[p_motor].flags & MOTOR_ROT_FLAG)) {
		motors[p_motor].ez_center_val = ((REF_DISTANCE * SENSOR_WIDTH * MOVE_PROPORTION) / camera_focal_length) * INCH_PER_METER;

		//USBSerial.print("The linear SMS center value is: ");
		//USBSerial.print(motors[p_motor].ez_center_val, 5);
		//USBSerial.println(" in/move");
	}

	// If continuous mode is enabled, calculate the speed required to move the same distance per shot as in SMS mode.
	// Divide the distance by the time between camera shots, then convert units to in/min or deg/min by multiplying by SEC_PER_MIN.
	if (!motion_sms) {

		// Determine the speed necessary to emulate the SMS move center value at a continuous speed
		motors[p_motor].ez_center_val = (motors[p_motor].ez_center_val / camera_delay) * SEC_PER_MIN;

		// Multiply that speed by the ez_adjust value
		float speed_calc = motors[p_motor].ez_center_val*motors[p_motor].ez_adjust;
		setTargetSpeed(p_motor, speed_calc);

		// Set the speed percentage based upon the calculated speed
		motorSpeedCalc(p_motor, motors[p_motor].target_speed, false);

		//USBSerial.print("The camera delay value is: ");
		//USBSerial.println(camera_delay);
		//USBSerial.print("The continuous center value is: ");
		//USBSerial.print(motors[p_motor].ez_center_val);
		//if (motors[p_motor].flags & MOTOR_ROT_FLAG)
		//	USBSerial.println(" deg/min");
		//else
		//	USBSerial.println(" in/min");
	}
	// SMS mode
	else {

		// Save the calculated speed
		float speed_temp = motors[p_motor].ez_center_val*motors[p_motor].ez_adjust;
		setTargetDist(p_motor, speed_temp);

		// Set the speed percentage based upon the calculated speed
		motorSpeedCalc(p_motor, motors[p_motor].target_sms_distance, false);
	}
}



/** Get Actual Speed for a Given Motor

 Returns the currently set speed for the motor, expressed as the output
 ratio (RPM * Ratio * Speed) for linear, and degrees (RPM * Ratio * Speed * 360).
 
 M. Ploof: --> "output ratio" doesn't make sense here. The returned value is a 
 speed, either in linear in/min or cm/min or rotary degrees/min, not a ratio!

 
 @param p_motor
 The motor to get the speed for, 0, 1, or 2
 
 @return
 A floating point integer, from 0.0 to 1.0 (motor power setting)
 
  @author 
 C. A. Church

 M. Ploof: Modified to calculate the speed based upon the RMS voltage supplied via PWM
 */
 
float motorSpeedCalc(byte p_motor) {
  
  
 /*
  float spdPct = motorSpeed(p_motor);

  // The speed correction factors were determined with a battery
  // voltage of 12.19v. Use this to find a final correction factor
  // that accounts for falling voltage as the battery is discharged
  const float REF_VOLTAGE = 12.19;

  
*/

	float voltage;

	// If the value is 0 (or close to it) don't bother calculating and just call the speed/distance 0
	if (motorSpeed(p_motor) < 0.05)
		return 0.0;

	// For continuous motion
	if (!motion_sms) {
		// Get the voltage output to the motor requested
		voltage = pwmRMS(motorSpeed(p_motor), false);
		//USBSerial.print("Output RMS Voltage");
		//USBSerial.println(voltage, 3);
	}
	// For SMS motion
	else {
		// Get the PWM RMS voltage for full power
		voltage = pwmRMS(1.0, false);
	}
	
	// Calculate the motor speed (without gearbox)
	float spd = (FIT_A * voltage + FIT_B) * FIT_C; // Linear fit equation for RPMs/V, derived from motor w/ 42RPM gearbox (reduction ratio FIT_C == 72.0) 
	//USBSerial.print("Motor RPMs");
	//USBSerial.println(spd, 3);

	// Calculate the output rpm based on the gearbox ratio
	spd /= (MOTOR_MAX_RPM / motors[p_motor].rpm);
	//USBSerial.print("Output RPMs: ");
	//USBSerial.println(spd, 3);

	// Calculate the linear or rotational speed based on the external gearing
	if (motors[p_motor].flags & MOTOR_ROT_FLAG) {

		spd *= motors[p_motor].ratio * 360; // Output in deg/min
	}
	else {

		spd *= motors[p_motor].ratio; // Output in in/min
		
	}
	//USBSerial.print("Current ratio is: ");
	//USBSerial.println(motors[p_motor].ratio, 3);
	//USBSerial.print("Speed % is: ");
	//USBSerial.println(motors[p_motor].speed, 3);
	//USBSerial.print("Distance per minute is: ");
	//USBSerial.println(spd, 3);


	// Recalculate as distance for SMS motion
	if (motion_sms) {
		// SMS distance is the speed percentage * full power speed in inches/min or deg/min
		spd *= motorSpeed(p_motor);
	}
	//USBSerial.print("Distance per move is: ");
	//USBSerial.println(spd, 3);
	//USBSerial.println("");


 /*   // handle rotary different than linear
  if (motors[p_motor].flags & MOTOR_ROT_FLAG) {
	  spd = (spd * 360.0 / motors[p_motor].ratio) * weighted;
  }
  else {
	  spd = (spd * motors[p_motor].ratio) * weighted;
  }*/
 
	return(spd);
}

/** Set Speed as in/min, cm/min, deg/min for a Given Motor

 Sets the current speed for the motor, expressed as the linear distance
 (in/min or mm/min, which corresponds to (Max RPM * Pulley Pitch Diameter * Speed %) 
 for linear, and degrees/min (Max RPM * Rotary Gear/Pulley Ratio * Speed % * 360) for
 rotary.

 @param p_motor
 The motor to get the speed for, 0, 1, or 2

 @param p_spd_desired
 This is the speed in in/min or deg/min or SMS distance in in/move or deg/move that user wants to set
 
 @param p_ratio
 A floating point integer, from 0.0 to maximum possible travel
 
 @param p_ramp
 (Optional, default = false) Whether or not this speed change is part of a ramp cycle
 
  @author 
 C. A. Church
 */
 
void motorSpeedCalc(byte p_motor, float p_desired_spd_dist, boolean p_ramp) {
  
	const int DEG_PER_ROT = 360; // Number of degrees per rotation
	float rpms;					 // Desired RPM output directly from the motor without any gearing
	float voltage_computed = 0;	 // Used to store actual and nominal computed voltage values
	float power_pct;				 // Percent from 0.0 to 1.0 of maximum motor speed
	float gearbox_ratio = (MOTOR_MAX_RPM / motors[p_motor].rpm);
	float sms_time;

	// If the set distance or speed is 0 (or close to it), don't calculate, just set the power to 0
	if (p_desired_spd_dist < .01){
		power_pct = 0.0;
		motorSpeed(p_motor, power_pct, p_ramp);
		return;
	}

	// For continuous mode
	if (!motion_sms){

		// In rotary mode 
		if (motors[p_motor].flags & MOTOR_ROT_FLAG)
			rpms = (p_desired_spd_dist * gearbox_ratio) / (DEG_PER_ROT * motors[p_motor].ratio);
		// In linear mode
		else
			rpms = (p_desired_spd_dist * gearbox_ratio) / motors[p_motor].ratio;

		//USBSerial.print("**********Setting Motor ");
		//USBSerial.print(p_motor);
		//USBSerial.println(" Power Via Speed Value**********");
		//USBSerial.println("");

		//USBSerial.print("The desired RPMs from the gearbox is: ");
		//USBSerial.println(rpms / gearbox_ratio);

		//USBSerial.print("The desired RPMs from the motor is: ");
		//USBSerial.println(rpms);

		// Find the voltage required to produce the desired motor RPM output
		voltage_computed = ((rpms / FIT_C) - FIT_B) / FIT_A; // Linear fit equation for RPMs/V solved for voltage

		//USBSerial.print("The voltage needed for this RPM setting is: ");
		//USBSerial.println(voltage_computed);

		// Compensate for current battery voltage
		voltage_computed = voltageCompensation(voltage_computed);

		//USBSerial.print("The look-up voltage is: ");
		//USBSerial.println(voltage_computed);

		// Look up the power percent setting needed to achieve the nominal voltage value
		power_pct = pwmRMS(voltage_computed, true);

		//// Make sure we don't try to use a power setting outside 0.0-1.0
		//power_pct = power_pct > 1.0 ? 1.0 : power_pct < 0.0 ? 0.0 : power_pct;

		//USBSerial.print("The power percent setting is: ");
		//USBSerial.println(power_pct, 3);
		//USBSerial.println("");


	}

	// For SMS mode
	else {

		//USBSerial.print("**********Setting Motor ");
		//USBSerial.print(p_motor);
		//USBSerial.println(" Power Via SMS Distance Value**********");
		//USBSerial.println("");

		//// power_pct here is actually the time in minutes that the motor needs to run to complete the SMS move
		//
		//USBSerial.print("Desired distance move");
		//USBSerial.println(p_desired_spd_dist);
		//USBSerial.print("Max motor output RPM");
		//USBSerial.println(motorMaxSpeed(p_motor));
		
		power_pct = p_desired_spd_dist / motorMaxSpeed(p_motor); 
		//
		//USBSerial.print("SMS power_pct setting before voltage compensation");
		//USBSerial.println(power_pct, 5);
		//
		// Compesnate for the current battery voltage
		power_pct = voltageCompensation(power_pct);
		
		//USBSerial.print("SMS power_pct setting after voltage compensation");
		//USBSerial.println(power_pct, 5);

	}
  
	// Set the motorSpeed() with the calculated power setting
	motorSpeed(p_motor, power_pct, p_ramp);
}

/** Compensate for battery voltage

	Returns a nominal voltage or time (in minutes) that will maintain 
	speed or distance settings as battery voltage falls

	@param compensated_value
	Input to the function, which is multiplied by the correction factor

	@author
	M. Ploof
*/

float voltageCompensation(float compensated_value) {

	const float EMPERICAL_ADJUST = 1.06; // Returned voltages seemed to be about 6% low, so multiply by this factor for continuous moves
	
	// For continuous moves, apply the emperical adjustment factor
	if (!motion_sms)
		compensated_value *= ((REFERENCE_VOLTAGE / averageVoltage()) * EMPERICAL_ADJUST);
	else
		compensated_value *= (REFERENCE_VOLTAGE / averageVoltage());

	return compensated_value;
}


/** Get the Average Battery Voltage

	The sensorVoltage() value jumps around quite a bit, so average that over a few readings to get a more stable value
	
	@author
	M. Ploof
*/

float averageVoltage() {

	const byte READINGS = 5;			 // How many voltage readings to take when finding the average value
	float average_volts = 0.0;			 // Zero out the average volts value before averaging again
	// Find the average voltage for a number of readings
	for (byte i = 0; i < READINGS; i++) {
		float temp_volts = sensorVoltage();
		average_volts += temp_volts;
	}
	average_volts /= (float)READINGS;
	return average_volts;
}



/** Get Maximum Possible Speed as Output Ratio for a Given Motor

 Gets the current max speed for the motor, expressed as the output
 ratio (RPM * Ratio * Speed) for linear, and degrees (RPM * Ratio * Speed * 360).
 
 @param p_motor
 The motor to get the speed for, 0, 1, or 2
 
 @return p_ratio
 A floating point integer
 
  @author 
 C. A. Church
 */
 
float motorMaxSpeed(byte p_motor) {
  float spd = motors[p_motor].rpm * motors[p_motor].ratio;
  
  if( motors[p_motor].flags & MOTOR_ROT_FLAG )
      spd *= 360.0;
      
  return spd;
}

/** Get Maximum Possible Speed as Output Ratio for a Given Motor w/ Voltage Compensation

Gets the current max speed for the motor, expressed as the output
ratio (RPM * Ratio * Speed) for linear, and degrees (RPM * Ratio * Speed * 360), compensated for battery voltage

@param p_motor
The motor to get the speed for, 0, 1, or 2

@return p_ratio
A floating point integer

@author
M. Ploof
*/

float motorMaxSpeedCompensated(byte p_motor) {

	float spd = motorMaxSpeed(p_motor) * averageVoltage() / REFERENCE_VOLTAGE_NOMINAL;
	return spd;
}

/** Control Motor Direction
  
  @param p_motor
  Motor (0, 1, or 2)
  
  @param p_dir
  Direction (true or false)
  
  @author
  C. A. Church
  */
  
void motorDir(byte p_motor, boolean p_dir) {
  
  motors[p_motor].desiredDirection = p_dir;
  
//   bool flag_on = false;
//
//  //disable enable flag while switching directions
//  if(motors[p_motor].flags & MOTOR_ENABLE_FLAG){
//    flag_on = true;
//    motors[p_motor].flags &= ~MOTOR_ENABLE_FLAG;
//  }
//  
//  
//  //Turns off the motor diver so that it won't trip the battery when switching directions
//  if (p_motor == 0)
//    MOTOR_DRV_PREG &= ~(B00000001 << 2);
//  else if (p_motor == 1)
//    MOTOR_DRV_PREG &= ~(B00000001 << 5);
//  else 
//    MOTOR_DRV_PREG2 &= ~(B00000001);             
//  
//  //turns off driver
//  MOTOR_DRV_PREG  &= ~(B00000011 << (p_motor*3));
//    
//  //Set direction  
//  if (p_dir)
//    motors[p_motor].flags |= (MOTOR_CDIR_FLAG); 
//  else
//    motors[p_motor].flags &= ~(MOTOR_CDIR_FLAG);  
//        
//  //delays motor to prevent battery tripping
//  delayMicroseconds(65534);
//  delayMicroseconds(65534);
//  delayMicroseconds(65534);
//  delayMicroseconds(65534);
//  delayMicroseconds(65534);
//  delayMicroseconds(65534);
//  delayMicroseconds(65534);
//  delayMicroseconds(65534);
//  delayMicroseconds(65534);
//  
//  
//  //Turns the motor driver back on
//  if (p_motor == 0)
//    MOTOR_DRV_PREG |= (B00000001 << 2);
//  else if (p_motor == 1)
//    MOTOR_DRV_PREG |= (B00000001 << 5);
//  else 
//    MOTOR_DRV_PREG2 |= (B00000001); 
//  
//  //tursn the flag back on if it was on  
//  if(flag_on){
//    motors[p_motor].flags |= MOTOR_ENABLE_FLAG;
//  }
  
}


/** Flip All Motor Directions

 Inverts the direction of all motors.
 
 @author
 C. A. Church
 */
 
void motorDirFlip() {
    // foreach motor, invert its direction
    
  for(int i = 0; i < MOTOR_COUNT; i++)
  {
    motorDirFlip(i);
  }
    
}

/** Flip Motor Direction

 Inverts the direction of a single motor.
 
 @param p_motor
 Motor to flip direction
 
 @author
 C. A. Church
 */
 
void motorDirFlip(byte p_motor) { 
  
  if (!(motor_running))
  {
    byte p_dir = ! (boolean) (motors[p_motor].flags & MOTOR_CDIR_FLAG);
    if (p_dir)
      motors[p_motor].flags |= (MOTOR_CDIR_FLAG); 
    else
      motors[p_motor].flags &= ~(MOTOR_CDIR_FLAG); 

  }
 
  motorDir(p_motor, ! (boolean) (motors[p_motor].desiredDirection) );   //.flags & MOTOR_CDIR_FLAG));
  
}

/** Start All Motors Running

 Causes all user-enabled motors to begin running. If motors are not
 already running, will start required timer1 and attach interrupt.
 
 @param p_once
 Run motors once (i.e. shoot-move-shoot)?  defaults to false
 
 @author 
 C. A. Church
 */
 
void motorRun(boolean p_once) {
 
  if( motor_running )
    return;
    
    // (program)-enable each motor, if it does not have a lead-in/out
  for(byte i = 0; i < MOTOR_COUNT; i++ ) {
    if( ! motors[i].lead )
      motors[i].flags |= MOTOR_ENABLE_FLAG;
    
  }
    
  motorStartISR(p_once);
  
}

/** Start One Motor Running

 Initializes timer, if not already initialized, and sets one motor 
 to be program-enabled, allowing it to run.  
 
 You may call this multiple times to start multiple individual motors.
 
 @author
 C. A. Church
 */
 
void motorRun(boolean p_once, byte p_motor) {
 
 motors[p_motor].flags |= MOTOR_ENABLE_FLAG;
 
 if( ! running )
   motorStartISR(p_once);

}

/** Stop All Motors Running

 Stops execution of the timer for all motor movement, and sets all
 motors to be program-disabled, allowing a subsequent start one motor
 command to start only that motor.
 
 
 @author 
 C. A. Church
 */
 
void motorStop(boolean p_once) {
  motor_running = false;
  Timer1.detachInterrupt();
  for(byte i = 0; i < MOTOR_COUNT; i++ )
    motors[i].onCycleRatio = 0;
  
  
  if( ! p_once ) {
      // force stop on all motors (only do this UI inputs or continuous motion)
      // SMS ISR routine should not perform this activity
    for(byte i = 0; i < MOTOR_COUNT; i++ )
      motorStopThis(i);
  }
  
}


/** Stop this Motor

 Performs the activities for stopping one motor.
 
 */
 
void motorStopThis(byte p_motor) {
         // disable motor, set high flag to low, and disable force ramp flag
     motors[p_motor].flags &= (~( MOTOR_ENABLE_FLAG | MOTOR_HIGH_FLAG | MOTOR_RAMP_FLAG ) );
        // change output pin state
     MOTOR_DRV_PREG  &= (~(B00000011 << (p_motor*3)));
        // if speed != setspeed, set new speed back!
     motorSpeed(p_motor, motors[p_motor].setSpeed);
        // get rid of forced ramp
     motors[p_motor].forceRampStart = 0; 
         // clear ramp flag
     motors[p_motor].inRamp = 0;
}

/** Start Motor Driving Interrupt Service 

 Starts interrupt, sets data to indicate motors are running
 
 @param p_once
   Whether shooting continuous (false) or shootmoveshoot (true)
 @author
 C. A. Church
 */
 
void motorStartISR(boolean p_once) {
  
  motor_pwm_maxperiod  = ( 60000000.0 / (float) motor_pwm_minperiod );
  
  if( ! motor_running ) { 
      motor_running = true;
      Timer1.initialize(motor_pwm_minperiod);
      
      if( p_once ) {
        motor_flushSMS = true;
        Timer1.attachInterrupt(motorRunISRSMS);
      }
      else
        Timer1.attachInterrupt(motorRunISR);
  }
  
}


/** Motor Driving Interrupt Service Routine (SMS)

   This routine moves each motor for the time period specified:
   
     motor_pwm_maxperiod * motor[each].speed
     
   It will stop the motors once each motor that must move completes the move.
   
   @author
   C. A. Church
*/

void motorRunISRSMS() {
  
  static byte startedMoving;
  static byte doneMoving;
   
    // we've got to be careful when stopping during a move
  if( motor_flushSMS ) {   //1
    startedMoving = 0;      //2
    doneMoving   = 0;       //2
    motor_flushSMS = false;  //2
  }
  
  for( byte i = 0; i < MOTOR_COUNT; i++ ) {  //5
  
  
   // Function that ramps the motor up and down to prevent battery tripping. 
   // This is checked every 15 cycles, even when running at constant speed.
   if(motors[i].speedSteps >= 15){
   
      //determine current speed based on time periods that the motor PWM is high
      float curSpeed = motors[i].onCycleRatio / (1 + motors[i].onCycleRatio);
	  
      //account for the direction 
      if ((motors[i].flags & MOTOR_CDIR_FLAG))   // Current direction
        curSpeed *= -1;
      if ((motors[i].flags & MOTOR_INVERT_FLAG)) // Has the user flipped the directions?
        curSpeed *= -1;
        
      //determine desired speed and account for desired direction
      float desired = 1.0; 
      if (motors[i].desiredDirection)				// Current direction
        desired *= -1;
	  if ((motors[i].flags & MOTOR_INVERT_FLAG))	// Has the user flipped the directions ?
        desired *= -1;

      //increment speed toward desired
      float change = 0.1;
      if (curSpeed >= (.1 + desired)){
//         USBSerial.print(" Motor is ");            
//         USBSerial.print(i);
//         USBSerial.print(" onCycleRatio is ");            
//         USBSerial.print(motors[i].onCycleRatio);
//         USBSerial.print(" current speed is ");  
//         USBSerial.println(curSpeed);
        curSpeed -= change;
      }
      else if (curSpeed <= (desired - 0.1)){
        curSpeed += change;
      }
      else {
        curSpeed = desired;
      }
      
      //determine if the current direction flag needs to be switched
      if( (curSpeed < 0.1) && (curSpeed > -0.1) && (((motors[i].flags & MOTOR_CDIR_FLAG) > 0) != (motors[i].desiredDirection > 0))){
        motors[i].flags ^= MOTOR_CDIR_FLAG;
      } 
      
      //recalculate onCycleRatio
      if( abs(curSpeed) < 0.001 ) {
          motors[i].onCycleRatio = 0.0;
      }
      else if( abs(curSpeed) >= 1.0 ) {
        motors[i].onCycleRatio = motor_pwm_maxperiod;
      }
      else {
        motors[i].onCycleRatio = abs(curSpeed) / (1 - abs(curSpeed));
      }
	  // Reset the counter so we wait another 15 cycles
      motors[i].speedSteps = 0;
   }
   // Increment this variable until we get to 15, after which the ramping function above will run
   motors[i].speedSteps++;

  
      
    // if the motor is enabled
    if( (motors[i].flags & MOTOR_ENABLE_FLAG) && (motors[i].flags & MOTOR_UEN_FLAG)) {
		// if the motor has not started moving yet
       if( !(motors[i].flags & MOTOR_HIGH_FLAG) && (motors[i].speed > 0.0)) {
		 //pin register - sets the appropriate pin to drive the motor for each motor
         if( motors[i].flags & MOTOR_INVERT_FLAG ){
           MOTOR_DRV_PREG  |= (B00000001 << (i*3 + (!((motors[i].flags & MOTOR_CDIR_FLAG) >> 2))));
         } 
		 else {
           MOTOR_DRV_PREG  |= (B00000001 << (i*3 + (((motors[i].flags & MOTOR_CDIR_FLAG) >> 2)))); 
         }

		 //sets flag to high to indicate moving
		 motors[i].flags |= MOTOR_HIGH_FLAG; 
         
         motors[i].currentMoveCycles = 1;
         motors[i].pwmOnCycles = 1;
         
		 //Add to movement count. This keeps track of how many motors have started a move.
		 startedMoving++;
         
       }

	   // if the motor is currently moving
       else {     
		 // If the current move cycle count is greater than or equal to the distance of the move (in terms of time measured in cycles).
         // The distance is the speed % (as a 0.0-1.0 float) * 60 seconds * RPM at full power * gearing ratios
         if( motors[i].currentMoveCycles >= (motor_pwm_maxperiod * motors[i].speed)) {
		   // Add to the count of motors that have finished moving
		   doneMoving++;
           // Turn enable and high flags off
           motors[i].flags &= (~( MOTOR_ENABLE_FLAG | MOTOR_HIGH_FLAG ) );
           // disable output pin
		   MOTOR_DRV_PREG  &= ~(B00000011 << (i*3));
         } 

		 // The current move cycle count has not exceeded the distance (in terms of time measured in cycles)
		 else {  
		   // If there are more cycles required before PWM going low
           if (motors[i].pwmOnCycles < motors[i].onCycleRatio){             
			 //check if the invert dir flag is on
             if( motors[i].flags & MOTOR_INVERT_FLAG ){
			   
			   // Turn on input for appropriate motor and direction
               MOTOR_DRV_PREG  |= (B00000001 << (i*3 + (!((motors[i].flags & MOTOR_CDIR_FLAG) >> 2))));
			   // Turn off the other input for that motor
               MOTOR_DRV_PREG  &= ~(B00000001 << (i*3 + (((motors[i].flags & MOTOR_CDIR_FLAG) >> 2))));

             } else {
			   // Turn on input for appropriate motor and direction
               MOTOR_DRV_PREG  |= (B00000001 << (i*3 + (((motors[i].flags & MOTOR_CDIR_FLAG) >> 2))));
			   // Turn off the other input for that motor
               MOTOR_DRV_PREG  &= ~(B00000001 << (i*3 + (!((motors[i].flags & MOTOR_CDIR_FLAG) >> 2))));
             }
			 // Add to the count keeping track of how many cycles the PWM has been high
             motors[i].pwmOnCycles++;
           } 
           // We have had enough high PWM cycles and need a low cycle
           else {             
             // Turns off both motor driver inputs
             MOTOR_DRV_PREG  &= ~(B00000011 << (i*3));
             motors[i].pwmOnCycles = 0;
           }
		   // Add to the count of how many PWM cycles (high or low) have occurred for the current move
           motors[i].currentMoveCycles++;       
         }
       }
    }
	// If the motor is not enabled, make sure its inputs are off. Motors are disabled at the end of each move.
    else {
     MOTOR_DRV_PREG  &= ~(B00000011 << (i*3));
    }
  }
 
  
  // Check to see if the same number of motors have finished moving as had
  // started moving. If so, then we're done with this round of SMS movement.
  if( doneMoving == startedMoving ) {
    startedMoving = 0;
    doneMoving   = 0;
    motorStop(true);
  }
  
}

/** Motor Driving Interrupt Service Routine

 This routine is run via an interrupt on a mS-level basis (timer1, 4, or 2).
 
 We effectively create three independent channels of software PWM, giving us full
 control over the the pulse width and the duty cycle.
 
 This is a simple time-proportioning algorithm, with each channel sharing
 a starting sample edge.
 
 It controls the on/off state of three motors, based on the contents
 of the MotorState structure for each of the three motors.
 
 Each motor has three flags:
 
  - motor (programatically) enabled
  - motor (user) enabled
  - motor drive pin currently held high
 
 Motor PWM input pins are REQUIRED to be sequential on the SAME port register!
 
 @author 
 C. A. Church
 
 */

void motorRunISR() {
  
    // check status of each motor
	cycle_count++;
	
 for(byte i = 0; i < MOTOR_COUNT; i++) {
   

	   //function the ramps the motor up and down to prevent battery tripping
	   if(motors[i].speedSteps >= 15){
			
			  //determine current speed based on time periods that the motor PWM is high
			 float curSpeed = motors[i].onCycleRatio / (1 + motors[i].onCycleRatio);
				
			  //account for the direction 
			  if ((motors[i].flags & MOTOR_CDIR_FLAG))
				curSpeed *= -1;
			  if ((motors[i].flags & MOTOR_INVERT_FLAG))
				curSpeed *= -1;
				
			  //determine desired speed and account for desired direction
			  float desired = motors[i].speed; 
			  if (motors[i].desiredDirection)
				desired *= -1;
			  if ((motors[i].flags & MOTOR_INVERT_FLAG))
				desired *= -1;

			  //increment speed toward desired
			  float change = 0.1;
			  if (curSpeed >= (.1 + desired)){
				curSpeed -= change;
			  }
			  else if (curSpeed <= (desired - 0.1)){
				curSpeed += change;
			  }
			  else {
				curSpeed = desired;
			  }
			  
			  //determine if the current direction flag needs to be switched
			  
			  if( (curSpeed < 0.1) && (curSpeed > -0.1) && (((motors[i].flags & MOTOR_CDIR_FLAG) > 0) != (motors[i].desiredDirection > 0))){
				motors[i].flags ^= MOTOR_CDIR_FLAG;
			  } 
			  
			  //recalculate onCycleRatio
			  if( abs(curSpeed) < 0.001 ) {
				motors[i].onCycleRatio = 0.0;
			  }
			  else if( abs(curSpeed) >= 1.0 ) {
				motors[i].onCycleRatio = motor_pwm_maxperiod;
			  }
			  else {
				motors[i].onCycleRatio = abs(curSpeed) / (1 - abs(curSpeed));
			  }
			  motors[i].speedSteps=0;
	    }
		
		motors[i].speedSteps++;
	    
		// motor is enabled
	    if( motors[i].flags & MOTOR_ENABLE_FLAG && motors[i].flags & MOTOR_UEN_FLAG) {
		   
		 // motor is currently not moving
		 if( ! (motors[i].flags & MOTOR_HIGH_FLAG) && motors[i].speed > 0.0) {
			   
			   
		   bool goHigh = false;
		    // we are 1 off period for every X on periods	  
			if( motors[i].onCycleRatio >= 1.0 ) {
				
				goHigh = true;
			}
			else {
				motors[i].offError += motors[i].onCycleRatio;
			}

		   // we are off more often than on, and we've been off long enough
		   if( motors[i].offError >= 1.0 ) {
			   
			  goHigh = true;
			 motors[i].offError -= 1.0;
			}
		   
		   // going up, enable output pin
		   if( goHigh ) {
				   
				if( motors[i].flags & MOTOR_INVERT_FLAG ){
					MOTOR_DRV_PREG  |= (B00000001 << (i*3 + (!((motors[i].flags & MOTOR_CDIR_FLAG) >> 2))));            
				} 
				else { 
				MOTOR_DRV_PREG  |= (B00000001 << (i*3 + (((motors[i].flags & MOTOR_CDIR_FLAG) >> 2))));            
				}
			  motors[i].flags |= MOTOR_HIGH_FLAG;
			}

		 }
		 // motor is currently moving
		 else {
					
					
			 boolean goLow = false;
			 // we are on more often than off
			 if( motors[i].onCycleRatio > 0 ) {
				 
			   // have gone enough periods. 
			   if( motors[i].currentMoveCycles >= motors[i].onCycleRatio ) {
				  // but wait!  We've accumulated enough on error to wait one more cycle 
				 if( motors[i].onError >= 1.0 ) {
					 
				   motors[i].onError -= 1.0;
				   continue;
				 }
				 // not enough on error to wait one more cycle
				 else {
					 
				   goLow = true;
				 }
			   } // end if( ... currentMoveCycles
			 } // end if onCycleRatio
			 else {
			   goLow = true;
			 }

			 // going down, disable output pin
			 if( goLow ) {
			   
			   
				MOTOR_DRV_PREG  &= ~(B00000011 << (i*3));
				motors[i].flags &= ~(MOTOR_HIGH_FLAG);
				  // accumulate off-period error for one period
				motors[i].currentMoveCycles = 0;
				motors[i].onError += motors[i].onCycleRatio - (unsigned long) motors[i].onCycleRatio;
			 }
			 else {
			   motors[i].currentMoveCycles++;
			   if( motors[i].flags & MOTOR_INVERT_FLAG ){
				 MOTOR_DRV_PREG  |= (B00000001 << (i*3 + (!((motors[i].flags & MOTOR_CDIR_FLAG) >> 2))));
				 MOTOR_DRV_PREG  &= ~(B00000001 << (i*3 + (((motors[i].flags & MOTOR_CDIR_FLAG) >> 2))));
				 
			   } 
			   else { 
				 MOTOR_DRV_PREG  |= (B00000001 << (i*3 + (((motors[i].flags & MOTOR_CDIR_FLAG) >> 2))));
				 MOTOR_DRV_PREG  &= ~(B00000001 << (i*3 + (!((motors[i].flags & MOTOR_CDIR_FLAG) >> 2))));
			   }
			   
			 }
				 
		 } //end else (motor currently high)
		 
	    } // end if motor enabled
	    else {
		 MOTOR_DRV_PREG  &= ~(B00000011 << (i*3));
	    }
      
	} // end for...
 
}


/** Set Motor Enable Based on Lead-in
 
   Sets motor program-enable flag based on the lead-in amount for
   the motor, and the current shot count
   
   @author
   C. A. Church
   */
   
void motorCheckLead() {
  
     for( byte i = 0; i < MOTOR_COUNT; i++ ) {
//       USBSerial.print(" startShots is ");            
//       USBSerial.print(motors[i].startShots);
//       USBSerial.print(" .lead is ");  
//       USBSerial.print(motors[i].lead);
//       USBSerial.print(" .ramp_start is ");  
//       USBSerial.println(motors[i].ramp_start);
       
      if( motors[i].lead > 0 && ( camera_fired <= (motors[i].lead + motors[i].startShots) || camera_fired >= (camera_max_shots - (motors[i].lead + motors[i].startShots) ) ) )
        motors[i].flags &= ~(MOTOR_ENABLE_FLAG);
      else if( motors[i].lead > 0 && ( camera_fired > (motors[i].lead + motors[i].startShots) || camera_fired < (camera_max_shots - (motors[i].lead + motors[i].startShots)) ) )
        motors[i].flags |= MOTOR_ENABLE_FLAG;
        
    } 
}


/** Set Motor Speed Based on Ramp

  Controls linear ramping for each motor
  
  @author
  C. A. Church
  */
  
void motorCheckRamp() {
  
      for( byte i = 0; i < MOTOR_COUNT; i++ ) {
//        
//        USBSerial.print("camera shots = ");
//        USBSerial.print(camera_fired);
//        USBSerial.print(" flag is ");  
//        USBSerial.print(motors[i].flags,BIN); 
//        USBSerial.print("      "); 

        //check to see if motor is user enabled
        if(motors[i].flags & MOTOR_UEN_FLAG ) {
          
          if( motors[i].ramp_start > 0 || motors[i].ramp_end > 0 ) {
            
              // do not apply ramp if still in lead-in, or in lead-out
            if( motors[i].lead > 0 && ( camera_fired <= (motors[i].lead + motors[i].startShots) || camera_fired > (camera_max_shots - (motors[i].lead + motors[i].startShots)) ) )
              continue;
             
            unsigned int rampEnd = motors[i].lead  + motors[i].ramp_start;
            unsigned int rampEndF = motors[i].lead + motors[i].ramp_end;
            float diff; 
            
            
              // if we started during run time, set new ramp start point, in shots
            if( motors[i].startShots > 0 )
              rampEnd = rampEnd + motors[i].startShots;
			
			// force ramp down?
            if( motors[i].flags & MOTOR_RAMP_FLAG ) {
              diff = (float) (motors[i].ramp_end - (camera_fired - motors[i].forceRampStart)) / (float) motors[i].ramp_end;
			  
			  // disable motor (user wanted the motor off when ramp completed...)  
              if( diff <= 0.0 ) {
            
                  motors[i].flags &= ~(MOTOR_RAMP_FLAG | MOTOR_UEN_FLAG);
                  motors[i].inRamp = 0;
                  motorSpeed(i, motors[i].setSpeed);
                  continue;
              }
              // set motor currently ramping flag
              motors[i].inRamp = 1;
            }
            else if( camera_fired == rampEnd ) {
              diff = 1.0;
                // reset startshots, just in case they were set - since our lead-out/ramp-out is
                // doesn't care about that part.
              motors[i].startShots = 0;
                // disable motor currently ramping flag
              motors[i].inRamp = 0;
            }
            else if( camera_fired < rampEnd ) {
              
              diff = (float) (camera_fired - motors[i].startShots - motors[i].lead) / (float) motors[i].ramp_start;
                 // set motor currently ramping flag
              motors[i].inRamp = 1;
            }
            else if( camera_fired > (camera_max_shots - rampEndF) ) {
              diff = (float) (camera_max_shots - camera_fired - motors[i].lead)  / (float) motors[i].ramp_end;
                 // set motor currently ramping flag
              motors[i].inRamp = 1;
            }
            else {
              continue;
            }

            diff = motors[i].setSpeed * diff;
//            USBSerial.print(" startShots is ");            
//            USBSerial.print(motors[i].startShots);
//            USBSerial.print(" .lead is ");  
//            USBSerial.print(motors[i].lead);
//            USBSerial.print(" .ramp_start is ");  
//            USBSerial.print(motors[i].ramp_start);           
//            USBSerial.print(" the difference for motor speed is ");
//            USBSerial.println(diff);

            motorSpeed(i, diff, true);
          } // end if motors[i].ramp  
        } // end if motors[i].flags
      } // end for loop
}

/** Force Down Ramp of Motor

 Forces a motor to start a down ramp now.
 
 @param p_motor
 The motor to force to ramp down (0-2)
 
 @author
 C. A. Church
 */
 
void motorForceRamp(byte p_motor) {
  motors[p_motor].flags |= MOTOR_RAMP_FLAG;
  motors[p_motor].forceRampStart = camera_fired;
}