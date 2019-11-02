// File Inclusion
#include "hardware.h"
#include "swDelay.h"
#include <plib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <Math.h>
#include "RC.h"

int rc[NRC];
int rc_set[NRC];

const int RCMid = 50;
const int RCRight = 130;
const int RCLeft = 0;

int RC1Pos = 50;
int RC2Pos = 50;
/* -------------------------------- rcUpdate ---------------------------------
  @ Summary
	 Updates information stored in array rc based on rc_set as it cycles
	 through states based on the value of static variable rc_state and
	 information from RC_SERVO_MIN and RC_SERVO_MAX. 
  @ Parameters
	 None
  @ Returns
	 None
  ---------------------------------------------------------------------------- */
void rcUpdateServos(void) {
	static int channel = NRCSPEEDCONTROLLERS;   // Channel index 0<= channel NRC
	static int rc_state = 0;			        // Initial signal state
	static int rc1, rc2;				      // Signal period timers
	int i;

    invLED1();

	switch (rc_state) {
		case 0:		// Initial high period
			rc_output(channel, TRUE);	// Turn channel on
			rc1 = RC_SERVO_MIN + rc[channel];	// Compute on time
			if (rc1 > RC_SERVO_MAX)
				rc1 = RC_SERVO_MAX;
			rc2 = RC_SERVO_MAX - rc1;			// Time to end of cycle
			if (rc2 < 0)
				rc2 = 0;
			rc_state++;
			break;							
		case 1:
			if (--rc1 <= 0) {			// Count down cycle ON time
				rc_output(channel, FALSE);	// Turn channel off
				rc_state++;
			}
			break;
		case 2:
			if(--rc2 <= 0) {			// Count down Cycle off time
				rc_state = 0;			// Reset state counter
				channel = NRCSPEEDCONTROLLERS + (((channel-NRCSPEEDCONTROLLERS) + 1) % (NRCSERVOS));	// Next channel
				for (i = NRCSPEEDCONTROLLERS; i < NRC; i++) {
					rc[i] = rc_set[i];
				}
			}
			break;
		default:
			rc_state = 0;
	}
}

/* ------------------------------- rcUpdateSpeedControllers ------------------------------
 @ Summary
    Updates speed of drive motors using balues in the rc array and manages pwm signals
    based on those values.
 @ Parameters
    None
 @ Return Value
    None
  ---------------------------------------------------------------------------- */
void rcUpdateSpeedControllers(void) {
	//static int channel = NRCSERVOS;	 // Channel index starts right after all Servo motors
	static int rc_state = 0;			// Initial signal state
	static int rc1, rc2;				// Signal period timers
	int i;

    invLED2();

	switch (rc_state) {
		case 0:		// Initial high period
            for(i = 0 ; i < NRCSPEEDCONTROLLERS ; i++)
            {
                rc_output(i, TRUE);	// Turn channel on
            }
            if(rc[0] > 50) {
                rc1 = RC_SPEED_CONTROLLER_NEUTRAL + (((rc[0] - 50)/50)*(RC_SPEED_CONTROLLER_MAX-RC_SPEED_CONTROLLER_NEUTRAL));
            }
            else {
                rc1 = RC_SPEED_CONTROLLER_NEUTRAL - (((50 - rc[0])/50)*(RC_SPEED_CONTROLLER_NEUTRAL-RC_SPEED_CONTROLLER_MIN));
            }
			if (rc1 > RC_SPEED_CONTROLLER_MAX)
				rc1 = RC_SPEED_CONTROLLER_MAX;
            else if (rc1 < RC_SPEED_CONTROLLER_MIN)
				rc1 = RC_SPEED_CONTROLLER_MIN;
			rc2 = RC_SPEED_CONTROLLER_PERIOD - rc1;			// Time to end of cycle
			if (rc2 < 0)
				rc2 = 0;
			rc_state++;
			break;							
		case 1:
			if (--rc1 <= 0) {			// Count down cycle ON time
                for(i = 0 ; i < NRCSPEEDCONTROLLERS ; i++)
                {
                    rc_output(i, FALSE);	// Turn channel off
                }
				rc_state++;
			}
			break;
		case 2:
			if(--rc2 <= 0) {			// Count down Cycle off time
				rc_state = 0;			// Reset state counter
				for (i = 0; i < NRCSPEEDCONTROLLERS; i++) {
					rc[i] = rc_set[i];
				}
			}
			break;
		default:
			rc_state = 0;
	}
}
/* --------------------------------- set_rc ----------------------------------
  @ Summary
	 Sets values of global variable rc_set used in rcUpdateServos() and 
     rcUpdateSpeedControllers() to the values of its four parameters. 
     These values designate the on time of each of the four channels.
  @ Parameters
	 @ param1 : An integer (percentage) that corresponds to rc_set[0]
	 @ param2 : An integer (percentage) that corresponds to rc_set[1]
	 @ param3 : An integer (percentage) that corresponds to rc_set[2]
	 @ param4 : An integer (percentage) that corresponds to rc_set[3]
  @ Returns
	 None
  ---------------------------------------------------------------------------- */
