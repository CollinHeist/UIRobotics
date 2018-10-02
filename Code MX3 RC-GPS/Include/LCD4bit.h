/* 
** LCD4bit.h
** Professor Richard Wall
** Department of Electrical and Computer Engineering
** University of Idaho
** Moscow, ID 83844-1023
** rwall@uidaho.edu
**
** Date:	August 18, 2012
**
** Provides a 4 data bit LCD interface for
** the Cerebot PIC32 MXck.
**
** 
*/


#ifndef LCD_4BIT_4LINE_H
	#define LCD_4BIT_4LINE_H

	#define LCD_DELAY		1			// LCD data delay - 10 us

//LCD setup commands
	#define LCD_CFG_8		0x3C		// Configuration mode for 8 bit data bus
	#define LCD_CFG_4		0x2C		// Configuration mode for 8 bit data bus
	#define LCD_4BIT		0x20		// 4 bit data mode
	#define LCD_ENTRY		0x06		// Block flashing cursor - increment power
	#define LCD_ON			0x0E		// Activate LCD
	#define LCD_HOME		0x02		// Put cursor in left most position
	#define LCD_CLR			0x01		// Clear characers off LCD
	#define LCD_4BIT		0x20		// 4 bit data mode

//LCD status definitions
	#define LCD_ADDR		0x7F		// Address mask for status register
	#define LCD_BF			0x80		// Busy flag mask for status register
	#define LCD_BF_bit		7			// Busy flag bit for status register
	#define LCD_DATA_BITS	0x00FF
	#define LCD_DATA_BITS_4	0x00F0

//LCD Addess control constants
	#define MAX_LCD_POS		80       	// Maximum number of positions for LCD
	#define NEW_LINE		20       	// number of spaces before a new line
	#define LCD_DDRAM		0x80		// DD Ram Set Control
	#define LCD_LINE1		20			// End of LCD line #1
	#define LCD_LINE2		40			// Start of LCD line #2
	#define LCD_LINE3		60			// Start of LCD line #3
	#define LCD_LINE4		80			// Start of LCD line #4

//Outputs for LCD control bit definitions

	#define LCD_RS				LATEbits.LATE0 	
	#define LCD_RS_TRIS			TRISEbits.TRISE0 
	#define LCD_RW				LATEbits.LATE1	
	#define LCD_RW_TRIS			TRISEbits.TRISE1 
	#define LCD_EN				LATEbits.LATE2 	
	#define LCD_EN_TRIS			TRISEbits.TRISE2 
	#define LCD_SET_OUTPUT()	TRISE = TRISE & ~LCD_DATA_BITS_4
	#define LCD_SET_INPUT()		TRISE = TRISE |  LCD_DATA_BITS_4
	#define LCD_DATA_WRITE(x)	LATE = ((x & LCD_DATA_BITS_4) | (LATE & ~LCD_DATA_BITS_4))
	#define LCD_DATA_READ()		PORTE & LCD_DATA_BITS_4

//Terminal count, size definitions, and misc definitions
	#define CR			   	0x0D    	// "Carriage Return": Cursor moves to position 1
	#define LF   			0x0A    	// "Line Feed": Clear display, move to position 1
	#define FF  			0x0C    	// "Form Feed": Same as LF

//lcd function prototypes
	void LCD_4bit_init(void);
	int LCD_4bit_get_status(void);
	void LCD_4bit_goto_pos(int pos);
	void LCD_4bit_putc(char x);
	void LCD_4bit_puts(char *char_string);
	void LCD_8bit_command(int command);
	void LCD_4bit_command(int command);

	#define LCD_ERASE()		{LCD_4bit_command(LCD_CLR); DelayMs(1);}

#endif

