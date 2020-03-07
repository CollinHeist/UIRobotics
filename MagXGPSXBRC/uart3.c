/* ----------------------------------- File Inclusion ----------------------------------- */

#include <plib.h>
#include <stdio.h>		// Required for printf 

#include "hardware.h"	// Has info regarding the PB clock
#include "uart3.h"
#include "SensorData.h"	// Contains structure where data is stored

/* -------------------------- Global Variables and Structures --------------------------- */

unsigned int parseSensorString(SensorData *data);	// Prototype here to avoid errors on the SensorData struct

/* ---------------------------------- Public Functions ---------------------------------- */

/*
 *	Summary
 *		Function to initialize UART3 - the Arduino UART - for a given baud and parity.
 *	Parameters
 *		baud[in]:   Frequency the UART3 bus should operate at.
 *		parity[in]: What parity to operate the UART3 bus at.
 *	Returns
 *		Unsigned int that corresponds to whether an error occurred or not.
 */
unsigned int initializeUART3(const unsigned int baud, const int parity) {
    ANSELGbits.ANSG7 = 0;
    ANSELGbits.ANSG8 = 0;
    TRISGbits.TRISG7 = 1;   // UART3 Input RPG7 - JA8
    TRISGbits.TRISG8 = 0;   // UART3 Output RPG8 - JA9

    RPG8R = 0x01;   // Mapping U3TX to RPG8 - JA9
    U3RXR = 0x01;   // Mapping U3RX to RPG7 - JA8

    UARTConfigure(UART3, UART_ENABLE_PINS_TX_RX_ONLY );
    UARTSetDataRate(UART3, GetPeripheralClock(), baud);
	
    // Note the need to specify the UART number twice in the following statement    
    UARTEnable(UART3, UART_ENABLE_FLAGS(UART_ENABLE | UART3 | UART_RX | UART_TX));
 
    switch (parity) {
        case NO_PARITY:
            UARTSetLineControl(UART3, UART_DATA_SIZE_8_BITS | UART_PARITY_NONE | UART_STOP_BITS_1);
            break;
        case ODD_PARITY:
            UARTSetLineControl(UART3, UART_DATA_SIZE_8_BITS | UART_PARITY_ODD | UART_STOP_BITS_1);
            break;
        case EVEN_PARITY:
            UARTSetLineControl(UART3, UART_DATA_SIZE_8_BITS | UART_PARITY_EVEN | UART_STOP_BITS_1);
            break;
    }
    U3STAbits.UTXEN = 1;
    U3STAbits.URXEN = 1;
    U3MODEbits.ON = 1;
	
    putStringUART4("UART Serial Port 3 ready.\n\n\r");
	
	return NO_ERROR;
}

/*
 *	Summary
 *		Send a single character out over the UART3 TX bus.
 *	Parameters
 *		ch[in]: What character to place on the bus. 
 *	Returns
 *		Unsigned integer that is 1 of the transmission was successful, and 0 if
 *		the transmission was unsuccessful.
 */
unsigned int putCharacterUART3(const char ch) {
    if (U3STAbits.TRMT) {
        U3TXREG = ch;
        return 1;
    }
	
    return 0;
}

/*
 *	Summary
 *		Function to place an entire string on the UART3 TX bus.
 *	Parameters
 *		s[in]: Pointer to the first character in the string to be sent out.
 *	Returns
 *		None.
 */
void putStringUART3(const char* s) {
    unsigned int ch_sent;
    while (*s) {
        do {
            ch_sent = putCharacterUART3(*s++); // Wall has the s++ part outside the loop, I believe that's wrong
        } while(ch_sent == 0);
    }
    do { ch_sent = putCharacterUART3('\r'); } while(!ch_sent);
    do { ch_sent = putCharacterUART3('\n'); } while(!ch_sent);
}

/*
 *	Summary
 *		Function to get a single character from UART3.
 *	Parameters
 *		ch[out]: Pointer to the character to be 'filled' with the latest character
 *			received over UART3.
 *	Returns
 *		Integer that is either 0 or 1 if the received operation was successful.
 */
