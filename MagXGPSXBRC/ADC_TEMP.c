#include "ADC_TEMP.h"
#include "hardware.h"
#include <plib.h>

static int temp1;
static int temp2;



/* -------------------------------- init_temps -------------------------------
  @ Summary
	 Function that initializes the system to run the Analog to Digital Converter.
  @ Parameters
	 None
  @ Returns
	 None
  ---------------------------------------------------------------------------- */
void initTemperature(void) {
	TRISGbits.TRISG6 = 1;	// Set pin as input
	ANSELGbits.ANSG6 = 0;	// Set pin as analog input
	TRISGbits.TRISG9 = 1;	// Set pin as input
	ANSELGbits.ANSG9 = 0;	// Set pin as analog input
	CloseADC10();

	// Configure to sampel AN16 and AN19
	SetChanADC10(ADC_CH0_NEG_SAMPLEA_NVREF | ADC_CH0_POS_SAMPLEA_AN16 |\
				 ADC_CH0_NEG_SAMPLEB_NVREF | ADC_CH0_POS_SAMPLEB_AN19); 
	// Configure ADC using the above parameter definitions
	OpenADC10G(ADC_PARAM1, ADC_PARAM2, ADC_PARAM3, ADC_PARAM4, ADC_PARAM5); 
	EnableADC10();			// Enable the ADC
	while (!mAD1GetIntFlag()) {}
}

/* ----------------------------- readTemperature ----------------------------
  @ Summary
	 Function to read data from the Analog to Digital Converter.
  @ Parameters
	 @ param1 : Pointer to result of temperature on ADC1
	 @ param2 : Pointer to result of temperature on ADC2
  @ Returns
	 None, adjusts the values of t1 and t2
  ---------------------------------------------------------------------------- */
void readTemperature(int *t1, int *t2)  {
	int offset;
	int adc1, adc2;
	float v1, v2, tempC;

	ConvertADC10();
	// Wait for the first conversion to complete so there will be vaild data in ADC result registers
	while (!mAD1GetIntFlag()) {} 
	offset = 8 * ((~ReadActiveBufferADC10() & 0x01));  // Determine which buffer is idle and create an offset
	adc1 = ReadADC10(offset);
	adc2 = ReadADC10(offset + 1);

	/* -------------------- Convert the first ADC Channel -------------------- */
	v1    = ((adc1 * 3.3) / ADCMAX);    //Convert output bits from TMP36 to actual voltage on sensor
	tempC = (v1 - .5) * 100.0;          //Remove the .5 volt offset of sensor, and then multiply by the 100 degrees C per volt
    *t1   = ((tempC * 9) / 5) + 32;     //Convert from C to F

	/* -------------------- Convert the second ADC Channel ------------------- */
    v1    = ((adc2 * 3.3) / ADCMAX);    //Convert output bits from TMP36 to actual voltage on sensor
	tempC = (v1 - .5) * 100.0;          //Remove the .5 volt offset of sensor, and then multiply by the 100 degrees C per volt
    *t2   = ((tempC * 9) / 5) + 32;     //Convert from C to F
    
    temp1 = *t1;
    temp2 = *t2;
}


/* -------------------------------- readTemperatureStore -------------------------------
  @ Summary
	 readTemperatureStore's function is to store temperature values of the motor in readable terms for an American (aka Fahrenheit)
  @ Parameters
	 None
  @ Returns
	 None
  ---------------------------------------------------------------------------- */
void readTemperatureStore()  {
	int offset;
	int adc1, adc2;
	float v1, v2, tempC;

	ConvertADC10();
	// Wait for the first conversion to complete so there will be vaild data in ADC result registers
	while (!mAD1GetIntFlag()) {} 
	offset = 8 * ((~ReadActiveBufferADC10() & 0x01));  // Determine which buffer is idle and create an offset
	adc1 = ReadADC10(offset);
	adc2 = ReadADC10(offset + 1);

	/* -------------------- Convert the first ADC Channel -------------------- */
	v1    = ((adc1 * 3.3) / ADCMAX);    //Convert output bits from TMP36 to actual voltage on sensor
	tempC = (v1 - .5) * 100.0;          //Remove the .5 volt offset of sensor, and then multiply by the 100 degrees C per volt
    temp1   = ((tempC * 9) / 5) + 32;     //Convert from C to F

	/* -------------------- Convert the second ADC Channel ------------------- */
    v1    = ((adc2 * 3.3) / ADCMAX);    //Convert output bits from TMP36 to actual voltage on sensor
	tempC = (v1 - .5) * 100.0;          //Remove the .5 volt offset of sensor, and then multiply by the 100 degrees C per volt
    temp2   = ((tempC * 9) / 5) + 32;     //Convert from C to F
}

// Function to return a value of how much to scale the motor speed by
/* -------------------------------- checkThresh -------------------------------
  @ Summary
	 checkThresh's function is to act as a watchdog when checking temperature values of the motor,
		if the motor is too hot, the motor will be scaled back to prevent increasing temperatures,
		and to prevent the motor from spinning in a counter direction to its previous.
		If the motor is within a healthy range, the motor will be allowed to continue running.
  @ Parameters
	motor1scale and motor2scale indicates the motors rotational speed
  @ Returns
	 None
  ---------------------------------------------------------------------------- */
void checkThresh(float *motor1Scale, float *motor2Scale) {
	int t1 = 0;
	int t2 = 0;

	// Read current motor temperatures, set scale to decimal descale if out-of-range
	readTemperature(&t1, &t2);	// Read current motor temperatures
	*motor1Scale = (t1 >= SLOW_TEMP) ? 1 - (t1 - SLOW_TEMP) / (STOP_TEMP - SLOW_TEMP) : 1.0;
	*motor2Scale = (t2 >= SLOW_TEMP) ? 1 - (t2 - SLOW_TEMP) / (STOP_TEMP - SLOW_TEMP) : 1.0;

	// If the motor is beyond stopping temperature, set scale to 0.0
	*motor1Scale = (*motor1Scale < 0.0) ? 0.0 : *motor1Scale;
	*motor2Scale = (*motor2Scale < 0.0) ? 0.0 : *motor2Scale;
}

int getTemp1()
{
    return temp1; 
}

int getTemp2()
{
    return temp2;
}