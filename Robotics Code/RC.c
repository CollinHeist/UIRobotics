#include "RC.h"

/* --------------------------- Public global variables ----------------------- */


/* ---------------------------- Main Project Function ------------------------ */
int main(void) {
    I2C_RESULT r = Hardware_Setup();	// Initialize common Cerebot hardware
	uart4_init(38400, NO_PARITY);		// Initialize UART
	initRC();				// Initialize RC channel output pins
	DelayMs(2000);			// Wait two seconds
	printf("\n\rEnter servo position\n\r");
	return (EXIT_FAILURE);	// The program should never get to this
}