unsigned int getCharacterUART3(char *ch) {
    BOOL done = FALSE;
    if (UARTReceivedDataIsAvailable(UART3)) {	// wait for new char to arrive
        UART_DATA c = UARTGetData(UART3);		// read the char from receive buffer
        *ch = (c.data8bit);
        done = TRUE;							// Return new data available flag
    }   
	
    return (int) done;
}
          
/*
 *	Summary
 *		Function to get a complete string from the UART3 TX line.
 *	Parameters
 *		s[out]:	 A pointer to the first character of the buffer to be filled.
 *		len[in]: Unsigned integer that is the maximum number of bytes to fill s with.
 *	Returns
 *		Unsigned integer that is either 0 or 1 if a complete message hasn't or has
 *		been received (respectively).
 */
unsigned int getStringUART3(char* s, const unsigned int len) {
    static int eol = 1;             // End of input string flag
    static unsigned int buf_len;    // Number of received characters
    static char *p1;                // Copy #1 of buffer pointer
    static char *p2;                // Copy #2 of buffer pointer
    char ch;                        // New character receiver
    
    if (eol) {                      // Initial function call - New line
        p1      = s;                // Copy pointer twice, one for receiving
        p2      = s;                // and one for marking start address
        eol     = 0;                // We need the second for backspaces
        buf_len = len;
    }
    
    if (!(getCharacterUART3(&ch))) {	// Check for character received
        return 0;						// No character received
    }
    else {
        *p1 = ch;                   // Save new character in buffer
        switch (ch) {               // Look for control characters
            case BACKSPACE:
                if (p1 > p2) {
                    putCharacterUART3(' ');    // Overwrite last character
                    putCharacterUART3(BACKSPACE);
                    buf_len++;
                    p1--;           // Move pointer back
                }
                break;
            case '\r':              // Return signals the EOL
                eol = 1;
                break;
            case '\n':              // Ignore all line feeds
                break;
            default:
                p1++;               // Increment buffer pointer
                buf_len--;          // Decrement length counter
        }
    }
    
    if (buf_len == 0 || eol) {      // Check for a full buffer / EOL
        *p1 = '\0';                 // Null terminate the string
        return 1;
    }
    
    return 0;
}

/*
 *	Summary
 *		Function to parse the latest sensor message data from the Arduino.
 *	Parameters
 *		data[out]: Pointer to a SensorData structure that will be filled with the
 *			contents of the Arduino sensor string.
 *	Returns
 *		Unsigned integer that is one of the return encodings (see header) to indicate
 *		whether the string is invalid, done or still being parsed.
 */
unsigned int parseSensorString(SensorData* data) {
	static char sensorString[SENSOR_STRING_WIDTH] = {'\0'};
	static unsigned int stringIndex = 0;
	char c;
	unsigned int flag = getCharacterUART3(&c);
	if (flag) {
		// If a character is received, place it inside sensorString
		sensorString[stringIndex++] = c;
		if (c == '\n') {
			// When the newline character was sent - indicating the EOS - parse contents
			if (sensorString[0] != START_CHARACTER) {
				// First character should be valid, otherwise return an error
				clearBuffer(sensorString, (unsigned int) SENSOR_STRING_WIDTH);
				stringIndex = 0;
				
				return INVALID_STRING;
			}
			// Parse contents of the sensor string  into the passed SensorData structure.
			sscanf(sensorString, "$%d:%d:%d:%d:%d:%d:%d:%d:%d:%d:%d:%fN:%fW\n",
				&(data->temperatureF), &(data->relativeHumidity), &(data->magX), 
				&(data->magY), &(data->magZ), &(data->hour), &(data->min), &(data->sec), 
				&(data->month), &(data->day), &(data->year), &(data->latitude), &(data->longitude));
			
			clearBuffer(sensorString, (unsigned int) SENSOR_STRING_WIDTH);	// Clear buffer to all zeros
			stringIndex = 0;
		}
		
		return DONE_PARSING;
	}
	
	return STILL_PARSING;
}

/* --------------------------------- Private Functions ---------------------------------- */

/* ----------------------------- Interrupt Service Routines ----------------------------- */
