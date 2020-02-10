/* ----------------------------------- File Inclusion ----------------------------------- */

#include <plib.h>

#include "hardware.h"

/* -------------------------- Global Variables and Structures --------------------------- */

unsigned int millisec;

/* ---------------------------------- Public Functions ---------------------------------- */

/*
 *	Summary
 *		Setup all commonly-required hardware.
 *	Parameters
 *		None.
 *	Returns
 *		Unsigned integer that corresponds to whether an error occurred or not.
 */
unsigned int hardwareSetup(void) {
	SYSTEMConfig(GetSystemClock(), SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE);
	DDPCONbits.JTAGEN = 0;	// Statement is required to use Pin RA0 as IO

	ALL_DIGITAL_IO();		// Sets all LED, switches and push buttons for digital IO
	SET_MIC_ANALOG();		// Sets microphone input for analog
	SET_POT_ANALOG();		// Sets ANALOG INPUT CONTROL for analog input

	Set_All_LEDs_Output();	// Sets Basys MX3 LED0 through LED7 as output
	Set_All_LEDs_Off();		// Sets Basys MX3 LED0 through LED7 off
	SWcfg();				// Sets Basys MX3 SW0 through SW7 as input
	Set_All_PBs_Input();	// Sets Basys MX3 push buttons as input
	Set_RGB_Output();		// Sets Basys MX3 RGB LED as output
	Set_LED8_RGB(0);		// Sets Basys MX3 RGB LED off
	initializeTimer1();
	
	INTConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);	// Do only once
	INTEnableInterrupts();	//	Do as needed for global interrupt control
	
	return NO_ERROR;
}

/* --------------------------------- Private Functions ---------------------------------- */

/*
 *	Summary
 *		Configure T1 for a 10 microsecond interrupt.
 *	Parameters
 *		None.
 *	Returns
 *		None.
 */
static void initializeTimer1(void) {
	PORTSetPinsDigitalOut(IOPORT_B, BIT_8);
	millisec = 0;	   // Global millisecond counter
	OpenTimer1(T1_ON | T1_SOURCE_INT | T1_PS_1_1, TIMER1_TICK);

	// Set Timer 1 interrupt with a priority of 2
	ConfigIntTimer1(T1_INT_ON | T1_INT_PRIOR_2);
	mT1IntEnable(1);		   // Enable interrupts of T1
}

/* ----------------------------- Interrupt Service Routines ----------------------------- */

/*
 *	Summary
 *		ISR for the 10 microsecond timer 1 interrupt.
 *	Parameters
 *		None.
 *	Returns
 *		None.
 */
void __ISR(_TIMER_1_VECTOR, IPL2SOFT) interruptTimer1(void) {
	static int ms_count = TIMER1_MS_COUNT;  // How many counts are necessary to reach 1 ms

	ms_count--;					// Decrement the millisecond counter
	if (ms_count <= 0) {
		millisec++;		
		ms_count = TIMER1_MS_COUNT;
	}

//	rcUpdateServos();		// This updates the RC outputs for the servos
//	rcUpdateSpeedControllers();	// This updates the RC outputs for the speed controllers

	mT1ClearIntFlag();	// Clear the interrupt flag	
}