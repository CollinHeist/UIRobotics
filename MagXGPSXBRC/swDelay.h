#ifndef __SW_DELAY_H__ // Guard against multiple inclusion 
	#define __SW_DELAY_H__

	// File inclusion
	#include "hardware.h"
	#include <plib.h>
	#include "swDelay.h"
#endif

// Function Prototypes
void msDelay(unsigned int mS);
void usDelay(unsigned int uS);
void nsDelay(unsigned int ns);
int PeriodMs(unsigned int msec);
void DelayMs(unsigned int mS);