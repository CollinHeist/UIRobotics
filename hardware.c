// System included files
#include "hardware.h"
#include <plib.h>
  
void Hardware_Setup(void) {
    SYSTEMConfig(GetSystemClock(), SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE);
    DDPCONbits.JTAGEN = 0;  // Statement is required to use Pin RA0 as IO
  
    ALL_DIGITAL_IO();   // Sets all LED, switches and push buttons for digital IO
    SET_MIC_ANALOG();   // Sets microphone input for analog
    SET_POT_ANALOG();   // Sets ANALOG INPUT CONTROL for analog input
    
    Set_All_LEDs_Output();	// Sets Basys MX3 LED0 through LED7 as output
    Set_All_LEDs_Off();		// Sets Basys MX3 LED0 through LED7 off
    SWcfg();                // Sets Basys MX3 SW0 through SW7 as input
    Set_All_PBs_Input();	// Sets Basys MX3 push buttons as input
	Set_RGB_Output();		// Sets Basys MX3 RGB LED as output
	Set_LED8_RGB(0);        // Sets Basys MX3 RGB LED off
	MCInit();
    
    /* Enable multi-vector interrupts */
    INTConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);  /* Do only once */
    INTEnableInterrupts();   /*Do as needed for global interrupt control */
    
}
