/* ************************************************************************** */
/** Descriptive File Name: comm_lib.c - UART Serial communications. 

  @ Date: 
	 Created:   February 2, 2018
  
  @Summary
	UART 4 initialization and character and string I/O.

  @Description
	UART initialization can be set for any BAUD rate using EVEN, ODD, or NO 
	parity. Character and string input functions are non blocking functions.
	The serial interface uses UART channel 3.

  @Remarks
	This code also uses the "printf" function on UART Serial Port 4

**************************************************************************** */

// File Inclusion
#include "hardware.h"	// Has info regarding the PB clock
#include <plib.h>
#include <stdio.h>		// Required for printf 
#include "uart2.h"

/* uart2_init FUNCTION DESCRIPTION *************************************
 @ SYNTAX: void uart1_init(unsigned int baud, int parity);

 @Summary
	UART 2 initialization and character and string I/O.

 @Description
	UART initialization can be set for any BAUD rate using EVEN, ODD, or NO 
	parity. Character and string input functions are non blocking functions.
	The serial interface uses UART channel 3.

 @ PARAMETERS
	@param1:		integer Baud rate
	@param2:		integer (parity, NO_PARITY, ODD_PARITY, or EVEN_PARITY)
 @RETURN VALUE:	 None

  @Remarks
	9 bit mode MARK or SPACE parity is not supported

 * END DESCRIPTION **********************************************************/
void uart2_init(unsigned int baud, int parity) {
	unsigned int config1, config2, ubrg;
	
	RPG7R = 0x01;  // Mapping U2TX to RPG7
	U2RXR = 0x01;  // Mapping U2RX to RPG8
	 
	switch (parity) {
		case NO_PARITY:
			config1 = UART_EN | UART_RX_TX | UART_BRGH_SIXTEEN | UART_NO_PAR_8BIT;					
			break;
		case ODD_PARITY:
			config1 = UART_EN | UART_RX_TX | UART_BRGH_SIXTEEN | UART_EVEN_PAR_8BIT;					
			break;
		case EVEN_PARITY:
			config1 = UART_EN | UART_RX_TX | UART_BRGH_SIXTEEN | UART_ODD_PAR_8BIT;					
			break;
	}
	config2 = UART_TX_PIN_LOW |  UART_RX_ENABLE |  UART_TX_ENABLE;
	ubrg = GetPeripheralClock() / (baud * 16);
	
	UARTConfigure(UART2, UART_ENABLE_PINS_TX_RX_ONLY);
	UARTSetFifoMode(UART2, UART_INTERRUPT_ON_TX_NOT_FULL | UART_INTERRUPT_ON_RX_NOT_EMPTY);
	UARTSetLineControl(UART2, UART_DATA_SIZE_8_BITS | UART_PARITY_NONE | UART_STOP_BITS_1);
	UARTSetDataRate(UART2, GetPeripheralClock(), 9600);
	UARTEnable(UART2, UART_ENABLE_FLAGS(UART_PERIPHERAL | UART_RX | UART_TX));
	
	//OpenUART2(config1, config2, ubrg);
	printf("UART Serial Port 2 ready\n\n\r");
	
	/*
	// Configure UART2 RX Interrupt
	INTClearFlag(INT_SOURCE_UART_TX(UART2));
	INTClearFlag(INT_SOURCE_UART_RX(UART2));
	INTEnable(INT_SOURCE_UART_RX(UART2), INT_ENABLED);
	INTSetVectorPriority(INT_VECTOR_UART(UART2), INT_PRIORITY_LEVEL_3);
	INTSetVectorSubPriority(INT_VECTOR_UART(UART2), INT_SUB_PRIORITY_LEVEL_0);
	 */
}

/* putcU3 FUNCTION DESCRIPTION ********************************************
 @SYNTAX:		   BOOL putU2( int c);
  
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
		result = putcU3(ch);
 
 * END DESCRIPTION **********************************************************/
int putcU2(int ch) {
	char c = (char) ch;;
	int done = 0;

	// If a transmission can be written
	if (U2STAbits.TRMT) {
		WriteUART2(c);
		done = 1;
	}

	return done;
}

