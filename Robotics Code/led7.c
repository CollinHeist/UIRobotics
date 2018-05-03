/* --------------------------------- led7.c ----------------------------------
  @ Summary
     Four digit - seven segment LED display library.
  @ Description
     Displays decimal values of a four digit - seven segment display
  ---------------------------------------------------------------------------- */

/* --------------------- Necessary system include statements ----------------- */
#include "swDelay.h"
#include "led7.h"
#include "hardware.h"
#include <plib.h>
#include <stdint.h>

/* ------------------ (Global) yet local variable declarations --------------- */
int16_t led_value = 0;
BOOL led_flag = 0;

/* -----------------------------. seg7_init() ---------------------------------
  @ Syntax
     seg7_init();
  @ Description
     The assigned PIC32 IO pins are assigned as outputs and the digit LED 
     anodes are set to zero. This function initializes all the 7-segment IO
     pins as outputs.
     This should be called at the beginning of the main project.
  @ Parameters
     None
  @ Returns
     None
  ---------------------------------------------------------------------------- */
void seg7_init(void) {
    Seg7cfg();
    DIGITS_OFF();
    clr_dsp();
    test_7seg_leds();
    DIGITS_OFF();
    clr_dsp();
    
    /* -------- This interrupts signals when to switch the LED digits -------- */
    OpenTimer1(T1_ON | T1_SOURCE_INT | T1_PS_1_8, T1_TICK);
    mT1SetIntPriority(1);      // Set Timer 1 group priority level 1
    mT1SetIntSubPriority(1);   // Set Timer 1 subgroup priority level 1 
    mT1IntEnable(1);           // Enable T1 interrupts 
    
    led_flag = 1;
    led_value = 8888;
    DelayMs(1000);

    led_value = 0;
    DelayMs(1000);

    led_value = 8888;
    DelayMs(1000);

    led_value = 0;
    DelayMs(100);
    led_flag = 0;
}

/* ------------------------------- clr_dsp() ---------------------------------
  @ Syntax
     clr_dsp();
  @ Description
     Turns off all segments of the LEDs.
  @ Parameters
     None
  @ Returns
     None
  ---------------------------------------------------------------------------- */
void clr_dsp (void) {
    SEG_CA(0);      
    SEG_CB(0);
    SEG_CC(0);
    SEG_CD(0);
    SEG_CE(0);
    SEG_CF(0);
    SEG_CG(0);
    SEG_DP(0);
}

/* ------------------------------ set_digit()  -------------------------------
  @ Syntax
     set_digit(int dsp, int value);
  @ Description
     Turn on the passed display [0, 3] of the seven-segment display to the passed
     character. Uses the DIG_AN#() functions that are active-low. If a non-accepted
     display value is passed, then all the LEDs are turned off.
  @ Parameters
     @ param1 : integer number of which LED to turn on. Expects 0, 1, 2, or 3
     @ param2 : integer value to display to the passed LED.
  @ Returns
     None
  ---------------------------------------------------------------------------- */
void set_digit(int dsp, int value) {
    dsp_digit(value);       // Set LED cathodes according to segment number
    switch (dsp) {
        case 0:
            DIG_AN0(0);
            DIG_AN1(1);
            DIG_AN2(1);
            DIG_AN3(1);
            break;
        case 1:
            DIG_AN0(1);
            DIG_AN1(0);
            DIG_AN2(1);
            DIG_AN3(1);
            break;
        case 2:
            DIG_AN0(1);
            DIG_AN1(1);
            DIG_AN2(0);
            DIG_AN3(1);
            break;
        case 3:
            DIG_AN0(1);
            DIG_AN1(1);
            DIG_AN2(1);
            DIG_AN3(0);
            break;
        default:
            DIG_AN0(1);
            DIG_AN1(1);
            DIG_AN2(1);
            DIG_AN3(1);
    }
}

/* ------------------------------ dsp_digit()  -------------------------------
  @ Syntax
     dsp_digit(9);
  @ Description
     Turns on the necessary LED segments to display the required digit.
  @ Parameters
     @ param1 : integer value to be displayed at the LEDs.
  @ Returns
     None
  ---------------------------------------------------------------------------- */
void dsp_digit (int value) {
    switch (value) {      
        case 0:
            SEG_LED0
            break;
        case 1:
            SEG_LED1
            break;
        case 2:
            SEG_LED2
            break;
        case 3:
            SEG_LED3
            break;
        case 4:
            SEG_LED4
            break;
        case 5:
            SEG_LED5
            break;
        case 6:
            SEG_LED6
            break;
        case 7:
            SEG_LED7
            break;
        case 8:
            SEG_LED8
            break;
        case 9:
            SEG_LED9
            break;
        case -1:
            SEG_NEG    // generate negative sign
            break;            
        default:
            SEG_OFF    // Blank the display
    }
}

