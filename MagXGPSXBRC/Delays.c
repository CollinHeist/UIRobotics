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
 *		ms[in]: unsigned int that signifies the number of milliseconds to delay.
 *	Returns
 *		None.
 */
void delayMS(unsigned int ms) {
	unsigned int tWait, tStart;

	tStart = ReadCoreTimer();
	tWait = (CORE_MS_TICK_RATE * ms);

	while ((ReadCoreTimer() - tStart) < tWait);	// Delay
}

/*
 *	Summary
 *		A blocking microsecond delay function that uses the core timer.
 *	Parameters
 *		us[in]: unsigned int that signifies the number of microseconds to delay.
 *	Returns
 *		None.
 */
void delayUS(unsigned int us) {
	unsigned int tWait, tStart;

	tStart = ReadCoreTimer();
	tWait = (CORE_MS_TICK_RATE * us / 1000);

	while ((ReadCoreTimer() - tStart) < tWait);	// Delay
}

/*
 *	Summary
 *		A blocking nanosecond delay function that uses the core timer.
 *	Parameters
 *		ns[in]: unsigned int that signifies the number of nanoseconds to delay.
 *	Returns
 *		None.
 */
void delayNS(unsigned int ns) {
	unsigned int tWait, tStart;

	tStart = ReadCoreTimer();
	tWait = (CORE_MS_TICK_RATE * ns / 10000);

	while ((ReadCoreTimer() - tStart) < tWait);	// Delay
}

/*
 *	Summary
 *		A non-blocking millisecond delay function.
 *	Parameters
 *		ms[in]: unsigned int that signifies the number of milliseconds to 'delay'.
 *	Returns
 *		int that is non-zero (TRUE) if an old delay is in progress, and zero
 *		(FALSE) if no previous delay is in progress.
 *	Notes
 *		To check the status of a previous delay, pass a 0 to this function.
 */
int nonBlockingDelayMS(unsigned int ms) {
	static int delayInProgress = FALSE;
	static unsigned int tWait, tStart;

	if (delayInProgress == FALSE) {		// New Period
		tStart = ReadCoreTimer();		// Capture starting time
		tWait = (CORE_MS_TICK_RATE * ms);	// Compute delay period
		delayInProgress = TRUE;			// Set operation flag
	}

	if ((ReadCoreTimer() - tStart) >= tWait)	// Check if period is over
		delayInProgress = FALSE;

	return delayInProgress;
}

/* --------------------------------- Private Functions ---------------------------------- */

/* ----------------------------- Interrupt Service Routines ----------------------------- */