/* putsU3 FUNCTION DESCRIPTION ********************************************
 @SYNTAX:		  int putsU2( const char *s);

 @KEYWORDS:		UART, string

 @DESCRIPTION:	 Sends a NULL terminates text string to UART2 with
				   CR and LF appended
 @PARAMETER
	@param1:	   pointer to text string
 
 @RETURN VALUE:	Logical TRUE

 @REMARKS:		 This function will block until space is available
 *				 in the transmit buffer
 * END DESCRIPTION **********************************************************/
int putsU2(const char *s) {
	BOOL ch_sent;

	while(*s) {
		do {
			ch_sent = putcU2(*s);
		} while(ch_sent ==  0);
		s++;
	}
	do { ch_sent = putcU2( '\r'); } while(!ch_sent);
	do { ch_sent = putcU2( '\n'); } while(!ch_sent);

	return 1;
}

/* getcU2 FUNCTION DESCRIPTION ********************************************
 @SYNTAX:		   int getcU2( char *ch);

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
		result = getcU3(&ch);
 
 * END DESCRIPTION ********************************************************/
int getcU2( char *ch) {
	char c;
	int dr2, dr4;
	int done = 0;
	unsigned int lineStatus;

	lineStatus = UART2GetErrors();  // Check for UART errors
	if (lineStatus) {
		// Handle receiver error
		printf("Error: 0x%08x  %6d\n\r", lineStatus, lineStatus);
		
		UART2ClearAllErrors();
		getcUART2();		
	}
	else {	   
		dr2 = DataRdyUART2();
		if(dr2)	// wait for new char to arrive
		{
			c = getcUART2();	// read the char from receive buffer
			*ch = c;
			done = 1;		   // Return new data available flag 
		}
	}
	return done;				// Return new data not available flag
}/* End of getU3 */

/* getstrU3 FUNCTION DESCRIPTION ********************************************
 @SYNTAX:		   int getstrU2( char *s, unsigned int len );
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
		result = getstrU2(str, len);
 
* END DESCRIPTION ************************************************************/
int getstrU2( char *s, unsigned int len )
{
static int eol = 1;			  // End of input string flag
static unsigned int buf_len;		// Number of received characters
static char *p1;					// copy #1 of the buffer pointer 
static char *p2;					// copy #2 of the buffer pointer
char ch;							// Received new character

	if(eol)						 // Initial call to function - new line
	{							   // Make two copies of pointer - one for
		p1 = s;					 // receiving characters and one for marking
		p2 = s;					 // the starting address of the string.  The
		eol = 0;					// second copy is needed for backspacing.
		buf_len = len;			  // Save maximum buffer length
	}

	if(!(getcU2(&ch)))			  //  Check for character received
	{
		return 0;			   // Bail out if not 
	}
	else
	{
		*p1 = ch;				   // Save new character in string buffer
//		putcU4( *p1);			   // echo character
		switch(ch)				  // Test for control characters
		{
			case BACKSPACE:
				if ( p1>p2)
				{
					putcU2( ' ');   // overwrite the last character
					putcU2( BACKSPACE);
					buf_len++;
					p1--;		   // back off the pointer
				}
				break;
			case '\r':			  // end of line, end loop
//				putcU4( '\n');	  // add line feed
				eol = 1;			// Mark end of line
				break;
			case '\n':			  // line feed, ignore it
				break;
			default:
				p1++;			   // increment buffer pointer
				buf_len--;		  // decrement length counter
		}
	}
	if( buf_len == 0 || eol)		// Check for buffer full or end of line
	{
		*p1 = '\0';				 // add null terminate the string
		return 1;				   // Set EOL flag 
	}
	return 1;					   // Not EOL
} // End of getstrU2

// UART 2 interrupt handler - This triggers when a character is sent or receieved over UART2
void __ISR(_UART2_VECTOR, IPL3SOFT) IntUart2Handler(void) {
	char ch;
	// Is this an RX interrupt?
	if (INTGetFlag(INT_SOURCE_UART_RX(UART2))) {
		// Echo what we just received.
		handleInput();
		
		// Clear the RX interrupt Flag
		INTClearFlag(INT_SOURCE_UART_RX(UART2));

		// Toggle LED to indicate UART activity
		mPORTAToggleBits(BIT_7);

	}

	// We don't care about TX interrupt
	if (INTGetFlag(INT_SOURCE_UART_TX(UART2))) {
		INTClearFlag(INT_SOURCE_UART_TX(UART2));
	}
}