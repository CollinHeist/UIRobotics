
/*
 * File:   TMP03.c
 * Author: rwall
 *
 * Created on January 1, 2014
 */

#include "hardware.h"
#include <xc.h> 
#include <sys/attribs.h>
#include <plib.h>

#if defined(USE_TEMPERATUER_TMP03)

#include "TMP03.h"
extern float temp1;
extern float temp2;

/* tmp03_init Function Description ******************************************
SYNTAX:		void tmp03_init(void);
KEYWORDS:	Capture, Compare, Timer3, multi vector interrupt
DESCRIPTION:	Initializes Input Capture 5 for generate interrupt on each
		signal edge using Timer 3 / 256 for its time base.
PARAMETERS:	None
RETURN VALUE:	None
NOTES:		See C32 Peripheral Library Guide Section 12, page 175
END DESCRIPTION ************************************************************/

void tmp03_init(void)
{
unsigned int con_config;

    IC3R = 0x0A;        // Map IC3 to Port C pin 1 (JA2))
    IC4R = 0x0A;        //Map IC3 to Port C pin 4 (JA3)
/* Set RD3 is set but not used for this application. */
    PORTSetPinsDigitalIn(IOPORT_D, BIT_3 | BIT_12);

    mIC3ClearIntFlag();	/* Clear pending input capture 3 interrupts */
    mIC4ClearIntFlag();	/* Clear pending input capture 4 interrupts */

/*  Setting up Timer 3 *******************************************************
 * Parameter 1          - Configuration bits
 * T3_ON            - Timer 3 set to on
 * T3_PS_1_64		- Timer 3 prescale 1:64
 * T3_SOURCE_INT	- Timer 3 input source is PBCLK
 * Parameter 2 - 	- Terminal count period = FPB/64/65536 = 2.36 Hz
 *              	  or 0.419 second period
*************************************************************************** */
    OpenTimer3(T3_ON | T3_PS_1_64 | T3_SOURCE_INT, 0xffff);

/* Timer 3 interrupt is not used in this aplication */
   ConfigIntTimer3(T3_INT_ON | T3_INT_PRIOR_2 | T3_INT_SUB_PRIOR_2);

/*  Setting up Input Capture 5 operations ************************************
 * Parameter 1         - Configuration bits
 * TC_ON		- Input capture set to on
 * IC_CAP_16BIT         - Timer 3 prescale 1:256
 * IC_IDLE_STOP         - No interrupts when idle
 * IC_TIMER3_SRC	- Use Timer 3
 * IC_FEDGE_FALL	- First edge is a falling transition
 * IC_INT_1CAPTURE	- 1 capture per interrupt
 * IC_EVERY_EDGE	- Capture both rising and falling edges
*************************************************************************** */
    con_config = (IC_ON|IC_CAP_16BIT|IC_IDLE_STOP|IC_FEDGE_FALL|IC_TIMER3_SRC\
                  |IC_INT_1CAPTURE|IC_EVERY_EDGE);
    OpenCapture3(con_config);
    OpenCapture4(con_config);

/*  Setting up Input Capture 5 interrupt ***********************************
 * Parameter 1          - Configuration bits
 * IC_INT_ON		- Input capture interrupt set to on
 * IC_INT_PRIOR_3 	- Interrupt subgroup priority  level 3
 * IC_INT_SUB_PRIOR_0	- Interrupt group priority  level 0
*************************************************************************** */
    con_config = (IC_INT_ON | IC_INT_PRIOR_3 | IC_INT_SUB_PRIOR_0);
    ConfigIntCapture3(con_config);
    con_config = (IC_INT_ON | IC_INT_PRIOR_3 | IC_INT_SUB_PRIOR_1);
    ConfigIntCapture4(con_config);
}

/* T3Interrupt Function Description ******************************************
 * SYNTAX:          void T3Interrupt(void);
 * KEYWORDS:        Timer3 interrupt service routine
 * DESCRIPTION:     Toggles SM LED4 every Timer 3 interrupt at interrutpt group
 *                  level 2 sublevel 2.
 * PARAMETERS:      None
 * RETURN VALUE:    None
 *
 * NOTES:	See initCAPT and __ISR( _INPUT_CAPTURE_5_VECTOR, ipl5) Capture5
 * END DESCRIPTION **********************************************************/
