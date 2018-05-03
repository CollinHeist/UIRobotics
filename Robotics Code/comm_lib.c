/* -------------------------- Required library includes ---------------------- */
#include "hardware.h"
#include <plib.h>
#include <stdio.h>
#include "comm_lib.h"

BOOL lineRdy = FALSE;
char uartRxBuffer[40] = {0};

/* ----------------------------- uart4_init() --------------------------------
  @ Syntax
     uart4_init(38400, NO_PARITY);
  @ Description
     Initializes UART4 comm port for the specified BAUD rate using the
     assigned parity. Call at the beginning of the project.
  @ Parameters
     @ param1 : Integer BAUD rate to open the UART4 port to
     @ param2 : Which parity for the UART4 port (NO_PARITY, ODD_PARITY, EVEN_PARITY)
  @ Returns
     None
  ---------------------------------------------------------------------------- */
void uart4_init(unsigned int baud, int parity) {
    RPF12R = 0x02;  // Mapping U4TX to RPF12;
    U4RXR = 0x09;   // Mapping U4RX to RPF13

    UARTConfigure(UART4, UART_ENABLE_PINS_TX_RX_ONLY );
    UARTSetDataRate(UART4, GetPeripheralClock(), baud);
    
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
    }

    /* ------------------------ Enable UART4 Interrupts ---------------------- */
    IEC2bits.U4RXIE = 1;        // Enable Rx interrupt
    IEC2bits.U4TXIE = 0;        // Disable Tx interrupt
    IPC9bits.U4IP = 1;          // Set interrupt priority to 1
    IPC9bits.U4IS = 0;          // Set interrupt sub-priority to 0        
    lineRdy = FALSE;
            
    printf("\n\rUART Serial Port 4 ready\n\n\r");
}

/* ------------------------------ _mon_putc() --------------------------------
  @ Syntax
     _mon_putc(char c);
  @ Description
     Sets up Serial Port to function as console for printf(). Used only by the system.
  @ Parameters
     @ param1 : Character value that is passed to the UART transmit buffer.
  @ Returns
     None
  ---------------------------------------------------------------------------- */
void _mon_putc(char c) {
    while (!UARTTransmitterIsReady(UART4));
    UARTSendDataByte(UART4, c);
}

/* ------------------------------- __ISR() -----------------------------------
  @ Syntax
     __ISR((INT_VECTOR) 39, IPL3SOFT);
  @ Description
     Assembles a line of text until the number of characters assembled exceed the
     buffer length or an ASCII CR control character is received. This function
     echos each received character to the UART. Handles the proper setting of
     uartRxBuffer (to the current UART buffer), and lineRdy to be true when an
     end character is received. 
  @ Parameters
     None
  @ Returns
     None
  ---------------------------------------------------------------------------- */
void __ISR((INT_VECTOR) 39, IPL3SOFT) IntUart4Handler(void) {
    static BOOL eol = TRUE;         // End of input string flag
    static unsigned int buf_len;
    static char *p1;                // copy #1 of the buffer pointer
    static char *p2;                // copy #2 of the buffer pointer
    char ch;                        // Received new character
    if (INTGetFlag(INT_U4RX)) {
        INTClearFlag(INT_U4RX);
        /* -------------------------------------------------------------------
           Make two copies of the pointer. One for receiving characters and
           one for marking the starting address in memory of the string. 
           This second marker is needed for backspacing.
           ------------------------------------------------------------------- */
        if (eol) {
            p1      = uartRxBuffer;
            p2      = uartRxBuffer;
            eol     = FALSE;
            buf_len = sizeof(uartRxBuffer) - 1;
        }

        if (getcU4(&ch)) {
            *p1 = ch;
            putcU4(*p1);
            switch (ch) {
                case BACKSPACE:
                    if (p1 > p2) {
                        putcU4(' ');     // overwrite the last character
                        putcU4(BACKSPACE);
                        buf_len++;
                        p1--;            // back off the pointer
                    }
                    break;
                case '\r':               // end of line, end loop
                    putcU4( '\n');       // add line feed
                    eol = TRUE;          // Mark end of line
                    break;
                case '\n':               // line feed, ignore it
                    eol = TRUE;          // Mark end of line
                    break;
                default:
                    p1++;                // increment buffer pointer
                    buf_len--;           // decrement length counter
            }
            if (buf_len == 0 || eol) {
                *p1 = '\0';              // add null terminate the string
                lineRdy = TRUE;          // Set line ready flag
                eol = TRUE;              // Mark end of line in case of buffer overflow
            }
        }
    
        else {
            if (INTGetFlag(INT_U4TX)) {
                INTClearFlag(INT_U4TX);
            }
        }
    }    
}

/* ------------------------------- putcU4() ----------------------------------
  @ Syntax
     BOOL a = purcU4(int a);
  @ Description
     Waits while UART1 is busy (meaning the buffer is full) and then sends a
     single byte to UART1. This function is non-blocking if there is no
     available space in the transmit buffer.
  @ Parameters
     @ param1 : The integer representation of the character to be put on UART
  @ Returns
     TRUE  : The passed character is sent
     FALSE : The passed character is not sent 
  ---------------------------------------------------------------------------- */
BOOL putcU4(int ch) {
    UART_DATA c;
    BOOL done = FALSE;
    c.data8bit = (char) ch;
    if (UARTTransmitterIsReady(UART4)) {
    	UARTSendDataByte(UART4, c.data8bit);
        done = TRUE;
    }
    
    return done;
}

/* ------------------------------- getcU4() ----------------------------------
  @ Syntax
     BOOL a = getcU4(char *ch);
  @ Description
     Checks for a new character to arrive to the UART serial port.
  @ Parameters
     @ param1 : A pointer to the character variable that is passed into the UART.
  @ Returns
     TRUE  : A new character has been received 
     FALSE : No new character has been received
  ---------------------------------------------------------------------------- */
BOOL getcU4(char *ch) {
    UART_DATA c;
    BOOL done = FALSE;
    /* ------- Act only if there is a new character in the UART buffer ------- */
    if (UARTReceivedDataIsAvailable(UART4)) {
        c = UARTGetData(UART4);	// read the char from receive buffer
        *ch = (c.data8bit);
        done = TRUE;		    // Return new data available flag
    }
    
    return done;
}

/* ------------------------------- putsU4() ----------------------------------
  @ Syntax
     int a = putsU4(char b); 
  @ Description
     Sends a NULL terminated text string to UART4 with CR and LF appended.
  @ Parameters
     @ param1 : Pointer to the text string
  @ Returns
     1 : Logical true that the string has been sent.
  ---------------------------------------------------------------------------- */
int putsU4 (const char *s) {
    BOOL ch_sent;
    puts(s);
    while (*s) {
        do {
            ch_sent = putcU4(*s++);
        } while(ch_sent == FALSE);
    }
    do { ch_sent = putcU4('\r'); } while(!ch_sent);
    do { ch_sent = putcU4('\n'); } while(!ch_sent);
    return 1;
}
