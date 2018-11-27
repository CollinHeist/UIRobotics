/* ------------- Hardware and Processor Initialization Includes -------------- */
#include "config_bits.h"
#include "hardware.h"

/* -------------------------- XC32 Public Libraries -------------------------- */
#include <plib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <Math.h>

/* ----------------------- Project-specific include files -------------------- */
#include "AD22100.h"
#include "GPS_I2C.h"
#include "i2c_lib.h"
#include "LCDlib.h"
#include "led7.h"
#include "MAG3110.h"
#include "RC.h"
#include "swDelay.h"
#include "uart2.h"
#include "uart4.h"
#include "Variable.h"

//extern unsigned int millisec;	// Global millisecond counter

static void initialize_nav_sens(void);

/* ---------------------------- Main Project Function ------------------------ */
int main(void) {
	/* -------------- Variable declarations and initializations -------------- */
	struct VariablesList Var;			//Initialize VARIABLE FILE
	Var.timer = millisec;				// Time marker to limit output readings
	Var.GPSMagTimer = millisec;			// Timer used to update the GPS & Magnometer

	
    /* ---- Initialize all external communications (UART2, UART4, I2C...) ---- */
	I2C_RESULT i2c_flag = Hardware_Setup();
	initialize_nav_sens();
	// initLCD();
	uart4_init(38400, NO_PARITY);		// PC Terminal
	uart2_init(9600, NO_PARITY);		// X-Bee, most likely...

	if (i2c_flag == I2C_SUCCESS) {
		printf("System configuration completed.\n\r");

						//THIS IS THE CURRENT OPERATIONAL CODE LOOP

		while (1) {
			/* ---------- Read from the UART2 Channel, i.e. PC Input --------- */
			Var.uart2_flag = getcU2(&Var.uart2_ch);
			if (Var.uart2_flag) {			// Character has been recieved
				// Character processing code should go here
				putcU4(Var.uart2_ch);		// Put the character on the monitor
			}

			/* -------- Read from the UART4 Channel; the X-Bee Module -------- */
			Var.uart4_flag = getcU4(&Var.uart4_ch);
			if (Var.uart4_flag) {
				// Character processing code should go here
				putcU4(Var.uart4_ch);	// Put the character on the monitor?
			}

									//THIS IS WHERE SAM'S CODE SITS
			
			/* -------- Read from the I2C GPS and Magnetometer Module -------- */
			//WRITE XBOX VARIABLES UPDATE COMMAND HERE	//Update every pass
			if((millisec-Var.GPSMagTimer)>2000){	//Update every 2 seconds
				i2c_flag = ReportGPS(TRUE);	// Read RMC only, send to the terminal
				i2c_flag = MAG3110_readMag(&Var.x, &Var.y, &Var.z);
				if (i2c_flag == I2C_SUCCESS) {
					Var.heading = MAG3110_readHeading();
					Var.heading = (Var.heading < 0.0 ? Var.heading + 360.0 : Var.heading);
					printf("Magnetic X: %4d, Y: %4d, Z: %4d, Dir: %f\n\r", Var.x, Var.y, Var.z, Var.heading);
				}
				Var.GPSMagTimer = millisec;
			}
/*
			while((millisec-TEMPVAR)>1){	//Update _____ Variables every 1 millisecond
				TEMPVAR = millisec;
			}
*/
			//INSERT COMMAND HERE
			//INSERT COMMAND HERE
			//INSERT COMMAND HERE
		}
	}
	else {
		printf("System failed to initialze properly.\n\r");
	}

	return EXIT_FAILURE;
}

static void initialize_nav_sens(void) {
	char lcdstr[40];
	I2C_RESULT i2cFlag;
	int len = 255;
	int pkt_len = 0;

	initLCD();
	uart4_init(38400, NO_PARITY);
	// initTimer1();	// Initialized in hardware.c
	init_ad22100();
	i2cFlag = I2C_Init(I2C2, I2C_SPEED_STANDARD);

	if (!i2cFlag) {
		printf("I2C Setup Error");
	}
	len = 1;

	i2cFlag = I2C_Read(I2C2, GPS_DEV_ID, gpsStr, &len);

	if (i2cFlag == I2C_SUCCESS) {
		printf("GPS module online.\n\r");
	}
    else {
        printf("GPS Module not successfully read from.\n\r");
    }

	DelayMs(2000);
}
