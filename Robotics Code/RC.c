/* ------------- Hardware and Processor Initialization Includes -------------- */
#include "config_bits.h"
#include "hardware.h"

/* -------------------------- XC32 Public Libraries -------------------------- */
#include <plib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <Math.h>

/* ----------------------- Project-specific include files -------------------- */
#include "comm_lib.h"
#include "swDelay.h"
#include "RC.h"

/* --------------------------- Public global variables ----------------------- */
/* -------- The 'extern' keyword variables are initalized in comm_lib -------- */
extern BOOL lineRdy;
extern char uartRxBuffer;
/* -- These are predefined paths to the channel of the left and right motor -- */
unsigned int LEFT_MOTOR  = 1;
unsigned int RIGHT_MOTOR = 2;
typedef struct {
	float latitude;
	float longitude;
	float heading;
} Position;

/* ---------------------------- Main Project Function ------------------------ */
int main(void) {
    unsigned int ch, pos, rc_pwm;
    unsigned int leftCh, rightCh;
    int leftPercent, rightPercent;

	Hardware_Setup();	    // Initialize common Cerebot hardware
	uart4_init(38400, NO_PARITY);				// Initialize UART
	initRC();				// Initialize RC channel output pins
	DelayMs(2000);			// Wait two seconds
	printf("\n\rEnter servo channel and position\n\r");
	while (1) {
        /* ------------- lineRdy is set to True inside of comm_lib -----------
           ----- upon end line detection inside the UART channel (PuTTY) ----- */
        if (lineRdy) {
            sscanf(uartRxBuffer,"%d %d %d %d", &leftCh, &leftPercent, &rightCh, &rightPercent);
           
            /* -- Runs the motors at the specified percentages for 2 seconds - */
            powerMotors(leftCh, leftPercent, rightCh, rightPercent, 2);		
            lineRdy = FALSE;
    	}
	}
	return (EXIT_FAILURE);		// The program should never get to this
}


/* ----------------------------- powerMotors() -------------------------------
  @ Syntax
     int a = powerMotors(int leftCh, float leftPercent, int rightCh,
                         float rightPercent, float duration);
  @ Summary
     Blocking function that runs the left and right motors at the passed
     percentages for the passed amount of time.
     This should perhaps be changed to utilize a non-blocking wait-time.
     Currently, in swDelay.c, the function PeriodMs is non-blocking.
  @ Parameters
     param1 : An integer that corresponds to which channel the left motor is on
     param2 : A float ranged between [-100, 100] that is the percentage at which
              to run the left motor at. Negative values are backwards.
	 param3 : An integer that corresponds to which channel the right motor is on
     param4 : A float ranged between [-100, 100] that is the percentage at which
              to run the right motor at. Negative values are backwards.
     param5 : A float that is the number of seconds to run the motors at.
  @ Returns
     1 : Both of the motors were powered successfully. No errors.
	 0 : Error code. One or both of the motors were unable to be powered at the
	     provided levels for the provided time.
  ---------------------------------------------------------------------------- */
int powerMotors (int leftCh, float leftPercent, int rightCh, float rightPercent, float duration) {
    /* ------ Verify that the passed values are within a suitable range ------ */
    if (leftCh >= 1 && rightCh >= 1 && leftCh <= NRC && rightCh <= NRC) {
        if (leftPercent >= -100 && rightPercent >= -100 && leftPercent <= 100 && rightPercent <= 100) {
            /* ---------- Scale the percentage between -100% and 100% -------- */
            float leftPos  = (leftPercent  + 100.0) / 2.0;
            float rightPos = (rightPercent + 100.0) / 2.0;
            
            /* ----- Calculate the PWM corresponding to the motor values ----- */
            int leftPWM  = RC_MIN + (leftPos  * RC_SPAN / 100.0);
            int rightPWM = RC_MIN + (rightPos * RC_SPAN / 100.0);
            
            /* ------------------- Actually power the motors ----------------- */
            SetDCOC4PWM(leftPWM);
            SetDCOC5PWM(rightPWM);
            
            /* ---------------- Wait the passed amount of time --------------- */
            /* -------- Should perhaps implement a non-blocking wait --------- */
            int msDuration = duration * 1000;
            DelayMs(msDuration);
            
            /* --- Stop both of the motors after the passed amount of time --- */
            int stopPWM = RC_MIN + (50.0 * RC_SPAN / 100.0);
            SetDCOC4PWM(stopPWM);
            SetDCOC5PWM(stopPWM);
        }
        else { return 0; }
    }
    else { return 0; }
}

