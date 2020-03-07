#ifndef __UART_4_H__
	#define __UART_4_H__

	// UART4 Configurations
	#define UART4_MAX_SEND_TIME_MS		(500)

	// Function Prototypes
	unsigned int initializeUART4(const unsigned int baud, const unsigned int parity);
	void _mon_putc(char c);
	unsigned int putCharacterUART4(const char c);
	unsigned int getCharacterUART4(char *ch);
	unsigned int putStringUART4(const char *s); 
	int getStringUART4(char *s, const unsigned int len);
#endif