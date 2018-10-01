/* ************************************************************************** */
/** Descriptive File Name

  @ Author
    Richard Wall
 
  @ Date
    May 28, 2016
  @Revised
	October 24, 2016 - revised DelayMs

  @Company
    Digilent       

   @File Name
    swDelay.c

  @Summary
    Millisecond blocking time delay

   @Precondition
        "config_bits* must be included in the project

   @Description
    Delays the number of milliseconds specified the integer value passed when 
    called.  This is a preemptive blocking function. Interrupts may affect the 
    the length of the total delay if the execution time of the interrupt 
    extends the core timer beyond time recorded as the end of the delay period.
 */
/* ************************************************************************** */

#include "hardware.h"
#include "swDelay.h"
#include <plib.h>

/*
 * 
 */
/*msDelay   Function Description *********************************************
SYNTAX:         void DelayMs(unsigned int mS);
DESCRIPTION:    This is a millisecond delay function uses the core time
                to set the base millisecond delay period. Delay periods of
                zero are permitted. 
KEYWORDS:       delay, ms, milliseconds, software delay, core timer
PARAMETER1:     mS - the number of milliseconds to delay
RETURN VALUE:   None:
END DESCRIPTION ************************************************************/
void DelayMs(unsigned int mS)
{
unsigned int tWait, tStart;
    tStart = ReadCoreTimer();
    tWait = (CORE_MS_TICK_RATE * mS);
    while((ReadCoreTimer()- tStart) < tWait); /* wait for the time to pass */
} /* End of DelayMs */

/* DelayUs Function Description ***********************************************
SYNTAX:         void DelayUs(unsigned int us);
PARAMETER1:     us - the number of microseconds to delay
KEYWORDS:       delay, us, microseconds, software delay, core timer
DESCRIPTION:    This is a microsecond delay function that will repeat a
                specified number of times.  It also has nodebug active to
                increase accuracy of measurement.
RETURN VALUE:   None
Notes:          None
END DESCRIPTION ************************************************************/
void DelayUs(unsigned int us)
{
unsigned int tWait, tStart;

    tStart = ReadCoreTimer();
    tWait = (CORE_MS_TICK_RATE * us / 1000);
    while((ReadCoreTimer()- tStart) < tWait); /* wait for the time to pass */
}/* End of DelayUs */
/* DelayUs Function Description ***********************************************
SYNTAX:         void DelayNs(unsigned int ns);
PARAMETER1:     ns - the number of 10's of microseconds to delay
KEYWORDS:       delay, ns, microseconds, software delay, core timer
DESCRIPTION:    This is a microsecond delay function that will repeat a
                specified number of times.  
RETURN VALUE:   None
Notes:          None
END DESCRIPTION ************************************************************/
void DelayNs(unsigned int ns)
{
unsigned int tWait, tStart;

    tStart = ReadCoreTimer();
    tWait = (CORE_MS_TICK_RATE * ns / 10000);
    while((ReadCoreTimer()- tStart) < tWait); /* wait for the time to pass */
}/* End of DelayUs */
/* PeriodMs Function Description *********************************************
 * SYNTAX:          int PeriodMs(unsigned int msec);
 * KEYWORDS:        delay, ms, milliseconds, software delay, core timer,
 *                  non-blocking
 * DESCRIPTION:     This is a non-blocking millisecond delay function that
 *                  can be use for periodic activation of code.
 * PARAMETER1:      int msec - the number of milliseconds to delay
 * RETURN VALUE:    integer - delay in progress TURE / FALSE
 * Notes:           Calling this function with a zero time parameter can be
 *                  used to see if this function is currently in use.  A TRUE
 *                  is returned if a period timing is in progress.
 * END DESCRIPTION ************************************************************/
int PeriodMs(unsigned int msec)
{
static int delay_in_progross_flag = FALSE;
static unsigned int tWait, tStart;

    if(delay_in_progross_flag == 0)             /* New Period */
    {
	tStart = ReadCoreTimer();               /* Capture starting time */
        tWait = (CORE_MS_TICK_RATE * msec);     /* Compute delay period */
        delay_in_progross_flag = TRUE;          /* Set operation flag */
    }
    if((ReadCoreTimer()-tStart) >= tWait) /* Check if period is over */
        delay_in_progross_flag = FALSE;
    return delay_in_progross_flag;
}/* End of PeriodMs */

/* *****************************************************************************
  End of swDelay.c 
 */
