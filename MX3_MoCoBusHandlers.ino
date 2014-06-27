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
MoCoBus handlers and helper functions
========================================

*/

/** Handler function for normal incoming MoCoBus commands

@author
Michael Ploof

*/

void commandHandler(byte packet_code, byte* comData) {
	/*
	// Print info about the command to the USB serial terminal
	USBSerial.println("");
	USBSerial.println("Something happened!");
	USBSerial.print("Packet code: ");
	USBSerial.println(packet_code);
	USBSerial.print("comData (HEX): ");
	for (byte i = 0; i < 10; i++) {
		USBSerial.print(comData[i], HEX);
		USBSerial.print(", ");
	}
	USBSerial.println("");
	USBSerial.print("comData (BIN): ");
	for (byte i = 0; i < 10; i++) {
		USBSerial.print(comData[i], BIN);
		USBSerial.print(", ");
	}
	USBSerial.println("");
	USBSerial.println("");


	// Based upon the packet code, determine which sub-handler to call
	switch (packet_code)
	{
	case 2:
		USBSerial.println("Entering control handler");
		controlHandler(comData);
		break;
	case 3:
		USBSerial.println("Entering data handler");
		dataSetHandler(comData);
		break;
	default:
		break;
	}
	*/
}

/** Handler function for incoming broadcast MoCoBus commands

@author
Michael Ploof

*/

void bCastHandler(byte packet_code, byte* comData) {
	/*
	USBSerial.println("Something happened AND it's getting broadcast to everyone!!!");
	USBSerial.print("Command: ");
	USBSerial.println(packet_code);
	USBSerial.print("comData (HEX): ");
	for (byte i = 0; i < 10; i++) {
		USBSerial.print(comData[i], HEX);
		USBSerial.print(", ");
	}
	USBSerial.println("");
	USBSerial.print("comData (BIN): ");
	for (byte i = 0; i < 10; i++) {
		USBSerial.print(comData[i], BIN);
		USBSerial.print(", ");
	}
	USBSerial.println("");
	USBSerial.println("");

	*/
}


/** Handler function for control commands (those which cause physical action)

@author
Michael Ploof

*/

void controlHandler(byte* comData) {
	/*
	// Did the command execution fail in some way? Change later if yes.
	boolean fail = false;

	// Pull the command byte from the buffer
	byte command = comData[0];

	//Output for debugging
	USBSerial.print("The control handler command is: ");
	USBSerial.println(command);
	

	// Execute requested command (or throw and error for non-specified commands)
	switch (command)
	{

		// NOOP
		case 0:

			//Output for debugging
			USBSerial.println("NOOP command");			

			break;

		// Stop all motors
		case 1: {

			//Output for debugging
			USBSerial.println("Stopping all motors");

			// Force all motors to stop? Only use during continuous and user moves, not SMS
			boolean force_all = comData[1];
			motorStop(force_all);

			break;
		}
	
		// Stop one motor 
		case 2: {

			// Which motor?
			boolean motor = comData[1];
			motorStopThis(motor);

			//Output for debugging
			USBSerial.print("Stopping motor ");
			USBSerial.println(motor);

			break;
		}
	
		// Start all motors
		case 3: {
			// Check to see if motor is running in continuous or SMS mode
			boolean mode = comData[1];
			motorRun(mode);

			//Output for debugging
			USBSerial.println("Starting all motors");
			
			break;
		}

		// Start one motor
		case 4: {
			// Check to see if command is for continuous or SMS mode
			boolean mode = comData[1];
			byte motor = comData[2];
			motorRun(mode, motor);

			//Output for debugging
			USBSerial.print("Starting motor ");
			USBSerial.print(motor);
			if (mode) {
				USBSerial.println(" in SMS mode.");
			}
			else if (!mode) {
				USBSerial.println(" in continuous mode.");
			}
			else {
				USBSerial.println("...just kidding, there was an error and the motor isn't starting.");
			}
	
			break;
		}

		// Force down-ramp of motor
		case 5: {
			// Which motor?
			byte motor = comData[1];
			
			//Output for debugging
			USBSerial.print("Forcing motor ");
			USBSerial.print(motor);
			USBSerial.println(" to ramp down.");
					
			motorForceRamp(motor);

			break;
		}

		// Error, invalid command
		default: {
			fail = true;
			break;
		}
	}

	// Report back to the master device whether the command execution was successful
	if (fail) {

		//Output for debugging
		USBSerial.println("There was an error in the command execution");
		
		Node.response(false);
	}
	else {

		//Output for debugging
		USBSerial.println("Command execution successful!");

		Node.response(true);
	}
		
		*/
}


/** Handler function for data commands (those which set and get motor and camera parameters)

@author
Michael Ploof

*/

