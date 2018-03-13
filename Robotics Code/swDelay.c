/** swDelay.c - software delay implemented by polling the core timer.
  @ Summary
     Blocking and non-blocking time delay
  @ Precondition
     "config_bits* must be included in the project that establishes the
     the value for CORE_MS_TICK_RATE based on the core timer frequency.
  @ Description
     Delays the number of milliseconds, microseconds, or picoseconds specified 
     the integer value passed when called.  This is a preemptive blocking 
     function. Interrupts may affect the the length of the total delay if the 
     execution time of the interrupt extends the core timer beyond time recorded
     as the end of the delay period.
     
     The PeriodMs function is a non-blocking function that must be called by 
     using function to determine ie the delay period has expired.
 ---------------------------------------------------------------------------- */
// System included files
#include "hardware.h"
#include <plib.h>

// Platform common included files
#include "swDelay.h"
/* ------------------------------ msDelay -----------------------------------
 @ Syntax
    void msDelay(unsigned int mS);
 @ Description
    This is a millisecond delay function uses the core time to set the base
    millisecond delay period. Delay periods of zero are permitted. 
 @ Parameters
    @ param1: mS - the number of milliseconds to delay
 @ Return Value
    None
 @ Notes
    The time delay accuracy will be adversely affected by the time to execute 
    interrupt service routines in preemptive systems.
 ---------------------------------------------------------------------------- */
void msDelay(unsigned int mS) {
unsigned int tWait, tStart;
    tStart = ReadCoreTimer();
    tWait = (CORE_MS_TICK_RATE * mS);
    while ((ReadCoreTimer()- tStart) < tWait); // wait for the time to pass
} /* End of DelayMs */

/* ------------------------------ usDelay -----------------------------------
 @ Syntax
    void usDelay(unsigned int us);
 @ Parameters
    @ param1: us - the number of microseconds to delay
 @ Descriptions
   This is a microsecond delay function that will repeat a specified number of times.  
 @ Return Value
    None
 @ Notes
    The time delay accuracy will be adversely affected by the time to execute
    interrupt service routines in preemptive systems.
 ---------------------------------------------------------------------------- */
void usDelay(unsigned int us) {
    unsigned int tWait, tStart;
    tStart = ReadCoreTimer();
    tWait = (CORE_MS_TICK_RATE * us / 1000);
    while ((ReadCoreTimer() - tStart) < tWait); // wait for the time to pass
} /* End of DelayUs */

/* ------------------------------ nsDelay -----------------------------------
 @ Syntax
    void nsDelay(unsigned int ns);
 @ Parameter
    @ param1: ns - the number of 10's of microseconds to delay
 @ Keywords
    delay, ns, microseconds, software delay, core timer
 @ Description
    This is a 10 nanosecond delay function that will repeat a specified
    number of times.  
 @ Return Value
    None
 @ Notes
    The time delay accuracy will be adversely affected by the time
    to execute interrupt service routines in preemptive systems.
 ---------------------------------------------------------------------------- */
void nsDelay(unsigned int ns) {
    unsigned int tWait, tStart;
    tStart = ReadCoreTimer();
    tWait = (CORE_MS_TICK_RATE * ns / 10000);
    while ((ReadCoreTimer() - tStart) < tWait); // wait for the time to pass
} /* End of DelayUs */

/* ------------------------------ periodMs -----------------------------------
 @ Syntax
    int periodMs(unsigned int msec);
 @ Description
    This is a non-blocking millisecond delay function that can be use for
    periodic activation of code.
 @ Parameter
    @ param1: int msec - the number of milliseconds to delay
 @ Return Value
    integer - delay in progress TURE / FALSE
 @ Notes
    Calling this function with a zero time parameter can be used to see if
    this function is currently in use.  A TRUE is returned if a period timing
    is in progress. The time delay accuracy will be adversely affected by the
    time to execute interrupt service routines in preemptive systems.
 ---------------------------------------------------------------------------- */
int PeriodMs(unsigned int msec) {
    static int delay_in_progross_flag = FALSE;
    static unsigned int tWait, tStart;
    if (delay_in_progross_flag == 0) {          // New Period
	tStart = ReadCoreTimer();               // Capture starting time
        tWait = (CORE_MS_TICK_RATE * msec);     // Compute delay period
        delay_in_progross_flag = TRUE;          // Set operation flag
    }
    if ((ReadCoreTimer() - tStart) >= tWait)    // Check if period is over
        delay_in_progross_flag = FALSE;
    return delay_in_progross_flag;
} /* End of PeriodMs */
