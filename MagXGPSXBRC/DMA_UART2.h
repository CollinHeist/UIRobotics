#ifndef _DMA_UART2_H
	#define _DMA_UART2_H

	#define DMA_BUFFER_SIZE 256

	// Function Prototypes
	void DmaUartRxInit(void);
	void DmaUartRx(void);
#endif

extern int			DmaIntFlag;	// flag used in interrupts
extern int			bufferNum;	// DMA Buffer number
extern char			dmaBuff[];	// Master DMA UART Rx Buffer
extern char			dmaBuff1[];	// DMA UART Rx Buffer 1
extern char			dmaBuff2[];	// DMA UART Rx Buffer 1