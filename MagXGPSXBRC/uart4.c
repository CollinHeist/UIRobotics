/* ----------------------------------- File Inclusion ----------------------------------- */

#include <plib.h>
#include <stdio.h>
#include <GenericTypeDefs.h>

#include "uart4.h"
#include "hardware.h"
#include "Delays.h"

/* -------------------------- Global Variables and Structures --------------------------- */

/* ---------------------------------- Public Functions ---------------------------------- */

/*
 *	Summary
 *		Initialize UART4 to the desired baud and parity.
 *	Parameters
 *		baud[in]: Frequency the UART bus should operate at.
 *		parity[in]: What parity to operate the bus at.
 *	Returns
 *		Unsigned int that corresponds to whether an error occurred or not.
 */
unsigned int initializeUART4(unsigned int baud, int parity) {
	RPF12R = 0x02;	// Mapping U4TX to RPF12;
	U4RXR = 0x09;	// Mapping U4RX to RPF13

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
		default:
			return ERROR;
	}
	putStringUART4("\n\rUART Serial Port 4 ready\n\n\r");
	
	return NO_ERROR;
}

/*
 *	Summary
 *		System function override to enable printf() capability.
 *	Parameters
 *		c[in]: Character to be sent over the UART4 bus.
 *	Returns
 *		None.
 */
void _mon_putc(char c) {
	while(!UARTTransmitterIsReady(UART4));
	UARTSendDataByte(UART4, c);
}

/*
 *	Summary
 *		Place a given character over the UART4 bus.
 *	Parameters
 *		ch[in]: Character to be placed in the UART buffer.
 *	Returns
 *		Unsigned int that is either TRUE if a new character was sent, or FALSE if it wasnt.
 */
unsigned int putCharacterUART4(int ch) {
	UART_DATA c;
	unsigned int done = (unsigned int) FALSE;
	c.data8bit = (char) ch;
	if (UARTTransmitterIsReady(UART4)) {
		UARTSendDataByte(UART4, c.data8bit);
		done = TRUE;
	}

	return done;
}

/*
 *	Summary
 *		Receive the latest character on the UART4 buffer.
 *	Parameters
 *		ch[out]: Character pointer to be filled with the contents of the UART4 buffer.
 *	Returns
 *		Unsigned int that is either TRUE if a new character was received, or FALSE if none was.
 */
unsigned int getCharacterUART4(char *ch) {
	UART_DATA c;
	unsigned int done = (unsigned int) FALSE;
	if (UARTReceivedDataIsAvailable(UART4)) {	// wait for new char to arrive
		c = UARTGetData(UART4);		// read the char from receive buffer
		*ch = (c.data8bit);
		done = (unsigned int) TRUE;	// Return new data available flag
	}
	
	return (unsigned int) done;		// Return new data not available flag
}

/*
 *	Summary
 *		Place a given string on the UART4 buffer.
 *	Parameters
 *		s[in]: Character pointer to the string that should be placed on the buffer.
 *		This string must be null-terminated.
 *	Returns
 *		Unsigned int that is NO_ERROR if the string was successfully sent within
 *		UART4_MAX_SEND_TIME_MS (see header), and ERROR otherwise.
 */
unsigned int putStringUART4(const char *s) {
	// Begin the non-blocking delay and try and send the string for as long as possible
	nonBlockingDelayMS(UART4_MAX_SEND_TIME_MS);
	while (*s) {
		putCharacterUART4(*s);
		s++;
	}

	return NO_ERROR;
}

/*
 *	Summary
 *		Get a string of a given length on the UART4 buffer.
 *	Parameters
 *		s[out]: Character pointer to the string that should be filled by this function.
 *		len[in]: Unsigned integer that is how many bytes to read from the buffer.
 *	Returns
 *		Integer that is TRUE if a return character was received, FALSE if it is still
 *		waiting for the end of line.
 */
int getStringUART4(char *s, unsigned int len) {
	static int eol = TRUE;			// End of input string flag
	static unsigned int buf_len;	// Number of received characters
	static char *p1;				// copy #1 of the buffer pointer 
	static char *p2;				// copy #2 of the buffer pointer
	char ch;						// Received new character

	if (eol) {						// Make two copies of pointer - one for
		p1 = s;						// receiving characters and one for marking
		p2 = s;						// the starting address of the string.  The
		eol = FALSE;				// second copy is needed for backspacing.
		buf_len = len;				// Save maximum buffer length
	}

	if (!(getCharacterUART4(&ch))) {// Check for character received
		return FALSE;				// Bail out if not 
	}
	else {
		*p1 = ch;					// Save new character in string buffer
		putCharacterUART4( *p1);	// echo character
		switch (ch)	{				// Test for control characters
			case BACKSPACE:
				if (p1 > p2) {
					putCharacterUART4(' ');	// overwrite the last character
					putCharacterUART4(BACKSPACE);
					buf_len++;
					p1--;			// back off the pointer
				}
				break;
			case '\r':				// end of line, end loop
				putCharacterUART4( '\n');	// add line feed
				eol = TRUE;			// Mark end of line
				break;
			case '\n':				// line feed, ignore it
				break;
			default:
				p1++;				// increment buffer pointer
				buf_len--;			// decrement length counter
		}
	}

	if (buf_len == 0 || eol) {		// Check for buffer full or end of line
		*p1 = '\0';					// add null terminate the string
		return TRUE;				// Set EOL flag 
	}

	return FALSE;					// Not EOL
}

/* --------------------------------- Private Functions ---------------------------------- */

/* ----------------------------- Interrupt Service Routines ----------------------------- */