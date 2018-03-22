//NOW THAT I HAVE THIS POWER, I WILL SPAM YOU TO DEATH
/* ---------- Character LCD driver using the PIC32 PMP interface -------------
  @ Summary
    Character LCD control using the PIC32 parallel Master Port (PMP).
  @ Description
     This program provides low level LCD IO control using the PIC32
     PMP bus. The initLCD function must be call before any ASCII
     characters can be written to the LCD. The readLCD and writeLCD
     functions handle access to both the LCD control registers
     (addr = 0) and the LCD DDRAM/CGRAM (addr = 1). The putsLCD function
     processes an ASCII string of characters that can contain ASCII
     control characters LF, CR, and TAB.  The TAB control moves the
     cursor 8 spaces to the right.
 
     Macros provided in LCDlib.h provide LCD cursor control and character
     based operations
  ---------------------------------------------------------------------------- */

// System included files
#include "hardware.h"
#include <plib.h>

// Application included files
#include "swDelay.h"    // Required for DelayMs function
#include "LCDlib.h"
#include <stdint.h>

// Graphical characters for generatting FFT display
unsigned char lcd_blks[64] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F,
                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x1F,
                              0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x1F, 0x1F,
                              0x00, 0x00, 0x00, 0x00, 0x1F, 0x1F, 0x1F, 0x1F,
                              0x00, 0x00, 0x00, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F,
                              0x00, 0x00, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F,
                              0x00, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F,
                              0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F};

/* ------------------------------- initLCD() ---------------------------------
  @ Syntax
     void initLCD(void);
  @ Description
     Sets up the PMP interface and Initializes a 16x2 character LCD
  @ Parameters
     None
  @ Return Value
     None
  ---------------------------------------------------------------------------- */
void initLCD(void) {
    int config1 = PMP_ON|PMP_READ_WRITE_EN|PMP_READ_POL_HI|PMP_WRITE_POL_HI;
    int config2 = PMP_DATA_BUS_8 | PMP_MODE_MASTER1 |
                  PMP_WAIT_BEG_4 | PMP_WAIT_MID_15 | PMP_WAIT_END_4;
    int config3 = PMP_PEN_0;        // only PMA0 enabled
    int config4 = PMP_INT_OFF;      // no interrupts used

	PORTSetPinsDigitalIn (IOPORT_E, LCD_DATAbits);	// RE0:7
	PORTSetPinsDigitalOut(IOPORT_D, ENpin);		// RD4
	PORTSetPinsDigitalOut(IOPORT_D, RWpin);		// RD5
	PORTSetPinsDigitalOut(IOPORT_B, RSpin);		// RB15

    mPMPOpen(config1, config2, config3, config4);       // PMP initialization
    
    msDelay (20); // wait for > 20ms
    msDelay (50); // Settling time - not required
} // End of initLCD

/* ------------------------------- readLCD() ---------------------------------
  @ Syntax
     char readLCD(int addr);
  @ Description
     Writes a single character from the LCD.
  @ Parameter
     @ param1: addr - sets the RS pin
  @ Return Value
     Byte read from LCD
  @ Notes
     The two read cycles are required as per peripheral library
  ---------------------------------------------------------------------------- */
unsigned char readLCD (int addr) {
    unsigned char data; 
    PMPSetAddress(addr);            // select register
    data = mPMPMasterReadByte();    // initiate read sequence
    data = mPMPMasterReadByte();    // read actual data
    return data;
} // End of readLCD

/* ------------------------------- writeLCD() --------------------------------
  @ Syntax
     void writeLCD(int addr, char c);
  @ Description
     Writes a single character to the LCD.
  @ Parameters
     @ param1: addr - sets the RS pin
     @ param2: c    - 8 bit data to write to LCD
  @ Return Value
     None
  @ Notes
     This is a blocking function waiting for the LCD busy flag to be set clear.
  ---------------------------------------------------------------------------- */
void writeLCD (int addr, char c) {
    PMPSetAddress(addr);    // select LCD register 
    nsDelay(10);            // Wait 10 ns
    PMPMasterWrite(c);      // initiate write sequence
} // End of writeLCD
    
/* ------------------------------- putcLCD() --------------------------------
  @ Syntax
     void putcLCD(int rs, char c);
  @ Description
     Writes a single character to the LCD.
  @ Parameters
    @ param1: rs - the RS pin
    @ param2: c  - 8 bit data to write to LCD
  @ Return Value
     None
  @ Notes
     This is a blocking function waiting for the LCD busy flag to be set
     clear. The LCD data pins must use bit set and bit clear instructions
     to prevent altering PORT E pins that are not connected to the LCD. 
     Implements minimum delays for setup and hold times.
  ---------------------------------------------------------------------------- */
