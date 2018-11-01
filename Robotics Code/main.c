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

//extern unsigned int millisec;	// Global millisecond counter

static void initialize_nav_sens(void);

/* ---------------------------- Main Project Function ------------------------ */
int main(void) {
	/* -------------- Variable declarations and initializations -------------- */
	char uart2_ch, uart4_ch;			// Recieved character
	BOOL uart2_flag, uart4_flag;		// Character ready flags
	int x, y, z;						// GPS coordinates
	float heading;						// GPS Heading
	int timer = millisec;				// Time marker to limit output readings

	/* ---- Initialize all external communications (UART2, UART4, I2C...) ---- */
	I2C_RESULT i2c_flag = Hardware_Setup();
	initialize_nav_sens();
	// initLCD();
	uart4_init(38400, NO_PARITY);		// PC Terminal
	uart2_init(9600, NO_PARITY);		// X-Bee, most likely...

	if (i2c_flag == I2C_SUCCESS) {
		printf("System configuration completed.\n\r");
		while (1) {
			/* ---------- Read from the UART2 Channel, i.e. PC Input --------- */
			uart2_flag = getcU2(&uart2_ch);
			if (uart2_flag) {			// Character has been recieved
				// Character processing code should go here
				putcU4(uart2_ch);		// Put the character on the monitor
			}

			/* -------- Read from the UART4 Channel; the X-Bee Module -------- */
			uart4_flag = getcU4(&uart4_ch);
			if (uart4_flag) {
				// Character processing code should go here
				putcU4(uart4_ch);	// Put the character on the monitor?
			}

			/* -------- Read from the I2C GPS and Magnetometer Module -------- */
			i2c_flag = ReportGPS(TRUE);	// Read RMC only, send to the terminal
			i2c_flag = MAG3110_readMag(&x, &y, &z);
			if (i2c_flag == I2C_SUCCESS && (millisec - timer > 1000)) { // Only output every second
				timer = millisec;
				heading = MAG3110_readHeading();
				heading = (heading < 0.0 ? heading + 360.0 : heading);
				printf("Magnetic X: %4d, Y: %4d, Z: %4d, Dir: %f\n\r", x, y, z, heading);
			}
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