void dataSetHandler(byte* comData) {
	/*
	// Did the command execution fail in some way? Change later if yes.
	boolean fail = false;

	// Is the command a data request? Set to true later if 
	// needed to prevent duplicate responses to master device
	boolean request = false;


	// Pull command and sub-command from buffer
	byte command = comData[0];
	byte sub_command = comData[1];

	// Is the command to set camera data?
	if (command == 1) {

		// Location of future camera commands.

	}

	// Is the command to set motor data?
	else if (command == 2) {
		
		switch (sub_command)
		{
			
			// Set speed as percent for one motor 
			case 1: {

				// Which motor?
				byte motor = comData[2];

				// How fast as 0.0-1.0 percent? Need to create temporary array
				// and pointer for Node.ntof() helper function to operate upon
				byte speed_bytes[4];
				for (byte i = 0; i < sizeof(speed_bytes); i++) {
					speed_bytes[i] = comData[3 + i];
				}
				byte* speed_pointer = speed_bytes;
				float set_speed = Node.ntof(speed_pointer);

				// Report float value over USB terminal to see if this actually worked
				USBSerial.print("Result of ntof() function: ");
				USBSerial.println(set_speed);

				// Is the speed change part of a ramp cycle?
				byte ramp = comData[7];

				motorSpeed(motor, set_speed, ramp);

				break;
			}

			// Set speed as output ratio for one motor
			case 2: {

				// I don't really get what this function is supposed to be doing,
				// so I'm going to come back to it later
				// motorSpeedCalc(motor, ratio, ramp);

				USBSerial.println("Haven't programmed this one yet, come back later.");

				break;
			}

			// Set direction of one motor
			case 3: {

				byte motor = comData[2];
				boolean dir = comData[3];
				motorDir(motor, dir);

				//Output for debugging
				USBSerial.print("Setting speed of motor ");
				USBSerial.println(motor);

				break;
			}

			// Flip all motor directions
			case 4: {

				motorDirFlip();

				//Output for debugging
				USBSerial.println("Flipping direction of all motors");

				break;
			}

			// Flip one motor direction
			case 5: {

				byte motor = comData[2];
				motorDirFlip(motor);

				//Output for debugging
				USBSerial.print("Flipping direction of motor ");
				USBSerial.println(motor);

				break;
			}

			// Set motor enable based on lead-in
			case 6: {

				motorCheckLead();

				//Output for debugging
				USBSerial.println("Setting motor enable based on lead-in");
				
				break;
			}

			// Set motor speed based on ramp
			case 7: {

				motorCheckRamp();

				//Output for debugging
				USBSerial.println("Setting motor speed based on ramp");

				break;
			}

			// Error, invalid command
			default: {
				USBSerial.println("Danger, Will Robinson, danger! That motor data-setting command is not defined!");
				fail = true;
				break;
			}
		}
	}

	// Is the command to get motor data?
	else if (command == 3) {

		// Set request flag so response to master device isn't duplicated later
		request = true;

		switch (sub_command)
		{

			// Get speed percent for one motor
			case 1: {

				// Which motor?
				byte motor = comData[2];

				// Get the speed
				float current_speed = motorSpeed(motor);

				// Report to USB terminal for debugging
				USBSerial.print("Speed for motor ");
				USBSerial.print(motor);
				USBSerial.print(" is currently: ");
				USBSerial.println(current_speed);

				// Report success and current speed to master device
				Node.response(true, current_speed);

				break;
			}

			// Get speed as output ratio for one motor
			case 2: {	
				
				// Whitch motor?
				byte motor = comData[2];
				float speed_ratio = motorSpeedCalc(motor);

				// Report to USB terminal for debugging
				USBSerial.print("Speed ratio for motor ");
				USBSerial.print(motor);
				USBSerial.print(" is currently: ");
				USBSerial.print(speed_ratio);

				// Report sucess and output ratio to master device
				Node.response(true, speed_ratio);

				break;
			}

			// Get maximum possible speed as output ratio for one motor
			case 3: {

				// Whitch motor?
				byte motor = comData[2];
				float max_speed_ratio = motorMaxSpeed(motor);

				// Report to USB terminal for debugging
				USBSerial.print("Maximum speed ratio for motor ");
				USBSerial.print(motor);
				USBSerial.print(" is currently: ");
				USBSerial.print(max_speed_ratio);

				// Report sucess and output ratio to master device
				Node.response(true, max_speed_ratio);
				
				break;
			}

			// Sub-command is undefined
			default: {

				// Report error to USB terminal for debugging
				USBSerial.println("Undefined motor data request command!");

				// Report an error to the master device
				Node.response(false);

				 break;
			}
		}
	}

	// Command is undefined
	else {
		USBSerial.println("That hardware select command is not defined!");
		fail = true;
	}

	// Report back to the master device whether the command execution was successful,
	// but only if the command wasn't a request (in which case, the response has already been sent)
	if (fail && request == false) {

		//Output for debugging
		USBSerial.println("There was an error in the data-command execution");

		Node.response(false);
	}
	else if (!fail && request == false) {

		//Output for debugging
		USBSerial.println("Data-command execution successful!");

		Node.response(true);
	}
	*/
}


/** Work-around function takes the place of apparently 
	non-functional Arduino Serial.flush() function

@author
Michael Ploof

*/

void flushSerial() {
	while (Serial.available()) {
		Serial.read();
	}
}

