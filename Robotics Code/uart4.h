/* --------------------- Guarding against multiple inclusion ----------------- */
#ifndef __UART4_H__
	#define __UART4_H__

	/* ----------------- Necessary system include statements ----------------- */
	#include <plib.h>
	#include <stdio.h>

	#include "hardware.h"

	/* ----------------- Public Global Variables / Constants ----------------- */
	#define BACKSPACE	0x08
	#define NO_PARITY	0
	#define ODD_PARITY	1
	#define EVEN_PARITY	2

	/* ---------------------- Public Function declarations ------------------- */
	void uart4_init(unsigned int baud, int parity);
	void _mon_putc(char c);
	BOOL putcU4(int c);
	BOOL getcU4(char *ch);
	int putsU4(const char *s); 
	int getstrU4(char *s, unsigned int len);
#endif
