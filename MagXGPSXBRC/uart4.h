#ifndef __MX370_COMM_H__
    #define __MX370_COMM_H__
    #define _UART4

	// ASCII Control Characters
	#define BACKSPACE		0x08
	#define NO_PARITY		0
	#define ODD_PARITY		1
	#define EVEN_PARITY		2
#endif

// Function Prototypes
void uart4_init(unsigned int baud, int parity);
void _mon_putc(char c);
int putsU4(const char *s);
int getstrU4(char *s, unsigned int len);
int getcIU4(char *ch);
int putcIU4(int ch);