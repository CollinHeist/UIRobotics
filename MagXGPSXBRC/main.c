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
#include "uart3.h"
#include "uart2.h"  // GPS UART
#include "Motor.h"

/* -------------------------- Global Variables and Structures --------------------------- */

extern char DMABuffer[DMA_BUFFER_SIZE+1];
char buff[DMA_BUFFER_SIZE+1];

SensorData latestReadings;

/* ---------------------------------- Public Functions ---------------------------------- */

int main(void) {
    unsigned int errorFlag = initializeSystem();

    if (errorFlag != NO_ERROR)
    	return ERROR;

    while (1) {
		putStringUART4(DMABuffer);
		while(getStringUART4(buff, 256)) {
			Nop();
		}
		restartDMATransfer();
		delayMS(500);
    }

    return ERROR;
}

unsigned int initializeSystem(void) {
    unsigned int errorFlag = initializeHardware();
    
    errorFlag |= initializeUART4(PC_UART4_BAUD, PC_UART4_PARITY);
    errorFlag |= initializeUART2(XBEE_UART2_BAUD, XBEE_UART2_PARITY);
	errorFlag |= initializeUART3(ARDUINO_UART3_BAUD, ARDUINO_UART3_PARITY);
	restartDMATransfer();
    errorFlag |= initializePWM(MOTOR1_START_PWM_PERCENT, MOTOR2_START_PWM_PERCENT, MOTORS_PWM_FREQUENCY);
    errorFlag |= initializeTemperatureSensors(TEMPERATURE_TIMEOUT_MS);
    errorFlag |= initializeChangeNotice();
    
    return errorFlag;
}