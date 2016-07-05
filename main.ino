/*
 Name:		main.ino
 Created:	6/29/2016 12:03:48 PM
 Author:	brandon
*/

// the setup function runs once when you press reset or power the board
#include <OneWire.h>
#include <SPI.h>
#include <PID_v1.h>




void setup(void) {
	Setpoint = 71;
	Serial.begin(9600);

}

void loop(void) {
	discoverOneWireDevices();
	delay(2000);
	getSerialTemp();
	delay(2000);
}



