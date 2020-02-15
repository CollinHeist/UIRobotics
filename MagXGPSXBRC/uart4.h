#ifndef __UART_4_H__
	#define __UART_4_H__

	// UART4 Configurations
	#define UART4_MAX_SEND_TIME_MS		(250)

	// Function Prototypes
	unsigned int initializeUART4(unsigned int baud, int parity);
	void _mon_putc(char c);
	unsigned int putCharacterUART4(int ch);
	unsigned int getCharacterUART4(char *ch);
	unsigned int putStringUART4(const char *s); 
	int getStringUART4(char *s, unsigned int len);
#endif