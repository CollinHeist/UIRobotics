/* ----------------------- Header file for uart2.c -------------------------
  @ Summary
     UART 4 initialization and character and string I/O
  @ Description
     UART initialization can be set for any BAUD rate using EVEN, ODD, or NO 
     parity. Character and string input functions are non blocking functions.
     The serial interface uses UART channel 3.
  @ Remarks
     This code also uses the "printf" function on UART Serial Port 4
  -------------------------------------------------------------------------- */
#ifndef __UART2_H__
    #define __UART2_H__
    #define _UART2
    /* --------------------- ASCII Control Characters ---------------------- */
    #define BACKSPACE       0x08
    #define NO_PARITY       0
    #define ODD_PARITY      1
    #define EVEN_PARITY     2
#endif

/* ------------------------ Function declarations -------------------------- */
void uart2_init(unsigned int baud, int parity);
int putcU2(int c);
int getcU2( char *ch);
int putsU2(const char *s); 
int getstrU2(char *s, unsigned int len);
