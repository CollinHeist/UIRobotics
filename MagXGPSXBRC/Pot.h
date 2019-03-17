#ifndef _POT_H
	#define _POT_H

	#include "hardware.h"

	/*
	#define ADC_PIN 2
					// Turn module on | output in integer | trigger mode auto | enable autosample 
	#define ADC_PARAM1  ADC_MODULE_ON | ADC_FORMAT_INTG   | ADC_CLK_AUTO      | ADC_AUTO_SAMPLING_ON

	// 				    ADC ref external   | disable offset test	| disable scan mode | perform 2 samples     | use dual buffers | use alternate mode
	#define ADC_PARAM2  ADC_VREF_AVDD_AVSS | ADC_OFFSET_CAL_DISABLE | ADC_SCAN_OFF      | ADC_SAMPLES_PER_INT_2 | ADC_ALT_BUF_ON   | ADC_ALT_INPUT_ON

	// 					  use ADC internal clock | set sample time
	#define ADC_PARAM3  ADC_CONV_CLK_INTERNAL_RC | ADC_SAMPLE_TIME_15

	// 					do not assign channels to scan
	#define ADC_PARAM5	SKIP_SCAN_ALL

	// 					set AN2 as an analog input
	#define ADC_PARAM4	ENABLE_AN2_ANA
	*/	
#endif

// Function Prototypes
void init_analog(void);
int readAnalog(void);