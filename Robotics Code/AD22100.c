#include "AD22100.h"

/* ------------------------------- init_ad22100 ------------------------------
  @ Summary
	 Function that initializes the system to run the Analog to Digital Converter.
  @ Parameters
	 None
  @ Returns
	 None
  ---------------------------------------------------------------------------- */
void init_ad22100(void) {
	TRISGbits.TRISG6 = 1;	// Set pin as input
	ANSELGbits.ANSG6 = 0;	// Set pin as analog input
	TRISGbits.TRISG9 = 1;	// Set pin as input
	ANSELGbits.ANSG9 = 0;	// Set pin as analog input
	CloseADC10();

	// Configure to sampel AN16 and AN19
	SetChanADC10( ADC_CH0_NEG_SAMPLEA_NVREF | ADC_CH0_POS_SAMPLEA_AN16 |\
				  ADC_CH0_NEG_SAMPLEB_NVREF | ADC_CH0_POS_SAMPLEB_AN19); 
	// Configure ADC using the above parameter definitions
	OpenADC10G( ADC_PARAM1, ADC_PARAM2, ADC_PARAM3, ADC_PARAM4, ADC_PARAM5 ); 
	EnableADC10();			// Enable the ADC
	while (!mAD1GetIntFlag()) {}
}

/* ------------------------------- read_ad22100 ------------------------------
  @ Summary
	 Function to read data from the Analog to Digital Converter.
  @ Parameters
	 @ param1 : A pointer to a float variable to contain data from the first ADC result register
	 @ param2 : A pointer to a float variable to contain data from the second ADC result register
  @ Returns
	 None
  ---------------------------------------------------------------------------- */
void read_ad22100(float *t1, float *t2)  {
	int offset;
	int adc1, adc2;
	float v1, v2, tempC;

	ConvertADC10();
	// Wait for the first conversion to complete so there will be vaild data in ADC result registers
	while (!mAD1GetIntFlag()) { } 
	offset = 8 * ((~ReadActiveBufferADC10() & 0x01));  // Determine which buffer is idle and create an offset
	adc1 = ReadADC10(offset);
	adc2 = ReadADC10(offset + 1);

	/* -------------------- Convert the first ADC Channel -------------------- */
	v1    = ((adc1 * 3.3) / ADCMAX);
	v2    = v1 - 1.4025;
	tempC = v2 * 44.444;
	*t1   = ((tempC * 9) / 5) + 32;

	/* -------------------- Convert the second ADC Channel ------------------- */
	v1    = ((adc2 * 3.3) / ADCMAX);
	v2    = v1 - 1.4025;
	tempC = v2 * 44.444;
	*t2   = ((tempC * 9) / 5) + 32;
}