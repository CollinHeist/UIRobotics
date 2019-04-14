/* ************************************************************************** */
/** Descriptive File Name - BASys MX3 Analog input

  @Author
 Richard Wall

  @File Name
    Pot.h

  @Summary
    Analog input

  @Description
    Reads the BASys Analog input - AN2
 */
/* ************************************************************************** */

#ifndef _POT_H    /* Guard against multiple inclusion */
#define _POT_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */
#include "hardware.h"

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Constants                                                         */
/* ************************************************************************** */
/* ************************************************************************** */
/*
#define ADC_PIN 2
    // define setup parameters for OpenADC10
                // Turn module on | output in integer | trigger mode auto | enable autosample 
#define ADC_PARAM1  ADC_MODULE_ON | ADC_FORMAT_INTG | ADC_CLK_AUTO | ADC_AUTO_SAMPLING_ON

// define setup parameters for OpenADC10
                // ADC ref external    | disable offset test    | disable scan mode | perform 2 samples | use dual buffers | use alternate mode
#define ADC_PARAM2  ADC_VREF_AVDD_AVSS | ADC_OFFSET_CAL_DISABLE | ADC_SCAN_OFF | ADC_SAMPLES_PER_INT_2 | ADC_ALT_BUF_ON | ADC_ALT_INPUT_ON

// define setup parameters for OpenADC10
                   // use ADC internal clock | set sample time
#define ADC_PARAM3  ADC_CONV_CLK_INTERNAL_RC | ADC_SAMPLE_TIME_15

// define setup parameters for OpenADC10
      // do not assign channels to scan
#define ADC_PARAM5    SKIP_SCAN_ALL

// define setup parameters for OpenADC10
     // set AN2 as an analog input
#define ADC_PARAM4    ENABLE_AN2_ANA
*/

// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
// *****************************************************************************
// Section: Interface Functions
// *****************************************************************************
// *****************************************************************************

    void init_analog(void);
    int readAnalog(void);
    
    
#endif /* _POT_H */

/* *****************************************************************************
 End of File
 */
