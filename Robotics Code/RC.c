#include "RC.h"

int rc[NRC];
int rc_set[NRC];

/* -------------------------------- rcUpdate ---------------------------------
  @ Summary
	 Updates information stored in array rc based on rc_set as it cycles
	 through states based on the value of static variable rc_state and
	 information from RC_MIN and RC_MAX. 
  @ Parameters
	 None
  @ Returns
	 None
  ---------------------------------------------------------------------------- */
void rcUpdate(void) {
	static int channel = 0;				// Channel index 0<= channel NRC
	static int rc_state = 0;			// Initial signal state
	static int rc1, rc2;				// Signal period timers
	int i;

	switch (rc_state) {
		case 0:		// Initial high period
			rc_output(channel, TRUE);	// Turn channel on
			rc1 = RC_MIN + rc[channel];	// Compute on time
			if (rc1 > RC_MAX)
				rc1 = RC_MAX;
			rc2 = RC_MAX - rc1;			// Time to end of cycle
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
				channel = (channel +1) % NRC;	// Next channel
				for (i = 0; i < NRC; i++) {
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
	 Sets values of global variable rc_set used in rcUpdate() to the values
	 of its four parameters. These values designate the on time of
	 each of the four channels.
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