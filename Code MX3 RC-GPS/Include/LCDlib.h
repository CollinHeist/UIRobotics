/* 
** LCDlib.h
**
** Professor Richard Wall
** Department of Electrical and Computer Engineering
** University of Idaho
** Moscow, ID 83844-1023
** rwall@uidaho.edu
**
**	Date: August 18, 2012
**
*/


#ifndef __LCDLIB_H__
	#define __LCDLIB_H__

	#define HLCD    16      // LCD width = 16 characters 
	#define VLCD    2       // LCD height = 2 rows


	#define LCDDATA 1       // address of data register
	#define LCDCMD  0       // address of command register

	void initLCD( void);
	void writeLCD( int addr, char c);    
	char readLCD( int addr);
	void posLCD(int pos);

	#define putLCD( d)  writeLCD( LCDDATA, (d))
	#define cmdLCD( c)  writeLCD( LCDCMD, (c))

	#define clrLCD()    writeLCD( LCDCMD, 1); DelayMs(2)
	#define homeLCD()   writeLCD( LCDCMD, 2)   

	#define setLCDG( a) writeLCD( LCDCMD, (a & 0x3F) | 0x40)
	#define setLCDC( a) writeLCD( LCDCMD, (a & 0x7F) | 0x80)

	#define busyLCD()  	readLCD( LCDCMD) & 0x80
	#define addrLCD()  	readLCD( LCDCMD) & 0x7F
	#define getLCD()  	readLCD( LCDDATA)
#endif

void putsLCD( char *s);

