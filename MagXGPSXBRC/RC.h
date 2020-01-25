#ifndef __RC_H__
	#define __RC_H__

	#include <plib.h>
	
	#define cfgRC1()                PORTSetPinsDigitalOut(IOPORT_D, BIT_9)	// Wall has 4
	#define cfgRC2()                PORTSetPinsDigitalOut(IOPORT_D, BIT_11) // 8
	#define cfgRC3()                PORTSetPinsDigitalOut(IOPORT_D, BIT_10) // 7
	#define cfgRC4()                PORTSetPinsDigitalOut(IOPORT_D, BIT_8)  // 6

	/* -------------------- RC Channel pin assignment macros ----------------- */
	#define RC_1(a)                 (LATDbits.LATD9  = a)
	#define RC_2(a)                 (LATDbits.LATD11 = a)
	#define RC_3(a)                 (LATDbits.LATD10 = a)
	#define RC_4(a)                 (LATDbits.LATD8  = a)

    #define NRCSERVOS               (2)
    #define NRCSPEEDCONTROLLERS     (2)
	#define NRC                     (NRCSPEEDCONTROLLERS+NRCSERVOS)		// Number of RC channels

	/* --------- RC Setting Range - How wide the PWM Signal should be -------- */
	#define RC_SERVO_MIN                    100
	#define	RC_SERVO_MAX                    200
	#define RC_SPEED_CONTROLLER_MIN         108  // I rounded up for safety
	#define	RC_SPEED_CONTROLLER_MAX         190  // I rounded down for safety
	#define RC_SPEED_CONTROLLER_NEUTRAL     148  // Period of no movement
	#define	RC_SPEED_CONTROLLER_PERIOD      1490 // Measured PWM period
	#define RC_SPAN                         100

	/* ---------------------- Public Function Declarations ------------------- */
	void rcUpdateServos(void);
    void rcUpdateSpeedControllers(void);
    void set_rc(int rc1, int rc2, int rc3, int rc4);
    void rc_output(int ch, int ctrl);
    void initRC(void);
    int SetDefaultServoPosition(void);
    int TurnLeft(void);
    int TurnLeftPos(int movement);
    int TurnRight();
    int TurnRightPos(int movement);
    int ForwardPos(int movement);
    int BackwardPos(int movement);
#endif