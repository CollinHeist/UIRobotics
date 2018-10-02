#include "hardware.h"	/* Has info regarding the PB clock */
#include <plib.h>
#include <stdio.h>              /* Required for printf */
#include "comm_lib.h"

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
void uart4_init(unsigned int baud, int parity)
{
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
void _mon_putc(char c)
{
    while(!UARTTransmitterIsReady(UART4));
    UARTSendDataByte(UART4, c);
} /* End of _mon_putc */

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
} /* End of putsU4 */

/* getstrU4 FUNCTION DESCRIPTION ********************************************
 * SYNTAX:          int getstrU1( char *s, unsigned int len );
 * KEYWORDS:        string, get, UART
 * DESCRIPTION:     This function assembles a line of text until the number of
 *                  characters assembled exceed the buffer length or an ASCII
 *                  CR control character is received.  This function echo each
 *                  received character back to the UART. It also implements a
 *                  destructive backspace. ASCII LF control characters are
 *                  filtered out.  The returned string has the CR character
 *                  removed and a NULL character appended to terminate the text
 *                  string.
 * PARAMETER 1:     character pointer to string
 * PARAMETER 2:     integer maximum string length
 * RETURN VALUE:    TRUE = EOL signaled br receiving return character
 *                  FALSE = waiting for end of line
 * NOTES:           It is presumed that the buffer pointer or the buffer length
 *                  does not change after the initial call asking to
 *                  receive a new line of text.  This function does not block
 *                  for no character received. A timeout can be added to this
 *                  to free up resource. There is no way to restart the function
 *                  after the first call until a EOL has been received.  Hence
 *                  this function has denial of service security risks.
 * END DESCRIPTION **********************************************************/
int getstrU4( char *s, unsigned int len )
{
static int eol = TRUE;  /* End of input string flag*/
static unsigned int buf_len;
static char *p1;        /* copy #1 of the buffer pointer */
static char *p2;        /* copy #2 of the buffer pointer */
char ch;                /* Received new character */

    if(eol)		/* Initial call to function - new line */
    {			/* Make two copies of pointer - one for */
        p1 = s;		/* receiving characters and one for marking */
	p2 = s;		/* the starting address of the string.  The */
        eol = FALSE;	/* second copy is needed for backspacing. */
	buf_len = len;  /* Save maximum buffer length */
    }

    if(!(getcU4(&ch)))	/*  Check for character received */
    {
        return FALSE;	/* Bail out if not */
    }
    else
    {
        *p1 = ch;       /* Save new character in string buffer */
   	putcU4( *p1);   /* echo character */
	switch(ch)	/* Test for control characters */
	{
            case BACKSPACE:
                if ( p1>p2)
                {
                    putcU4( ' ');     /* overwrite the last character */
                    putcU4( BACKSPACE);
                    buf_len++;
                    p1--;            /* back off the pointer */
                }
                break;
            case '\r':              /* end of line, end loop */
                putcU4( '\n');      /* add line feed */
                eol = TRUE;         /* Mark end of line */
            	break;
            case '\n':              /* line feed, ignore it */
                break;
            default:
                p1++;               /* increment buffer pointer */
                buf_len--;          /* decrement length counter */
        }
    }
    if( buf_len == 0 || eol)    /* Check for buffer full or end of line */
    {
        *p1 = '\0';             /* add null terminate the string */
        return TRUE;            /* Set EOL flag */
    }
    return FALSE;               /* Not EOL */
} /* End of getstrU4 */

/* End of comm.c */