void __ISR( _TIMER_3_VECTOR, IPL2SOFT) T3Interrupt(void)
{
    invLED0();      /* Toggle LED0 for the input capture timer overflow */
    mT3ClearIntFlag();
} /* End of T3Interrupt ISR */

/* Capture3 Function Description *******************************************
 * SYNTAX:          void Capture3(void);
 * KEYWORDS:        Input capture, period, frequency
 * DESCRIPTION:     Input capture service routine for computing PWM ratio
 * PARAMETERS:      None
 * RETURN VALUE:    None
 *
 * NOTES:           Global variable temperature is modified.
 * END DESCRIPTION **********************************************************/
void __ISR( _INPUT_CAPTURE_3_VECTOR, IPL3SOFT) Capture3(void)
{
static unsigned int time1[8] = {0};     /* Most recent captured time */
static unsigned int time2[8] = {0};     /* Previous time captured */
static unsigned int time3 = 0;          /* Previous time captured */
static unsigned int time_T1;            /* High Period */
static unsigned int time_T2;            /* Low Period */
float temp_F;                           /* Temperature calculation */

    if(!PORTCbits.RC1)
    {
        time1[0] = mIC3ReadCapture();      /* Read negative capture into buffer */
//        ReadCapture3(time1);            /* Read captures into buffer */

        time_T1 = (time1[0] - time2[0]);    /* Determine cycle period */
        time_T2 = (time2[0] - time3);       /* Determine duty cycle */
        temp_F = 455.0 - (720.0 * time_T1)/(time_T2);
        time3 = time1[0];      /* Read captures into buffer */
        if( (temp_F <125.5) && (temp_F > -50.5) )
            temp1 = temp_F;
        invLED1();      /* Toggle LED1 each Temp1 transition */
    }
    else
    {
        time2[0] = mIC3ReadCapture();      /* Read positive captures into buffer */
//        ReadCapture3(time2);            /* Read captures into buffer */
    }

    mIC3ClearIntFlag();
} /* End of Capture5 ISR */

/* Capture3 Function Description *******************************************
 * SYNTAX:          void Capture4(void);
 * KEYWORDS:        Input capture, period, frequency
 * DESCRIPTION:     Input capture service routine for computing PWM ratio
 * PARAMETERS:      None
 * RETURN VALUE:    None
 *
 * NOTES:           Global variable temperature is modified.
 * END DESCRIPTION **********************************************************/
void __ISR( _INPUT_CAPTURE_4_VECTOR, IPL3SOFT) Capture4(void)
{
static unsigned int time1[8] = {0};     /* Most recent captured time */
static unsigned int time2[8] = {0};     /* Previous time captured */
static unsigned int time3 = 0;          /* Previous time captured */
static unsigned int time_T1;            /* High Period */
static unsigned int time_T2;            /* Low Period */
float temp_F;                           /* Temperature calculation */

    if(!PORTCbits.RC4)
    {
        time1[0] = mIC4ReadCapture();      /* Read negative capture into buffer */
//        ReadCapture4(time1);            /* Read captures into buffer */

        time_T1 = (time1[0] - time2[0]);    /* Determine cycle period */
        time_T2 = (time2[0] - time3);       /* Determine duty cycle */
        temp_F = 455.0 - (720.0 * time_T1)/(time_T2);
        time3 = time1[0];      /* Read captures into buffer */
        if( (temp_F <125.5) && (temp_F > -50.5) )
            temp2 = temp_F;
        invLED2();      /* Toggle LED2 each Temp1 transition */
    }
    else
    {
        time2[0] = mIC4ReadCapture();      /* Read positive captures into buffer */
//        ReadCapture4(time2);            /* Read captures into buffer */
    }

    mIC4ClearIntFlag();
} /* End of Capture5 ISR */

#endif
