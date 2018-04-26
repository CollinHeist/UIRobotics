/* ------------------------- Header file for motor.c -------------------------
  @ Summary
     Header file for the motor.c file, that allows us to power our motors
  @ Description 
     The primary functions we use here are RC_1() and RC_2(), but those are
     written into the wrapper function runMotor(). The rest of these
     functions are called from within this file itself. 
  ---------------------------------------------------------------------------- */
#ifndef _motor_
	#define _motor_

    #define cfgRC2()    do { PORTSetPinsDigitalOut(IOPORT_B, BIT_8);  RPB8R = 0x0B;  } while(0) // BTNR - OC5
    #define cfgRC1()    do { PORTSetPinsDigitalOut(IOPORT_A, BIT_15); RPA15R = 0x0B; } while(0) // BTND - OC4
    
    // RC Channel pin assignment macros
	#define RC_1(a)     SetDCOC4PWM(a); // LATBbits.LATB8 = a
	#define RC_2(a) 	SetDCOC5PWM(a); // LATAbits.LATA15 = a
	#define NRC		    2	            // Number of RC channels

    // RC Setting range
    #define PWM_RATE    50
    #define PWM_PD      GetPeripheralClock() / 4 / PWM_RATE     // TIMER Prescale = 4 
	#define RC_MIN		(PWM_PD / 20)
	#define	RC_MAX		(PWM_PD / 10 )
	#define RC_SPAN		(RC_MAX - RC_MIN)
	#define RC_CENTER   (RC_MIN + (RC_SPAN / 2))

    // Function Declarations
	void initRC(void);
	void set_RC(int ch, int ctrl);
	int runMotor(int channel, int position);
    int powerMotors(int leftChannel[], int rightChannel[], float duration);
#endif
