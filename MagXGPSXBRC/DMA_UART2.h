#ifndef __DMA_UART2_H__
	#define __DMA_UART2_H__

    // DMA Configurations
	#define DMA_BUFFER_SIZE     (256)

	// Function Prototypes
	void initializeDMAUART2RX(void);
	void restartDMATransfer(void);
#endif