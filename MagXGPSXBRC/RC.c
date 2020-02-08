/* ----------------------------------- File Inclusion ----------------------------------- */
#include <plib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <Math.h>

#include "hardware.h"
#include "RC.h"
#include "swDelay.h"

/* -------------------------- Global Variables and Structures --------------------------- */

/* ---------------------------------- Public Functions ---------------------------------- */

static unsigned int t2_tick;

unsigned int initializePWM(unsigned int m1DutyCycle, unsigned int m2DutyCycle, unsigned int pwmFreq) {
    // Timer 2 Initialization
    t2_tick = T2_CLOCK_RATE / pwmFreq;
    if (t2_tick > MAX_T2_VAL)
	return ERROR;
    
    OpenTimer2(T2_ON | T2_SOURCE_INT | T2_PS_1_1, t2_tick - 1);
    ConfigIntTimer2(T2_INT_ON | T2_INT_PRIOR_2);
    mT2IntEnable(1);
    
    MCInit();
    MCMODE(1);
//    PORTSetPinsDigitalOut(MOTOR1_PORT, MOTOR1_PIN);
//    PORTSetPinsDigitalOut(MOTOR2_PORT, MOTOR2_PIN);
    
    RPE8R = 0b01011;
    RPB5R = 0b01011;

    if (m1DutyCycle > 100 || m2DutyCycle > 100)
	return ERROR;

    // Output Compare Module
    unsigned int start_val1 = m1DutyCycle * (t2_tick - 1);
    unsigned int start_val2 = m2DutyCycle * (t2_tick - 1);
    OpenOC2(OC_ON | OC_TIMER_MODE16 | OC_TIMER2_SRC | OC_PWM_FAULT_PIN_DISABLE | OC_CONTINUE_PULSE, start_val1, start_val1);
//    ConfigIntOC2(OC_INT_PRIOR_3 | OC_INT_SUB_PRI_0 | OC_INT_ON);
//    mOC2IntEnable(1);
    
    OpenOC3(OC_ON | OC_TIMER_MODE16 | OC_TIMER2_SRC | OC_PWM_FAULT_PIN_DISABLE | OC_CONTINUE_PULSE, start_val2, start_val2);
//    ConfigIntOC3(OC_INT_PRIOR_3 | OC_INT_SUB_PRI_0 | OC_INT_ON);
//    mOC3IntEnable(1);
    
    // Motor Directionality
    TRISGbits.TRISG8 = 0;
    TRISGbits.TRISG9 = 0;
    
    return NO_ERROR;
}

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


/* ----------------------------- Interrupt Service Routines ----------------------------- */

void __ISR(_TIMER_2_VECTOR, IPL2SOFT) isrTimer2Handler(void) {
    mT2ClearIntFlag();
}