/* ----------------------------- led_number()  -------------------------------
  @ Syntax
     led_number(int value);
  @ Description
     Displays any number between [-999, 9999] on the 7-segment display with leading zero.
  @ Parameters
     @ param1 : Value to be displayed. Ranged between -999 and 9999.
  @ Returns
     None
  ---------------------------------------------------------------------------- */
void led_number (int value) {
    int d1, d2, d3, d4, dz = 0;     // Segment values and display zero flag
    
    /* --------- Test that the value is not greater than the maximum --------- */
    if (value < 10000) {
        value = ((value < 0) && (value > -1000)) ? -value : value % 1000;
        d4    = ((value < 0) && (value > -1000)) ? -1     : value / 1000;
//        if ((value < 0) && (value > -1000)) {
//            value = -value;         // Negate value to make positive
//            d4 = -1;                // Assign digit 4 to negative 1
//        }
//        else {
//            d4 = value / 1000;      // Isolate 1000's digit
//            value = value % 1000;   // Save remainder
//        }
        d3    = value / 100;          // Isolate 100's digit
        value = value % 100;          // Save remainder
        d2    = value / 10;           // Isolate 10's digit
        value = value % 10;           // Save remainder
        d1    = value;                // Isolate units digit

        if (d4) {
            set_digit(3, d4);       // If non zero or minus sign - display digit
            if (d4 > 0)
                dz = 1;             // Set display zero flag
        }
        else
            set_digit(3, 0xFF);     // If leading zero - blank digit
        
        DelayUs(DIG_DLY);           // Give the board time to turn on the display

        if (d3 || dz == 1) {        // Display if nonzero or dz flag is set
            set_digit(2, d3);
            dz = 1;                 // Set display zero flag
        }
        else
            set_digit(2, 0xFF);     // If leading zero - blank digit
        
        DelayUs(DIG_DLY);           // Give the board time to turn on the display

        if (d2 || dz == 1) {        // Display if nonzero or dz flag is set
            set_digit(1, d2);       // Set display zero flag
        }
        else
            set_digit(1, 0xFF);     // If leading zero - blank digit
        
        DelayUs(DIG_DLY);
        set_digit(0, d1);           // Display trailing zeros
        DelayUs(DIG_DLY);

        DIGITS_OFF(); 
    }
}

/* -------------------------- test_7seg_leds() -------------------------------
  @ Syntax
     test_7seg_leds();
  @ Description
     Cycles each of the 7-segments (A-F) are displayed. Then shifted left to right.
  @ Parameters
     None
  @ Returns
     None
  ---------------------------------------------------------------------------- */
void test_7seg_leds (void) {
    DIG_AN0(0);
    SEG_CA(1);
    DelayMs(100);
    SEG_CB(1);
    DelayMs(100);
    SEG_CC(1);
    DelayMs(100);
    SEG_CD(1);
    DelayMs(100);
    SEG_CE(1);
    DelayMs(100);
    SEG_CF(1);
    DelayMs(100);
    SEG_CG(1);
    DelayMs(100);
    SEG_DP(1);
    DelayMs(250);
    DIG_AN0(1);
    DIG_AN1(0);
    DelayMs(250);
    DIG_AN1(1);
    DIG_AN2(0);
    DelayMs(250);
    DIG_AN2(1);
    DIG_AN3(0);
    DelayMs(200);
    clr_dsp();
    DIG_AN3(1);   
}

/* ------------------------------- __ISR() ----------------------------------
  @ Syntax
     T1Interrupt();
  @ Description
     Updates the 4-digit 7-segment display according to the value set in the
     global variable "led_value." This can be toggled by setting led_flag to 0 or 1.
  @ Parameters
     None
  @ Returns
     None
  ---------------------------------------------------------------------------- */
void __ISR( _TIMER_1_VECTOR, IPL1SOFT) T1Interrupt(void) {
    static int led_disp = 3;        // Seven-segment LED digit display index
    static int led_digit;           // Seven-segment LED digit display value
    static int ms_cntr = 2;

    if (--ms_cntr <= 0) {           // Measure out 2 tenths of a second 
        ms_cntr = 2;
        if (led_flag && (led_value < 10000) && (led_value > -1000)) {
            switch (led_disp) {     // Determine the display digit
                case 0:
                    led_digit = abs(led_value) % 10;           // Units digit
                    break;
                case 1:
                    led_digit = (abs(led_value) % 100) / 10;   // Tens digit
                    break;
                case 2:
                    led_digit = (abs(led_value) % 1000) / 100; // 100s digit
                    break;
                case 3:
                    led_digit = ((led_value < 0) && (led_value > -1000)) ? -1 : led_value / 1000;
            }
            set_digit(led_disp, led_digit);       // Display digit value
            if (--led_disp < 0) {                 // Update display index modulo 4
                led_disp = 3;
            }
        }
        else {
            DIGITS_OFF();
            clr_dsp();
        }
    }
    
    mT1ClearIntFlag();
}