/* --------------------------- changeHeading() -------------------------------
 @ Syntax
     int a = changeHeading(Position currPos, Position desPos, float precision);
 @ Description
    This function turns the boat towards the desired latitude and longitude.
    This should be continually called until it returns a 1, indicating we're
    within the passed parameter of precision
 @ Parameters
    @ param1 : a Position structure that contains the current latitude,
			   longitude, and heading
    @ param2 : a Position structure that contains the desired latitude,
	 	       longitude, and heading
    @ param3 : a float representing how precise we want our heading to be.
	       A degree value (i.e. how far off our current heading and angle
               should be)
 @ Return Value
    1 : Successfully rotated the boat towards the desired location
    0 : The boat is still not at its desired heading - continue rotating 
  ---------------------------------------------------------------------------- */
int changeHeading(Position currPos, Position desPos, float precision) {
	float deltaLat  = desPos.latitude  - currPos.latitude;
	float deltaLong = desPos.longitude - currPos.longitude;
	
	float angleToRotate = -90; // Initialize to -90 to account for 0 denom.
	if (deltaLong != 0)
		angleToRotate = 180 / 3.141592654 * atan(deltaLat / deltaLong);
	
	/* --------------- Shift the rotation angle to the GPS scale ------------- */
	if (deltaLat > 0 && deltaLong > 0)
		angleToRotate = -(90 - angleToRotate);
	else if (deltaLat > 0 && deltaLong < 0)
		angleToRotate = -angleToRotate;
	else if (deltaLat < 0 && deltaLong > 0)
		angleToRotate = angleToRotate - 90;
	else
		angleToRotate = angleToRotate + 90;
	
	/* -------------- Account for other denominator of zero cases ------------ */
	angleToRotate = (deltaLong == 0 && deltaLat > 0) ? -90 : (deltaLong == 0 && deltaLat < 0) ? 90 : angleToRotate;
	
    /* ------ If we're within the margin of our desired precision, exit ------ */
	if (abs(currPos.heading - angleToRotate) < precision)
		return 1;
	else if ((currPos.heading - angleToRotate) < 0) // Left Turn
		powerMotors(LEFT_MOTOR, -100, RIGHT_MOTOR, 100, .5);
	else // Turn Left
		powerMotors(LEFT_MOTOR, 100, RIGHT_MOTOR, -100, .5);
	
	return 0; // Return 0 so long as we're not at the proper heading
}

void initRC (void) {
    #define TMR2_TICK	25000

	cfgRC1();
	cfgRC2();
    
    OpenTimer2(T2_ON | T2_SOURCE_INT | T2_PS_1_4, PWM_PD-1);
	/* -------------- Set Timer 2 interrupt with a priority of 2 ------------- */
    ConfigIntTimer2(T2_INT_ON | T2_INT_PRIOR_2);
    
    OpenOC5((OC_ON|OC_TIMER_MODE16|OC_TIMER2_SRC|OC_PWM_FAULT_PIN_DISABLE), 
            RC_CENTER, RC_CENTER);  // RC1
    OpenOC4((OC_ON|OC_TIMER_MODE16|OC_TIMER2_SRC|OC_PWM_FAULT_PIN_DISABLE), 
            RC_CENTER, RC_CENTER);  // RC2   
}

void __ISR(_TIMER_2_VECTOR, IPL2SOFT) Timer2Handler (void) {
    mT2ClearIntFlag();
}
