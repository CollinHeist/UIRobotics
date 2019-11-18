#ifndef __UART2_H__
	#define __UART2_H__

	// ASCII control characters
	#define BACKSPACE		0x08
	#define NO_PARITY		0
	#define ODD_PARITY		1
	#define EVEN_PARITY		2

	// Function Prototypes
	void initializeUART2(unsigned int baud, int parity);
	int putCharacterUART2(int c);
	int getCharacterUART2(char *ch);
	int putStringUART2(const char *s);
	int getStringUART2(char *s, unsigned int len);
#endif

