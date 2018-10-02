/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.c

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

/* This section lists the other files that are included in this file.
 */

#include "hardware.h"
#include <plib.h>
#include "AD22100.h"

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */

/* AD22100 temperature
 * 
 */

/* ************************************************************************** */
/** Descriptive Data Item Name

  @Summary
    Brief one-line summary of the data item.
    
  @Description
    Full description, explaining the purpose and usage of data item.
    <p>
    Additional description in consecutive paragraphs separated by HTML 
    paragraph breaks, as necessary.
    <p>
    Type "JavaDoc" in the "How Do I?" IDE toolbar for more information on tags.
    
  @Remarks
    Any additional remarks
 */
float ad22100;

/* ************************************************************************** */
/* ************************************************************************** */
// Section: Local Functions                                                   */
/* ************************************************************************** */
/* ************************************************************************** */

/*  A brief description of a section can be given directly below the section
    banner.
 */

/* ************************************************************************** */



/* ************************************************************************** */
/* ************************************************************************** */
// Section: Interface Functions                                               */
/* ************************************************************************** */
/* ************************************************************************** */

/*  A brief description of a section can be given directly below the section
    banner.
 */

// *****************************************************************************

/** 
  @Function
    void init_ad22100(void); 

  @Summary
    Initialized the analog channel to read the AD22100 temperature IC

  @Description

  @Precondition
    None

  @Parameters
    None
 
  @Returns
    None
    
  @Remarks
    Describe any special behavior not described above.
    <p>
    Any additional remarks.

  @Example
    @code
        void init_ad22100(void); 
    
 */
void init_ad22100(void) 
{
    TRISGbits.TRISG6 = 1;           // Set pin as input
    ANSELGbits.ANSG6 = 0;           // Set pin as analog input
    TRISGbits.TRISG9 = 1;           // Set pin as input
    ANSELGbits.ANSG9 = 0;           // Set pin as analog input
    CloseADC10();
// configure to sample AN16 and ANA19
    SetChanADC10( ADC_CH0_NEG_SAMPLEA_NVREF | ADC_CH0_POS_SAMPLEA_AN16 |\
                  ADC_CH0_NEG_SAMPLEB_NVREF | ADC_CH0_POS_SAMPLEB_AN19); 
// configure ADC using parameter define above
    OpenADC10G( ADC_PARAM1, ADC_PARAM2, ADC_PARAM3, ADC_PARAM4, ADC_PARAM5 ); 
    EnableADC10();      // Enable the ADC
    while ( ! mAD1GetIntFlag() ) 
    {
        
    }
}

void read_ad22100(float *t1, float *t2) 
{
int offset;
int adc1, adc2;
float v1, v2, tempC;

    ConvertADC10();
    while ( ! mAD1GetIntFlag() ) { } // wait for the first conversion to complete so there will be vaild data in ADC result registers
    offset = 8 * ((~ReadActiveBufferADC10() & 0x01));  // determine which buffer is idle and create an offset
    adc1 = ReadADC10(offset);    
    adc2 = ReadADC10(offset + 1);    

    v1 = ((adc1*3.3)/ADCMAX);
    v2 = v1 - 1.4025;
    tempC = v2 * 44.444;
    *t1 = ((tempC * 9)/5) + 32;

    v1 = ((adc2*3.3)/ADCMAX);
    v2 = v1 - 1.4025;
    tempC = v2 * 44.444;
    *t2 = ((tempC * 9)/5) + 32;
}
/* *****************************************************************************
 End of File
 */
