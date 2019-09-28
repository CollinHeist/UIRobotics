#include "ADC_TEMP.h"
#include "hardware.h"
#include <plib.h>

static int Temp1;
static int Temp2;

/* -------------------------------- init_temps -------------------------------
  @ Summary
	 Function that initializes the system to run the Analog to Digital Converter.
  @ Parameters
	 None
  @ Returns
	 None
  ---------------------------------------------------------------------------- */
void init_temperature(void) {
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

/* ----------------------------- read_temperature ----------------------------
  @ Summary
	 Function to read data from the Analog to Digital Converter.
  @ Parameters
	 @ param1 : Pointer to result of temperature on ADC1
	 @ param2 : Pointer to result of temperature on ADC2
  @ Returns
	 None, adjusts the values of t1 and t2
  ---------------------------------------------------------------------------- */
void read_temperature(int *t1, int *t2)  {
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
    
    Temp1 = *t1;
    Temp2 = *t2;
}

void read_temperature_store()  {
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
    Temp1   = ((tempC * 9) / 5) + 32;     //Convert from C to F

	/* -------------------- Convert the second ADC Channel ------------------- */
    v1    = ((adc2 * 3.3) / ADCMAX);    //Convert output bits from TMP36 to actual voltage on sensor
	tempC = (v1 - .5) * 100.0;          //Remove the .5 volt offset of sensor, and then multiply by the 100 degrees C per volt
    Temp2   = ((tempC * 9) / 5) + 32;     //Convert from C to F
}

// Function to return a value of how much to scale the motor speed by
void check_thresh(float *motor1_scale, float *motor2_scale) {
	int t1 = 0;
	int t2 = 0;

	// Read current motor temperatures, set scale to decimal descale if out-of-range
	read_temperature(&t1, &t2);	// Read current motor temperatures
	*motor1_scale = (t1 >= SLOW_TEMP) ? 1 - (t1 - SLOW_TEMP) / (STOP_TEMP - SLOW_TEMP) : 1.0;
	*motor2_scale = (t2 >= SLOW_TEMP) ? 1 - (t2 - SLOW_TEMP) / (STOP_TEMP - SLOW_TEMP) : 1.0;

	// If the motor is beyond stopping temperature, set scale to 0.0
	*motor1_scale = (*motor1_scale < 0.0) ? 0.0 : *motor1_scale;
	*motor2_scale = (*motor2_scale < 0.0) ? 0.0 : *motor2_scale;
}

int GetTemp1()
{
    return Temp1; 
}

int GetTemp2()
{
    return Temp2;
}