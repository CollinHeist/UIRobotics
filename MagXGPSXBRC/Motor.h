#ifndef __MOTOR_H__
	#define __MOTOR_H__
	
	// Timer 2 Configurations
	#define T2_PRESCALE			(1)
	#define T2_CLOCK_RATE		(GetPeripheralClock() / T2_PRESCALE)
    #define MAX_T2_VAL          (1 << 15)

    // Macro for not updating the PWM of a given channel
	#define DONT_UPDATE_PWM		(999)

	// Function Prototypes
	unsigned int initializePWM(unsigned int m1DutyCycle, unsigned int m2DutyCycle, unsigned int pwmFreq);
    unsigned int setPWM(unsigned int m1DutyCycle, unsigned int m2DutyCycle);
    static unsigned int initializeMotorPins(void);
#endif