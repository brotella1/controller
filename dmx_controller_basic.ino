#include <Adafruit_LiquidCrystal.h>
#include <Wire.h>
#include <Encoder.h>
#include <DmxSimple.h>

//Global definitions------------------------------------------
#define redADC 0
#define greenADC 1
#define blueADC 2
#define xAxis 3
#define yAxis 4
#define DmxOut 3
#define MenuBtn 30
#define EnterBtn 31
#define EscBtn 32

Encoder knobLeft(2, 3);
Encoder knobRight(18, 19);
long newLeft, newRight;

typedef struct //max 64 channels @ 8 devices
{
  uint8_t channelNumbers = 0;
  uint8_t masterDimmer = 0;
  uint8_t redChannel = 0;
  uint8_t redValue = 0;
  uint8_t greenChannel = 0;
  uint8_t greenValue = 0;
  uint8_t blueChannel = 0;
  uint8_t blueValue = 0;
  uint8_t strobeChannel = 0;
  uint8_t strobeValue = 0;
} StaticLight;
StaticLight StaticLightDevice[8];

typedef struct //max 56 channels @ 4 devices
{
  uint8_t channelNumbers = 0;
  uint8_t chXRotation = 0;
  uint8_t chXRotationVal = 0;
  uint8_t chXFine = 0;
  uint8_t chXFineVal = 0;
  uint8_t chYRotation = 0;
  uint8_t chYRotationVal = 0;
  uint8_t chYFine = 0;
  uint8_t chYFineVal = 0;
  uint8_t chXYSpeed = 0;
  uint8_t chXYSpeedVal = 0;
  uint8_t outputDimmer = 0;
  uint8_t outputDimmerVal = 0;
  uint8_t strobe = 0;
  uint8_t strobeVal = 0;
  uint8_t focus = 0;
  uint8_t focusVal = 0;
  uint8_t outputColour = 0;
  uint8_t outputColurVal = 0;
  uint8_t outputGobo = 0;
  uint8_t outputGoboVal = 0;
  uint8_t goboSelfRotation = 0;
  uint8_t goboSelfRotationVal = 0;
  uint8_t fixtureReset = 0;
  uint8_t fixtureResetVal = 0;
} Yoke;
Yoke MovingYokeDevice[4];

typedef struct //max 56 channels @ 4 devices
{
  uint8_t channelNumbers = 0;
  uint8_t chXRotation = 0;
  uint8_t chXFine = 0;
  uint8_t chYRotation = 0;
  uint8_t chYFine = 0;
  uint8_t chXYSpeed = 0;
  uint8_t outputDimmer = 0;
  uint8_t strobe = 0;
  uint8_t focus = 0;
  uint8_t outputColour = 0;
  uint8_t outputGobo = 0;
  uint8_t goboSelfRotation = 0;
  uint8_t fixtureReset = 0;
} Wash;
Wash MovingWashDevice[4];


typedef struct //fixture buttons
{
  int fixture1 = 10;
  int fixture2 = 11;
  //  int fixture3 = 12;
  //  int fixture4 = 13;
  //  int fixture5 = 14;
  //  int fixture6 = 15;
  //  int fixture7;
  //  int fixture8;
  //  int fixture9;
  //  int fixture10;
  //  int fixture11;
  //  int fixture12;
  //  int fixture13;
  //  int fixture14;
  //  int fixture15;
  //  int fixture16;
} Fixture;

Fixture FixtureBTN;

uint8_t staticDeviceCount = 0;
uint8_t yokeDeviceCount = 0;
uint8_t washDeviceCount = 0;

Adafruit_LiquidCrystal lcd(25, 24, 23, 22, 21, 20);

void setup() {
  // put your setup code here, to run once:
  pinMode(FixtureBTN.fixture1, INPUT);
  pinMode(FixtureBTN.fixture2, INPUT);
  pinMode(MenuBtn, INPUT);
  DmxSimple.usePin(DmxOut);
  DmxSimple.maxChannel(25);
  lcd.begin(16,2);

}

long positionLeft  = -999;
long positionRight = -999;

void loop() {
  // put your main code here, to run repeatedly:
	lcd.clear();
	lcd.print("Home Screen");
  if(MenuBtn){
    EnterMenu();
    
  }
}

void EnterMenu(){

	while (true) {
		lcd.clear();
		lcd.print("Main Menu");
		newLeft = knobLeft.read();
		newRight = knobRight.read();
		if (newLeft != positionLeft) {
			switch (newLeft) {
			case '1':
			{
				if (EnterBtn) {
					knobLeft.write(0);
					newLeft = 0;
					DeviceSetup();
				}

			}

			break;

			case '2':
			{
				if (EnterBtn) {

				}
			}
			break;

			default:
				break;
			}
		}
		if (EscBtn)
			break;
	}

}

void DeviceSetup(){
	while (true) {
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.println("Device Setup");
		newLeft = knobLeft.read();
		switch (newLeft) {
		case '0':
		{
			lcd.setCursor(1, 0);
			lcd.print("Add Device");
			if (EnterBtn) {
				AddDevice();
			}
		}
		break;
		case '1':
		{
			lcd.clear();
			lcd.setCursor(1, 0);
			lcd.print("Edit A device");
			if (EnterBtn) {

			}
		}
		break;

		case '2':
		{
			lcd.print("Delete Device");
			if (EnterBtn) {

			}
		}
		default:
			break;
		}
		if (EscBtn)
			break;
	}

}

void AddDevice() {
	while (true) {
		lcd.clear();
		lcd.print("Device Type");
		newRight = knobRight.read();
		switch (newRight) {
		case '0':
		{
			lcd.print("Static Fixture");
			if (EnterBtn) {

				AddStaticFixture();

			}
		}
		break;
		
		case '1':
		{
			lcd.print("Moving Yoke");
			if (EnterBtn) {
				newRight = 0;
				knobRight.write(0);
			}

		}
		break;

		case '2':
		{
			lcd.print("Moving Colour Wash");
				if (EnterBtn) {
					newRight = 0;
					knobRight.write(0);
				}

		}
		break;

		default:
			break;
		}

		if (EscBtn)
			break;
	}

}

void AddStaticFixture() {
	uint8_t i = 0; //fixture address variable
	uint8_t ch = 0; //number of channels variable
	lcd.print("Device: ");
	while (true) {
		newLeft = knobLeft.read();
		newRight = knobRight.read();
		lcd.print(newRight);
		if (EnterBtn) {
			i = newRight;
			break;
		}
		switch (newLeft)
		{
		case '0':
		{
			while (true)
			{
				lcd.setCursor(1, 0);
				lcd.print("Number of channels: ");
				ch = newRight;
				if (newRight > 8) {
					newRight = 0;
				}
				lcd.print(ch);
			}

		}
		}
	}
	

}

