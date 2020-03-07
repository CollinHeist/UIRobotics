/* ----------------------------------- File Inclusion ----------------------------------- */

#include <plib.h>

#include "hardware.h"	// Has info regarding the PB clock
#include "uart2.h"
#include "uart4.h"

/* -------------------------- Global Variables and Structures --------------------------- */

int DmaIntFlag;										// Flag used in interrupts
char DMABuffer[DMA_BUFFER_SIZE+1];					// Master DMA UART RX buffer
static char privateDMABuffer[DMA_BUFFER_SIZE+1];	// Private, temporary DMA buffer
static DmaChannel DMAChannel = DMA_CHANNEL1;		// Active DMA channel

/* ---------------------------------- Public Functions ---------------------------------- */

/*
 *	Summary
 *		Initialize UART2 to the desired baud and parity.
 *	Parameters
 *		baud[in]:   Frequency the UART bus should operate at.
 *		parity[in]: What parity to operate the bus at.
 *	Returns
 *		Unsigned int that corresponds to whether an error occurred or not.
 */
unsigned int initializeUART2(const unsigned int baud, const unsigned int parity) {
	// By default, operate in NO_PARITY mode
	unsigned int config1 = UART_EN | UART_RX_TX | UART_BRGH_SIXTEEN | UART_NO_PAR_8BIT;

	RPG7R = 0x01;  // Mapping U2TX to RPG7
	U2RXR = 0x01;  // Mapping U2RX to RPB5

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
	
	unsigned int brg = (unsigned int) (((float) GetPeripheralClock() / ((float) 4 * (float) baud))- (float) 0.5);

	OpenUART2(UART_EN | UART_BRGH_FOUR | UART_NO_PAR_8BIT, UART_RX_ENABLE | UART_TX_ENABLE, brg);
	mU2RXIntEnable(1);
	mU2SetIntPriority(3);
	mU2SetIntSubPriority(1);

	putStringUART4("\n\rXBee online\n\r");
	
	return initializeDMAUART2RX();
}

/*
 *	Summary
 *		Place a character on the UART2 bus.
 *	Parameters
 *		c[in]: Character to write to the bus.
 *	Returns
 *		Unsigned integer that is either 0 if the transmission failed, and 1 if successful.
 */
unsigned int putCharacterUART2(const char c) {
	if (U2STAbits.TRMT) {	// If a transmission can be written
		WriteUART2(c);
		
		return 1;
	}

	return 0;
}

/*
 *	Summary
 *		Place a string on the UART2 bus.
 *	Parameters
 *		s[in]: Character pointer that points to the string that will be written over UART2.
 *	Returns
 *		None.
 */
void putStringUART2(const char *s) {
	unsigned int ch_sent;

	while (*s) {
		do {
			ch_sent = putCharacterUART2(*s);
		} while(ch_sent == 0);
		s++;
	}
	do { ch_sent = putCharacterUART2('\r'); } while(!ch_sent);
	do { ch_sent = putCharacterUART2('\n'); } while(!ch_sent);
}

/*
 *	Summary
 *		Receive the latest character on the UART2 buffer.
 *	Parameters
 *		ch[out]: Character pointer to be filled with the contents of the UART4 buffer.
 *	Returns
 *		Unsigned int that is either TRUE if a new character was received, or FALSE if none was.
 */
unsigned int getCharacterUART2(char *ch) {
	if (UART2GetErrors()) {
		// Handle receiver error
		putStringUART2("Error on UART2.");

		UART2ClearAllErrors();
		getCharacterUART2(ch);	// Try again
	}
	else {	   
		if (DataRdyUART2()) {	// Wait for new char to arrive
			*ch = ReadUART2();
			
			return UART2_NEW_DATA_AVAILABLE;
		}
	}
	
	return UART2_NO_NEW_DATA_AVAILABLE;
}

/*
 *	Summary
 *		Get a string of a given length on the UART2 buffer.
 *	Parameters
 *		s[out]:		Character pointer to the string that should be filled by this function.
 *		length[in]: Unsigned integer that is how many bytes to read from the buffer.
 *	Returns
 *		Unsigned integer that is TRUE if a return character was received, FALSE if
 *		it is still waiting for the end of line.
 */
