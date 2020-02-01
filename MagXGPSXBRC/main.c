/* ----------------------------------- File Inclusion ----------------------------------- */
#include "config_bits.h"
#include "hardware.h"
#include "main.h"
#include "Pot.h"
#include <plib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "i2c_lib.h"
#include "GPS_I2C.h"
#include "Gamepad.h"
#include "TempADC.h"
#include "Notice.h"

// Platform common included files
#include "swDelay.h"

// Application included files
#include "UART4.h"  // Monitor UART
#include "UART2.h"  // GPS UART
#include "LCDlib.h"
#include "RC.h"
#include "DMA_UART2.h"
#include "MAG3110.h"
#include "Stepper.h"

/* -------------------------- Global Variables and Structures --------------------------- */

#define RC_CW   0   // RC Direction of rotation
#define RC_CCW  1

char GetMsg(char Char) { return Char;}
int InitializeModules(I2C_RESULT* I2cResultFlag);
void print_pretty_table(int use_uart);
I2C_RESULT InitMag();

// Global variables
static char Char;
int gps_message = 0;        // Active GPS sentence 

/* ---------------------------------- Public Functions ---------------------------------- */

int main(void) {
    I2C_RESULT * test;
    Hardware_Setup();
    
//    TRISEbits.TRISE8 = 0; LATEbits.LATE8 = 0;
//    TRISBbits.TRISB5 = 0; ANSELBbits.ANSB5 = 0; LATBbits.LATB5 = 0;
    mPORTBSetPinsDigitalOut(BIT_3);
    
    unsigned int i = initialize_pwm(0, 0, 1000);
    
    i = 0;
    while(1) {
	unsigned int error = set_pwm(i, i);
	delayMS(100);
	
	i = i++ % 101;
    }
    
    /*
    // Local variables
    I2C_RESULT I2cResultFlag;   // I2C Init Result flag
    I2C_RESULT I2cReadFlag;   // I2C Init Result flag
    int16_t x, y, z;
    unsigned GPSInterval = 1000;
    unsigned MagInterval = 60;
    unsigned ADCTemperatureInterval = 60000;
    unsigned MovementInterval = 20;
    unsigned ActualGPSInterval = GPSInterval;
    unsigned GPSIntervalMark = 0;
    unsigned MagIntervalMark = 0;
    unsigned ADCIntervalMark = 0;
    unsigned MovementIntervalMark = 0;
    unsigned ActualMagInterval = MagInterval;
    unsigned ActualADCInterval = ADCTemperatureInterval;
    unsigned ActualMovementInterval = MovementInterval;
    float heading = 0;

    // Init. the DMA flag
    DmaIntFlag = 0;

    // Initialization
    I2cResultFlag = InitializeModules(&I2cResultFlag);	// Init all I/O modules
    unsigned int error_flag |= initializeDMAUART2RX();

    // Re-enable global interrupts

    // Set the default position
    SetDefaultServoPosition();

    MAG3110_EnvCalibrate();
    
    while (1) {  // Forever process loop	
	if (DmaIntFlag) {	    // clear the interrupt flag
	    DmaIntFlag = 0;         // Reset DMA Rx block flag
	    printf("message received %s\n", DMABuffer);
	    restartDMATransfer();
	    HandleInput();	    // Handles all user input from the XB device
	    getStringUART2("A", 1);
	}

	if (SW0()) {
	    SetDefaultServoPosition();
	}

	// GPS receives data
	if ((millisec - GPSIntervalMark) >= GPSInterval) {
	    //I2cReadFlag = ReportGPS(TRUE);	
	    //GPSIntervalMark = millisec;
	}

	// GPS receives data
	if ((millisec - MovementIntervalMark) >= MovementInterval) {
	    Move();								// Read from the GPS and display to the screen
	    MovementIntervalMark = millisec;
	}


	// Mag receives data
	if ((millisec - MagIntervalMark) >= MagInterval) {
	    I2cResultFlag = MAG3110_readMag(&x,&y,&z);

	    if (I2cResultFlag == I2C_SUCCESS) {
		MAG3110_readHeading(&heading);
		if(heading < 0) heading += 360;
		if(heading >= 360) heading -= 360;
		clrLCD();
		printf("%f,%d,%d,%d\n\r", heading, x, y, z);  
	    }
	    else {
		printf("Readmag error\n");
	    }

	    MagIntervalMark = millisec;
	}


	// Get data from the ADC temperature sensors
	if ((millisec - ADCIntervalMark) >= ADCTemperatureInterval) {
	    read_temperature_store();			// Reads in the temperature
	    ADCIntervalMark = millisec;
	}

    }
    
    return EXIT_FAILURE; // Code execution should never get to this statement 
    */
}


void print_pretty_table(int use_uart) {
    char lcdStr[40];
    int tempC, tempF;
    int16_t x,y,z;
    tempC = (int)((35 + ((10 * 0) - (10 * 0)) / 452));
    tempF = ((tempC * 9) / 5) + 32;
    
    I2C_RESULT i2cFlag = MAG3110_readMag(&x,&y,&z);

    if (use_uart) {
	printf("RAW data values\n\r");
	printf("X:%6d ", x);
	printf("Y:%6d ", y);
	printf("Z:%6d\n\r ", z);
    }
    sprintf(lcdStr, "X:%5d Y:%5d", x, y);
    sprintf(lcdStr, "T1:%5d T:%5d", getTemp1(), getTemp1());
}


int InitializeModules(I2C_RESULT* I2cResultFlag) {
    BOOL Result;
    Hardware_Setup();				// Initialize common IO
    initializeUART4(38400, NO_PARITY);		// PC Terminal
    initializeUART2(9600, NO_PARITY);		// XBEE
    putStringUART2("\n\rXBee online\n\r");	// Send message to PC
    init_analog();				// Initialize AN2 to read Pot
    initializeTemperatureSensors();
    int16_t x,y,z;
    
    *I2cResultFlag = I2C_Init(I2C1, 100000);
    initializeChangeNotice();
    initializeStepper();
    
    // Mag init procedure
    if (!(Result = MAG3110_initialize())) {
	printf("MAG3110 failed to init");
    }
    
    
    printf("Magnetometer is calibrating\n\r");
    MAG3110_enterCalMode();
        
    while (MAG3110_isCalibrating()) {
        MAG3110_calibrate();
        MAG3110_readMag(&x,&y,&z);
        printf("%d,%d,%d\n\r",x,y,z);
        delayMS(10);
    }
    
    if (MAG3110_isCalibrated()) {
        printf("Magnetometer is calibrated\n\r");
    }
    
    Result = setGPS_RMC();
    
    return Result; 
}

//
// InitMag()
//
//
I2C_RESULT InitMag() {
    I2C_RESULT IResult;
    BOOL Result;
    int16_t x,y,z;

    // Mag init procedure
    if (!(Result = MAG3110_initialize())) {
	printf("MAG3110 failed to init");
    }

    printf("Magnetometer is calibrating\n\r");
    MAG3110_enterCalMode();

    while (MAG3110_isCalibrating()) {
	MAG3110_calibrate();
	MAG3110_readMag(&x, &y, &z);
	printf("%d,%d,%d\n\r", x, y, z);
	delayMS(10);
    }

    if (MAG3110_isCalibrated()) {
	printf("Magnetometer is calibrated\n\r");
    }
}