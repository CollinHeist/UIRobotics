/* -------------------------- Required library includes ---------------------- */
#include "hardware.h"
#include <plib.h>
#include <stdio.h>
#include "comm_lib.h"

BOOL lineRdy = FALSE;
char uartRxBuffer[40] = {0};

/* uart4_init FUNCTION DESCRIPTION *************************************
 * SYNTAX:          void uart1_init(unsigned int baud, int parity);
 * KEYWORDS:        UART, initialization, parity
 * DESCRIPTION:     Initializes UART4 comm port for specified baud rate using
 *                  the assigned parity
 * PARAMETER 1:     integer Baud rate
 * PARAMETER 1:     integer (parity, NO_PARITY, ODD_PARITY, or EVEN_PARITY)
 * RETURN VALUE:    None
 *
 * NOTES:           9 bit mode MARK or SPACE parity is not supported
 * END DESCRIPTION **********************************************************/
void uart4_init(unsigned int baud, int parity) {
    RPF12R = 0x02;  // Mapping U4TX to RPF12;
    U4RXR = 0x09;   // Mapping U4RX to RPF13

    UARTConfigure(UART4, UART_ENABLE_PINS_TX_RX_ONLY );
    UARTSetDataRate(UART4, GetPeripheralClock(), baud);
    
// Note the need to specify the UART number twice in the following statement    
    UARTEnable(UART4, UART_ENABLE_FLAGS(UART_ENABLE | UART4 | UART_RX | UART_TX));  
    
    switch(parity)
    {
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

// ****************** Enable UART 4 interrupts **********************
    IEC2bits.U4RXIE = 1;        // Enable Rx interrupt
    IEC2bits.U4TXIE = 0;        // Disable Tx interrupt
    IPC9bits.U4IP = 1;          // Set interrupt priority to 1
    IPC9bits.U4IS = 0;          // Set interrupt sub-priority to 0        
// ******************************************************************

    lineRdy = FALSE;
            
    printf("\n\rUART Serial Port 4 ready\n\n\r");
}

/* _mon_putc FUNCTION DESCRIPTION ******************************************
 * SYNTAX:          void _mon_putc(char c);
 * KEYWORDS:        printf, console, monitor
 * DESCRIPTION:     Sets up serial port to function as console for printf.
 *                  Used only by system.
 * PARAMETER 1:     Character to send to monitor
 * RETURN VALUE:    None
 * NOTES:           This function will block until space is available
 *                  in the transmit buffer
 * END DESCRIPTION **********************************************************/
void _mon_putc(char c) {
    while (!UARTTransmitterIsReady(UART4));
    UARTSendDataByte(UART4, c);
}

/* getstrU4 FUNCTION DESCRIPTION ********************************************
 * SYNTAX:          __ISR( (INT_VECTOR) 39, IPL3SOFT) IntUart4Handler(void)
 * KEYWORDS:        string, get, UART
 * DESCRIPTION:     This function assembles a line of text until the number of
 *                  characters assembled exceed the buffer length or an ASCII
 *                  CR control character is received.  This function echo each
 *                  received character back to the UART. It also implements a
 *                  destructive backspace. ASCII LF control characters are
 *                  filtered out.  The global character string uartRxBuffer has 
 *                  the CR character removed and a NULL character appended to 
 *                  terminate the text array. The global variable, lineRdy, is 
 *                  set TRUE when a complete line of text has been assembled as
 *                  denoted by the CR character. The users application is 
 *                  obligated to reset lineRdy when finished with the 
 *                  uartRxBuffer.
 *                  
 * PARAMETERS:      None
 * RETURN VALUE:    None
 *                  
 * NOTES:           The value for the interrupt vector is specified in Table 7-1
 *                  of PIC32MX3xx-4xx Users Manual. This function implements a 
 *                  local echo to the terminal. 
 * 
 * END DESCRIPTION **********************************************************/
void __ISR( (INT_VECTOR) 39, IPL3SOFT) IntUart4Handler(void)
{
static BOOL eol = TRUE;         // End of input string flag
static unsigned int buf_len;
static char *p1;                // copy #1 of the buffer pointer
static char *p2;                // copy #2 of the buffer pointer
char ch;                        // Received new character
    if( INTGetFlag(INT_U4RX))
    {
        INTClearFlag(INT_U4RX);
        if(eol)                 // Initial call to function - new line
        {                       // Make two copies of pointer - one for
            p1 = uartRxBuffer;  // receiving characters and one for marking
            p2 = uartRxBuffer;  // the starting address of the string.  The
            eol = FALSE;        // second copy is needed for backspacing.
            buf_len = sizeof(uartRxBuffer)-1;  // Save maximum buffer length
        }

        if(getcU4(&ch))         //  Check for character received
        {
            *p1 = ch;           // Save new character in string buffer 
            putcU4( *p1);       // echo character 
            switch(ch)          // Test for control characters 
            {
                case BACKSPACE:
                    if ( p1>p2)
                    {
                        putcU4( ' ');     // overwrite the last character
                        putcU4( BACKSPACE);
                        buf_len++;
                        p1--;            // back off the pointer
                    }
                    break;
                case '\r':              // end of line, end loop
                    putcU4( '\n');      // add line feed
                    eol = TRUE;         // Mark end of line
                    break;
                case '\n':              // line feed, ignore it
                    eol = TRUE;         // Mark end of line
                    break;
                default:
                    p1++;               // increment buffer pointer
                    buf_len--;          // decrement length counter
            }
            if( buf_len == 0 || eol)    // Check for buffer full or end of line
            {
                *p1 = '\0';             // add null terminate the string
                lineRdy = TRUE;         // Set line ready flag
                eol = TRUE;     // Mark end of line in case of buffer overflow
            }
        }
    
        else
        {
            if ( INTGetFlag(INT_U4TX))
            {
                INTClearFlag(INT_U4TX);
                
                //code for Tx
            }
        }
    }    
}

/* putU1 FUNCTION DESCRIPTION ********************************************
 * SYNTAX:          BOOL putU1( int c);
 * KEYWORDS:        UART, character
 * DESCRIPTION:     Waits while UART1 is busy (buffer full) and then sends a
 *                  single byte to UART1
 * PARAMETER 1:     character to send
 * RETURN VALUE:    TRUE = new character sent
 *                  FALSE = character not sent
 * NOTES:           This function will not block if space is not available
 *                  in the transmit buffer
 * END DESCRIPTION **********************************************************/
BOOL putcU4( int ch)
{
UART_DATA c;
BOOL done = FALSE;
    c.data8bit = (char) ch;
    if(UARTTransmitterIsReady(UART4))
    {
    	UARTSendDataByte(UART4, c.data8bit);
	done = TRUE;
    }
   return done;
} /* End of putU4 */

/* getU4 FUNCTION DESCRIPTION ********************************************
 * SYNTAX:          BOOL getU1( char *ch);
 * KEYWORDS:        character, get
 * DESCRIPTION:     Checks for a new character to arrive to the UART1 serial port.
 * PARAMETER 1:     character pointer to character variable
 * RETURN VALUE:    TRUE = new character received
 *                  FALSE = No new character
 * NOTES:           This function does not block for no character received
 * END DESCRIPTION ********************************************************/
BOOL getcU4( char *ch)
{
UART_DATA c;
BOOL done = FALSE;
    if(UARTReceivedDataIsAvailable(UART4))	/* wait for new char to arrive */
    {
        c = UARTGetData(UART4);	/* read the char from receive buffer */
        *ch = (c.data8bit);
        done = TRUE;		/* Return new data available flag */
    }
    return done;            /* Return new data not available flag */
}/* End of getU4 */

/* putsU1 FUNCTION DESCRIPTION ********************************************
 * SYNTAX:          int putsU1( const char *s);
 * KEYWORDS:        UART, string
 * DESCRIPTION:     Sends a NULL terminates text string to UART1 with
 *                  CR and LF appended
 * PARAMETER 1:     pointer to text string
 * RETURN VALUE:    Logical TRUE
 * NOTES:           This function will block until space is available
 *                  in the transmit buffer
 * END DESCRIPTION **********************************************************/
int putsU4( const char *s)
{
BOOL ch_sent;
    puts(s);
    while(*s)
    {
        do
        {
            ch_sent = putcU4(*s++);
        } while(ch_sent ==  FALSE);
    }
    do { ch_sent = putcU4( '\r'); } while(!ch_sent);
    do { ch_sent = putcU4( '\n'); } while(!ch_sent);
    return 1;
}