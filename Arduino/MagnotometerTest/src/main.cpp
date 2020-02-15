#include <StepperMotor.h>
#include <Stepper.h>
#include <SparkFun_I2C_GPS_Arduino_Library.h>
#include <TinyGPS++.h>
#include <DHT.h>
#include <DHT_U.h>

#include "Mag3110.h"

// CAL magnetometer calibration mode
#define CAL A2
#define tp 9 // Timing test point

// Temperature and Humidity application
#define DHTTYPE DHT11
#define DHTPIN 8

static DHT dht(DHTPIN, DHTTYPE);
static StepperMotor stepper = StepperMotor(true, 2, 3, 4, 5);
static MAG3110 mag = MAG3110(&stepper); //Instantiate MAG3110
static I2CGPS gpsModule;				//Hook object to the library
static TinyGPSPlus gps;					//Declare gps object
static int TempPin = A1;				// Analog temperature sensor
static uint32_t run_time;				// Loop time keeper
static char txtStr[30];					// Global text output string
static double latN, lngW;				// GPS position
static char lngS[12], latS[12];
static int hr, mn, sc; // GPS time
static int mon, day, yr;

static void displayGPSInfo();

void setup()
{
	Serial.begin(9600);
	Serial.println("Begin");

	Wire.begin(); //setup I2C bus

	if (gpsModule.begin() == false) // Checks for successful initialization of GPS
	{
		Serial.println("No GPS attached");
	}
	else
	{
		Serial.println("GPS attached");
	}

	/*
	dht.begin(); // Start temperature and humidity sensing

	pinMode(CAL, INPUT);	// Used as a test signal - want to use it as an input for
	digitalWrite(CAL, LOW); // running the magnetometer calibration switch
	pinMode(tp, OUTPUT); // Timing test pin
	digitalWrite(tp, LOW);
	*/

	delay(1000); //Allow time for the sensors to initialize.

	mag.initialize(); //Initialize the MAG3110
}

void loop()
{
	int x, y, z;
	float temperature; // DHT11 temperature sensors degrees F
	float humidity;	// DHT11 humidity sensor %RH
	int32_t TempF;	 // TMP36 analog temperature sensor degrees F
	int16_t tF;		   // Computed temperature degrees F
	int16_t RH;		   // Output relative humidity
	int16_t TF;		   // Output temperature degrees F
	int32_t adc;	   // Analog input

	if ((millis() - run_time) < 1000 && gpsModule.available())
	{
		gps.encode(gpsModule.read()); // Feed the GPS parser read UART if character ready
	}

	//	digitalWrite(tp, HIGH); // Temporary digital output for timing loop processing time.

	while (!mag.isCalibrated()) //If we're not calibrated
	{
		if (!mag.isCalibrating()) //And we're not currently calibrating
		{
			Serial.println("Entering calibration mode");
			mag.enterCalMode(); //This sets the output data rate to the highest possible and puts the mag sensor in active mode
		}
		else
		{
			//Must call every loop while calibrating to collect calibration data
			//This will automatically exit calibration
			//You can terminate calibration early by calling mag.exitCalMode();
			mag.calibrate();
		}
	}

	mag.readMag(&x, &y, &z);

	//temperature = dht.readTemperature(true);
	//humidity = dht.readHumidity();
	//TF = (int16_t)temperature;
	//RH = (int16_t)humidity;

	//	adc = (int32_t)analogRead(TempPin); // Analog temperature sensor
	//	TempF = (((adc * 500) - 51250) / 569) + 32;

	//  sprintf(txtStr, "%3dF %3dF %3dF %2d%%", tF, (uint16_t)TempF, (uint16_t)temperature, (uint16_t)humidity);

	//  Send sensor data  out as UART text stream
	//	sprintf(txtStr, "$%d:%d:%d:%d:%d:", TF, RH, x, y, z);
	//	Serial.println(txtStr);
	if (gps.time.isUpdated()) //Check to see if new GPS info is available
	{
		displayGPSInfo(); // Read GPS
	}

	//	digitalWrite(tp, LOW);

	// magnotometer values
	Serial.print("Heading: ");
	Serial.println(mag.readHeading());

	Serial.println("--------");

	// update the runtime
	run_time = millis();

	delay(25);
}

//Display new GPS info
static void displayGPSInfo(void)
{
	if (gps.time.isValid())
	{
		mn = gps.date.month() + 1; // January = 1
		day = gps.date.day();
		yr = gps.date.year();

		hr = gps.time.hour() - 8;
		if (hr < 0)
		{
			hr += 24;
		}
		mn = gps.time.minute();
		sc = gps.time.second();
		sprintf(txtStr, "%2d:%2d:%2d ", hr, mn, sc);
	}

	if (!gps.location.isValid())
	{
		latN = 0.0;
		lngW = 0.0;
	}
	else
	{
		latN = gps.location.lat();
		lngW = gps.location.lng();
	}
	dtostrf(latN, 8, 6, latS); // Convert double to string
	sprintf(txtStr, "%sN", latS);
	dtostrf(lngW, 9, 6, lngS); // Convert double to string
	sprintf(txtStr, "%sW", lngS);
	sprintf(txtStr, "%d:%d:%d:%d:%d:%d:", hr, mn, sc, mon, day, yr);
	Serial.println(txtStr);
	sprintf(txtStr, "%sN:%sW\n", latS, lngS);
	Serial.println(txtStr);
}
