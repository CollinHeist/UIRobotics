
/* ************************************************************************** */
/** Descriptive File Name:  main.c 

  @ Author
    Richard Wall
 
  @ Date
    Created;    February 11, 2019

  @Development Environment
    MPLAB X IDE x5.05 - http://www.microchip.com/mplab/mplab-x-ide 
	XC32 2.10 - http://www.microchip.com/mplab/compilers
	PLIB 3/7/20162 - http://www.microchip.com/SWLibraryWeb/product.aspx?product=PIC32%20Peripheral%20Library
  @File Name
    main.c

  @Summary
 * This project demonstrates how to use CHANGE NOTICE on digital input pins.
 * PIC32MX3xx-4xx processors can be configured to generate CHANGE NOTICE 
 * interrupts for all port pins. This example uses the Digilent BASys MX3  
 * board. The main function loop has a 2 second software delay loop. If BTNR
 * is pressed, LED5 is turned on otherwise it is turned off. The user will see 
 * significant response delay for the LED to match the BUTR operation. BTND 
 * controls LRD6 using a CHANGE NOTICE interrupt. The user will see that LED6
 * immediately matches the operation of BTND.
 * 
 * LED3 flashes on or off each second because of a TIMER1 interrupt. 
 * (See hardware.c)
 * 
 
  @Remarks
    Reference:     PIC32MX2xx/4xx Users Manual - 
        http://ww1.microchip.com/downloads/en/DeviceDoc/61143H.pdf
    
    Uses XC32 peripheral library. See 
 http://ww1.microchip.com/downloads/en/DeviceDoc/32bitPeripheralLibraryGuide.pdf
 http://www.microchip.com/SWLibraryWeb/product.aspx?product=PIC32%20Peripheral%20Library
  
 ****   This application uses a peripheral bus clock set to 80MHz.  ****
    PBCLK set for 80 MHz, Set parameters in config_bits.h and hardware.h

 ******************************************************************************/

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */

// System included files
#include "hardware.h"
#include "Gamepad.h"
#include "main.h"
#include <plib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// Platform common included files
#include "swDelay.h"

int Status1 = 0;
int Status2 = 0;


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */


/** 
  @Function
    int main( void ); 

  @Summary
    Initializes the hardware platform and performs the calculator operations.

  @Description
    System initialization and infinite application loop. Reads the GPS sentence 
    and decodes the sentence before displaying on the LCD. Also facilitates
    changing GPS sentences.

  @Precondition:    None

  @Parameters:      None

  @Returns:      The system has failed if the last statement is executed. 
      <li> EXIT_FAILURE (1)   Indicates an error occurred
      <li> EXIT_SUCCESS (0)   Indicates an error did not occur

  @Remarks : PIC32MX3xx-4xx Users Manual Section 12.1.4 
 */

void initChangeNotice(void);

void initChangeNotice(void) 
{
int JA1;
int JA2;
  PORTSetPinsDigitalIn(IOPORT_C, BIT_13 | BIT_14); // Declare BTND as CN pin as input
  JA2 = mPORTCRead() & BIT_13;     // Read BTND IO pin to set initial pin status
  JA1 = mPORTCRead() & BIT_14; 
/* Do not use ENABLE PULLUPS on BAsys MX3 button inputs because buttons are 
   active high with external pull down resistors. Hence when internal pullups 
   are set on, the pin always reads high. */
   //mCNAOpen((CNA_ON | CNA_IDLE_CON), (CNA15_ENABLE), 0);
    
/* JA and JB pins should not have internal pullups enabled unless external 
   pullups are added. */
  mCNCOpen((CNC_ON | CNC_IDLE_CON), (CNC13_ENABLE | CNC14_ENABLE), (CNC13_PULLUP_ENABLE | CNC14_PULLUP_ENABLE)); 

/* Pins can have the CN ENABLED or DISABLED during run time.*/
   EnableCNC13;    // Enable interrupts for each individual CN pin
   EnableCNC14;    // Enable interrupts for each individual CN pin
/* All CN interrupts are vectored to a single CN ISR    */
   ConfigIntCNC((CHANGE_INT_ON | CHANGE_INT_PRI_1));
}

/* ------------------------------ Timer1Handler ------------------------------
  @ Summary
 * A common CN ISE is used for all CN interrupts regardless of IO port and 
 * pin.
  @ Parameter:  None, no parameters can be pass to an ISR.  
  @ Returns:    None, no parameters can be returned from an ISR.  
 *
 * Remarks:  The IO Pin(s) responsible for generating the CN interrupt are
 * determined by reading the CN status registers (CNSTATx) and checking the
 * set bits. CNSTATx bits will be set for changes on only pins that have had
 * the CN enabled for each port. If pins on other IO ports are enabled, the
 * CN status register for that port must be checked as well. Reading the IO 
 * port clears all standing CNSTAT bits for the IO port read.
  ---------------------------------------------------------------------------- */
void __ISR(_CHANGE_NOTICE_VECTOR, IPL1SOFT) ChangeNoticeHandler(void) 
{
   
     unsigned int JA1;
     unsigned int JA2;
    //JA1/RC2
    if(CNSTATC & BIT_13) // Check to see which pin(s) created the interrupt
    {
        JA1 = mPORTCRead() & BIT_13; // Clear all standing CNSTATA bits.
        SetDefaultServoPosition();
        ClearLeftStick();
        
        Status1++;
        Status1 %= 2;
// Also can be written as temp = BTND();)  
        //setLED6(btnd);  // Show that the ISR has been served - for test only.
    }
    //JA2/RC1
    if(CNSTATC & BIT_14) // Check to see which pin(s) created the interrupt
    {
        JA2 = mPORTCRead() & BIT_14; // Clear all standing CNSTATA bits.
        SetDefaultServoPosition();
        ClearLeftStick();
        
        Status2++;
        Status2 %= 2;
// Also can be written as temp = BTND();)  
        //setLED5(btnd);  // Show that the ISR has been served - for test only.
    }
    // clear the interrupt flag
 	mCNCClearIntFlag(); // Clear the CN interrupt flag
}

/* End of main.c */
