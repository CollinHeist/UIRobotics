/* ----------------------------------- File Inclusion ----------------------------------- */

#include <plib.h>

#include "hardware.h"
#include "Motor.h"

/* -------------------------- Global Variables and Structures --------------------------- */

static unsigned int t2_tick;	// Internal variable used for T2 tick

/* ---------------------------------- Public Functions ---------------------------------- */

/*
 *	Summary
 *		Initialize the PWM motor outputs to a given starting duty cycle and operating frequency.
 *	Parameters
 *		m1DutyCycle[in]: unsigned int that is the percent [0-100] to start motor 1 at.
 *		m2DutyCycle[in]: unsigned int that is the percent [0-100] to start motor 2 at.
 *		pwmFreq[in]: unsigned int that corresponds to what frequency Timer 2 should operate at.
 *	Returns
 *		Unsigned int that corresponds to whether an error occurred or not.
 */
unsigned int initializePWM(unsigned int m1DutyCycle, unsigned int m2DutyCycle, unsigned int pwmFreq) {
	// Timer 2 Initialization
	t2_tick = T2_CLOCK_RATE / pwmFreq;
	if (t2_tick > MAX_T2_VAL)
	return ERROR;
	
	OpenTimer2(T2_ON | T2_SOURCE_INT | T2_PS_1_1, t2_tick - 1);
	ConfigIntTimer2(T2_INT_ON | T2_INT_PRIOR_2);
	mT2IntEnable(1);
	
	// Initialize Motor Pins
	if (initializeMotorPins() == ERROR)
	return ERROR;
	
	if (m1DutyCycle > 100 || m2DutyCycle > 100)
	return ERROR;

	// Output Compare Module
	unsigned int start_val1 = m1DutyCycle * (t2_tick - 1);
	unsigned int start_val2 = m2DutyCycle * (t2_tick - 1);
	OpenOC2(OC_ON | OC_TIMER_MODE16 | OC_TIMER2_SRC | OC_PWM_FAULT_PIN_DISABLE | OC_CONTINUE_PULSE, start_val1, start_val1);
	OpenOC3(OC_ON | OC_TIMER_MODE16 | OC_TIMER2_SRC | OC_PWM_FAULT_PIN_DISABLE | OC_CONTINUE_PULSE, start_val2, start_val2);
	
	return NO_ERROR;
}

/*
 *	Summary
 *		Set the duty cycle for both motors.
 *	Parameters
 *		m1DutyCycle[in]: unsigned int that is the percent [0-100] to start motor 1 at.
 *		m2DutyCycle[in]: unsigned int that is the percent [0-100] to start motor 2 at.
 *	Returns
 *		Unsigned int that corresponds to whether an error occurred or not.
 *	Notes
 *		If either input variable is DONT_UPDATE_PWM (see header), that output isn't changed.
 */
unsigned int setPWM(unsigned int m1DutyCycle, unsigned int m2DutyCycle) {
	if ((m1DutyCycle > 100 && m1DutyCycle != DONT_UPDATE_PWM) || (m2DutyCycle > 100 && m2DutyCycle != DONT_UPDATE_PWM))
	return ERROR;

	if (m1DutyCycle != DONT_UPDATE_PWM)
	SetDCOC2PWM((int) ((float) m1DutyCycle / 100.0 * (t2_tick - 1)));
	if (m2DutyCycle != DONT_UPDATE_PWM)
	SetDCOC3PWM((int) ((float) m2DutyCycle / 100.0 * (t2_tick - 1)));

	return NO_ERROR;
}

/* --------------------------------- Private Functions ---------------------------------- */

/*
 *	Summary
 *		Initialize the motor pins for OC output, etc.
 *	Parameters
 *		None.
 *	Returns
 *		Unsigned int that corresponds to whether an error occurred or not.
 */
static unsigned int initializeMotorPins(void) {
	// Motor A1
	TRISBbits.TRISB3 = 0;
	ANSELBbits.ANSB3 = 0;
	LATBbits.LATB3 = 0;
	
	// Motor B1
	TRISEbits.TRISE9 = 0;
	LATEbits.LATE9 = 0;
	
	// Motor Mode select (A1-B1 vs. A2-B2))
	TRISFbits.TRISF1 = 0;
	LATFbits.LATF1 = 1;
	
	// Configure Peripheral Pin Select
	RPE8R = 0b01011;
	RPB5R = 0b01011;
	
	// Motor Directionality
	TRISGbits.TRISG8 = 0;
	TRISGbits.TRISG9 = 0;
	
	return NO_ERROR;
}

/* ----------------------------- Interrupt Service Routines ----------------------------- */

/*
 *	Summary
 *		Interrupt service routine for the output-compare PWM timer - timer 2.
 *	Parameters
 *		None.
 *	Returns
 *		None.
 *	Notes
 *		This currently does nothing - but is explicitly written for future implementation.
 */
void __ISR(_TIMER_2_VECTOR, IPL2SOFT) isrTimer2Handler(void) {
	mT2ClearIntFlag();
}
