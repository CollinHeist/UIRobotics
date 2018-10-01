/* ************************************************************************** */
/** Descriptive File Name: swDelay.h - header file for software delay 
                           implemented by polling the core timer.

  @Summary
    Millisecond, microsecond, 10 nanosecond delays using software polling of 
    the core timer

  @Description
    Provides interface to a millisecond and microsecond delay based on the 
    speed of the core timer.
 
  @Precondition
    "config_bits* must be included in the project that establishes the
    the value for CORE_MS_TICK_RATE based on the core timer frequency.

 **************************************************************************** */

#ifndef _SW_DELAY_H    // Guard against multiple inclusion 
    #define _SW_DELAY_H

#endif

//  Millisecond software blocking delay function
void msDelay(unsigned int mS);

//  Microsecond software blocking delay function
void usDelay(unsigned int uS);

//  10 Nanosecond software blocking delay function
void nsDelay(unsigned int ns);

//  Millisecond software nonblocking delay function
int PeriodMs(unsigned int msec);
