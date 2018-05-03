/* ------------------- Hardware and Common Library Includes ------------------ */
#include "hardware.h"
#include <plib.h>

/* ---------------------------- Hardware_Setup() -----------------------------
  @ Summary
     Initializes PIC32 pins commonly used for IO on the Basys MX3 Trainer 
     processor board.
  @ Description
     Initializes PIC32 digital IO pins to provide functionality for the 
     slide switches, push buttons, and LEDs
  @ Parameters
     None
  @ Returns
     None
  ---------------------------------------------------------------------------- */
void Hardware_Setup(void) {
    /* -----------------------------------------------------------------------
       Statement configure cache, wait states and peripheral bus clock
       Configure the device for maximum performance but do not change the PBDIV
       Given the options, this function will change the flash wait states, RAM
       wait state and enable prefetch cache but will not change the PBDIV.
       The PBDIV value is already set via the pragma FPBDIV option above..
       ----------------------------------------------------------------------- */
    SYSTEMConfig(GetSystemClock(), SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE);

    DDPCONbits.JTAGEN = 0;  // Statement is required to use Pin RA0 as IO

    SetChanADC10( ADC_CH0_NEG_SAMPLEA_NVREF | ADC_CH0_POS_SAMPLEA_AN2); 
    OpenADC10( ADC_PARAM1, ADC_PARAM2, ADC_PARAM3, ADC_PARAM4, ADC_PARAM5 ); 
    EnableADC10();

	SWcfg();
    LEDcfg();
    Set_All_LEDs_Off();    
    RBG_LEDcfg();
   
    TRISBbits.TRISB9 = 1;
    ANSELBbits.ANSB9 = 0;
    
    /* --------------------- Enable Multi-vector Interrupts ------------------ */
    INTConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);
    INTEnableInterrupts();
}
