#ifndef __RC_H__
	#define __RC_H__
	
	#define AIN1cfg()   (TRISBbits.TRISB3 = 0, ANSELBbits.ANSB3 = 0, LATBbits.LATB3 = 0) // OC4
//	#define AIN2cfg()   (TRISEbits.TRISE8 = 0, LATEbits.LATE8 = 0) // OC2
	#define BIN1cfg()   (TRISEbits.TRISE9 = 0, LATEbits.LATE9 = 0) // OC5
//	#define BIN2cfg()   (TRISBbits.TRISB5 = 0, ANSELBbits.ANSB5 = 0, LATBbits.LATB5 = 0) // OC3
	#define MCMODEcfg() (TRISFbits.TRISF1 = 0, LATFbits.LATF1 = 0)
	#define MCInit()	(AIN1cfg(), BIN1cfg(), MCMODEcfg())
    #define MCMODE(a);  {if(a) LATFbits.LATF1 = 1; else LATFbits.LATF1 = 0;}

	// Timer 2 Macros
	#define T2_PRESCALE			(1)
	#define T2_CLOCK_RATE		(GetPeripheralClock() / T2_PRESCALE)
    #define MAX_T2_VAL          (1 << 15)

    // PWM Output Configurations
    #define MOTOR1_PIN          (BIT_3)
    #define MOTOR1_PORT         (IOPORT_B)
    #define MOTOR2_PIN          (BIT_8)
    #define MOTOR2_PORT         (IOPORT_E)

    // Macro for not updating the PWM of a given channel
	#define DONT_UPDATE_PWM		(999)

	// Function Prototypes
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