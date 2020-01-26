#ifndef __SW_DELAY_H__ // Guard against multiple inclusion 
	#define __SW_DELAY_H__

	// Tick rate used for core-timer assisted delays
	#define CORE_MS_TICK_RATE	 (unsigned int) (GetCoreClock() / 1000UL)

	// Function Prototypes
	void delayMS(unsigned int ms);
	void delayUS(unsigned int us);
	void delayNS(unsigned int ns);
	int nonBlockingDelayMS(unsigned int ms);
#endif

