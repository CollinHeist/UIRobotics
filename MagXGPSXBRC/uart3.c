/* ----------------------------------- File Inclusion ----------------------------------- */

#include <plib.h>
#include <stdio.h>		// Required for printf 

#include "hardware.h"	// Has info regarding the PB clock
#include "uart3.h"
#include "SensorData.h"	// Contains structure where data is stored

/* -------------------------- Global Variables and Structures --------------------------- */

unsigned int parseSensorString(SensorData *data);	// Prototype here to avoid errors on the SensorData struct

/* ---------------------------------- Public Functions ---------------------------------- */


unsigned int initializeUART3(unsigned int baud, int parity) {
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

/* -------------------------------- putcU2 -----------------------------------
  @ Description
     Waits while UART2 is busy (meaning the buffer is full) and then sends a
     single byte to UART3. This function is non-blocking if there is no
     available space in the transmit buffer.
  @ Parameters
     @ param1 : The integer representation of the character to be put on UART
  @ Returns
     1 : The passed character is sent
     0 : The passed character is not sent 
  ---------------------------------------------------------------------------- */
int putCharacterUART3(int ch) {
    char c = (char) ch;
    int done = 0;
    
    if (U3STAbits.TRMT) {
        U3TXREG = c;
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
int putStringUART3(const char *s) {
    int ch_sent;
    while (*s) {
        do {
            ch_sent = putCharacterUART3(*s++); // Wall has the s++ part outside the loop, I believe that's wrong
        } while(ch_sent == 0);
    }
    do { ch_sent = putCharacterUART3('\r'); } while(!ch_sent);
    do { ch_sent = putCharacterUART3('\n'); } while(!ch_sent);
    
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
int getCharacterUART3(char *ch) {
    UART_DATA c;
    BOOL done = FALSE;
    if (UARTReceivedDataIsAvailable(UART3)) {	// wait for new char to arrive
        c = UARTGetData(UART3);					// read the char from receive buffer
        *ch = (c.data8bit);
        done = TRUE;							// Return new data available flag
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
int getStringUART3(char *s, unsigned int len) {
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

unsigned int parseSensorString(SensorData* data) {
	static char sensorString[SENSOR_STRING_WIDTH] = {'\0'};
	static unsigned int stringIndex = 0;
	char c;
	unsigned int flag = getCharacterUART3(&c);
	if (flag) {
		sensorString[stringIndex++] = c;
		if (c == '\n') {
			if (sensorString[0] != START_CHARACTER) {
				clearBuffer(sensorString, (unsigned int) SENSOR_STRING_WIDTH);
				stringIndex = 0;
				return INVALID_STRING;
			}
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
