/*
 *  RC.c
 *  Commenter: Sam
 * 
 *  Purpose: Enable functionality of the RC motors 
 */

// File Inclusion
#include "Hardware.h"
#include "SWDelay.h"
#include <plib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <Math.h>
#include "RC.h"

int rc[NRC];
int rcSet[NRC];

const int rcMid = 50;
const int rcRight = 130;
const int rcLeft = 0;

int rc1Pos = 50;
int rc2Pos = 50;
/* -------------------------------- rcUpdate ---------------------------------
  @ Summary
	 Updates information stored in array rc based on rcSet as it cycles
	 through states based on the value of static variable rcState and
	 information from RC_SERVO_MIN and RC_SERVO_MAX. 
  @ Parameters
	 None
  @ Returns
	 None
  ---------------------------------------------------------------------------- */
void rcUpdateServos(void) {
	static int channel = NRCSPEEDCONTROLLERS;   // Channel index 0<= channel NRC
	static int rcState = 0;			        // Initial signal state
	static int rc1, rc2;				      // Signal period timers
	int i;

    invLED1();

	switch (rcState) {
		case 0:		// Initial high period
			rcOutput(channel, TRUE);	// Turn channel on
			rc1 = RC_SERVO_MIN + rc[channel];	// Compute on time
			if (rc1 > RC_SERVO_MAX)
				rc1 = RC_SERVO_MAX;
			rc2 = RC_SERVO_MAX - rc1;			// Time to end of cycle
			if (rc2 < 0)
				rc2 = 0;
			rcState++;
			break;							
		case 1:
			if (--rc1 <= 0) {			// Count down cycle ON time
				rcOutput(channel, FALSE);	// Turn channel off
				rcState++;
			}
			break;
		case 2:
			if(--rc2 <= 0) {			// Count down Cycle off time
				rcState = 0;			// Reset state counter
				channel = NRCSPEEDCONTROLLERS + (((channel-NRCSPEEDCONTROLLERS) + 1) % (NRCSERVOS));	// Next channel
				for (i = NRCSPEEDCONTROLLERS; i < NRC; i++) {
					rc[i] = rcSet[i];
				}
			}
			break;
		default:
			rcState = 0;
	}
}

void rcUpdateSpeedControllers(void) {
	//static int channel = NRCSERVOS;	 // Channel index starts right after all Servo motors
	static int rcState = 0;			// Initial signal state
	static int rc1, rc2;				// Signal period timers
	int i;

    invLED2();

	switch (rcState) {
		case 0:		// Initial high period
            for(i = 0 ; i < NRCSPEEDCONTROLLERS ; i++)
            {
                rcOutput(i, TRUE);	// Turn channel on
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
			rcState++;
			break;							
		case 1:
			if (--rc1 <= 0) {			// Count down cycle ON time
                for(i = 0 ; i < NRCSPEEDCONTROLLERS ; i++)
                {
                    rcOutput(i, FALSE);	// Turn channel off
                }
				rcState++;
			}
			break;
		case 2:
			if(--rc2 <= 0) {			// Count down Cycle off time
				rcState = 0;			// Reset state counter
				for (i = 0; i < NRCSPEEDCONTROLLERS; i++) {
					rc[i] = rcSet[i];
				}
			}
			break;
		default:
			rcState = 0;
	}
}
/* --------------------------------- setRC ----------------------------------
  @ Summary
	 Sets values of global variable rcSet used in rcUpdateServos() and 
     rcUpdateSpeedControllers() to the values of its four parameters. 
     These values designate the on time of each of the four channels.
  @ Parameters
	 @ param1 : An integer (percentage) that corresponds to rcSet[0]
	 @ param2 : An integer (percentage) that corresponds to rcSet[1]
	 @ param3 : An integer (percentage) that corresponds to rcSet[2]
	 @ param4 : An integer (percentage) that corresponds to rcSet[3]
  @ Returns
	 None
  ---------------------------------------------------------------------------- */
void setRC(int rc1, int rc2, int rc3, int rc4) {
	rcSet[0] = rc1;
	rcSet[1] = rc2;
	rcSet[2] = rc3;
	rcSet[3] = rc4;
}

/* -------------------------------- rcOutput --------------------------------
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
void rcOutput(int ch, int ctrl) {
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
		rcSet[i] = 0;
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
    rc1Pos = rcMid;
    setRC(rc2Pos, rc2Pos, rcMid, rcMid);
}

//
// TurnLeft() 
// Early version of the a turn left function for the 
// motor angle servo motors, in pin positions JB3 and JB4.
// This function is designed so that the motor will turn left 
// from its default position until the device is properly aligned.
// Then the function will reset the motor back into its default position.
//
int turnLeft() 
{ 
    // Local variables
    static int pos = 0;
    int aligned = 0;
    int i = 0;
    
    pos--;
    
    if(pos == rcLeft) pos = 0;
    
    setRC(rc1Pos--, pos, pos, pos);
    
    // Until the device is aligned in the correct position 
    // Right now this is a busy loop just to be a placeholder so that when the 
    // additional functions get made, they can be inserted more easily.
    /*
    while(!aligned)
    {   
        i++;
        if(i == 9000000)
            aligned = 1;  // Ifaligned() // Returns a variable to indicate if aligned
    }*/
    DelayMs(1);      // However much additional time for the boat to turn
    //SetDefaultServoPosition();
    
    return 0; 
}

