/* ************************************************************************** */
/** Descriptive File Name
  @ Author
    Richard Wall
 
  @ Date
    December 30, 2016

  @Company
    Digilent       

   @File Name
    PICmx370.c

  @Summary
    Definition of constants and macro routines 

  @Description
    The #define statements and macro C code provide high level access to the 
    "Trainer" boards switches, push buttons, and LEDs.
    
 */
/* ************************************************************************** */

/* This included file provides access to the peripheral library functions and
   must be installed after the XC32 compiler. See
http://ww1.microchip.com/downloads/en/DeviceDoc/32bitPeripheralLibraryGuide.pdf
http://www.microchip.com/SWLibraryWeb/product.aspx?product=PIC32%20Peripheral%20Library
 */

#include "hardware.h"
#include <plib.h>

// *****************************************************************************
/**
    @Function
        void Hardware_Setup(void);

    @Summary
        Initializes PIC32 pins commonly used for IO on the Trainer processor
        board.

    @Description
        Initializes PIC32 digital IO pins to provide functionality for the 
        slide switches, push buttons, and LEDs

    @Precondition
        "config_bits* must be included in the project

    @Parameters
        None
        @param param2: pointer to y in range of 0b0000 to 0b1111 ( 0 - 15d )

    @Returns
        None

    @Remarks
    */
     
void Hardware_Setup(void)
{
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Statement configure cache, wait states and peripheral bus clock
 * Configure the device for maximum performance but do not change the PBDIV
 * Given the options, this function will change the flash wait states, RAM
 * wait state and enable prefetch cache but will not change the PBDIV.
 * The PBDIV value is already set via the pragma FPBDIV option above..
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    SYSTEMConfig(GetSystemClock(), SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE);

//    DDPCONbits.JTAGEN = 0;  // Statement is required to use Pin RA0 as IO
    
    LEDcfg();
//    BTNcfgIn();   // BTNR and BTND are used for RC1 and RC2
    RBG_LEDcfg();
	SWcfg();
    MCInit();
   
/* Enable multi-vector interrupts */
    INTConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);  /* Do only once */
    INTEnableInterrupts();   /*Do as needed for global interrupt control */

} /* End of BAsys MX3A Setup */

/* START FUNCTION DESCRIPTION ********************************************

SYNTAX:  		void initTimer1{void);
KEYWORDS:		initialization system timer 1
DESCRIPTION:	Sets timer 1 for an 10 us interrupt
RETURN VALUE:   none
NOTES:			None
END DESCRIPTION **********************************************************/

void initTimer1(void)
{
#define TMR1_TICK	100

    millisec = 0;
    OpenTimer1(T1_ON | T1_SOURCE_INT | T1_PS_1_1, TMR1_TICK);
    
	// Set Timer 1 interrupt with a priority of 2
    ConfigIntTimer1(T1_INT_ON | T1_INT_PRIOR_2);

	// Enable multi-vector interrupts
    INTEnableSystemMultiVectoredInt();

}

unsigned millis(void)
{
    return millisec;
}