void putcLCD(int rs, char c) {
    char bf;
    {
        LATBSET = BIT_3;        // LCD Busy Flag set
        do { bf = readLCD( LCDCMD) & LCD_BF; }
        while (bf);
        LATBCLR = BIT_3;        // LCD Busy Flag reset
    }
    
    writeLCD(rs, c);
} // End of putcLCD

/* ------------------------------- putsLCD() ---------------------------------
  @ Syntax
     void putsLCD(char *s);
  @ Description
     Writes a NULL terminated ASCII string of characters to the LCD.
  @ Parameters
    @ param1: character pointer to text string
  @ Return Value
     None
  @ Notes
     LF is converted to CLEAR DISPLAY.  CR is converted to LINE RETURN,
     TAB inserts SPACE character to the end of line.
  ---------------------------------------------------------------------------- */
void putsLCD(char *s) {  
    char c;
    while (*s) {
        switch (*s) {           // Check for ASCII control characters
        case '\n':              // point to second line
            waitLCD();          // Ensure cursor addr is valid
            setLCDC(0x40);
            break;
        case '\r':              // home, point to first line 
            waitLCD();          // Ensure cursor addr is valid
            setLCDC(0);
            break;
        case '\t':              // advance next TAB (8) positions 
            waitLCD();          // Ensure cursor address is valid
            c = addrLCD();      // Read cursor address
            while (c % TABSIZE);// Move to next TAB position - even if past EOL
            {
                putcLCD(LCDDATA, ' ');  // Insert space characters to EOL
                waitLCD();              // Ensure cursor addr is valid
                c = addrLCD();          // Read cursor address
            }

            if ((c >= LCD_LINE1) && (c < LCD_LINE2)) { //if necessary reposition
                setLCDC(LCD_LINE2);   // Reposition cursor
            }
            break;                
        default:                // print character
            waitLCD();          // Ensure cursor address is valid
            c = addrLCD();      // Get cursor address
            if (( c >= LCD_LINE1) && (c < LCD_LINE2)) { //if necessary reposition
                waitLCD();              // Ensure cursor address is valid
                setLCDC(LCD_LINE2);   // Reposition cursor
            }
            putcLCD(LCDDATA, *s);       // BF is checked before writing char
            break;
        } // end switch-case
        s++;
    } // End while
} // End of putsLCD

/* ------------------------------ initLCDGr() --------------------------------
  @ Syntax
     void initLCDGr(void);
  @ Description
     Programs the 8 CGRAM characters FFT Display
  @ Parameter
     None
  @ Return Value
     None
  ---------------------------------------------------------------------------- */
void initLCDGr(void) {
    int i;
    clrLCD();
    for (i = 0; i < 8; i++) {
        writeLCD(LCDDATA, i);
    }

//  Program graphical characters
    setLCDG(0); // Set CGRAM pointer to 0
    for (i = 0; i < 64; i++) {
        putcLCD(LCDDATA, lcd_blks[i]);
    }
    gotoLCD(0); // Set DDRAM to 0
    for (i = 0; i < 8; i++) {
        putcLCD(LCDDATA, i);
    }
}

/* ------------------------------ FFT_Disp() ---------------------------------
  @ Syntax
     void FFT_Disp(int16_t* mag, int N);
  @ Description
     Display the 16 FFT bins using the 8 graphical characters
  @ Parameters
    @ param1: mag - Array containing the bin magnitudes
    @ param2: N   - Number of bins to display
  @ Return Value
     None
  ---------------------------------------------------------------------------- */
void FFT_Disp (int16_t* mag, int N) {
    int i;
    uint8_t amp[32];
    uint8_t a;

    clrLCD();
    for (i = 0; i < N; i++) {
        a = (uint8_t) (mag[i]);
        if (a > 7) {
            amp[i+16] = 7;
            amp[i] =  a-8;
        }
        else {
            amp[i+16] = a;
            amp[i] = 0;           
        }
    }
    for (i = 0; i < N; i++) {
        if (amp[i] == 0) {
            putcLCD(LCDDATA, ' ');
        }
        else {
            putcLCD(LCDDATA, amp[i]);            
        }
    }
    gotoLCD (NEW_LINE);
    for (i = 0; i < N; i++) {
        if (amp == 0) {
            putcLCD(LCDDATA, ' ');
        }
        else {
            putcLCD(LCDDATA, amp[i+16]);            
        }
    }
}
