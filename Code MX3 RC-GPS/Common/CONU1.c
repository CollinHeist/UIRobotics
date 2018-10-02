/* 
** CONU1.c
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
** Modified for Cerebot 32MX7 - R. W. Wall,  October 2, 2011
** Uses PBDIV = 8, SYS_OSC = 80,000,000
** Revised:	11/25/2012
*/

#include <plib.h>
#include <stdio.h>

#include "Cerebot32mx7.h"
#include "conU1.h"

// timing and baud rate settings 
#define BRATE		19200      		// 19200 baud (BREGH=1)
#define U_ENABLE	0x8008			// enable the UART peripheral
#define U_TX		0x0400			// enable transmission

/* START FUNCTION DESCRIPTION ********************************************
SYNTAX:  		void initU1( void);
KEYWORDS:		UART, console, monitor
DESCRIPTION:	Initializes UART1 at 19200 BAUD
				8 data bite, No parity, 1 stop bit
PARAMETERS:		None
RETURN VALUE:   None
NOTES:			
END DESCRIPTION **********************************************************/
void initU1( void)
{
unsigned int BRG;

	BRG = (unsigned short)(( (float)FPB / ( (float)4 * (float) BRATE ) ) - (float)0.5);
	OpenUART1( UART_EN | UART_BRGH_FOUR, UART_RX_ENABLE | UART_TX_ENABLE , BRG );

	printf("\n\rPIC32 MX7 Serial Port 1 ready\n\r");
} // initU1

/* START FUNCTION DESCRIPTION ********************************************
SYNTAX:  		int putU1( int c);
KEYWORDS:		UART, console, character, send
DESCRIPTION:	Sends a single character to UART1
PARAMETERS:		Character to send
RETURN VALUE:   The character sent 
NOTES:			
END DESCRIPTION **********************************************************/
int putU1( int c)
{
	while ( U1STAbits.UTXBF);   // wait while Tx buffer full
	U1TXREG = c;
	return c;
} // putU1


/* START FUNCTION DESCRIPTION ********************************************
SYNTAX:  		char getU1( void);
KEYWORDS:		UART, console, character, receive
DESCRIPTION:	Waits for a single character to UART1
PARAMETERS:		None
RETURN VALUE:   Character received
NOTES:			This is a blocking function
END DESCRIPTION **********************************************************/
char getU1( void)
{
	while ( !U1STAbits.URXDA);	// wait for new char to arrive
	return U1RXREG;		// read the char from receive buffer
}// getU1

/* START FUNCTION DESCRIPTION ********************************************
SYNTAX:  		char getU1( void);
KEYWORDS:		UART, console, character, send, text, string
DESCRIPTION:	Send a null terminated string to the UART1 serial port
PARAMETERS:		pointer to string to send
RETURN VALUE:   Returns zero
NOTES:			This is a blocking function until all characters are sent.				
END DESCRIPTION **********************************************************/
int putsU1(char *s)
{
	while( *s)			// loop until *s == '\0' end of string
		putU1( *s++);	// send char and point to the next one
    putU1( '\r');       // terminate with a cr / line feed
    putU1( '\n');       // terminate with a cr / line feed
	return 0;
} // puts


/* START FUNCTION DESCRIPTION ********************************************
SYNTAX:  		char *getsn( char *s, int len);
KEYWORDS:		UART, console, character, receive, text, string
DESCRIPTION:	Receives a LF or CR terminated string from the 
				UART1 serial port. Backspace is destructive 
PARAMETERS:		pointer to string to send
RETURN VALUE:   Returns zero
NOTES:			This is a blocking function until all characters are sent.				
END DESCRIPTION **********************************************************/
char *getsn( char *s, int len)
{
char *p;
	p = s;            // copy the buffer pointer 
    do{
        *s = getU1();       // wait for a new character
        putU1( *s);         // echo character
        
        if (( *s==BACKSPACE)&&( s>p))
        {
            putU1( ' ');     // overwrite the last character
            putU1( BACKSPACE);
            len++;
            s--;            // back the pointer
            continue;
        }
        if ( *s=='\n')      // line feed, ignore it
            continue;
        if ( *s=='\r')      // end of line, end loop
            break;          
        s++;                // increment buffer pointer
        len--;
    } while ( len>1 );      // until buffer full
 
    *s = '\0';              // null terminate the string 
    
    return p;               // return buffer pointer
} // getsn
/* START FUNCTION DESCRIPTION ********************************************
_mon_putc
SYNTAX:  		void _mon_putc(char c);
KEYWORDS:		printf, console, monitor
DESCRIPTION:	Sets up serial port to function as console for printf.
				Used by system only.
PARAMETER 1:	Character to send to monitor
RETURN VALUE:   None
NOTES:			This function will block until space is available
				in the transmitt buffer	
END DESCRIPTION **********************************************************/
void _mon_putc(char c)
{
	while(U1STAbits.UTXBF);
	U1TXREG = c;
}	

/* START FUNCTION DESCRIPTION ********************************************
SYNTAX:  		int getUARTstr(char *s);
KEYWORDS:		UART, console, character, receive, text, string
DESCRIPTION:	Receives a LF or CR terminated string from the 
				UART1 serial port. Backspace is destructive 
PARAMETERS:		pointer to string to send
RETURN VALUE:   Returns 0 if line has not been terminated or the number
				of characters in the entered line
NOTES:			This is a nonblocking function. Maximum nuumber of 
				characters is 80.
END DESCRIPTION **********************************************************/
int getUARTstr(char *s)
{
#define STR_MAX_LEN	80
static int idx = 0;
static char str[STR_MAX_LEN +1];
int len;

	if( !U1STAbits.URXDA)	// Test for new char to arrive
	{
		return FALSE;
	}
	else
	{
   	    str[idx] = getU1();       // wait for a new character
       	putU1( str[idx]);         // echo character
        if (( *s==BACKSPACE) && ( idx > 0))
   	    {
       	    putU1( ' ');     // overwrite the last character
           	putU1( BACKSPACE);
            idx--;
       	    return FALSE;
        }
   	    else
		{	// Determine if this is the end of line entry
			if ( str[idx] =='\n' || str[idx] =='\r' || idx >= STR_MAX_LEN)     
			{
			    str[idx] = 0;              // null terminate the string 
				strcpy(s,str);
				len = idx;
				idx = 0;
				putU1('\n');
				putU1('\r');
				return len;
			}
       		else
			{
				idx++;                // increment buffer pointer
				return FALSE;
			}
		}
   	} 
}
    
