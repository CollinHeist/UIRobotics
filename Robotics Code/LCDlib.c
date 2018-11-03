#include "LCDlib.h"

#define PMDATA  PMDIN 		// Redefinition of the PMP data bit

unsigned char lcd_blks[64] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F,
                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x1F,
                              0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x1F, 0x1F,
                              0x00, 0x00, 0x00, 0x00, 0x1F, 0x1F, 0x1F, 0x1F,
                              0x00, 0x00, 0x00, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F,
                              0x00, 0x00, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F,
                              0x00, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F,
                              0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F};

/* -------------------------------- initLCD() --------------------------------
 @ Summary
    Initialize the LCD module for use
 @ Description
    Sets up the PMP interface and initializes a 16x2 LCD screen
 @ Parameters
    None
 @ Return Value
    None
  ---------------------------------------------------------------------------- */
void initLCD(void) {
	int config1 = PMP_ON|PMP_READ_WRITE_EN|PMP_READ_POL_HI|PMP_WRITE_POL_HI;
	int config2 = PMP_DATA_BUS_8 | PMP_MODE_MASTER1 |
	              PMP_WAIT_BEG_4 | PMP_WAIT_MID_15 | PMP_WAIT_END_4;
	int config3 = PMP_PEN_0;        // Only PMA0 enabled
	int config4 = PMP_INT_OFF;      // No interrupts used

	PORTSetPinsDigitalIn(IOPORT_E, LCD_DATAbits);		// RE0:7
	PORTSetPinsDigitalOut(IOPORT_D, ENpin);				// RD4
	PORTSetPinsDigitalOut(IOPORT_D, RWpin);				// RD5
	PORTSetPinsDigitalOut(IOPORT_B, RSpin);				// RB15

    mPMPOpen(config1, config2, config3, config4); // PMP initialization
    
    DelayMs(20);   // Wait for > 20ms
    
    // Initialize the HD44780 display 8-bit init sequence
    PMPSetAddress(LCDCMD);		// Select command register
    PMPMasterWrite(LCD_CFG);	// 8-bit int, 2 lines, 5x7
    DelayMs(39);				// > 48ms
    
    PMPMasterWrite(LCD_ON);		// ON, no cursor, no blink
    DelayMs(39);				// > 37ms
    
    PMPMasterWrite(LCD_CLR);	// Clear display
    DelayMs(2);					// > 1.6ms
    
    PMPMasterWrite(LCD_ENTRY); 	// ON, no cursor, no blink
    DelayMs(50);               	// Settling time - not required
}

/* -------------------------------- readLCD() --------------------------------
 @ Summary
    Read the current data from the LCD
 @ Parameters
    @ param1 : Integer value of the address to read from on the LCD
 @ Return Value
    Unsigned character corresponding to the byte at the provided address
  ---------------------------------------------------------------------------- */
unsigned char readLCD(int addr) {
	unsigned char data; 
	PMPSetAddress(addr);			// Select register
	data = mPMPMasterReadByte();    // Initiate read sequence
	data = mPMPMasterReadByte();    // Read actual data
    
	return data;
}

/* ------------------------------- writeLCD() --------------------------------
 @ Summary
    Write the provided character to the provided address of the LCD
 @ Parameters
    @ param1 : Integer address to write to
    @ param2 : Character to write to the provided address
 @ Return Value
    None
  ---------------------------------------------------------------------------- */
void writeLCD(int addr, char c) {
	PMPSetAddress(addr);	// Select register
	DelayNs(10);
	PMPMasterWrite( c);		// Initiate write sequence
}
    
/* -------------------------------- putcLCD() --------------------------------
 @ Summary
    Write a character to the LCD at the address
 @ Description
    This is a blocking LCD write function
 @ Parameters
    @ param1 : Integer address to write to
    @ param2 : Character to write to the provided address
 @ Return Value
    None
  ---------------------------------------------------------------------------- */
void putcLCD(int rs, char c) {
	waitLCD();
	writeLCD(rs, c);
}

/* -------------------------------- putsLCD() --------------------------------
 @ Summary
    Writes a string to the LCD
 @ Description
    The string needs to be null-terminated to properly work
 @ Parameters
    @ param1 : string to be written to LCD
 @ Return Value
    None
  ---------------------------------------------------------------------------- */
void putsLCD(char *s) {
	char c;
    
	while (*s) {
		switch (*s) {
			case '\n':		// Point to second line
				setLCDC(0x40);
				break;
        	case '\r':		// Home, point to first line
				setLCDC(0);
				break;
			case '\t':		// Advance next tab (8) positions
				c = addrLCD();
				while (c & 7) {
					putcLCD(1, ' '); 
					c++;
				}
				if (c > 15)	// If necessary, move to second line
					setLCDC(0x40);
				break;                
			default:		// Generic character, put on the screen
            	putcLCD(1, *s);
            	break;
        }
        s++;
    }
}

/* ------------------------------- initLCDGr() -------------------------------
 @ Summary
    Initalize and clear the LCD
 @ Parameters
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

	// Program graphical characters
	setLCDG(0); // Set CGRAM pointer to 0
	for (i = 0; i < 64; i++) {
        putcLCD(LCDDATA, lcd_blks[i]);
    }
    gotoLCD(0); // Set DDRAM to 0
    for (i = 0; i < 8; i++) {
		putcLCD(LCDDATA, i);
	}
}

/* -------------------------------- FFT_Disp() -------------------------------
 @ Summary
    
 @ Parameters
    @ param1 : 
    @ param2 : 
 @ Return Value
    None
  ---------------------------------------------------------------------------- */
void FFT_Disp(int16_t* mag, int N) {
	int i;
	uint8_t amp[32];
	uint8_t a;

	clrLCD();
	for(i = 0; i < N; i++) {
		a = (uint8_t) (mag[i]);
		if (a > 7) {
			amp[i + 16] = 7;
			amp[i] = a - 8;
		}
		else {
			amp[i + 16] = a;
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
	gotoLCD(NEW_LINE);
	for (i = 0; i < N; i++) {
		if (amp == 0) {
			putcLCD(LCDDATA, ' ');
		}
		else {
			putcLCD(LCDDATA, amp[i+16]);            
		}
	}
}