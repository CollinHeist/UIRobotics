/* 
** CONU1.h
** Professor Richard Wall
** Department of Electrical and Computer Engineering
** University of Idaho
** Moscow, ID 83844-1023
** rwall@uidaho.edu
**
** October 2, 2011
** Revised:	11/25/2012 - added nonblocking get string function
**
** Serial port interface using PIC32 UART1 - "printf" function
** uses UART1 
**
*/

#ifndef __CONU1__

	#define __CONU1__
	#define _UART1 

// I/O definitions for the Explorer16
// No hardware handshaking supported
#define CTS 	_RF12   // Cleart To Send, input, HW handshake
#define RTS     _RF13   // Request To Send, output, HW handshake
#define BACKSPACE 0x8   // ASCII backspace character code            

// init the serial port (UART2, 115200 baud, 8, N, 1, CTS/RTS )
void initU1( void);

// send a character to the serial port
int putU1( int c);

// wait for a new character to arrive to the serial port
char getU1( void);

// send a null terminated string to the serial port
int putsU1(char *s);

// receive a null terminated string in a buffer of len char
// Function is blocking
char * getsn( char *s, int n);

// Receive a CR or LF terminated string in a buffer
// Function is nonblocking - RWW 11/25/2012
int getUARTstr(char *s);

// useful macros
#define clrscr() puts( "\x1b[2J") 
#define home()   puts( "\x1b[1,1H") 

/*	Printf support
** Richard Wall
** October 2, 2011
*/
	void _mon_putc(char c);  // Used by system only

#endif
