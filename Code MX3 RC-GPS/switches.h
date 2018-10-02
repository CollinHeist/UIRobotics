#ifndef _SWITCHES_H    /* Guard against multiple inclusion */
#define _SWITCHES_H

#include "hardware.h"
#include <plib.h>

    /** Descriptive Constant Name

      @Summary
        Definitions of pin numbers and reading functiuons.
    
      @Description
         Define statements macro definitions to avoid magic numbers    
      @Remarks
        Any additional remarks
     */
/* Macros to configure PIC pins as inputs to sense switch settings */
    #define SW0_bit         BIT_3   // RF3
    #define SW1_bit         BIT_5   // RF5
    #define SW2_bit         BIT_4   // RF4
    #define SW3_bit         BIT_15  // RD15
    #define SW4_bit         BIT_14  // RD14
    #define SW5_bit         BIT_11  // RB11
    #define SW6_bit         BIT_10  // RB10
    #define SW7_bit         BIT_9   // RB9

    #define SW0cfg()    TRISFbits.TRISF3 = 1 
    #define SW1cfg()    TRISFbits.TRISF5 = 1 
    #define SW2cfg()    TRISFbits.TRISF4 = 1 
    #define SW3cfg()    TRISDbits.TRISD15 = 1
    #define SW4cfg()    TRISDbits.TRISD14 = 1
    #define SW5cfg()    (TRISBbits.TRISB11 = 1, ANSELBbits.ANSB11 = 0)
    #define SW6cfg()    (TRISBbits.TRISB10 = 1, ANSELBbits.ANSB10 = 0)
    #define SW7cfg()    (TRISBbits.TRISB9  = 1, ANSELBbits.ANSB9  = 0)
    #define SWcfg()     (SW0cfg(), SW1cfg(), SW2cfg(), SW3cfg(), SW4cfg(), SW5cfg(), SW6cfg(), SW7cfg())

    #define SW0()       PORTFbits.RF3   //PORTReadBits(IOPORT_F, SW0_bit) ? 1 : 0
    #define SW1()       PORTFbits.RF5   //PORTReadBits(IOPORT_F, SW1_bit) ? 1 : 0
    #define SW2()       PORTFbits.RF4   //PORTReadBits(IOPORT_F, SW2_bit) ? 1 : 0
    #define SW3()       PORTDbits.RD15  //PORTReadBits(IOPORT_D, SW3_bit) ? 1 : 0
    #define SW4()       PORTDbits.RD14  //PORTReadBits(IOPORT_D, SW4_bit) ? 1 : 0
    #define SW5()       PORTBbits.RB11  //PORTReadBits(IOPORT_B, SW5_bit) ? 1 : 0
    #define SW6()       PORTBbits.RB10  //PORTReadBits(IOPORT_B, SW6_bit) ? 1 : 0
    #define SW7()       PORTBbits.RB9   //PORTReadBits(IOPORT_B, SW7_bit) ? 1 : 0 

void Switches_init(void);
int Switch2Binary(void);

#endif