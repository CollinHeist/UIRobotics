/* ----------------------------------- File Inclusion ----------------------------------- */

#include <plib.h>
#include "hardware.h"

#include "Notice.h"
#include "Gamepad.h"

/* -------------------------- Global Variables and Structures --------------------------- */

static int Status1 = 0;
static int Status2 = 0;

/* ---------------------------------- Public Functions ---------------------------------- */

void initializeChangeNotice(void)  {
	PORTSetPinsDigitalIn(IOPORT_C, BIT_13 | BIT_14); // Declare BTND as CN pin as input
	int JA2 = mPORTCRead() & BIT_13;     // Read BTND IO pin to set initial pin status
	int JA1 = mPORTCRead() & BIT_14; 
	//mCNAOpen((CNA_ON | CNA_IDLE_CON), (CNA15_ENABLE), 0);

	// JA and JB pins should not have internal pullups enabled unless external pullups are added.
	mCNCOpen((CNC_ON | CNC_IDLE_CON), (CNC13_ENABLE | CNC14_ENABLE), (CNC13_PULLUP_ENABLE | CNC14_PULLUP_ENABLE)); 

	// Pins can have the CN ENABLED or DISABLED during run time
	EnableCNC13;    // Enable interrupts for each individual CN pin
	EnableCNC14;    // Enable interrupts for each individual CN pin

	ConfigIntCNC((CHANGE_INT_ON | CHANGE_INT_PRI_1));
}

void __ISR(_CHANGE_NOTICE_VECTOR, IPL1SOFT) interruptChangeNoticeHandler(void) {
    unsigned int JA1;
    unsigned int JA2;

    // Check to see which pin(s) created the interrupt
    if (CNSTATC & BIT_13) {
	JA1 = mPORTCRead() & BIT_13; // Clear all standing CNSTATA bits.
//	SetDefaultServoPosition();
	ClearLeftStick();

	Status1++;
	Status1 %= 2;
    }

    if (CNSTATC & BIT_14) {
	JA2 = mPORTCRead() & BIT_14; // Clear all standing CNSTATA bits.
//	SetDefaultServoPosition();
	ClearLeftStick();

	Status2++;
	Status2 %= 2;
    }

    mCNCClearIntFlag(); // Clear the CN interrupt flag
}