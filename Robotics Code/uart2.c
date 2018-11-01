#include "uart2.h"

/* ------------------------------ uart2_init ---------------------------------
  @ Description
	 Initializes UART2 comm port for the specified BAUD rate using the
	 assigned parity. Call at the beginning of the project.
  @ Parameters
	 @ param1 : Integer BAUD rate to open the UART2 port to
	 @ param2 : Which parity for the UART2 port (NO_PARITY, ODD_PARITY, EVEN_PARITY)
  @ Returns
	 None
  ---------------------------------------------------------------------------- */
void uart2_init(unsigned int baud, int parity) {
	unsigned int config1, config2, ubrg;
	
	RPG7R = 0x01;   // Mapping U2TX to RPG7
	U2RXR = 0x01;   // Mapping U2RX to RPG8

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
	
	config2 = UART_TX_PIN_LOW | UART_RX_ENABLE | UART_TX_ENABLE;
	ubrg = GetPeripheralClock() / (baud * 16);
	
	OpenUART2(config1, config2, ubrg);
	printf("UART Serial Port 2 Ready\n\n\r");
}

/* -------------------------------- putcU2 -----------------------------------
  @ Description
	 Waits while UART2 is busy (meaning the buffer is full) and then sends a
	 single byte to UART2. This function is non-blocking if there is no
	 available space in the transmit buffer.
  @ Parameters
	 @ param1 : The integer representation of the character to be put on UART
  @ Returns
	 1 : The passed character is sent
	 0 : The passed character is not sent 
  ---------------------------------------------------------------------------- */
int putcU2(int ch) {
	char c = (char) ch;
	int done = 0;
	
	if (U2STAbits.TRMT) {
		WriteUART2(c);
		done = 1;
	}
	
	return done;
}

/* -------------------------------- putsU2 -----------------------------------
  @ Description
	 Sends a NULL terminated text string to UART2 with CR and LF appended.
  @ Parameters
	 @ param1 : Pointer to the text string
  @ Returns
	 1 : Logical true that the string has been sent.
  ---------------------------------------------------------------------------- */
int putsU2(const char *s) {
	int ch_sent;
	while (*s) {
		do {
			ch_sent = putcU2(*s++); // Wall has the s++ part outside the loop, I believe that's wrong
		} while(ch_sent == 0);
	}
	do { ch_sent = putcU2('\r'); } while(!ch_sent);
	do { ch_sent = putcU2('\n'); } while(!ch_sent);
	
	return 1;
}

/* -------------------------------- getcU2 -----------------------------------
  @ Description
	 Checks for a new character to arrive to the UART2 serial port.
  @ Parameters
	 @ param1 : A pointer to the character variable that is passed into the UART.
  @ Returns
	 1 : A new character has been received 
	 0 : No new character has been received
  ---------------------------------------------------------------------------- */
int getcU2(char *ch) {
	char c;
	int dr2, dr4;
	int done = 0;
	unsigned int lineStatus;
	
	lineStatus = UART2GetErrors();  // Check for UART errors
	if (lineStatus != 0) {  // An error was received
		printf("Error 0x%08x  %6d\n\r", lineStatus, lineStatus);
		UART2ClearAllErrors();
		getcUART2();
	}
	else {				  // No errors were present
		dr2 = DataRdyUART2();
		if (dr2) {  // Wait for new character to arrive
			c	= getcUART2(); // Read from the receive buffer
			*ch  = c;
			done = 1;		   // Update flag
		}
	}
	
	return done;
}

/* ------------------------------- getstrU2 ----------------------------------
  @ Description
	 Assemble a line of text until the number of characters either exceeds
	 the buffer length, or an ASCII CR (\r) is received. The returned string
	 has the CR removed, but includes the null char (\0) at the end
  @ Parameters
	 @ param1 : A pointer to the string that's filled from UART
	 @ param2 : Maximum length of the string
  @ Returns
	 1 : Return character has been received (EOL read)
	 0 : No return character received
  ---------------------------------------------------------------------------- */
int getstrU2(char *s, unsigned int len) {
	static int eol = 1;			 // End of input string flag
	static unsigned int buf_len;	// Number of received characters
	static char *p1;				// Copy #1 of buffer pointer
	static char *p2;				// Copy #2 of buffer pointer
	char ch;						// New character receiver
	
	if (eol) {						// Initial function call - New line
		p1	  = s;					// Copy pointer twice, one for receiving
		p2	  = s;					// and one for marking start address
		eol	 = 0;					// We need the second for backspaces
		buf_len = len;
	}
	
	if (!(getcU2(&ch))) {			// Check for character received
		return 0;					// No character received
	}
	else {
		*p1 = ch;					// Save new character in buffer
		switch (ch) {				// Look for control characters
			case BACKSPACE:
				if (p1 > p2) {
					putcU2(' ');	// Overwrite last character
					putcU2(BACKSPACE);
					buf_len++;
					p1--;			// Move pointer back
				}
				break;
			case '\r':				// Return signals the EOL
				eol = 1;
				break;
			case '\n':				// Ignore all line feeds
				break;
			default:
				p1++;				// Increment buffer pointer
				buf_len--;			// Decrement length counter
		}
	}
	
	if (buf_len == 0 || eol) {		// Check for a full buffer / EOL
		*p1 = '\0';					// Null terminate the string
		return 1;
	}
	
	return 0;
}
