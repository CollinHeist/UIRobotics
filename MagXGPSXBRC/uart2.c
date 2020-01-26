// File Inclusion
#include "hardware.h"	// Has info regarding the PB clock
#include <plib.h>
#include <stdio.h>		// Required for printf 
#include "UART2.h"

void initializeUART2(unsigned int baud, int parity) {
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

	// UARTConfigure(UART2, UART_ENABLE_PINS_TX_RX_ONLY);
	// UARTSetFifoMode(UART2, UART_INTERRUPT_ON_TX_NOT_FULL | UART_INTERRUPT_ON_RX_NOT_EMPTY);
	// UARTSetLineControl(UART2, UART_DATA_SIZE_8_BITS | UART_PARITY_NONE | UART_STOP_BITS_1);
	// UARTSetDataRate(UART2, GetPeripheralClock(), urbg);
	// UARTEnable(UART2, UART_ENABLE_FLAGS(UART_PERIPHERAL | UART_RX | UART_TX));

	OpenUART2(config1, config2, ubrg);

	/*
	// Configure UART2 RX Interrupt
	INTClearFlag(INT_SOURCE_UART_TX(UART2));
	INTClearFlag(INT_SOURCE_UART_RX(UART2));
	INTEnable(INT_SOURCE_UART_RX(UART2), INT_ENABLED);
	INTSetVectorPriority(INT_VECTOR_UART(UART2), INT_PRIORITY_LEVEL_3);
	INTSetVectorSubPriority(INT_VECTOR_UART(UART2), INT_SUB_PRIORITY_LEVEL_0);
	 */
}

int putCharacterUART2(int ch) {
	char c = (char) ch;;
	int done = 0;

	// If a transmission can be written
	if (U2STAbits.TRMT) {
		WriteUART2(c);
		done = 1;
	}

	return done;
}

int putStringUART2(const char *s) {
	BOOL ch_sent;

	while(*s) {
	do {
		ch_sent = putCharacterUART2(*s);
	} while(ch_sent ==  0);
	s++;
	}
	do { ch_sent = putCharacterUART2('\r'); } while(!ch_sent);
	do { ch_sent = putCharacterUART2('\n'); } while(!ch_sent);

	return 1;
}

int getCharacterUART2(char *ch) {
	char c;
	int dr2, dr4;
	int done = 0;
	unsigned int lineStatus;

	lineStatus = UART2GetErrors();	// Check for UART errors
	if (lineStatus) {
	// Handle receiver error
	printf("Error: 0x%08x  %6d\n\r", lineStatus, lineStatus);

	UART2ClearAllErrors();
	getCharacterUART2(ch);		
	}
	else {	   
	dr2 = DataRdyUART2();
	if (dr2) {		// Wait for new char to arrive
		c = ReadUART2();	// Read the char from receive buffer
		*ch = c;
		done = 1;		// Return new data available flag 
	}
	}
	return done;					// Return new data not available flag
}

int getStringUART2(char *s, unsigned int len) {
	static int eol = 1;				// End of input string flag
	static unsigned int buf_len;	// Number of received characters
	static char *p1;				// copy #1 of the buffer pointer 
	static char *p2;				// copy #2 of the buffer pointer
	char ch;						// Received new character

	if (eol) {						// Make two copies of pointer - one for
	p1 = s;						// receiving characters and one for marking
	p2 = s;						// the starting address of the string.  The
	eol = 0;					// second copy is needed for backspacing.
	buf_len = len;				// Save maximum buffer length
	}

	if (!(getCharacterUART2(&ch))) {// Check for character received
	return 0;					// Bail out if not 
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
	return 1;					// Set EOL flag 
	}
	
	return 1;						// Not EOL
}

// UART 2 interrupt handler - This triggers when a character is sent or receieved over UART2
void __ISR(_UART2_VECTOR, IPL3SOFT) interruptUART2Handler(void) {
	// Is this an RX interrupt?
	if (INTGetFlag(INT_SOURCE_UART_RX(UART2))) {
		// Echo what we just received.
		HandleInput();

		// Clear the RX interrupt Flag
		INTClearFlag(INT_SOURCE_UART_RX(UART2));
	}

	// We don't care about TX interrupt
	if (INTGetFlag(INT_SOURCE_UART_TX(UART2))) {
		INTClearFlag(INT_SOURCE_UART_TX(UART2));
	}
}