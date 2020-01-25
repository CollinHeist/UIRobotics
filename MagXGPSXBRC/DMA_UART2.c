/* ----------------------------------- File Inclusion ----------------------------------- */

#include "DMA_UART2.h"
#include "hardware.h"
#include <plib.h>
#include <xc.h>
#include <stdlib.h>
#include <string.h>

/* -------------------------- Global Variables and Structures --------------------------- */

int DmaIntFlag = 0; 			// flag used in interrupts
char dmaBuff[DMA_BUFFER_SIZE+1];	 // Master DMA Uart RX buffer
char dmaBuff1[DMA_BUFFER_SIZE+1]; // DMA Uart RX buffer 1
char dmaBuff2[DMA_BUFFER_SIZE+1]; // DMA Uart RX buffer 2
DmaChannel  dmaChn = DMA_CHANNEL1;	// DMA channel

/* ---------------------------------- Public Functions ---------------------------------- */

/*********************************************************************
 * Function:		void DmaUartRxInit(void);
 * PreCondition:	None
 * Input:			None
 * Output:		  None
 * Side Effects:	None
 * Overview:		Initialization for UART2 serial block receive.
 *				  The transfer is complete when a null character is 
 *				  received.
 * Note:			None.
 ********************************************************************/
void DmaUartRxInit(void) {
    INTClearFlag(INT_SOURCE_DMA(dmaChn));	// Clear any standing DNA interrupts
    DmaChnOpen(dmaChn, DMA_CHN_PRI2, DMA_OPEN_MATCH);
    DmaChnSetMatchPattern(dmaChn, 0);	// set null as ending character
    // set the events: we want the UART2 rx interrupt to start our transfer
    // also we want to enable the pattern match: transfer stops upon detection of CR
    DmaChnSetEventControl(dmaChn, DMA_EV_START_IRQ_EN|DMA_EV_MATCH_EN|DMA_EV_START_IRQ(_UART2_RX_IRQ));
    DmaChnSetTxfer(dmaChn, (void*)&U2RXREG, dmaBuff2, 1, DMA_BUFFER_SIZE, 1);
    // enable the transfer done interrupt: pattern match or all the characters transferred
    DmaChnSetEvEnableFlags(dmaChn, DMA_EV_BLOCK_DONE);		
    INTEnable(INT_SOURCE_DMA(dmaChn), INT_ENABLED);		// enable the chn interrupt in the INT controller

    INTSetVectorPriority(INT_VECTOR_DMA(dmaChn), INT_PRIORITY_LEVEL_5);		// set INT controller priority
    INTSetVectorSubPriority(INT_VECTOR_DMA(dmaChn), INT_SUB_PRIORITY_LEVEL_3);		// set INT controller sub-priority
    // enable the // enable the chn interrupt in the INT controller interrupt in the INT controller
    INTEnable(INT_SOURCE_DMA(dmaChn), INT_ENABLED);		
    //	DmaIntFlag = 0;				// clear the interrupt flag

    // enable the dmaChn
    DmaChnEnable(dmaChn);
}

/*********************************************************************
 * Function:		void DmaUartRx(void);
 * PreCondition:	None
 * Input:			None
 * Output:		  None
 * Side Effects:	None
 *
 * Overview:		
 * This function starts a DMA RX block receive UART2 
 * 
 *
 * Note:			None.
 ********************************************************************/
void DmaUartRx(void) {
	DmaChnSetTxfer(dmaChn, (void*)&U2RXREG, dmaBuff2, 1, DMA_BUFFER_SIZE, 1);
	DmaChnEnable(dmaChn);
}

// handler for the DMA channel 1 interrupt
void __ISR(_DMA1_VECTOR, IPL5SOFT) DmaHandler1(void) {
	int	evFlags;	// event flags when getting the interrupt

// release the interrupt in the INT controller, we're servicing int
	evFlags=DmaChnGetEvFlags(dmaChn);	// get the event flags
	INTClearFlag(INT_SOURCE_DMA(dmaChn));	
	
	if(evFlags & DMA_EV_ALL_EVNTS)
	{ // just a sanity check. we enabled just the DMA_EV_BLOCK_DONE transfer 
	  // done interrupt
		DmaIntFlag=1;
		strcpy(dmaBuff,dmaBuff2);
	}
}
