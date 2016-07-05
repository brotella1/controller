double Setpoint;
double Input, Output;

//Specify the links and initial tuning parameters

float Kp = 200;
float Ki = 50;
float Kd = 0;

PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

int WindowSize = 2000;
unsigned long windowStartTime;

const int numReadings = 30;

double readings[numReadings]; // the readings from the analog input
int index = 0; // the index of the current reading
double total = 0; // the running total
double input = 0; // the average


void pidConfig(void)
{
	pinMode(6, OUTPUT);
	windowStartTime = millis();
	//tell the PID to range between 0 and the full window size
	myPID.SetOutputLimits(0, WindowSize);

	//turn the PID on
	myPID.SetMode(AUTOMATIC);

	for (int thisReading = 0; thisReading < numReadings; thisReading++)
		readings[thisReading] = 0;

	analogReference(EXTERNAL);
}


