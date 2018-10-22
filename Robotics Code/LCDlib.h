/* --------------------- Guard Against Multiple Inclusions ------------------- */
#include "swDelay.h"
#include <stdio.h>
#include <stdint.h>

/* --------------------- Guard Against Multiple Inclusions ------------------- */
#ifndef __LCDLIB_H__
	#define __LCDLIB_H__

  #define LCD_DATAbits 	0x0ff
	#define LCD_DB0	 		(1 << 0)
	#define LCD_DB1	 		(1 << 1)
	#define LCD_DB2	 		(1 << 2)
	#define LCD_DB3	 		(1 << 3)
	#define LCD_DB4	 		(1 << 4)
	#define LCD_DB5	 		(1 << 5)
	#define LCD_DB6	 		(1 << 6)
	#define LCD_DB7	 		(1 << 7)

	#define ENpin	   		(1 << 4)
	#define LCD_HIGH		1
	#define LCD_LOW	 		0
	#define LCD_EN(a)		{ if (a) LATDSET = ENpin; else LATDCLR = ENpin; }	

	#define RWpin	   		(1 << 5)
	#define LCD_WR	  		0
	#define LCD_RD	  		1
	#define LCD_RW(a)		{ if(a) LATDSET = RWpin; else LATDCLR = RWpin; }

	#define RSpin	   		(1 << 15)
	#define LCDDATA	 		1 		// Address of data register
	#define LCDCMD	  		0		// Address of command register
	#define LCD_RS(a)		{ if(a) LATBSET = RSpin; else LATBCLR = RSpin; }

	#define HLCD			16		// LCD width, 16 characters
	#define VLCD			2 		// LCD height, 2 rows

	#define LCD_CFG	 		0x38	// Configuration mode for 8 bit data bus
	#define LCD_ENTRY   	0x06	// Block flashing cursor - increment cursor
	#define LCD_ON	  		0x0F	// Activate LCD
	#define LCD_HOME		0x02	// Put cursor in left most position
	#define LCD_CLR	 		0x01	// Clear characers off LCD
	#define MAX_LCD_POS 	32		// Maximum number of positions for LCD
	#define NEW_LINE		16		// number of spaces before a new line

	/* ----------------------- LCD Status Definitions ------------------------ */
	#define LCD_ADDR		0x7F	// Address mask for status register
	#define LCD_BF	  		0x80	// Busy flag mask for status register
	#define LCD_BF_bit  	7 		// Busy flag bit for status register

	/* -------------------- LCD Address Control Constants -------------------- */
	#define LCD_DDRAM   	0x80	// DD Ram Set Control
	#define LCD_LINE1   	0x10	// End of LCD line #1
	#define LCD_LINE2   	0x40	// Start of LCD line #2

	// Read LCD Busy Flag (No Delay)
	#define busyLCD()		readLCD(LCDCMD) & LCD_BF  
	#define waitLCD()		while (busyLCD()) 

	// Read Cursor Address
	#define addrLCD()		readLCD(LCDCMD) & LCD_ADDR  

	// Read RAM at the cursor position
	#define getLCD()		readLCD(LCDDATA) 

	// Write a single BYTE to the display, takes ~50us
	#define cmdLCD(c)		{ waitLCD(); writeLCD(LCDCMD, (c)); }

	// Clear the LCD entirely, takes about 1,750us
	#define clrLCD()		{ waitLCD(); writeLCD(LCDCMD, 1); DelayUs(2000); }

	// Put the cursor at the left of the screen, takes about 1,750s
	#define homeLCD()		{ waitLCD(); writeLCD(LCDCMD, 2); DelayUs(1750); }

	// Set the cursor position, must be between 0 and 31
	#define gotoLCD(p)		{ waitLCD(); (p >= 16) ? writeLCD(LCDCMD, ((p + 0x30) | LCD_DDRAM)) : writeLCD(LCDCMD, (p | LCD_DDRAM)); }

	// Set address pointer for CGRAM
	#define setLCDG(a)		{ waitLCD(); writeLCD(LCDCMD, (a & 0x3F) | 0x40); }
	// Set address pointer for DDRAM
	#define setLCDC(a)		{ waitLCD(); writeLCD(LCDCMD, (a & 0x7F) | 0x80); }

	/* ---------------------- Public Function declarations ------------------- */
	void initLCD(void);
	void writeLCD(int rs, char c);
	unsigned char readLCD(int addr);
	void putsLCD(char *s);
	void putcLCD(int rs, char c);
	void initLCDGr(void);
	void FFT_Disp(int16_t* amplitude, int N);
#endif
