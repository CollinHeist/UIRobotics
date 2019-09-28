/** Descriptive File Name: UART Serial communications header file for 
 *  comm_lib.c. This declares all public functions.

  @ Date: 
	 Created:  February 2, 2018 for Basys MX3

  @Summary
	UART 4 initialization and character and string I/O.

  @Description
	UART initialization can be set for any BAUD rate using EVEN, ODD, or NO 
	parity. Character and string input functions are non blocking functions.
	The serial interface uses UART channel 3.

**************************************************************************** */

#ifndef __UART2_H__
	#define __UART2_H__
	#define _UART2

	// ASCII control characters
	#define BACKSPACE		0x08
	#define NO_PARITY		0
	#define ODD_PARITY		1
	#define EVEN_PARITY		2
#endif

// Function Prototypes
void uart2_init(unsigned int baud, int parity);
int putcU2(int c);
int getcU2(char *ch);
int putsU2(const char *s);
int getstrU2(char *s, unsigned int len);