/* 
** LCD4bit.c
** Professor Richard Wall
** Department of Electrical and Computer Engineering
** University of Idaho
** Moscow, ID 83844-1023
** rwall@uidaho.edu
**
** Date:	August 18, 2012
**
** This is the driver program for a 4x20 Character LCD
** using a 4 bit interface.  PORT E is used for the 7
** data and control signals (see LCD.h used with this
** project). Each line is expected to be self contained.
** The LCDputs does not test for end of line address
** because the check status function is not reliable.
** Hence all LCD handshaking uses time delays.
**
*/

#include <plib.h>
#include <string.h>
#include "LCD4bit.h"

/* START Function Description ***********************************************
SYNTAX:			LCD_4bit_init;
PARAMETER1:		No Parameters
KEYWORDS:		initialize, lcd, configuration
DESCRIPTION:	This function goes through a list of configuration options and
DESCRIPTION:	sends the parameters to the lcd so it will display information
DESCRIPTION:	correctly. It also puts a delay between the operations so the
DESCRIPTION:	lcd doesn't get overloaded
RETURN VALUE:	None.
END DESCRIPTION ************************************************************/
void LCD_4bit_init(void)
{
	//Set LCD control pins for outputs
	LCD_RS = 0;
	LCD_RW = 0;
	LCD_EN = 0;

	LCD_EN_TRIS = 0;
	LCD_RW_TRIS = 0;
	LCD_RS_TRIS = 0;

	DelayMs(30);
	LCD_8bit_command(LCD_CFG_4);
   	DelayMs(15);
	LCD_8bit_command(LCD_CFG_4);
	DelayMs(15);
	LCD_4bit_command(LCD_CFG_4);
   	DelayMs(5);
	LCD_4bit_command(LCD_CFG_4);
   	DelayMs(5);
   	LCD_4bit_command(LCD_ENTRY);
   	DelayMs(15);
   	LCD_4bit_command(LCD_ON);
   	DelayMs(15);
   	LCD_4bit_command(LCD_CLR);
   	DelayMs(15);
}

/* START Function Description ***********************************************
SYNTAX:			void LCD_8bit_command(int command);
PARAMETER1:		command - The command that will be given to the lcd
KEYWORDS:		lcd, control
DESCRIPTION:	This function takes care of setting the lcd control lines
				low or high correctly to have the lcd recieve the command
RETURN VALUE:	None.
NOTES:			This is an 8 bit write command
END DESCRIPTION ************************************************************/
void LCD_8bit_command(int command)
{
	LCD_RW = 0; 
	LCD_RS = 0; 
	DelayUs(LCD_DELAY);
	LCD_SET_OUTPUT(); 			//	Set Data output
	LCD_DATA_WRITE (command); 	//	write command 
	LCD_EN = 1; 
	DelayUs(LCD_DELAY);
	LCD_EN = 0; 
}

/* START Function Description ***********************************************
SYNTAX:			void LCD_4bit_command(int command);
PARAMETER1:		command - The command that will be given to the lcd
KEYWORDS:		lcd, control
DESCRIPTION:	This function takes care of setting the lcd control lines
DESCRIPTION:	low or high correctly to have the lcd recieve the command
RETURN VALUE:	None.
NOTES:			This is a 4 bit write command
END DESCRIPTION ************************************************************/
void LCD_4bit_command(int command)
{
	LCD_RW = 0; 
	LCD_RS = 0; 
	LCD_SET_OUTPUT(); 							//	Set Data output
	DelayUs(LCD_DELAY);
	
	LCD_DATA_WRITE (command & LCD_DATA_BITS); 	//write high nibble command to RE
	LCD_EN = 1; 		
	DelayUs(LCD_DELAY);
	LCD_EN = 0; 		
	
	DelayUs(LCD_DELAY);
	LCD_DATA_WRITE ((command<<4) & LCD_DATA_BITS); 	//write low nibble command to RE

	LCD_EN = 1; 		
	DelayUs(LCD_DELAY);
	LCD_EN = 0; 		
    DelayMs(1);   //wait 1ms so we don't overload the lcd
}

