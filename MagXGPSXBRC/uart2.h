#ifndef __UART2_H__
	#define __UART2_H__

    // DMA Configurations
    #define DMA_BUFFER_SIZE     (256)

	// Function Prototypes
	unsigned int initializeUART2(unsigned int baud, int parity);
	int putCharacterUART2(int c);
	int getCharacterUART2(char *ch);
	int putStringUART2(const char *s);
	int getStringUART2(char *s, unsigned int len);
    static unsigned int initializeDMAUART2RX(void);
#endif

