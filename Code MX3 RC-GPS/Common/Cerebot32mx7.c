/* 
** Cerebot32MX7.c
**
** Professor Richard Wall
** Department of Electrical and Computer Engineering
** University of Idaho
** Moscow, ID 83844-1023
** rwall@uidaho.edu
**
**	Date:		9/30/2011
**
**	Initializes LED1 through LED4 for output
**  Initializes BTN1, BTN2 and BTN3 for input
**	Initializes LEDA through LEDH as outputs for stepper motors if needed
*/

#include <plib.h>
#include "Cerebot32mx7.h"


void Cerebot32mx7_setup(void)
{
	DDPCONbits.JTAGEN = 0;
	PORTSetPinsDigitalIn(IOPORT_G, BIT_6 | BIT_7); // Set BTN1 and BTN2 as inputs
	PORTSetPinsDigitalIn(IOPORT_A, BIT_0 ); // Set BTN3 as input
              
// Set LED1 through LED4 as outputs
	PORTSetPinsDigitalOut(IOPORT_G, BIT_12 | BIT_13 | BIT_14 | BIT_15); 
	LATGCLR = RGLEDS;

// Set LEDA-LEDH as outouts
	PORTSetPinsDigitalOut(IOPORT_B, RBLEDS); 
	LATBCLR = RBLEDS;

}

