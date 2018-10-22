#ifndef __RC_H__
	#define __RC_H__

	// We might need these? Old format, not sure if we're using it
	#define cfgRC1()    PORTSetPinsDigitalOut(IOPORT_D, BIT_4)	// Was 9
	#define cfgRC2()    PORTSetPinsDigitalOut(IOPORT_G, BIT_8)	// D-11
	#define cfgRC3()    PORTSetPinsDigitalOut(IOPORT_G, BIT_7)	// D-10
	#define cfgRC4()    PORTSetPinsDigitalOut(IOPORT_G, BIT_6)	// D-8

	/* -------------------- RC Channel pin assignment macros ----------------- */
	#define RC_1(a)     LATDbits.LATD4  = a	// Was 9
	#define RC_2(a) 	LATDbits.LATD8 = a	// 11
	#define RC_3(a)		LATDbits.LATD7 = a	// 10
	#define RC_4(a)		LATDbits.LATD6  = a	// 8

	#define NRC			4		// Number of RC channels

	/* --------- RC Setting Range - How wide the PWM Signal should be -------- */
	#define RC_MIN		100
	#define	RC_MAX		200
	#define RC_SPAN		100

	/* ---------------------- Public Function declarations ------------------- */
	void initRC(void);
	void rc_output(int ch, int ctrl);
	void rcUpdate(void);
	void set_rc(int ch, int ctrl);
#endif
