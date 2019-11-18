// File Inclusion
#include "hardware.h"
#include <plib.h>
#include "swDelay.h"

/*msDelay   Function Description *********************************************
SYNTAX:		 void msDelay(unsigned int mS);
DESCRIPTION:	This is a millisecond delay function uses the core time
				to set the base millisecond delay period. Delay periods of
				zero are permitted. 
KEYWORDS:	   delay, ms, milliseconds, software delay, core timer
PARAMETER1:	 mS - the number of milliseconds to delay
RETURN VALUE:   None:
NOTES:		  The time delay accuracy will be adversely affected by the time
				to execute interrupt service routines in preemptive systems.
END DESCRIPTION ************************************************************/
void msDelay(unsigned int mS) {
	unsigned int tWait, tStart;
	tStart = ReadCoreTimer();
	tWait = (CORE_MS_TICK_RATE * mS);
	while((ReadCoreTimer()- tStart) < tWait); // wait for the time to pass
}

/* usDelay Function Description ***********************************************
SYNTAX:		 void usDelay(unsigned int us);
PARAMETER1:	 us - the number of microseconds to delay
KEYWORDS:	   delay, us, microseconds, software delay, core timer
DESCRIPTION:	This is a microsecond delay function that will repeat a
				specified number of times.  
RETURN VALUE:   None
NOTES:		  The time delay accuracy will be adversely affected by the time
				to execute interrupt service routines in preemptive systems.
END DESCRIPTION ************************************************************/
void usDelay(unsigned int us) {
	unsigned int tWait, tStart;

	tStart = ReadCoreTimer();
	tWait = (CORE_MS_TICK_RATE * us / 1000);
	while((ReadCoreTimer()- tStart) < tWait); // wait for the time to pass
}

/* nsDelay Function Description ***********************************************
SYNTAX:		 void nsDelay(unsigned int ns);
PARAMETER1:	 ns - the number of 10's of microseconds to delay
KEYWORDS:	   delay, ns, microseconds, software delay, core timer
DESCRIPTION:	This is a 10 nanosecond delay function that will repeat a
				specified number of times.  
RETURN VALUE:   None
NOTES:		  The time delay accuracy will be adversely affected by the time
				to execute interrupt service routines in preemptive systems.
END DESCRIPTION ************************************************************/
void nsDelay(unsigned int ns) {
	unsigned int tWait, tStart;

	tStart = ReadCoreTimer();
	tWait = (CORE_MS_TICK_RATE * ns / 10000);
	while((ReadCoreTimer()- tStart) < tWait); // wait for the time to pass
}

/* PeriodMs Function Description *********************************************
 * SYNTAX:		int PeriodMs(unsigned int msec);
 * KEYWORDS:	  delay, ms, milliseconds, software delay, core timer,
 *				non-blocking
 * DESCRIPTION:   This is a non-blocking millisecond delay function that
 *				can be use for periodic activation of code.
 * PARAMETER1:	int msec - the number of milliseconds to delay
 * RETURN VALUE:  integer - delay in progress TURE / FALSE
 * NOTES:		 Calling this function with a zero time parameter can be
 *				used to see if this function is currently in use.  A TRUE
 *				is returned if a period timing is in progress.
 * 
 *				The time delay accuracy will be adversely affected by the time
 *				to execute interrupt service routines in preemptive systems.
 * END DESCRIPTION ************************************************************/
int PeriodMs(unsigned int msec) {
	static int delay_in_progross_flag = FALSE;
	static unsigned int tWait, tStart;

	if(delay_in_progross_flag == 0)			// New Period
	{
	tStart = ReadCoreTimer();				// Capture starting time
		tWait = (CORE_MS_TICK_RATE * msec);	// Compute delay period
		delay_in_progross_flag = TRUE;		// Set operation flag
	}
	if ((ReadCoreTimer()-tStart) >= tWait)	// Check if period is over
		delay_in_progross_flag = FALSE;
	return delay_in_progross_flag;
}

/* ------------------------------ DelayMs() ----------------------------------
 @ Description
	This is a millisecond delay function uses the core time to set the base
	millisecond delay period. Delay periods of zero are permitted. 
 @ Parameters
	@ param1 : mS - the number of milliseconds to delay
 @ Return Value
	None
 @ Notes
	The time delay accuracy will be adversely affected by the time to execute 
	interrupt service routines in preemptive systems.
 ----------------------------------------------------------------------------- */
void DelayMs(unsigned int mS) {
	unsigned int tWait, tStart;
	tStart = ReadCoreTimer();
	tWait  = (CORE_MS_TICK_RATE * mS);
	while ((ReadCoreTimer() - tStart) < tWait);
}