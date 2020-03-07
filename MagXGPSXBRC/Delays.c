/* ----------------------------------- File Inclusion ----------------------------------- */

#include <plib.h>
#include "hardware.h"

#include "Delays.h"

/* -------------------------- Global Variables and Structures --------------------------- */

/* ---------------------------------- Public Functions ---------------------------------- */

/*
 *	Summary
 *		A blocking millisecond delay function that uses the core timer.
 *	Parameters
 *		ms[in]: Unsigned integer that signifies the number of milliseconds to delay.
 *	Returns
 *		None.
 */
void delayMS(const unsigned int ms) {
	unsigned int tStart = ReadCoreTimer();
	unsigned int tWait = (CORE_MS_TICK_RATE * ms);

	while ((ReadCoreTimer() - tStart) < tWait);	// Delay
}

/*
 *	Summary
 *		A blocking microsecond delay function that uses the core timer.
 *	Parameters
 *		us[in]: Unsigned integer that signifies the number of microseconds to delay.
 *	Returns
 *		None.
 */
void delayUS(const unsigned int us) {
	unsigned int tStart = ReadCoreTimer();
	unsigned int tWait = (CORE_MS_TICK_RATE * us / 1000);

	while ((ReadCoreTimer() - tStart) < tWait);	// Delay
}

/*
 *	Summary
 *		A blocking nanosecond delay function that uses the core timer.
 *	Parameters
 *		ns[in]: Unsigned integer that signifies the number of nanoseconds to delay.
 *	Returns
 *		None.
 */
void delayNS(const unsigned int ns) {
	unsigned int tWait, tStart;

	tStart = ReadCoreTimer();
	tWait = (CORE_MS_TICK_RATE * ns / 10000);

	while ((ReadCoreTimer() - tStart) < tWait);	// Delay
}

/*
 *	Summary
 *		A non-blocking millisecond delay function.
 *	Parameters
 *		ms[in]: Unsigned integer that signifies the number of milliseconds to 'delay'.
 *	Returns
 *		Unsigned integer that is DELAY_IN_PROGRESS or NO_DELAY_IN_PROGRESS (see header).
 *	Notes
 *		To check the status of a previous delay, pass a 0 to this function.
 */
unsigned int nonBlockingDelayMS(const unsigned int ms) {
	static int delayInProgress = FALSE;
	static unsigned int tWait, tStart;

	if (delayInProgress == NO_DELAY_IN_PROGRESS) {
		tStart = ReadCoreTimer();			// Capture starting time
		tWait = (CORE_MS_TICK_RATE * ms);	// Compute delay period
		delayInProgress = DELAY_IN_PROGRESS;
	}

	if ((ReadCoreTimer() - tStart) >= tWait)// Check if period is over
		delayInProgress = NO_DELAY_IN_PROGRESS;

	return delayInProgress;
}

/* --------------------------------- Private Functions ---------------------------------- */

/* ----------------------------- Interrupt Service Routines ----------------------------- */
