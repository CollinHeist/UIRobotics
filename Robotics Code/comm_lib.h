/* --------------------- Guarding against multiple inclusion ----------------- */
#ifndef __MX370_COMM_H__
    #define __MX370_COMM_H__
    #define _UART4
#endif

#ifndef __COMM_H__
    #define __COMM_M__
    #define BACKSPACE       0x08
    #define NO_PARITY       0
    #define ODD_PARITY      1
    #define EVEN_PARITY     2

    /* ---------------------- Public Function declarations ------------------- */
    void uart4_init(unsigned int baud, int parity);
    void _mon_putc(char c);
    BOOL putcU4(int c);
    BOOL getcU4(char *ch);
    int putsU4(const char *s);
    int getstrU4(char *s, unsigned int len);
#endif
