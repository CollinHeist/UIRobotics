/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.c

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

/* This section lists the other files that are included in this file.
 */

#include "hardware.h"       // Platform declarations

// XC32 libraries
#include <plib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

// Included files for platform 
#include "swDelay.h"
#include "RC.h"
/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */

/*  A brief description of a section can be given directly below the section
    banner.
 */

/* ************************************************************************** */
/** Descriptive Data Item Name

  @Summary
    Brief one-line summary of the data item.
    
  @Description
    Full description, explaining the purpose and usage of data item.
    <p>
    Additional description in consecutive paragraphs separated by HTML 
    paragraph breaks, as necessary.
    <p>
    Type "JavaDoc" in the "How Do I?" IDE toolbar for more information on tags.
    
  @Remarks
    Any additional remarks
 */
int rc[NRC];                    // Array of RC Channel position Range = 0-100
char BLANK_LINE[] = "                    ";
int switch_flag = TRUE;                 // Force LCD update after reset
int button_flag;

/* ************************************************************************** */
/* ************************************************************************** */
// Section: Local Functions                                                   */
/* ************************************************************************** */
/* ************************************************************************** */

/*  A brief description of a section can be given directly below the section
    banner.
 */

/* ************************************************************************** */

/** 
  @Function
    int ExampleLocalFunctionName ( int param1, int param2 ) 

  @Summary
    Brief one-line description of the function.

  @Description
    Full description, explaining the purpose and usage of the function.
    <p>
    Additional description in consecutive paragraphs separated by HTML 
    paragraph breaks, as necessary.
    <p>
    Type "JavaDoc" in the "How Do I?" IDE toolbar for more information on tags.

  @Precondition
    List and describe any required preconditions. If there are no preconditions,
    enter "None."

  @Parameters
    @param param1 Describe the first parameter to the function.
    
    @param param2 Describe the second parameter to the function.

  @Returns
    List (if feasible) and describe the return values of the function.
    <ul>
      <li>1   Indicates an error occurred
      <li>0   Indicates an error did not occur
    </ul>

  @Remarks
    Describe any special behavior not described above.
    <p>
    Any additional remarks.

  @Example
    @code
    if(ExampleFunctionName(1, 2) == 0)
    {
        return 3;
    }
 */
//static int ExampleLocalFunction(int param1, int param2) {
//    return 0;
// }


/* ************************************************************************** */
/* ************************************************************************** */
// Section: Interface Functions                                               */
/* ************************************************************************** */
/* ************************************************************************** */

/*  A brief description of a section can be given directly below the section
    banner.
 */

// *****************************************************************************

/* START FUNCTION DESCRIPTION ********************************************
Timer1Handler
SYNTAX:			Timer1Handler(void);
KEYWORDS:		hardware delay, interrupt, Timer 1
DESCRIPTION:   	Processes the code associated with a Timer 1 interrupt.
				It is responsible for generating the signals for each 
				RC channel in two phases: The three state FSM control the
				on period and the off period.
PARAMETERS:		None are allowed for an ISR
RETURN VALUE:  	None are allowed for an ISR
Notes:			This function cannot be called from any other function.
END DESCRIPTION **********************************************************/
void __ISR(_TIMER_1_VECTOR, IPL2SOFT) Timer1Handler(void)

{
static int channel = 0;			// Channel index 
static int rc_state = 0;		// Initial signal state
static int rc1, rc2;			// Signal period timers
static ms = 0;
    
    mT1ClearIntFlag();			// clear the interrupt flag
    ms++;
    if(ms>=0)
    {
        millisec++;
        ms = 0;
    }
	
	switch(rc_state)
	{
		case 0:		// Initial high period - rc1 and rc2 are set here
			set_RC(channel, TRUE);		// Turn channel on
			rc1 = RC_MIN + rc[channel];	// Compute on time
			if(rc1 > RC_MAX)			// Check limits
				rc1 = RC_MAX;
			rc2 = RC_MAX - rc1;			// Time to end of cycle
			if(rc2 < 0)					// Check limits
				rc2 = 0;
			rc_state++;					// Set for next state
			break;							
		case 1:
			if(--rc1 <= 0)				// Count down cycle ON time
			{
				set_RC(channel, FALSE);	// Turn channel off
				rc_state++;				// Set for next state
			}
			break;
		case 2:
			if(--rc2 <= 0)				// Count down Cycle off time
			{
				rc_state = 0;			// Reset to initial state 
				channel = (channel +1) % NRC;  // Next channel
				if(channel == 0)		// Update channel times only once a cycle
					button_flag = check_buttons();	// Use set inputs	
			}
			break;
	}
}

