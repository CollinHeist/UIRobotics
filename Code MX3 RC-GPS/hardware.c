#include "hardware.h"
#include <plib.h>

void Hardware_Setup(void) {
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

}