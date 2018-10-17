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
int rc_set[NRC];


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
void rcUpdate(void)
{
static int channel = 0;			// Channel index 0<= channel NRC
static int rc_state = 0;		// Initial signal state
static int rc1, rc2;            // Signal period timers
int i;

/* RC output control */
	switch(rc_state)
	{
		case 0:		// Initial high period - rc1 through rc4 are set here
            rc_output(channel, TRUE);	// Turn channel on
            rc1 = RC_MIN + rc[channel];	// Compute on time
            if(rc1 > RC_MAX)
                rc1 = RC_MAX;
            rc2 = RC_MAX - rc1;		// Time to end of cycle
            if(rc2 < 0)
                rc2 = 0;
            rc_state++;
			break;							
		case 1:
            if(--rc1 <= 0)	// Count down cycle ON time
            {
                rc_output(channel, FALSE);	// Turn channel off
                rc_state++;
            }
			break;
		case 2:
            if(--rc2 <= 0)	// Count down Cycle off time
            {
                rc_state = 0;		// Reset state counter
                channel = (channel +1) % NRC;  // Next channel
                for(i=0; i< NRC; i++)
                {
                    rc[i] = rc_set[i];
                }
            }
            break;
        default:
            rc_state = 0;
	}
}

/* START FUNCTION DESCRIPTION ********************************************
SYNTAX:			void rc_output(int ch, int ctrl);
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

void set_rc(int rc1, int rc2, int rc3, int rc4)
{
	rc_set[0] = rc1;
	rc_set[1] = rc2;
	rc_set[2] = rc3;
	rc_set[3] = rc4;
}


/* START FUNCTION DESCRIPTION ********************************************
SYNTAX:			void rc_output(int ch, int ctrl);
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

void rc_output(int ch, int ctrl)
{
	switch(ch)
	{
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
// add additional channels here.
		default:
			break;
	}
}

/* START FUNCTION DESCRIPTION ********************************************
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
int i;
    for(i=0; i< NRC; i++)
    {
        rc[i] = 0;                    // Array of RC Channel position Range = 0-100
        rc_set[i] = 0;
    }
	cfgRC1();		// Set RC pins for output
	cfgRC2();		
	cfgRC3();		
	cfgRC4();		
    RC_1(0);        // Reset all RC pins to zero
    RC_2(0);   
    RC_3(0);   
    RC_4(0);   
    
}

/* *****************************************************************************
 End of File
 */
