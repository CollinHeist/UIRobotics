#include "config_bits.h"    // Processor configuration
#include "hardware.h"       // Platform declarations

// XC32 libraries
#include <plib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

// Included files for platform 
#include "LCDlib.h"
#include "led7.h"
#include "comm_lib.h"
#include "swDelay.h"
#include "RC-GPS.h"
#include "GPS.h"

int rc[NRC];		// Array of RC Channel position Range = 0-100
char BLANK_LINE[] = "                    ";
int button_flag = TRUE;	// Force LCD update after reset
float length;
float angle;
/* 4146.21 meters at 168.57 degrees*/
float lat1 =  46.760931;
float lon1 = -117.015126;
float lat2 =  46.797898;
float lon2 = -117.025901;

int main(void) {
	char lcdstr[40];	// Local LCD display buffers
	char uartstr[80];	// Local UART input buffer
	int ch, pos;		// Used for UART entry

	Hardware_Setup();       // Initialize common hardware
	initLCD();				// Initialize LCD
	uart4_init(38400, NO_PARITY);	// Initialize UART
	initRC();				// Initialize RC channel output pins
	DelayMs(2000);			// Wait a second
	clrLCD();				// Clear hello messages
	putsLCD("RC Demo");		// Hello messages
	printf("\n\rDr. Wall's RC Demo\n\r");
	printf("Enter servo channel and position\n\r");
	DelayMs(2000);			// Wait a second
	check_buttons();		// Initialize RC times
	initTimer1();			// Start timer interrupt
	clrLCD();				// Clear hello messages
	while(1) {
        length = distance(lat1, lon1, lat2, lon2, 'K');
    	sprintf(lcdstr,"Meters = %d", (int)length );
    	clrLCD();				// Clear hello messages
		putsLCD(lcdstr);	// Post channel settings to LCD line 1			
        printf("%s at ", lcdstr);	// Send channel setting to UART
        angle = angleFromCoordinate(lat1, lon1, lat2, lon2);
    	sprintf(lcdstr,"%d degrees", (int)angle );
		gotoLCD(16);			// Blank out LCD line 1    
		putsLCD(lcdstr);	// Post channel settings to LCD line 1			
        printf("%s deg. \n\r", lcdstr);	// Send channel setting to UART
    	DelayMs(2000);			// Wait a second

        
/*
		if(button_flag)		// Update HMIs if button CoS
		{	
			button_flag = FALSE;;
			sprintf(lcdstr,"%3d %3d",rc[0],rc[1]);
			gotoLCD(0);			// Blank out LCD line 1
			putsLCD(BLANK_LINE);
			gotoLCD(0);
			putsLCD(lcdstr);	// Post channel settings to LCD line 1
			printf("%s\n\r", lcdstr);	// Send channel setting to UART
		}
*/
/*
		if(getstrU4( uartstr, strlen(uartstr) ) > 0)	// Test for serial string ready
		{
			gotoLCD(16);			// Blank out LCD line 1
			putsLCD(BLANK_LINE);
			gotoLCD(16);			// Blank out LCD line 1
			putsLCD(uartstr);	// Copy string to LCD line 2
			sscanf(uartstr,"%d %d",&ch, &pos); // Decode entry
			if(ch >= 0 && ch < NRC)	// Check RC channel number range
			{
				if(pos >=0 && pos <= 100)	// Check RC position range
				{
					rc[ch] = pos*RC_SPAN/100; // Update RC channel position
					button_flag = TRUE;	// Force update of LCD Line 1
				}
			}		
		}
*/
	}
	return 0;
}

void initRC(void) {
	cfgRC1();		// Set RC pins for output
	cfgRC2();		
    RC_1(0);        // Reset all RC pins to zero
    RC_2(0);
    
}

void initTimer1(void) {
	#define TMR1_TICK	100

    OpenTimer1(T1_ON | T1_SOURCE_INT | T1_PS_1_1, TMR1_TICK);
    
	// Set Timer 1 interrupt with a priority of 2
    ConfigIntTimer1(T1_INT_ON | T1_INT_PRIOR_2);

	// Enable multi-vector interrupts
    INTEnableSystemMultiVectoredInt();

}
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
void __ISR(_TIMER_1_VECTOR, IPL2SOFT) Timer1Handler(void) {
	static int channel = 0;			// Channel index 
	static int rc_state = 0;		// Initial signal state
	static int rc1, rc2;			// Signal period timers

    mT1ClearIntFlag();			// clear the interrupt flag
	
	switch (rc_state) {
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
}