/* ************************************************************************** */
/** Descriptive File Name: UART Serial communications header file for 
 *  comm_lib.c. This declares all public functions.

  @Summary
    UART 4 initialization and character and string I/O.

  @Description
    UART initialization can be set for any BAUD rate using EVEN, ODD, or NO 
    parity. Character and string input functions are non blocking functions.
    The serial interface uses UART channel 3.

  @Remarks
    This code also uses the "printf" function on UART Serial Port 4

**************************************************************************** */

#ifndef __UART2_H__

    #define __UART2_H__
    #define _UART2

/* ASCII control characters */
    #define BACKSPACE       0x08
    #define NO_PARITY       0
    #define ODD_PARITY      1
    #define EVEN_PARITY     2

#endif

// ----------------------------------------------------------------------------
/* uart2_init FUNCTION DESCRIPTION *************************************
 @ SYNTAX: void uart2_init(unsigned int baud, int parity);

 @Summary
    UART 3 initialization and character and string I/O.

 @Description
    UART initialization can be set for any BAUD rate using EVEN, ODD, or NO 
    parity. Character and string input functions are non blocking functions.
    The serial interface uses UART channel 4.

 @ PARAMETERS
    @param1:        integer Baud rate
    @param2:        integer (parity, NO_PARITY, ODD_PARITY, or EVEN_PARITY)
 @RETURN VALUE:     None

  @Remarks
    This code also uses the "printf" function on UART Serial Port 4
    9 bit mode MARK or SPACE parity is not supported

 
 * END DESCRIPTION **********************************************************/
void uart2_init(unsigned int baud, int parity);

/* putU3 FUNCTION DESCRIPTION ********************************************
 @SYNTAX:           BOOL putU3( int c);
 * 
 @KEYWORDS:         UART, character
 * 
 @DESCRIPTION:      Send single character to UART4. Waits while UART4 is busy 
                    (buffer full) and then sends a single byte to UART1
 * 
 @PARAMETER
    @param1:        character to send
 
 @RETURN VALUE:     TRUE = new character sent
                    FALSE = character not sent
 
 @REMARKS:          This function will not block if space is not available
                    in the transmit buffer
@EXAMPLE
   @code
        BOOL result;
        char ch;
        result = putcU4(ch);
 
 * END DESCRIPTION **********************************************************/
int putcU2( int c);

/* getcU2 FUNCTION DESCRIPTION ********************************************
 @SYNTAX:           int getcU2( char *ch);

 @KEYWORDS:         character, get

 @DESCRIPTION:      Checks for a new character to arrive to the UART2 serial
                    port.
 @PARAMETER
    @param1:        character pointer to character variable

 @RETURN VALUE:     TRUE = new character received
                    FALSE = No new character

 @REMARKS:          This function does not block for no character received
 
 @EXAMPLE
   @code
        BOOL result;
        char ch;
        result = getcU2(&ch);
 
 * END DESCRIPTION ********************************************************/
int getcU2( char *ch);

/* putsU3 FUNCTION DESCRIPTION ********************************************
 @SYNTAX:          int putsU2( const char *s);

 @KEYWORDS:        UART, string

 @DESCRIPTION:     Sends a NULL terminates text string to UART2 with
                   CR and LF appended
 @PARAMETER
    @param1:       pointer to text string
 
 @RETURN VALUE:    Logical TRUE

 @REMARKS:         This function will block until space is available
 *                  in the transmit buffer
 * END DESCRIPTION **********************************************************/
int putsU2(const char *s); 

/* getstrU3 FUNCTION DESCRIPTION ********************************************
 @SYNTAX:           int getstrU3( char *s, unsigned int len );
 @KEYWORDS:         string, get, UART
 @DESCRIPTION:      This function assembles a line of text until the number of
                    characters assembled exceed the buffer length or an ASCII
                    CR control character is received.  This function echo each
                    received character back to the UART. It also implements a
                    destructive backspace. ASCII LF control characters are
                    filtered out.  The returned string has the CR character
                    removed and a NULL character appended to terminate the text
                    string.
 @PARAMETERS
    @param1:        character pointer to string
    @param1:        integer maximum string length

 @RETURN VALUE:     TRUE = EOL signals received return character
                    FALSE = waiting for end of line
 
 @REMARKS:          It is presumed that the buffer pointer or the buffer length
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
        result = getstrU3(str, len);
 
* END DESCRIPTION ************************************************************/
int getstrU2( char *s, unsigned int len );

/* End of uart2.h */
