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
    
    /*
    // Local variables
    I2C_RESULT I2cResultFlag;   // I2C Init Result flag
    I2C_RESULT I2cReadFlag;   // I2C Init Result flag
    int16_t x, y, z;
    unsigned ADCTemperatureInterval = 60000;
    unsigned MovementInterval = 20;
    unsigned ADCIntervalMark = 0;
    unsigned ActualADCInterval = ADCTemperatureInterval;
    unsigned ActualMovementInterval = MovementInterval;
    float heading = 0;

    // Init. the DMA flag
    DmaIntFlag = 0;

    // Initialization
    I2cResultFlag = InitializeModules(&I2cResultFlag);	// Init all I/O modules
    unsigned int error_flag |= initializeDMAUART2RX();

    // Re-enable global interrupts

    
    while (1) {  // Forever process loop	
	if (DmaIntFlag) {	    // clear the interrupt flag
	    DmaIntFlag = 0;         // Reset DMA Rx block flag
	    printf("message received %s\n", DMABuffer);
	    restartDMATransfer();
	    HandleInput();	    // Handles all user input from the XB device
	    getStringUART2("A", 1);
	}
     * 
	// Get data from the ADC temperature sensors
	if ((millisec - ADCIntervalMark) >= ADCTemperatureInterval) {
	    read_temperature_store();			// Reads in the temperature
	    ADCIntervalMark = millisec;
	}

    }
    
    return EXIT_FAILURE; // Code execution should never get to this statement 
    */
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