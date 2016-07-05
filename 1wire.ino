OneWire ourBus(DS18S20_Pin);
OneWire ds(DS18S20_Pin); // on digital pin 10

float getTemp() {
	//returns the temperature from one DS18S20 in DEG Celsius

	byte data[12];
	byte addr[8];

	if (!ds.search(addr)) {
		//no more sensors on chain, reset search
		ds.reset_search();
		return -1000;
	}

	if (OneWire::crc8(addr, 7) != addr[7]) {
		Serial.println("CRC is not valid!");
		return -1000;
	}

	if (addr[0] != 0x10 && addr[0] != 0x28) {
		Serial.print("Device is not recognized");
		return -1000;
	}

	ds.reset();
	ds.select(addr);
	ds.write(0x44, 1); // start conversion, with parasite power on at the end

	delay(1000);

	byte present = ds.reset();
	ds.select(addr);
	ds.write(0xBE); // Read Scratchpad


	for (int i = 0; i < 9; i++) { // we need 9 bytes
		data[i] = ds.read();
	}

	ds.reset_search();

	byte MSB = data[1];
	byte LSB = data[0];

	float tempRead = ((MSB << 8) | LSB); //using two's compliment
	float TemperatureSum = tempRead / 16;

	return TemperatureSum;

}

/*-----( Declare User-written Functions )-----*/
void discoverOneWireDevices(void) {
	byte i;
	byte present = 0;
	byte data[12];
	byte addr[8];

	Serial.print("Looking for 1-Wire devices...\n\r");// "\n\r" is NewLine 
	while (ourBus.search(addr)) {
		Serial.print("\n\r\n\rFound \'1-Wire\' device with address:\n\r");
		for (i = 0; i < 8; i++) {
			Serial.print("0x");
			if (addr[i] < 16) {
				Serial.print('0');
			}
			Serial.print(addr[i], HEX);
			if (i < 7) {
				Serial.print(", ");
			}
		}
		if (OneWire::crc8(addr, 7) != addr[7]) {
			Serial.print("CRC is not valid!\n\r");
			return;
		}
	}
	Serial.println();
	Serial.print("Done\n");
	ourBus.reset_search();
	return;
}

void getSerialTemp(void) {
	analogReference(DEFAULT);
	analogReference(EXTERNAL);
	total = total - readings[index];
	float temperature = getTemp() * 9 / 5 + 32;
	readings[index] = temperature;
	total = total + readings[index];

	index = index + 1;

	// if we're at the end of the array...
	if (index >= numReadings)
		// ...wrap around to the beginning:
		index = 0;

	// calculate the average:
	Input = total / numReadings;
	// send it to the computer as ASCII digits

	myPID.Compute();


	//Serial.print(Input * 1);
	//Serial.print(" ");
	//Serial.print(Output);
	//Serial.print(" ");
	Serial.print(Setpoint);
	Serial.print(" ");
	//Serial.print(index);
	//Serial.print(" ");
	//Serial.print(numReadings);
	//Serial.print(" ");
	//Serial.print(total);
	//Serial.print(" ");
	//Serial.print(readings[index]);
	//Serial.print(" ");

	Serial.println(temperature);



	/************************************************
	* turn the output pin on/off based on pid output
	************************************************/
	if (millis() - windowStartTime>WindowSize)
	{ //time to shift the Relay Window
		windowStartTime += WindowSize;
	}
	if (Output < millis() - windowStartTime) digitalWrite(RelayPin, LOW);
	else digitalWrite(RelayPin, HIGH);

	if (index == 29)
	{

	}
}