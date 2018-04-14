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

#include "comm_lib.h"
#include "swDelay.h"
#include "RC.h"

/* ----------------------------- runMotor() ----------------------------------
  @ Syntax
     int isValid = runMotor(int channel, int position);
  @ Summary
     Function that should be called to update the powers of the individual
	 motors specified in 'channel'. This power level will be set to 'position'.
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
	DelayMs(125);		// Wait an eighth of a second
	    
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

/* ----------------------------- powerMotors() -------------------------------
  @ Syntax
     powerMotors(int leftMotor, int rightMotor, float duration);
  @ Summary
     Public function that should be called by outside code, powers each
	 of the two motors at the specified power levels for the specified
	 duration. This code assumes that the left motor is on the first channel
  @ Parameters
     param1 : A array of length 2, where [0] is the channel of the left
	          motor, and [1] is the power level, between -100% and 100%,
			  at which to power the left motor.
     param2 : A array of length 2, where [0] is the channel of the right
	          motor, and [1] is the power level, between -100% and 100%,
			  at which to power the right motor.
	 param3 : The duration (in seconds) to power the motors at. At the end
	 	      of this duration, the motors are returned to off.
  @ Returns
     1 : Both of the motors were powered successfully. No errors from runMotor().
	 0 : Error code. One or both of the motors were unable to be powered at the
	     provided levels for the provided time, OR the provided input arrays
		 were not the proper dimensions.
  ---------------------------------------------------------------------------- */
int powerMotors(int[] leftMotor, int[] rightMotor, float duration) {
	/* -- Throw an error code if the passed values are incorrectly bounded --- */
	if (sizeof(leftMotor)  / sizeof(leftMotor[0] != 2  || 
        sizeof(rightMotor) / sizeof(rightMotor[0] != 2 ||
        duration <= 0) { return 0; }
	
	/* ----------------- Start and wait time - in milliseconds --------------- */
	unsigned int tWait, tStart;
	tStart = ReadCoreTimer();
	tWait  = CORE_MS_TICK_RATE * duration * 1000;
	
	/* ------ Power the motors at the passed values, for the passed time ----- */
	int leftMotorSuccess  = runMotor(leftMotor[0],  leftMotor[1]);
	int rightMotorSuccess = runMotor(rightMotor[0], rightMotor[1]);
	while (ReadCoreTimer() - tStart < tWait);
	
	/* -------- Turn off the motors by powering them to a level of 0% -------- */
	int tempL = runMotor(leftMotor[0], 0);
	int tempR = runMotor(rightMotor[0], 0);
	
	return (leftMotorSuccess && rightMotorSuccess);
}

/* ---------------------------- system_init() ---------------------------------
  @ Syntax
     void initRC()
  @ Summary
     Initialized the RC pins for PWM output
  @ Returns
     None
  ---------------------------------------------------------------------------- */
void initRC (void) {
    #define TMR2_TICK	25000
	/* --------------------- Set RC pins for PWM Output ---------------------- */
	cfgRC1();
	cfgRC2();

    OpenTimer2(T2_ON | T2_SOURCE_INT | T2_PS_1_4, PWM_PD-1);
	
	/* ------------- Set Timer 2 interrupt with a priority of 2 -------------- */
    ConfigIntTimer2(T2_INT_ON | T2_INT_PRIOR_2);
    
    OpenOC5((OC_ON|OC_TIMER_MODE16|OC_TIMER2_SRC|OC_PWM_FAULT_PIN_DISABLE), 
            RC_CENTER, RC_CENTER);  // RC1
    OpenOC4((OC_ON|OC_TIMER_MODE16|OC_TIMER2_SRC|OC_PWM_FAULT_PIN_DISABLE), 
            RC_CENTER, RC_CENTER);  // RC2   
}

/* -------------------------------- ISR --------------------------------------
  @ Syntax
     Automatically called function, no need to reference this, but honestly
	 I have no idea what it does, and am afraid to delete it :)
  @ Summary
     Clears the interrupt flag (whatever that means)
  @ Returns
     None
  @ Returns
     None
  ---------------------------------------------------------------------------- */
void __ISR(_TIMER_2_VECTOR, IPL2SOFT) Timer2Handler(void) { mT2ClearIntFlag(); }
