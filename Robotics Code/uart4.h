/* --------- UART Serial communications header file for comm_lib.c -----------
   ----------- This declares all public functions and constants --------------
  @ Summary
     UART 4 initialization and character and string I/O.
  @ Description
     UART initialization can be set for any BAUD rate using EVEN, ODD, or NO 
     parity. Character and string input functions are non blocking functions.
     The serial interface uses UART channel 4.
  @ Remarks
     This code also uses the "printf" function on UART Serial Port 4
  ---------------------------------------------------------------------------- */

#ifndef __MX370_COMM_H__
    #define __MX370_COMM_H__
    #define _UART4

    /* -------------------- ASCII Control Characters ------------------------- */
    #define BACKSPACE       0x08
    #define NO_PARITY       0
    #define ODD_PARITY      1
    #define EVEN_PARITY     2
#endif

/* --------------------------- Function declarations ------------------------- */
void uart4_init(unsigned int baud, int parity);
void _mon_putc(char c);
BOOL putcU4(int ch);
BOOL getcU4(char *ch);
int putsU4(const char *s); 
int getstrU4(char *s, unsigned int len);
