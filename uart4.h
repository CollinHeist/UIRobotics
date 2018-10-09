#ifndef __MX370_COMM_H__

    #define __MX370_COMM_H__
    #define _UART4_
#endif

#ifndef __UART4__

    #define __COMM_M__
    #define BACKSPACE       0x08
    #define NO_PARITY       0
    #define ODD_PARITY      1
    #define EVEN_PARITY     2

#endif

void uart4_init(unsigned int baud, int parity);
void _mon_putc(char c); /* Called by system to implement "printf" functions */
BOOL putcU4( int c);      /* Send single character to UART */
BOOL getcU4( char *ch);   /* Get single character from UART */
int putsU4( const char *s);     /* Send string to UART */
int getstrU4( char *s, unsigned int len ); /* Get CR terminated string */

/* End of comm.h */
