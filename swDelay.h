/* ************************************************************************** */
/** Descriptive File Name

  @ Author
    Richard Wall
 
  @ Date
    May 28, 2016

  @Company
    Digilent

  @File Name
        msDelay.h

  @Summary
        Millisecond delay using software polling of the core timer

  @Description
        Provides interface to the millisecond delay based on the speed of the 
        core timer.
 
   @Precondition
        "config_bits* must be included in the project
 
 */
/* ************************************************************************** */

#ifndef _SW_DELAY_H    /* Guard against multiple inclusion */
#define _SW_DELAY_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

    // *****************************************************************************
    // *****************************************************************************
    // Section: Interface Functions
    // *****************************************************************************
    // *****************************************************************************

    /*  Millisecond software blocking delay function
     */

void DelayMs(unsigned int mS);
void DelayUs(unsigned int us);
void DelayNs(unsigned int ns);
int PeriodMs(unsigned int msec);

#endif /* _SW_DELAY_H */

/* *****************************************************************************
  End of swDelay.h
 */