int turnLeftPos(int movement) 
{ 
    // Local variables
    static int pos = 50;
    int aligned = 0;
    int i = 0;
    rc1Pos-= movement;

    if(rc1Pos <= 0) 
    {   
        rc1Pos = 0;
    }
    else if(rc1Pos >= 130) 
    {   
        rc1Pos = 130;
    }
    else
    {
        setRC(rc2Pos, rc2Pos, rc1Pos, rc1Pos);
    }
    
    
    // Until the device is aligned in the correct position 
    // Right now this is a busy loop just to be a placeholder so that when the 
    // additional functions get made, they can be inserted more easily.
    /*
    while(!aligned)
    {   
        i++;
        if(i == 9000000)
            aligned = 1;  // Ifaligned() // Returns a variable to indicate if aligned
    }*/
    DelayMs(10);               // However much additional time for the boat to turn
    //SetDefaultServoPosition();
    
    return 0; 
}

//
// TurnRight() 
// Early version of the a turn right function for the 
// motor angle servo motors, in pin positions JB3 and JB4.
// This function is designed so that the motor will turn right 
// from its default position until the device is properly aligned.
// Then the function will reset the motor back into its default position.
//
int turnRight() 
{ 
    // Local variables
    int aligned = 0;
    int i = 0;
    
    setRC(rcRight, rcRight, rcRight, rcRight); // Set to the appropriate position to turn the motors
    
    // Until the device is aligned in the correct position
    // Right now this is a busy loop just to be a placeholder so that when the 
    // additional functions get made, they can be inserted more easily.
    while(!aligned)
    {   
        i++;
        if(i == 9000000)
            aligned = 1;  // Ifaligned() // Returns a variable to indicate if aligned
    }
    
    //DelayMs(3000);               // However much additional time for the boat to turn
    SetDefaultServoPosition();
    
    return 0; 
}

int TurnRightPos(int movement) 
{ 
    // Local variables
    static int pos = 50;
    int aligned = 0;
    int i = 0;
    rc1Pos-= movement;

    if(rc1Pos <= 0) 
    {   
        rc1Pos = 0;
    }
    else if(rc1Pos >= 130) 
    {   
        rc1Pos = 130;
    }
    else
    {
        pos++;
        setRC(rc2Pos, rc2Pos, rc1Pos, rc1Pos);
    }
    
    
    // Until the device is aligned in the correct position 
    // Right now this is a busy loop just to be a placeholder so that when the 
    // additional functions get made, they can be inserted more easily.
    /*
    while(!aligned)
    {   
        i++;
        if(i == 9000000)
            aligned = 1;  // Ifaligned() // Returns a variable to indicate if aligned
    }*/
    DelayMs(1);      // However much additional time for the boat to turn
    //SetDefaultServoPosition();
    
    return 0; 
}

int ForwardPos(int movement) 
{ 
    // Local variables                                                                                                                          
    static int pos = 50;
    int aligned = 0;
    int i = 0;
    rc2Pos-= movement;

    if(rc2Pos <= 0) 
    {   
        rc2Pos = 0;
    }
    else if(rc2Pos >= 100) 
    {   
        rc2Pos = 100;
    }
    else
    {
        setRC(rc2Pos, rc2Pos, rc1Pos, rc1Pos);
    }
    
    
    // Until the device is aligned in the correct position 
    // Right now this is a busy loop just to be a placeholder so that when the 
    // additional functions get made, they can be inserted more easily.
    /*
    while(!aligned)
    {   
        i++;
        if(i == 9000000)
            aligned = 1;  // Ifaligned() // Returns a variable to indicate if aligned
    }*/
    DelayMs(10);               // However much additional time for the boat to turn
    //SetDefaultServoPosition();
    
    return 0; 
}

int BackwardPos(int movement) 
{ 
    // Local variables
    static int pos = 50;
    int aligned = 0;
    int i = 0;
    rc2Pos+=movement;
    
    if(rc2Pos >= 100) 
    {   
        rc2Pos = 100;
    }
    else if(rc2Pos <= 0) 
    {   
        rc2Pos = 0;
    }
    else
    {
        pos++;
        setRC(rc2Pos, rc2Pos, rc1Pos, rc1Pos);
    }
    
    
    // Until the device is aligned in the correct position 
    // Right now this is a busy loop just to be a placeholder so that when the 
    // additional functions get made, they can be inserted more easily.
    /*
    while(!aligned)
    {   
        i++;
        if(i == 9000000)
            aligned = 1;  // Ifaligned() // Returns a variable to indicate if aligned
    }*/
    DelayMs(1);      // However much additional time for the boat to turn
    //SetDefaultServoPosition();
    
    return 0; 
}