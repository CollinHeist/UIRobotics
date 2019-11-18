// System included files
#include "hardware.h"
#include "Gamepad.h"
#include <plib.h>

static int Status1 = 0;
static int Status2 = 0;

void initializeChangeNotice(void)  {
	PORTSetPinsDigitalIn(IOPORT_C, BIT_13 | BIT_14); // Declare BTND as CN pin as input
	int JA2 = mPORTCRead() & BIT_13;     // Read BTND IO pin to set initial pin status
	int JA1 = mPORTCRead() & BIT_14; 
	/* Do not use ENABLE PULLUPS on BAsys MX3 button inputs because buttons are 
	active high with external pull down resistors. Hence when internal pullups 
	are set on, the pin always reads high. */
	//mCNAOpen((CNA_ON | CNA_IDLE_CON), (CNA15_ENABLE), 0);

	// JA and JB pins should not have internal pullups enabled unless external pullups are added.
	mCNCOpen((CNC_ON | CNC_IDLE_CON), (CNC13_ENABLE | CNC14_ENABLE), (CNC13_PULLUP_ENABLE | CNC14_PULLUP_ENABLE)); 

	// Pins can have the CN ENABLED or DISABLED during run time
	EnableCNC13;    // Enable interrupts for each individual CN pin
	EnableCNC14;    // Enable interrupts for each individual CN pin
	/* All CN interrupts are vectored to a single CN ISR    */
	ConfigIntCNC((CHANGE_INT_ON | CHANGE_INT_PRI_1));
}

/* ------------------------------ Timer1Handler ------------------------------
  @ Summary
 * A common CN ISE is used for all CN interrupts regardless of IO port and 
 * pin.
  @ Parameter:  None, no parameters can be pass to an ISR.  
  @ Returns:    None, no parameters can be returned from an ISR.  
 *
 * Remarks:  The IO Pin(s) responsible for generating the CN interrupt are
 * determined by reading the CN status registers (CNSTATx) and checking the
 * set bits. CNSTATx bits will be set for changes on only pins that have had
 * the CN enabled for each port. If pins on other IO ports are enabled, the
 * CN status register for that port must be checked as well. Reading the IO 
 * port clears all standing CNSTAT bits for the IO port read.
  ---------------------------------------------------------------------------- */
void __ISR(_CHANGE_NOTICE_VECTOR, IPL1SOFT) interruptChangeNoticeHandler(void) {
	unsigned int JA1;
	unsigned int JA2;
	if (CNSTATC & BIT_13) {	// Check to see which pin(s) created the interrupt
		JA1 = mPORTCRead() & BIT_13; // Clear all standing CNSTATA bits.
		SetDefaultServoPosition();
		ClearLeftStick();

		Status1++;
		Status1 %= 2;
	}

	if (CNSTATC & BIT_14) {// Check to see which pin(s) created the interrupt
	    JA2 = mPORTCRead() & BIT_14; // Clear all standing CNSTATA bits.
	    SetDefaultServoPosition();
	    ClearLeftStick();
	    
	    Status2++;
	    Status2 %= 2;
	}

	mCNCClearIntFlag(); // Clear the CN interrupt flag
}