void set_rc(int rc1, int rc2, int rc3, int rc4) {
	rc_set[0] = rc1;
	rc_set[1] = rc2;
	rc_set[2] = rc3;
	rc_set[3] = rc4;
}

/* -------------------------------- rc_output --------------------------------
  @ Summary
	 This function sets the RC channel specified by "ch" to the condition
	  specified by "ctrl". Using macros to define the IO pin allows the
	  developer to specify an arbitrary processor pin to each RC channel.
	  Additional channels can be added to the switch statement if so desired.
  @ Parameters
	 @ param1 : An integer corresponding to the channel number being addressed				 
	 @ param2 : An integer that corresponds to what you want the channel to do. 
			 OFF is 0 and On is 1. 
  @ Returns
	 This function has no return value.
  ---------------------------------------------------------------------------- */
void rc_output(int ch, int ctrl) {
	switch (ch) {
		case 0:
			RC_1(ctrl);
			break;
		case 1:
			RC_2(ctrl);
			break;
		case 2:
			RC_3(ctrl);
			break;
		case 3:
			RC_4(ctrl);
			break;
		default:
			break;
	}
}

/* --------------------------------- initRC ----------------------------------
  @ Summary
	 Initializes the PWM RC channels of the Basys MX3
  @ Parameters
	 None
  @ Returns
	 None
  ---------------------------------------------------------------------------- */
void initRC(void) {
	int i;
	for (i = 0; i < NRC; i++) {
		rc[i] = 0;		// Array of RC Channel position Range = 0-100
		rc_set[i] = 0;
	}
	cfgRC1();			// Set RC pins for output
	cfgRC2();
	cfgRC3();
	cfgRC4();

	/* -------------------- Reset all the RC Pins to zero -------------------- */
	RC_1(0);
	RC_2(0);
	RC_3(0);
	RC_4(0);
	
}

int SetDefaultServoPosition()
{
    //DelayMs(500);  
    RC1Pos = RCMid;
    set_rc(RC2Pos, RC2Pos, RCMid, RCMid);
}

/* ------------------------------- TurnLeftPos ------------------------------
 @ Summary
    Function controlling the orientation of the servo motors
 @ Parameters
    @ param1 : integer indicating change to servo orientation
 @ Return Value
    0
  ---------------------------------------------------------------------------- */
int TurnLeftPos(int movement) 
{ 
    // Local variables
    static int pos = 50;
    int Aligned = 0;
    int i = 0;
    RC1Pos-= movement;

    if(RC1Pos <= 0) 
    {   
        RC1Pos = 0;
    }
    else if(RC1Pos >= 130) 
    {   
        RC1Pos = 130;
    }
    else
    {
        set_rc(RC2Pos, RC2Pos, RC1Pos, RC1Pos);
    }
    
    
    // Until the device is aligned in the correct position 
    // Right now this is a busy loop just to be a placeholder so that when the 
    // additional functions get made, they can be inserted more easily.
    /*
    while(!Aligned)
    {   
        i++;
        if(i == 9000000)
            Aligned = 1;  // IfAligned() // Returns a variable to indicate if aligned
    }*/
    DelayMs(10);               // However much additional time for the boat to turn
    //SetDefaultServoPosition();
    
    return 0; 
}

