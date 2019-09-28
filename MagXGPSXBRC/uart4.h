/* ************************************************************************** */
/** Descriptive File Name: UART Serial communications header file for 
 *  comm_lib.c. This declares all public functions.

  @Summary
	UART 4 initialization and character and string I/O.

  @Description
	UART initialization can be set for any BAUD rate using EVEN, ODD, or NO 
	parity. Character and string input functions are non blocking functions.
	The serial interface uses UART channel 4.

**************************************************************************** */

#ifndef __UART_4_H__
	#define __UART_4_H__

	// ASCII control characters 
	#define BACKSPACE		0x08
	#define NO_PARITY		0
	#define ODD_PARITY		1
	#define EVEN_PARITY		2
#endif

// Function Prototypes
void uart4_init(unsigned int baud, int parity);
void _mon_putc(char c);
//BOOL putcU4( int c);
//BOOL getcU4( char *ch);
int putsU4(const char *s); 
int getstrU4( char *s, unsigned int len );
int getcIU4( char *ch);
int putcIU4( int ch);