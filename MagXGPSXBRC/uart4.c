/* ************************************************************************** */
/** Descriptive File Name: comm_lib.c - UART Serial communications. 

  @ Author  
	 Richard Wall
  
  @ Date: 
	 Created:	June 17, 2016 for Basys MX3
	 Verified:	 May 18, 2017

  @Company
	Digilent Inc

  @File Name
	comm_lib.c

  @Development Environment
	MPLAB X IDE x3.61 - http://www.microchip.com/mplab/mplab-x-ide 
	XC32 1.43 - http://www.microchip.com/mplab/compilers
	PLIB 3/7/20162 - http://www.microchip.com/SWLibraryWeb/product.aspx?product=PIC32%20Peripheral%20Library

  @Summary
	UART 4 initialization and character and string I/O.

  @Description
	UART initialization can be set for any BAUD rate using EVEN, ODD, or NO 
	parity. Character and string input functions are non blocking functions.
	The serial interface uses UART channel 4.

  @Remarks
	This code also uses the "printf" function on UART Serial Port 4

**************************************************************************** */

#include "hardware.h"	// Has info regarding the PB clock
#include <plib.h>
#include <stdio.h>	  // Required for printf 
#include "uart4.h"

/* uart4_init FUNCTION DESCRIPTION *************************************
 @ SYNTAX: void uart1_init(unsigned int baud, int parity);

 @Summary
	UART 4 initialization and character and string I/O.

 @Description
	UART initialization can be set for any BAUD rate using EVEN, ODD, or NO 
	parity. Character and string input functions are non blocking functions.
	The serial interface uses UART channel 4.

 @ PARAMETERS
	@param1:		integer Baud rate
	@param2:		integer (parity, NO_PARITY, ODD_PARITY, or EVEN_PARITY)
 @RETURN VALUE:	 None

  @Remarks
	This code also uses the "printf" function on UART Serial Port 4
	9 bit mode MARK or SPACE parity is not supported

 
 * END DESCRIPTION **********************************************************/
void uart4_init(unsigned int baud, int parity) {
	RPF12R = 0x02;  // Mapping U4TX to RPF12;
	U4RXR = 0x09;   // Mapping U4RX to RPF13

	UARTConfigure(UART4, UART_ENABLE_PINS_TX_RX_ONLY );
	UARTSetDataRate(UART4, GetPeripheralClock(), baud);
	
	// Note the need to specify the UART number twice in the following statement	
	UARTEnable(UART4, UART_ENABLE_FLAGS(UART_ENABLE | UART4 | UART_RX | UART_TX));  
	
	switch (parity) {
		case NO_PARITY:
			UARTSetLineControl(UART4, UART_DATA_SIZE_8_BITS | UART_PARITY_NONE | UART_STOP_BITS_1);
			break;
		case ODD_PARITY:
			UARTSetLineControl(UART4, UART_DATA_SIZE_8_BITS | UART_PARITY_ODD | UART_STOP_BITS_1);
			break;
		case EVEN_PARITY:
			UARTSetLineControl(UART4, UART_DATA_SIZE_8_BITS | UART_PARITY_EVEN | UART_STOP_BITS_1);
			break;
	}
	printf("\n\rUART Serial Port 4 ready\n\n\r");
}

/* _mon_putc FUNCTION DESCRIPTION ******************************************
 @SYNTAX:		   void _mon_putc(char c);
 
 @KEYWORDS:		 printf, console, monitor
 
 @DESCRIPTION:	  Sets up serial port to function as console for printf.
					Used only by system.
 
 @PARAMETERS 
	@param1:		Character to send to monitor

 @RETURN VALUE:	 None
 @REMARKS:		  This function will block until space is available
					in the transmit buffer. Called by system to implement 
					"printf" functions
 @EXAMPLE
   None - used by system only
 * END DESCRIPTION **********************************************************/
void _mon_putc(char c) {
	while(!UARTTransmitterIsReady(UART4));
	UARTSendDataByte(UART4, c);
}

/* putcU4 FUNCTION DESCRIPTION ********************************************
 @SYNTAX:		   BOOL putU4( int c);
  
 @KEYWORDS:		 UART, character
  
 @DESCRIPTION:	  Send single character to UART4. Waits while UART4 is busy 
					(buffer full) and then sends a single byte to UART1
  
 @PARAMETER
	@param1:		character to send
 
 @RETURN VALUE:	 TRUE = new character sent
					FALSE = character not sent
 
 @REMARKS:		  This function will not block if space is not available
					in the transmit buffer
 @EXAMPLE
   @code
		BOOL result;
		char ch;
		result = putcU4(ch);
 
 * END DESCRIPTION **********************************************************/
BOOL putcU4(int ch) {
	UART_DATA c;
	BOOL done = FALSE;
	c.data8bit = (char) ch;
	if(UARTTransmitterIsReady(UART4)) {
		UARTSendDataByte(UART4, c.data8bit);
		done = TRUE;
	}

	return done;
}

int putcIU4(int ch) {
	UART_DATA c;
	BOOL done = FALSE;
	c.data8bit = (char) ch;
	if(UARTTransmitterIsReady(UART4)) {
		UARTSendDataByte(UART4, c.data8bit);
		done = TRUE;
	}

	return done;
}

