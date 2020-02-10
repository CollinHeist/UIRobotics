/* ----------------------------------- File Inclusion ----------------------------------- */

#include <plib.h>
#include "hardware.h"
#include "DMA_UART2.h"
#include <xc.h>
#include <stdlib.h>
#include <string.h>

/* -------------------------- Global Variables and Structures --------------------------- */

int DmaIntFlag;						// Flag used in interrupts
char DMABuffer[DMA_BUFFER_SIZE+1];			// Master DMA Uart RX buffer
static char privateDMABuffer[DMA_BUFFER_SIZE+1];	// Private, temporary DMA buffer
static DmaChannel DMAChannel = DMA_CHANNEL1;		// Active DMA channel

/* ---------------------------------- Public Functions ---------------------------------- */

/*
 *	Summary
 *		Initialize the DMA to work for the UART2 RX channel.
 *  Parameters
 *		None.
 *	Returns
 *		unsigned int that corresponds to whether or not the initialization was successful.
 *	Notes
 *		The DMA stores UART2 messages in privateDMABuffer, and completed messages are transfered to DMABuffer
 */
unsigned int initializeDMAUART2RX(void) {
	INTClearFlag(INT_SOURCE_DMA(DMAChannel));	// Clear any standing DNA interrupts
	DmaChnOpen(DMAChannel, DMA_CHN_PRI2, DMA_OPEN_MATCH);
	DmaChnSetMatchPattern(DMAChannel, 0);	// set null as ending character
	// set the events: we want the UART2 rx interrupt to start our transfer
	// also we want to enable the pattern match: transfer stops upon detection of CR
	DmaChnSetEventControl(DMAChannel, DMA_EV_START_IRQ_EN|DMA_EV_MATCH_EN|DMA_EV_START_IRQ(_UART2_RX_IRQ));
	DmaChnSetTxfer(DMAChannel, (void*)&U2RXREG, privateDMABuffer, 1, DMA_BUFFER_SIZE, 1);

	// Enable the transfer done interrupt on pattern match for all the characters transferred
	DmaChnSetEvEnableFlags(DMAChannel, DMA_EV_BLOCK_DONE);		
	INTEnable(INT_SOURCE_DMA(DMAChannel), INT_ENABLED);

	INTSetVectorPriority(INT_VECTOR_DMA(DMAChannel), INT_PRIORITY_LEVEL_5);			// Set INT controller priority
	INTSetVectorSubPriority(INT_VECTOR_DMA(DMAChannel), INT_SUB_PRIORITY_LEVEL_3);	// Set INT controller sub-priority
	// Enable the channel interrupt
	INTEnable(INT_SOURCE_DMA(DMAChannel), INT_ENABLED);		

	// Enable the DMA channel now that it's been configured
	DmaChnEnable(DMAChannel);

	return NO_ERROR;
}

/*
 *	Summary
 *		Restart the DMA transfer from between the U2RX register and privateDMABuffer.
 *	Parameters
 *		None.
 *	Returns
 *		None.
 */
void restartDMATransfer(void) {
	DmaChnSetTxfer(DMAChannel, (void*)&U2RXREG, privateDMABuffer, 1, DMA_BUFFER_SIZE, 1);
	DmaChnEnable(DMAChannel);
}

/* --------------------------------- Private Functions ---------------------------------- */

/* ----------------------------- Interrupt Service Routines ----------------------------- */

/*
 *	Summary
 *		ISR for all DMA channel events.
 *	Parameters
 *		None.
 *	Returns
 *		None.
 */
void __ISR(_DMA1_VECTOR, IPL5SOFT) isrDMAHandler(void) {
	if (DmaChnGetEvFlags(DMAChannel) & DMA_EV_ALL_EVNTS) {
		DmaIntFlag = 1;
		strcpy(DMABuffer, privateDMABuffer);	// Change buffers
	}

	INTClearFlag(INT_SOURCE_DMA(DMAChannel));
}
