#ifndef __DELAYS_H__ // Guard against multiple inclusion 
	#define __DELAYS_H__

	// Tick rate used for core-timer assisted delays
	#define CORE_MS_TICK_RATE       ((unsigned int) (GetCoreClock() / 1000UL))

    // Return encodings
    #define DELAY_IN_PROGRESS       (1)
    #define NO_DELAY_IN_PROGRESS    (0)

	// Function Prototypes
	void delayMS(const unsigned int ms);
	void delayUS(const unsigned int us);
	void delayNS(const unsigned int ns);
	unsigned int nonBlockingDelayMS(const unsigned int ms);
#endif

