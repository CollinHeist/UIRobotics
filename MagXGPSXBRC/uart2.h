#ifndef __UART2_H__
	#define __UART2_H__
	#define _UART2

	// ASCII Control Characters
	#define BACKSPACE		0x08
	#define NO_PARITY		0
	#define ODD_PARITY		1
	#define EVEN_PARITY		2
#endif

// Function prototypes
void uart2_init(unsigned int baud, int parity);
int putcU2(int c);
int getcU2(char *ch);
int putsU2(const char *s);
int getstrU2(char *s, unsigned int len);