/* getU4 FUNCTION DESCRIPTION ********************************************
 @SYNTAX:		   BOOL getcU4( char *ch);

 @KEYWORDS:		 character, get

 @DESCRIPTION:	  Checks for a new character to arrive to the UART4 serial
					port.
 @PARAMETER
	@param1:		character pointer to character variable

 @RETURN VALUE:	 TRUE = new character received
					FALSE = No new character

 @REMARKS:		  This function does not block for no character received
 
 @EXAMPLE
   @code
		BOOL result;
		char ch;
		result = getcU4(&ch);
 
 * END DESCRIPTION ********************************************************/
BOOL getcU4(char *ch) {
	UART_DATA c;
	BOOL done = FALSE;
	if (UARTReceivedDataIsAvailable(UART4))	// wait for new char to arrive
	{
		c = UARTGetData(UART4);	// read the char from receive buffer
		*ch = (c.data8bit);
		done = TRUE;		// Return new data available flag
	}
	return done;			// Return new data not available flag
}

int getcIU4( char *ch) {
	UART_DATA c;
	BOOL done = FALSE;
	if(UARTReceivedDataIsAvailable(UART4))	// wait for new char to arrive
	{
		c = UARTGetData(UART4);	// read the char from receive buffer
		*ch = (c.data8bit);
		done = TRUE;		// Return new data available flag
	}
	return done;			// Return new data not available flag
}

/* putsU4 FUNCTION DESCRIPTION ********************************************
 @SYNTAX:		  int putsU4( const char *s);

 @KEYWORDS:		UART, string

 @DESCRIPTION:	 Sends a NULL terminates text string to UART4 with
				   CR and LF appended
 @PARAMETER
	@param1:	   pointer to text string
 
 @RETURN VALUE:	Logical TRUE

 @REMARKS:		 This function will block until space is available
 *				  in the transmit buffer
 * END DESCRIPTION **********************************************************/
int putsU4( const char *s) {
	BOOL ch_sent;
	while(*s) {
		do {
			ch_sent = putcU4(*s++);
		} while(ch_sent ==  FALSE);
	}
	do { ch_sent = putcU4( '\r'); } while(!ch_sent);
	do { ch_sent = putcU4( '\n'); } while(!ch_sent);

	return 1;
}

/* getstrU4 FUNCTION DESCRIPTION ********************************************
 @SYNTAX:		   int getstrU4( char *s, unsigned int len );
 @KEYWORDS:		 string, get, UART
 @DESCRIPTION:	  This function assembles a line of text until the number of
					characters assembled exceed the buffer length or an ASCII
					CR control character is received.  This function echo each
					received character back to the UART. It also implements a
					destructive backspace. ASCII LF control characters are
					filtered out.  The returned string has the CR character
					removed and a NULL character appended to terminate the text
					string.
 @PARAMETERS
	@param1:		character pointer to string
	@param1:		integer maximum string length

 @RETURN VALUE:	 TRUE = EOL signals received return character
					FALSE = waiting for end of line
 
 @REMARKS:		  It is presumed that the buffer pointer or the buffer length
					does not change after the initial call asking to
					receive a new line of text.  This function does not block
					for no character received. A timeout can be added to this
					to free up resource. There is no way to restart the function
					after the first call until a EOL has been received.  Hence
					this function has denial of service security risks.
 @EXAMPLE
   @code
		int result, len;
		char str[];
		result = getstrU4(str, len);
 
* END DESCRIPTION ************************************************************/
int getstrU4(char *s, unsigned int len) {
	static int eol = TRUE;			// End of input string flag
	static unsigned int buf_len;	// Number of received characters
	static char *p1;				// copy #1 of the buffer pointer 
	static char *p2;				// copy #2 of the buffer pointer
	char ch;						// Received new character

	if(eol)							// Initial call to function - new line
	{								// Make two copies of pointer - one for
		p1 = s;						// receiving characters and one for marking
		p2 = s;						// the starting address of the string.  The
		eol = FALSE;				// second copy is needed for backspacing.
		buf_len = len;				// Save maximum buffer length
	}

	if(!(getcU4(&ch)))				// Check for character received
	{
		return FALSE;				// Bail out if not 
	}
	else
	{
		*p1 = ch;					// Save new character in string buffer
		putcU4( *p1);				// echo character
		switch(ch)					// Test for control characters
		{
			case BACKSPACE:
				if ( p1>p2)
				{
					putcU4( ' ');	// overwrite the last character
					putcU4( BACKSPACE);
					buf_len++;
					p1--;			// back off the pointer
				}
				break;
			case '\r':				// end of line, end loop
				putcU4( '\n');		// add line feed
				eol = TRUE;			// Mark end of line
				break;
			case '\n':				// line feed, ignore it
				break;
			default:
				p1++;				// increment buffer pointer
				buf_len--;			// decrement length counter
		}
	}
	if( buf_len == 0 || eol)		// Check for buffer full or end of line
	{
		*p1 = '\0';					// add null terminate the string
		return TRUE;				// Set EOL flag 
	}
	return FALSE;					// Not EOL
}