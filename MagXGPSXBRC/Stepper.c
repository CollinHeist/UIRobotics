// File Inclusion
#include "hardware.h"
#include <plib.h>
#include "Stepper.h"

void initializeStepper(void)  {
	MCInit();
}

//
// step()
// direction is +1 or -1
// mode is 1 for half, 2 for full
//
void step(int direction, int mode) {
	static const unsigned char step_code[8] = {8, 9, 1, 5, 4, 6, 2, 10}; 
	static unsigned int step_ptr = 0;   // Initialize pointer only on power up

	step_ptr += (direction * mode);		// Compute step code pointer
	step_ptr &= sizeof(step_code)-1;	// force modulo 8

	AIN1(step_code[step_ptr] & 0x01);
	AIN2(step_code[step_ptr] & 0x02);
	BIN1(step_code[step_ptr] & 0x04);
	BIN2(step_code[step_ptr] & 0x08);
}

