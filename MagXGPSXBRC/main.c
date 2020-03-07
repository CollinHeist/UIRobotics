/* ----------------------------------- File Inclusion ----------------------------------- */
#include "config_bits.h"
#include "hardware.h"
#include "main.h"
#include <plib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "Gamepad.h"
#include "TempADC.h"
#include "Notice.h"
#include "SensorData.h"

// Platform common included files
#include "Delays.h"

// Application included files
#include "uart4.h"  // Monitor UART
#include "uart3.h"	// Arduino UART
#include "uart2.h"  // GPS UART
#include "Motor.h"

/* -------------------------- Global Variables and Structures --------------------------- */

extern char DMABuffer[DMA_BUFFER_SIZE+1];

static SensorData latestReadings;

/* ---------------------------------- Public Functions ---------------------------------- */

/*
 *	Summary
 *		Main background execution loop.
 *	Parameters
 *		None.
 *	Returns
 *		If this function returns, something went wrong..
 */
int main(void) {
    unsigned int errorFlag = initializeSystem();

    if (errorFlag != NO_ERROR)
    	return ERROR;

    while (1) {
		switch (parseSensorString(&latestReadings)) {
			case INVALID_STRING:
				putStringUART4("Invalid string sent from the Arduino.");
				break;
			case STILL_PARSING:	break;
			case DONE_PARSING:
				// Do something
				break;
			default:
				putStringUART4("Unknown return from parseSensorString().");
				break;
		}
    }

    return ERROR;
}

/*
 *	Summary
 *		Function to initialize all subsystems on the board.
 *	Parameters
 *		None.
 *	Returns
 *		Unsigned integer that is ERROR or NO_ERROR for if any initializations failed.
 */
static unsigned int initializeSystem(void) {
    unsigned int errorFlag = initializeHardware();
    
    errorFlag |= initializeUART4(PC_UART4_BAUD, PC_UART4_PARITY);
    errorFlag |= initializeUART2(XBEE_UART2_BAUD, XBEE_UART2_PARITY);
	errorFlag |= initializeUART3(ARDUINO_UART3_BAUD, ARDUINO_UART3_PARITY);
    errorFlag |= initializePWM(MOTOR1_START_PWM_PERCENT, MOTOR2_START_PWM_PERCENT, MOTORS_PWM_FREQUENCY);
    errorFlag |= initializeTemperatureSensors(TEMPERATURE_TIMEOUT_MS);
    errorFlag |= initializeChangeNotice();
    
    return errorFlag;
}