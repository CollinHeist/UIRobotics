/* ************************************************************************** */
/** Descriptive File Name - BASys MX3 Analog Input

  @Author
 Richard Wall

  @File Name
    Pot.c

  @Summary
    Analog input

  @Description
    Reads the BASys Analog input - AN2
 */
/* ************************************************************************** */

#include "hardware.h"
#include <plib.h>
#include "Pot.h"

/* ************************************************************************** */
void init_analog(void)
{


}

/* ************************************************************************** */
int readAnalog(void)
{
unsigned int an2;    // conversion result as read from result buffer
unsigned int an4;    // conversion result as read from result buffer
unsigned int offset;    // buffer offset to point to the base of the idle buffer    

    ConvertADC10();
    while ( ! mAD1GetIntFlag() ) { } // wait for the first conversion to complete so there will be vaild data in ADC result registers
    offset = 8 * ((~ReadActiveBufferADC10() & 0x01));  // determine which buffer is idle and create an offset
    an2 = ReadADC10(offset);      // read the result of channel 2 conversion from the idle buffer
    an4 = ReadADC10(offset + 1);  // read the result of channel 4 conversion from the idle buffer
    return an2;
}