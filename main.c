/* @Description
   Continuously reads the PmodGPS and displays the available time and date on 
   the system LCD. Pressing BTNR, BTND, or BTNC will change the GPS sentence
   type. LED0 is toggles each time a new sentence is received from the GPS.  
  */

// System included files
#include "config_bits.h"
#include "hardware.h"
#include "main.h"
#include <plib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// Platform common included files
#include "swDelay.h"

// Application included files
#include "uart4.h"  // Monitor UART
#include "uart2.h"  // GPS UART
#include "led7.h"
#include "LCDlib.h"

int calc_ck_sum(char *str);
// Global variables
int gps_message = 0;        // Active GPS sentence 
extern int16_t led_value;
extern BOOL led_flag;

/** 
  @Description
    System initialization and infinite application loop. Reads the GPS sentence 
    and decodes the sentence before displaying on the LCD. Also facilitates
    changing GPS sentences.

 */
int main(void) {
    char ch2, ch4;                                            // Received character
    BOOL rx2_flag, rx4_flag;                             // Character ready

    Hardware_Setup();               // Initialize common IO
    initLCD();                      // Local real time display
    uart4_init(38400, NO_PARITY);   // PC Terminal
    uart2_init(9600, NO_PARITY);    // GPS 
    seg7_init();                    // Displays seconds only
    led_flag = 1;
    
    while(1)	// Background process handles UART communications 
    {
        rx2_flag = getcU2(&ch2);  // Poll for character received
        if(rx2_flag)
        {
            putcU4(ch2);                 // Send received character to monitor
        }        

        rx4_flag = getcU4(&ch4);  // Poll for character received
        if(rx4_flag)
        {
            putcU2(ch4);                 // Send received character to monitor
        }        
    }
    return EXIT_FAILURE; // COde execution should never get to this statement 
}