/* START FUNCTION DESCRIPTION ********************************************
SYNTAX:			void set_RC(int ch, int ctrl);
KEYWORDS:		RC, control, output
DESCRIPTION:   	This function sets the RC channel specified by "ch" to the 
				the condition specified by "ctrl". Using macros to define the
				IO pin allows the developer to specify an arbitrary processor
				pin to each RC channel.
PARAMETER1:		channel number
PARAMETER2:		control - OFF - 0,  ON - 1  
RETURN VALUE:  	None
Notes:			Switch-case can be expanded to accommodate  additional 
				channels.  Most systems are limited to 10 channels
END DESCRIPTION **********************************************************/

void set_RC(int ch, int ctrl)
{
	switch(ch)
	{
		case 0:
			RC_1(ctrl);
			break;
		case 1:
			RC_2(ctrl);
			break;
// add additional channels here.
		default:
			break;
	}
}

/* START FUNCTION DESCRIPTION ********************************************
SYNTAX:			int check_buttons(void);
KEYWORDS:		buttons, decode
DESCRIPTION:   	This function polls the three bush buttons on the 
				Cerebot PIC32 MX7ck board and sets the RC positions
				to a common value. The RC channel positions are not changed
				unless there has been a change of button state.   
PARAMETERS:		None
RETURN VALUE:  	1 = new button condition, 0 = no new button condition
Notes:			The button(s) pressed are encoded using a 1 hot code.  
				Button1 = 0/1, Button2 = 0/2, and Button3 = 0/4;
END DESCRIPTION **********************************************************/
int check_buttons(void)
{
static int b2 = 0xff;	// force setting RC channels when initialized
int b1; 		// b1 does not need to be static - the newest status
int setting;	// RC servo position setting
int ch;			// RC channel index
	
	b1 = ((int) SW0()) | ((int) SW1()<<1) | ((int) SW2()<<2);	

	if(b1 != b2)  // Is this a new button setting?
	{
		switch(b1)	// Set all RC units to the same setting
		{
			case 0: 
				setting = RC_SPAN/2;
				break;
			case 1:				
				setting = 0;
				break;
			case 2:				
				setting = RC_SPAN/4;
				break;
			case 4:				
				setting = (3*RC_SPAN)/4;
				break;
			default:
				setting = RC_SPAN;
		}

		for(ch = 0; ch<NRC; ch++)	// Set all RC channels to the same setting
		{
			rc[ch] = setting;
		}
		b2 = b1;		// Remember this button setting
		return 1;		// Button CoS
	}
	else
		return 0;		// No button CoS
}/* START FUNCTION DESCRIPTION ********************************************
void initRC
SYNTAX:  		void initRC(void);
KEYWORDS:		RC
DESCRIPTION:	initialization two channel PWM RC channels of the BAsys MX3 
PARAMETERS:     none
RETURN VALUE:   none
NOTES:			See RC.h for pin definitions
END DESCRIPTION **********************************************************/
void initRC(void)
{
	cfgRC1();		// Set RC pins for output
	cfgRC2();		
    RC_1(0);        // Reset all RC pins to zero
    RC_2(0);   
}


/* START FUNCTION DESCRIPTION ********************************************
SYNTAX:			int check_switches(void);
KEYWORDS:		switches, decode
DESCRIPTION:   	This function polls the three bush buttons on the 
				Basys MX3 board and sets the RC positions
				to a common value. The RC channel positions are not changed
				unless there has been a change of button state.   
PARAMETERS:		None
RETURN VALUE:  	1 = new switch condition, 0 = no new switch condition
Notes:			The switches on are encoded using a 1 hot code.  
				SW0 = 0/1, SW1 = 0/2, and SW3 = 0/4;
END DESCRIPTION **********************************************************/
int check_switches(void)
{
static int s2 = 0xff;	// force setting RC channels when initialized
int s1; 		// s1 does not need to be static - the newest status
int setting;	// RC servo position setting
int ch;			// RC channel index
	
	s1 = ((int) SW0()) | ((int) SW1()<<1) | ((int) SW2()<<2);	

	if(s1 != s2)  // Is this a new switch setting?
	{
		switch(s1)	// Set all RC units to the same setting
		{
			case 0: 
				setting = RC_SPAN/2;
				break;
			case 1:				
				setting = 0;
				break;
			case 2:				
				setting = RC_SPAN/4;
				break;
			case 4:				
				setting = (3*RC_SPAN)/4;
				break;
			default:
				setting = RC_SPAN;
		}

		for(ch = 0; ch<NRC; ch++)	// Set all RC channels to the same setting
		{
			rc[ch] = setting;
		}
		s2 = s1;                    // Remember this switch setting
		return TRUE;                // switch CoS
	}
	else
		return FALSE;               // No switch CoS
}

/* *****************************************************************************
 End of File
 */
