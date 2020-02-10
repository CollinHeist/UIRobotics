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

// Platform common included files
#include "Delays.h"

// Application included files
#include "UART4.h"  // Monitor UART
#include "UART2.h"  // GPS UART
#include "Motor.h"
#include "DMA_UART2.h"

/* -------------------------- Global Variables and Structures --------------------------- */

/* ---------------------------------- Public Functions ---------------------------------- */

int main(void) {
    unsigned int errorFlag = NO_ERROR;
    
    errorFlag = hardwareSetup();
    errorFlag |= initializeModules();

    if (errorFlag != NO_ERROR)
    	return ERROR;

    while (1) {

    }

    return ERROR;
}

unsigned int initializeModules(void) {
    unsigned int errorFlag = NO_ERROR;
    
    errorFlag |= initializeUART4(PC_UART4_BAUD, PC_UART4_PARITY);
    errorFlag |= initializeUART2(XBEE_UART2_BAUD, XBEE_UART2_PARITY);
    errorFlag |= initializePWM(MOTOR1_START_PWM_PERCENT, MOTOR2_START_PWM_PERCENT, MOTORS_PWM_FREQUENCY);
    errorFlag |= initializeTemperatureSensors(TEMPERATURE_TIMEOUT_MS);
    
    initializeChangeNotice();
    
    return errorFlag; 
}