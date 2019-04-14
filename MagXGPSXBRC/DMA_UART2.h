/* ************************************************************************** */
/** Descriptive File Name: UART2 DMA Serial communications header file for 
 *  DMA_uart2.h. This declares all public functions.

  @ Author  
     Richard Wall
  
  @ Date: 
     Created:    February 2, 2018 for Basys MX3

  @File Name
    DMA_uart2.h

  @Development Environment
    MPLAB X IDE x3.61 or higher - http://www.microchip.com/mplab/mplab-x-ide 
	XC32 1.43 or higher - http://www.microchip.com/mplab/compilers
	PLIB 3/7/20162 - http://www.microchip.com/SWLibraryWeb/product.aspx?product=PIC32%20Peripheral%20Library

  @Summary
    DMA UART2 Serial receive initialization 

  @Description

  @Remarks

**************************************************************************** */


#ifndef _DMA_UART2_H
	#define _DMA_UART2_H
    #include "hardware.h"
	#include <plib.h>	

    #define DMA_BUFFER_SIZE 256
#endif

extern int			DmaIntFlag;			// flag used in interrupts
extern int          bufferNum;      // DMA Buffer number
extern char 		dmaBuff[];      // Master DMA UART Rx Buffer
extern char 		dmaBuff1[];     // DMA UART Rx Buffer 1
extern char 		dmaBuff2[];     // DMA UART Rx Buffer 1

void DmaUartRxInit(void);
void DmaUartRx(void);
