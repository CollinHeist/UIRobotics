#include "hardware.h"

unsigned int millisec = 0;

/* -------------------------- Function Prototyping --------------------------- */
static void initTimer1(void);

/* ----------------------------- Hardware_Setup ------------------------------
  @ Summary
	 Initializes PIC32 pins commonly used for IO on the Trainer processor 
	 board such as the slide switches, push buttons, and LEDs. It 
	 requires that ?config_bits? is included in the project.
  @ Parameters
	 None
  @ Returns
	 None
  @ Notes
  	 This function also initializes the I2C module for 
   --------------------------------------------------------------------------- */
I2C_RESULT Hardware_Setup(void) {
	I2C_RESULT i2cFlag_GPS, i2cFlag_MAG;
	int len;
	/* -----------------------------------------------------------------------
		Statement configure cache, wait states and peripheral bus clock
		Configure the device for maximum performance but do not change the PBDIV
		Given the options, this function will change the flash wait states, RAM
		wait state and enable prefetch cache but will not change the PBDIV.
		The PBDIV value is already set via the pragma FPBDIV option above..
	   ----------------------------------------------------------------------- */
	SYSTEMConfig(GetSystemClock(), SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE);
	DDPCONbits.JTAGEN = 0;  // Statement is required to use Pin RA0 as IO
	initRC();
	initTimer1();

	/* ---------------- Initialize the I2C2 Module - The GPS ----------------- */
	i2cFlag_GPS = I2C_Init(I2C2, I2C_SPEED_FAST); // I2C_SPEED_STANDARD);
	if (i2cFlag_GPS == I2C_SUCCESS) {
		// Reset GPS
		PORTSetPinsDigitalOut(IOPORT_A, BIT_14);
		LATAbits.LATA14 = 1;
		DelayMs(100);
		LATAbits.LATA14 = 0;
		DelayMs(100);
		LATAbits.LATA14 = 1;
		DelayMs(100);
		
		len = 255;
		i2cFlag_GPS = I2C_Read(I2C2, GPS_DEV_ID, gpsStr, &len);
		if (i2cFlag_GPS == I2C_SUCCESS) {
			printf("GPS on line\n\r");
			
			i2cFlag_GPS = setGPS_RMC();
			
			if (i2cFlag_GPS == I2C_SUCCESS) {
				printf("GPS RMC sentence set successfully\n\r");
			}
			else {
				printf("GPS RMC sentence failed\n\r");
			}
			 
		}	
		else {
			printf("GPS not available\n\r");	
		}
	}
	DelayMs(100);
	printf("Magnetometer is calibrating\n\r");

	/* --------------- Initialize the I2C1 Module - The MAG3110 -------------- */
	do {
		DelayMs(100);
		i2cFlag_MAG = MAG3110_initialize();
		if (!i2cFlag_MAG) {
			printf("Magnetometer not found..\n\r");
		}
	} while (!i2cFlag_MAG);

	printf("Magnetometer is calibrating.\n\r");
	MAG3110_enterCalMode();

	while (MAG3110_isCalibrating()) {
		MAG3110_calibrate();
	}
	if (MAG3110_isCalibrated()) {
		printf("Magnetometer is calibrated\n\r");
	}

	return (i2cFlag_GPS == I2C_SUCCESS && i2cFlag_MAG == I2C_SUCCESS ? I2C_SUCCESS : I2C_ERROR);
}

/* -------------------------------- initTimer1 -------------------------------
  @ Summary
	 Initializes Timer 1 for a 10us interrupt.
  @ Parameters
	 This function takes no parameters.
  @ Returns
	 This function has no return value.
   --------------------------------------------------------------------------- */
static void initTimer1(void) {
	#define TMR1_TICK		100
	
	PORTSetPinsDigitalOut(IOPORT_D, BIT_3);
	LATDbits.LATD3 = 0;
	millisec = 0;	   // Global millisecond counter
	OpenTimer1(T1_ON | T1_SOURCE_INT | T1_PS_1_1, TMR1_TICK);
	
	// Set Timer 1 interrupt with a priority of 2
	ConfigIntTimer1(T1_INT_ON | T1_INT_PRIOR_2);

	// Enable multi-vector interrupts
	INTConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);  // Do only once
	INTEnableInterrupts();   //Do as needed for global interrupt control
}

/* ------------------------------ Timer1Handler ------------------------------
  @ Summary
	 Processes the code associated with a Timer 1 interrupt. It is responsible
	 for generating the signals for each RC channel in two phases; the three 
	 state FSM controls the on period and the off period.
  @ Parameters
	 None, it is an ISR
  @ Returns
	 None
  ---------------------------------------------------------------------------- */
void __ISR(_TIMER_1_VECTOR, IPL2SOFT) Timer1Handler(void) {
	static ms = 100;			// Millisecond counter
	static int onesec = 10000;	// One second counter

	mT1ClearIntFlag();			// Clear the interrupt flag
	ms--;						// Increment the millisecond counter
	if (ms <= 0) {
		millisec++;
		ms = 100;
		onesec--;
		if (onesec <= 0) {
			onesec = 10000;
		}
	}

	rcUpdate();		 			// This updates the RC outputs
}

/* ----------------------------- millis() ----------------------------
  @ Summary
	 Returns millisecond count from global millisecond counter.
  @ Parameters
	 None
  @ Returns
	 unsigned int : millisecond count from global millisecond counter
  ---------------------------------------------------------------------------*/
unsigned int millis(void) {
	return millisec;
}