/* ------------------------------- TurnRightPos ------------------------------
 @ Summary
    Function controlling the orientation of the servo motors
 @ Parameters
    @ param1 : integer indicating change to servo orientation
 @ Return Value
    0
  ---------------------------------------------------------------------------- */
int TurnRightPos(int movement) 
{ 
    // Local variables
    static int pos = 50;
    int Aligned = 0;
    int i = 0;
    RC1Pos-= movement;

    if(RC1Pos <= 0) 
    {   
        RC1Pos = 0;
    }
    else if(RC1Pos >= 130) 
    {   
        RC1Pos = 130;
    }
    else
    {
        pos++;
        set_rc(RC2Pos, RC2Pos, RC1Pos, RC1Pos);
    }
    
    
    // Until the device is aligned in the correct position 
    // Right now this is a busy loop just to be a placeholder so that when the 
    // additional functions get made, they can be inserted more easily.
    /*
    while(!Aligned)
    {   
        i++;
        if(i == 9000000)
            Aligned = 1;  // IfAligned() // Returns a variable to indicate if aligned
    }*/
    DelayMs(1);      // However much additional time for the boat to turn
    //SetDefaultServoPosition();
    
    return 0; 
}

/* ------------------------------- ForwardPos ------------------------------
 @ Summary
    Function controlling decreases to power of drive motors
 @ Parameters
    @ param1 : integer indicating change to motor speed
 @ Return Value
    0
  ---------------------------------------------------------------------------- */
int ForwardPos(int movement) 
{ 
    // Local variables                                                                                                                          
    static int pos = 50;
    int Aligned = 0;
    int i = 0;
    RC2Pos-= movement;

    if(RC2Pos <= 0) 
    {   
        RC2Pos = 0;
    }
    else if(RC2Pos >= 100) 
    {   
        RC2Pos = 100;
    }
    else
    {
        set_rc(RC2Pos, RC2Pos, RC1Pos, RC1Pos);
    }
    
    
    // Until the device is aligned in the correct position 
    // Right now this is a busy loop just to be a placeholder so that when the 
    // additional functions get made, they can be inserted more easily.
    /*
    while(!Aligned)
    {   
        i++;
        if(i == 9000000)
            Aligned = 1;  // IfAligned() // Returns a variable to indicate if aligned
    }*/
    DelayMs(10);               // However much additional time for the boat to turn
    //SetDefaultServoPosition();
    
    return 0; 
}

/* ------------------------------- BackwardPos ------------------------------
 @ Summary
    Function controlling increases to power of drive motors
 @ Parameters
    @ param1 : integer indicating change to motor speed
 @ Return Value
    0
  ---------------------------------------------------------------------------- */
int BackwardPos(int movement) 
{ 
    // Local variables
    static int pos = 50;
    int Aligned = 0;
    int i = 0;
    RC2Pos+=movement;
    
    if(RC2Pos >= 100) 
    {   
        RC2Pos = 100;
    }
    else if(RC2Pos <= 0) 
    {   
        RC2Pos = 0;
    }
    else
    {
        pos++;
        set_rc(RC2Pos, RC2Pos, RC1Pos, RC1Pos);
    }
    
    
    // Until the device is aligned in the correct position 
    // Right now this is a busy loop just to be a placeholder so that when the 
    // additional functions get made, they can be inserted more easily.
    /*
    while(!Aligned)
    {   
        i++;
        if(i == 9000000)
            Aligned = 1;  // IfAligned() // Returns a variable to indicate if aligned
    }*/
    DelayMs(1);      // However much additional time for the boat to turn
    //SetDefaultServoPosition();
    
    return 0; 
}