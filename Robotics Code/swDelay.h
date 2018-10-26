/* ---------------------- Guard against multiple inclusion ------------------- */
#ifndef __SW_DELAY_H__
	#define __SW_DELAY_H__

	/* ----------------- Necessary system include statements ----------------- */
	#include <plib.h>

	#include "hardware.h"

	/* ---------------------- Public Function declarations ------------------- */
	void DelayMs(unsigned int mS);
	void DelayUs(unsigned int us);
	void DelayNs(unsigned int ns);
	int PeriodMs(unsigned int msec);
#endif
