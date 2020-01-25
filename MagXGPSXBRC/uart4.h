#ifndef __UART_4_H__
	#define __UART_4_H__

	// ASCII control characters 
	#define BACKSPACE		0x08
	#define NO_PARITY		0
	#define ODD_PARITY		1
	#define EVEN_PARITY		2

	// Function Prototypes
	void initializeUART4(unsigned int baud, int parity);
	void _mon_putc(char c);
	unsigned int putCharacterUART4(int ch);
	unsigned int getCharacterUART4(char *ch);
	int putStringUART4(const char *s); 
	int getStringUART4(char *s, unsigned int len);
#endif