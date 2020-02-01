/* ----------------------------------- File Inclusion ----------------------------------- */

#include <plib.h>

#include "hardware.h"
#include "TempADC.h"

/* -------------------------- Global Variables and Structures --------------------------- */

static int Temp1;
static int Temp2;

/* ---------------------------------- Public Functions ---------------------------------- */

/*
 *	Summary
 *		Initialize the temperature sensors, including the ADC peripheral.
 *	Parameters
 *		None.
 *	Returns
 *		None.
 */
void initializeTemperatureSensors(void) {
    TRISGbits.TRISG6 = 1;	// Set pin as input
    ANSELGbits.ANSG6 = 0;	// Set pin as analog input
    TRISGbits.TRISG9 = 1;	// Set pin as input
    ANSELGbits.ANSG9 = 0;	// Set pin as analog input
    CloseADC10();

    // Configure to sampel AN16 and AN19
    SetChanADC10(ADC_CH0_NEG_SAMPLEA_NVREF | ADC_CH0_POS_SAMPLEA_AN16 | ADC_CH0_NEG_SAMPLEB_NVREF | ADC_CH0_POS_SAMPLEB_AN19); 
    // Configure ADC using the above parameter definitions
    OpenADC10G(ADC_PARAM1, ADC_PARAM2, ADC_PARAM3, ADC_PARAM4, ADC_PARAM5); 
    EnableADC10();			// Enable the ADC
    while (!mAD1GetIntFlag()) {}
}

/*
 *	Summary
 *		Read both ADC converters for the two temperature sensors.
 *	Parameters
 *		t1[out]: int* that will be updated with the result of the ADC1.
 *		t2[out]: int* that will be updated with the result of ADC2.
 *	Returns
 *		None.
 */
void readTemperatures(int *t1, int *t2)  {
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
    v1	= ((adc1 * 3.3) / ADCMAX);	//Convert output bits from TMP36 to actual voltage on sensor
    tempC = (v1 - .5) * 100.0;		  //Remove the .5 volt offset of sensor, and then multiply by the 100 degrees C per volt
    *t1   = ((tempC * 9) / 5) + 32;	 //Convert from C to F

    /* -------------------- Convert the second ADC Channel ------------------- */
    v1	= ((adc2 * 3.3) / ADCMAX);	//Convert output bits from TMP36 to actual voltage on sensor
    tempC = (v1 - .5) * 100.0;		  //Remove the .5 volt offset of sensor, and then multiply by the 100 degrees C per volt
    *t2   = ((tempC * 9) / 5) + 32;	 //Convert from C to F

    Temp1 = *t1;
    Temp2 = *t2;
}

/*
 *	Summary
 *		Return a value of how much to scale the motor speed by.
 *	Parameters
 *		motor1_scale[out]: float* that will be updated with the scaling of motor 1.
 *		motor2_scale[out]: float* that will be updated with the scaling of motor 2.
 *	Returns
 *		None.
 */
void checkThresholds(float *motor1_scale, float *motor2_scale) {
    int t1 = 0;
    int t2 = 0;

    // Read current motor temperatures, set scale to decimal descale if out-of-range
    readTemperatures(&t1, &t2);	// Read current motor temperatures
    *motor1_scale = (t1 >= SLOW_TEMP) ? 1 - (t1 - SLOW_TEMP) / (STOP_TEMP - SLOW_TEMP) : 1.0;
    *motor2_scale = (t2 >= SLOW_TEMP) ? 1 - (t2 - SLOW_TEMP) / (STOP_TEMP - SLOW_TEMP) : 1.0;

    // If the motor is beyond stopping temperature, set scale to 0.0
    *motor1_scale = (*motor1_scale < 0.0) ? 0.0 : *motor1_scale;
    *motor2_scale = (*motor2_scale < 0.0) ? 0.0 : *motor2_scale;
}

/*
 *	Summary
 *		Return the last reading of ADC1.
 *	Parameters
 *		None.
 *	Returns
 *		int that is the latest value stored in Temp1.
 *	Notes
 *		Temp1 and Temp2 are updated by calls to readTemperatures();
 */
int getTemp1(void) {
    return Temp1; 
}

/*
 *	Summary
 *		Return the last reading of ADC2.
 *	Parameters
 *		None.
 *	Returns
 *		int that is the latest value stored in Temp2.
 *	Notes
 *		Temp1 and Temp2 are updated by calls to readTemperatures();
 */
int getTemp2(void) {
	return Temp2;
}

/* --------------------------------- Private Functions ---------------------------------- */

/* ----------------------------- Interrupt Service Routines ----------------------------- */

