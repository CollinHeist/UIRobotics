#ifndef __RC_H__
	#define __RC_H__

	/* ----------------- Necessary system include statements ----------------- */
	#include "hardware.h"
	#include "swDelay.h"

	#include <plib.h>
	#include <string.h>
	#include <stdio.h>
	#include <stdint.h>
	#include <Math.h>
	
	#define cfgRC1()	PORTSetPinsDigitalOut(IOPORT_D, BIT_9)	// Wall has 4
	#define cfgRC2()	PORTSetPinsDigitalOut(IOPORT_D, BIT_11) // 8
	#define cfgRC3()	PORTSetPinsDigitalOut(IOPORT_D, BIT_10) // 7
	#define cfgRC4()	PORTSetPinsDigitalOut(IOPORT_D, BIT_8)  // 6

	/* -------------------- RC Channel pin assignment macros ----------------- */
	#define RC_1(a)		LATDbits.LATD9  = a
	#define RC_2(a)		LATDbits.LATD11 = a
	#define RC_3(a)		LATDbits.LATD10 = a
	#define RC_4(a)		LATDbits.LATD8  = a

	#define NRC			4		// Number of RC channels

	/* --------- RC Setting Range - How wide the PWM Signal should be -------- */
	#define RC_MIN		100
	#define	RC_MAX		200
	#define RC_SPAN		100

	/* ---------------------- Public Function Declarations ------------------- */
	void initRC(void);
	void rc_output(int ch, int ctrl);
	void rcUpdate(void);
	void set_rc(int rc1, int rc2, int rc3, int rc4);
#endif
