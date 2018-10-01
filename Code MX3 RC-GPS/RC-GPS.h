#ifndef __RC__
	#define __RC__

    #define cfgRC1()    PORTSetPinsDigitalOut(IOPORT_B, BIT_8)
    #define cfgRC2()    PORTSetPinsDigitalOut(IOPORT_A, BIT_15)
// RC Channel pin assignment macros
	#define RC_1(a)     LATBbits.LATB8 = a
	#define RC_2(a) 	LATAbits.LATA15 = a

	#define NRC		2	// Number of RC channels

// RC Setting range
	#define RC_MIN		100
	#define	RC_MAX		200
	#define RC_SPAN		100

	void initTimer1(void);
	void initRC(void);
	void set_RC(int ch, int ctrl);
	int check_buttons(void);

#endif