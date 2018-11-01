/* --------------------- Guarding against multiple inclusion ----------------- */
#ifndef __UART2_H__
	#define __UART2_H__

	/* ----------------- Necessary system include statements ----------------- */
	#include "hardware.h"

	#include <plib.h>
	#include <stdio.h>

	/* ----------------- Public Global Variables / Constants ----------------- */
	#define BACKSPACE		0x08
	#define NO_PARITY		0
	#define ODD_PARITY		1
	#define EVEN_PARITY		2

	/* ---------------------- Public Function declarations ------------------- */
	void uart2_init(unsigned int baud, int parity);
	void _mon_putc(char c);
	int putcU2(int ch);
	int getcU2(char *ch);
	int putsU2(const char *s);
	int getstrU2(char *s, unsigned int len);
#endif
