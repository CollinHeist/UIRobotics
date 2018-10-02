/* 
** LCDlib.c
**
** Professor Richard Wall
** Department of Electrical and Computer Engineering
** University of Idaho
** Moscow, ID 83844-1023
** rwall@uidaho.edu
**
** Date: August 18, 2012
**
** This version of the LCD control allows for a bit-bang 
** 4 data bit interface or a 8 data bit interface that uses 
** the PMP bus
*/

// System included files
#include <plib.h>

// Platform included files
#include "Cerebot32mx7.h"

// Application included files
#include "sw_timer.h"		// Required for Delayms function
#include "LCDlib.h"

#define PMDATA  PMDIN

void initLCD( void)
{

#ifdef LCD_4BIT_4LINE
	LCD_4bit_init();
#else

    // PMP initialization 
    mPMPOpen( PMP_ON | PMP_READ_WRITE_EN | 3,
              PMP_DATA_BUS_8 | PMP_MODE_MASTER1 | 
              PMP_WAIT_BEG_4 | PMP_WAIT_MID_15 | 
              PMP_WAIT_END_4,
              0x0001,           // only PMA0 enabled 
              PMP_INT_OFF);     // no interrupts used       
        
    // wait for >30ms
    DelayMs( 30);
    
    //initiate the HD44780 display 8-bit init sequence
    PMPSetAddress( LCDCMD);     // select command register
    PMPMasterWrite( 0x38);      // 8-bit int, 2 lines, 5x7
    DelayMs( 1);                // > 48 us 
    
    PMPMasterWrite( 0x0c);      // ON, no cursor, no blink 
    DelayMs( 1);                // > 48 us 
    
    PMPMasterWrite( 0x01);      // clear display
    DelayMs( 2);                // > 1.6ms   
    
    PMPMasterWrite( 0x06);      // increment cursor, no shift
    DelayMs( 2);                // > 1.6ms   
	putsLCD("Exploring the \tUI Cerebot 32MX7");
#endif
} // initLCD


char readLCD( int addr)
{
#ifdef LCD_4BIT_4LINE
	return LCD_4bit_get_status();
#else
    PMPSetAddress( addr);       // select register
    mPMPMasterReadByte();       // initiate read sequence
    return mPMPMasterReadByte();// read actual data
#endif
} // readLCD


void writeLCD( int addr, char c)    
{
#ifdef LCD_4BIT_4LINE
	if(addr)
		LCD_4bit_putc(c);
	else
	{
		LCD_4bit_command(c);
		if(c == 1 || c==2)
			DelayMs(2);
	}	
#else
    while( busyLCD());
    PMPSetAddress( addr);       // select register
    PMPMasterWrite( c);         // initiate write sequence
#endif
} // writeLCD
    

void putsLCD( char *s)
{  
#ifdef LCD_4BIT_4LINE
	LCD_4bit_puts(s);
#else
    char c;
    
    while( *s) 
    {
        switch (*s)
        {
        case '\n':          // point to second line
            setLCDC( 0x40);
            break;
        case '\r':          // home, point to first line
            setLCDC( 0);
            break;
        case '\t':          // advance next tab (8) positions
            c = addrLCD();
            while( c & 7)
            {
                putLCD( ' '); 
                c++;
            }
            if ( c > 15)   // if necessary move to second line
                setLCDC( 0x40);
            break;                
        default:            // print character
            putLCD( *s);
            break;
        } //switch
        s++;
    } //while 
#endif
} //putsLCD



void posLCD(int pos)
{	
#ifdef LCD_4BIT_4LINE
	LCD_4bit_goto_pos( pos);
#else
   if(pos < HLCD)
		writeLCD( LCDCMD, pos|0x80);
	else
		writeLCD( LCDCMD, (pos-HLCD)|0xC0);	
#endif
}