unsigned int getStringUART2(char *s, const unsigned int length) {
	static int eol = 1;				// End of input string flag
	static unsigned int buf_len;	// Number of received characters
	static char *p1;				// copy #1 of the buffer pointer 
	static char *p2;				// copy #2 of the buffer pointer
	char ch;						// Received new character

	if (eol) {						// Make two copies of pointer - one for
		p1 = s;						// receiving characters and one for marking
		p2 = s;						// the starting address of the string.  The
		eol = 0;					// second copy is needed for backspacing.
		buf_len = length;			// Save maximum buffer length
	}

	if (getCharacterUART2(&ch) == UART2_NO_NEW_DATA_AVAILABLE) {
		return UART2_STILL_PARSING;
	}
	else {
		*p1 = ch;					// Save new character in string buffer
		switch (ch) {				// Test for control characters
			case BACKSPACE:
				if (p1 > p2) {
					putCharacterUART2(' ');	// overwrite the last character
					putCharacterUART2(BACKSPACE);
					buf_len++;
					p1--;			// back off the pointer
				}
				break;
			case '\r':				// end of line, end loop
				eol = 1;			// Mark end of line
				break;
			case '\n':				// line feed, ignore it
				break;
			default:
				p1++;				// increment buffer pointer
				buf_len--;		 	// decrement length counter
		}
	}

	if (buf_len == 0 || eol) {		// Check for buffer full or end of line
		*p1 = '\0';					// add null terminate the string
		
		return UART2_DONE_PARSING;
	}

	return UART2_STILL_PARSING;
}

/*
 *	Summary
 *		Restart the DMA transfer from between the U2RX register and privateDMABuffer.
 *	Parameters
 *		None.
 *	Returns
 *		None.
 */
void restartDMATransfer(void) {
	DmaChnSetTxfer(DMAChannel, (void*)&U2RXREG, privateDMABuffer, 1, DMA_BUFFER_SIZE, 1);
	DmaChnEnable(DMAChannel);
}

/* --------------------------------- Private Functions ---------------------------------- */

/*
 *	Summary
 *		Initialize the DMA to work for the UART2 RX channel.
 *  Parameters
 *		None.
 *	Returns
 *		Unsigned integer that corresponds to whether or not the initialization was successful.
 *	Notes
 *		The DMA stores UART2 messages in privateDMABuffer, and completed messages are transfered to DMABuffer
 */
static unsigned int initializeDMAUART2RX(void) {
	INTClearFlag(INT_SOURCE_DMA(DMAChannel));	// Clear any standing DNA interrupts
	DmaChnOpen(DMAChannel, DMA_CHN_PRI2, DMA_OPEN_MATCH);
	DmaChnSetMatchPattern(DMAChannel, 0);	// set null as ending character
	// set the events: we want the UART2 rx interrupt to start our transfer
	// also we want to enable the pattern match: transfer stops upon detection of CR
	DmaChnSetEventControl(DMAChannel, DMA_EV_START_IRQ_EN|DMA_EV_MATCH_EN|DMA_EV_START_IRQ(_UART2_RX_IRQ));
	DmaChnSetTxfer(DMAChannel, (void*)&U2RXREG, privateDMABuffer, 1, DMA_BUFFER_SIZE, 1);

	// Enable the transfer done interrupt on pattern match for all the characters transferred
	DmaChnSetEvEnableFlags(DMAChannel, DMA_EV_BLOCK_DONE);	
	
	INTEnable(INT_SOURCE_DMA(DMAChannel), INT_ENABLED);
	INTSetVectorPriority(INT_VECTOR_DMA(DMAChannel), INT_PRIORITY_LEVEL_5);			// Set INT controller priority
	INTSetVectorSubPriority(INT_VECTOR_DMA(DMAChannel), INT_SUB_PRIORITY_LEVEL_0);	// Set INT controller sub-priority
	INTEnable(INT_SOURCE_DMA(DMAChannel), INT_ENABLED);		

	// Enable the DMA channel now that it's been configured
	DmaChnEnable(DMAChannel);
	
	// Make sure the DMA is valid right-away.
	restartDMATransfer();

	return NO_ERROR;
}

/* ----------------------------- Interrupt Service Routines ----------------------------- */

/*
 *	Summary
 *		Interrupt service routine for the UART2 vector. Handled when character is sent
 *		or received via the UART2 buffer. Calls according functions.
 *	Parameters
 *		None.
 *	Returns
 *		None.
 */
void __ISR(_UART2_VECTOR, IPL3SOFT) isrUART2Handler(void) {
	// Is this an RX interrupt?
	if (INTGetFlag(INT_SOURCE_UART_RX(UART2))) {
		HandleInput();
	}

	mU2ClearAllIntFlags();	// Clear all UART2 flags
}

/*
 *	Summary
 *		ISR for all DMA channel events.
 *	Parameters
 *		None.
 *	Returns
 *		None.
 */
void __ISR(_DMA1_VECTOR, IPL5SOFT) isrDMAHandler(void) {
	if (DmaChnGetEvFlags(DMAChannel) & DMA_EV_ALL_EVNTS) {
		DmaIntFlag = 1;
		strcpy(DMABuffer, privateDMABuffer);	// Change buffers
	}

	INTClearFlag(INT_SOURCE_DMA(DMAChannel));
}