/* ------------------------ Motor controller project file --------------------
  @ Summary
     Provides the logic for powering two different motors on two different
     UART connections. Uses PWM, and has the configuration for two servos.
  ---------------------------------------------------------------------------- */
  
/* -------------------------- Project Include Files -------------------------- */
#include "config_bits.h"    // Processor configuration
#include "hardware.h"       // Platform declarations

#include <plib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

#include "led7.h"
#include "comm_lib.h"
#include "swDelay.h"
#include "RC.h"

/* ----------------------------- runMotor() ----------------------------------
  @ Syntax
     int isValid = runMotor(int channel, int position);
  @ Summary
     Public function that should be called to update the power of the motor
     specified in 'channel'. This power level will be set to 'position'.
     This function presently only sends one PWM signal, but should the motors
     require a second signal after a short delay, there is commented code for
     that.
  @ Parameters
     param1 : The channel number that should be updated. Expects a number
              between 1 and the NRC (defined in motor.h)
     param2 : The 'position' that is a percentage between -100% and 100%.
              Negative values are treated as 'reverse' while positive values
              are treated as forward. -100% would be full-speed reverse, etc.
  @ Returns
     1 : The motor was toggled appropriately. No errors in the input.
     0 : The provided input was not in the proper range, either the channel
         was not between 1 and the number of RC channels, or the position was
         not between -100 and 100.
  ---------------------------------------------------------------------------- */
int runMotor(int channel, int position) {
    initRC();			// Initialize RC channel output pins
	DelayMs(250);		// Wait a quarter of a second
	    
    /* ------- Make sure the provided RC Channel and position are valid ------ */
    unsigned int pos = position + 100 / 2; // Scale the % range to the PWM range
    if (channel >= 1 && channel <= NRC)	{ 
        if (pos >= 0 && pos <= 100) {
            unsigned int rc_pwm = RC_MIN + (pos * RC_SPAN / 100); 
            switch (channel) {
                case 1:
                    rc[0] = pos;
                    RC_1(rc_pwm); // Update RC channel position
                    //DelayMs(750); // Wait .75 seconds and send another pulse
                    //RC_1(rc_pwm);
                    return 1;
                case 2:
                    rc[1] = pos;
                    RC_2(rc_pwm); // Update RC channel position
                    //DelayMs(750); // Wait .75 seconds and send another pulse
                    //RC_2(rc_pwm);
                    return 1;
            }
        }
        else { return 0; }
    }
    else { return 0; }
}

/* --------------------------- system_init() ---------------------------------
  @ Syntax
     void initRC()
  @ Summary
     Initialized the RC pins for PWM output
  @ Returns
     None
  ---------------------------------------------------------------------------- */
void initRC (void) {
    #define TMR2_TICK	25000
	cfgRC1();		// Set RC pins for PWM output
	cfgRC2();

    OpenTimer2(T2_ON | T2_SOURCE_INT | T2_PS_1_4, PWM_PD-1);
	// Set Timer 2 interrupt with a priority of 2
    ConfigIntTimer2(T2_INT_ON | T2_INT_PRIOR_2);
    
    OpenOC5((OC_ON|OC_TIMER_MODE16|OC_TIMER2_SRC|OC_PWM_FAULT_PIN_DISABLE), 
            RC_CENTER, RC_CENTER);  // RC1
    OpenOC4((OC_ON|OC_TIMER_MODE16|OC_TIMER2_SRC|OC_PWM_FAULT_PIN_DISABLE), 
            RC_CENTER, RC_CENTER);  // RC2   
}

/* -------------------------------- ISR --------------------------------------
  @ Syntax
     Automatically called function, no need to reference this
  @ Summary
     None
  @ Returns
     None
  @ Returns
     None
  ---------------------------------------------------------------------------- */
void __ISR(_TIMER_2_VECTOR, IPL2SOFT) Timer2Handler(void) {
    mT2ClearIntFlag();			// clear the interrupt flag
}
