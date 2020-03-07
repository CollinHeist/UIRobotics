#ifndef __UART2_H__
	#define __UART2_H__

    // DMA Configurations
    #define DMA_BUFFER_SIZE     (256)

    // Return encodings
    #define UART2_NEW_DATA_AVAILABLE    (1)     // Used within getCharacterUART2() for character received
    #define UART2_NO_NEW_DATA_AVAILABLE (0)
    #define UART2_DONE_PARSING          (1)     // Used within getStringUART2() for a completed string received
    #define UART2_STILL_PARSING         (0)

	// Function Prototypes
	unsigned int initializeUART2(const unsigned int baud, const unsigned int parity);
	unsigned int putCharacterUART2(const char c);
	unsigned int getCharacterUART2(char *ch);
	void putStringUART2(const char *s);
	unsigned int getStringUART2(char *s, const unsigned int len);
    static unsigned int initializeDMAUART2RX(void);
#endif