/* START Function Description ***********************************************
SYNTAX:			int LCD_4bit_get_status();
PARAMETER1:		No Parameters
KEYWORDS:		lcd, status, busy flag
DESCRIPTION:	This function reads the busy flag on the lcd to see if it is
DESCRIPTION:	ready to recieve data
RETURN VALUE:	status - 0 means not busy 1 means busy
NOTES:			This functio is not used
END DESCRIPTION ************************************************************/
int LCD_4bit_get_status(void)
{
int s1, s2, status;
	LCD_SET_INPUT(); 		//	Set Data input
   	LCD_RS = 0; 			
   	LCD_RW = 1; 			

	DelayUs(LCD_DELAY);

   	LCD_EN = 1; 			// High order nibble
	DelayUs(LCD_DELAY);
	s1 = LCD_DATA_READ();
   	LCD_EN = 0; 			

	DelayUs(LCD_DELAY);

	LCD_EN = 1;
	DelayUs(LCD_DELAY);
	s2 = LCD_DATA_READ();
   	LCD_EN = 0; 			

	status = s1 | (s2>>4);
   	LCD_SET_OUTPUT(); 		//	Set Data output
   	return status;
}

/* START Function Description ***********************************************
SYNTAX:			void LCD_putc(char x);
PARAMETER1:		x - the character or command to be sent to the lcd.
KEYWORDS:		character, lcd, display
DESCRIPTION:	This function interprets a character as either a character or
DESCRIPTION:	a position command (i.e. carriage return) and sends the data
DESCRIPTION:	to the lcd accordingly.
RETURN VALUE:	None.
END DESCRIPTION ************************************************************/
void LCD_4bit_putc(char x)
{
//   int addr;
//	do
//   {
//   		addr = LCD_get_status();// & LCD_ADDR;
//   }
//   while(addr & LCD_BF); //wait for the busy flag

	switch(x)
	{
		case '\n':
		case FF:
		   	LCD_4bit_command(LCD_CLR);
			break;
		case  '\r':
		   	LCD_4bit_command(LCD_HOME);
			break;
   		default:
	   		LCD_RW = 0; 
			LCD_RS = 1; 

			LCD_SET_OUTPUT(); //RE_OUT);
			LCD_DATA_WRITE(x & LCD_DATA_BITS);
			LCD_EN = 1; 
			DelayUs(LCD_DELAY);
			LCD_EN = 0; 

			DelayUs(LCD_DELAY);

			LCD_DATA_WRITE((x<<4) & LCD_DATA_BITS);
			LCD_EN = 1; 
			DelayUs(LCD_DELAY);
			LCD_EN = 0; 
			DelayMs(1);   //wait 1ms so we don't overload the lcd
	}
}

/* START Function Description ***********************************************
SYNTAX:			void LCD_4bit_goto_pos(int pos);
PARAMETER1:		pos - the position (0 - 31)
KEYWORDS:		character, lcd, display, position
DESCRIPTION:	This function moves the cursor to the proper location
RETURN VALUE:	None.
END DESCRIPTION ************************************************************/
void LCD_4bit_goto_pos(int pos)
{
   if(pos < MAX_LCD_POS)
   {
		if(pos < LCD_LINE1)
			LCD_4bit_command(pos | LCD_DDRAM);
		else
		{
			if(pos < LCD_LINE2)
			{
				pos = (pos - LCD_LINE1) | 0x40;
		    	LCD_4bit_command(pos | LCD_DDRAM);
			}
			else
			{
				if(pos < LCD_LINE3)
				{
					pos = (pos - LCD_LINE1);
				    LCD_4bit_command(pos | LCD_DDRAM);
				}
				else
				{
					pos = (pos - LCD_LINE2) | 0x40;
				    LCD_4bit_command(pos | LCD_DDRAM);
				}
			}
		}
		DelayMs(1);
   }
}

/* START Function Description ***********************************************
SYNTAX:			voidLCD_4bit_puts(char *char_string);
PARAMETER1:		*char_string - Pointer to the beginning of a character string
KEYWORDS:		character, lcd, display, string
DESCRIPTION:	This function divides up a string of characters and sends them
DESCRIPTION:   to the LCD_putc function one by one
RETURN VALUE:	None.
END DESCRIPTION ************************************************************/
void  LCD_4bit_puts(char *char_string)
{
	while(*char_string)
	{
   		LCD_4bit_putc(*char_string++);
	}
}

