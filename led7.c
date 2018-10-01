
#include "swDelay.h"
#include "led7.h"
#include "hardware.h"
#include <plib.h>
#include <stdint.h>

int16_t led_value = 0;
BOOL led_flag = 0;
void seg7_init(void) {
    Seg7cfg();
    DIGITS_OFF();
    clr_dsp();
    test_7seg_leds();
    msDelay(500);
    DIGITS_OFF();
    clr_dsp();
    
    // Configure Timer 1 using internal clock, 1:256 pre-scale 
    OpenTimer1(T1_ON | T1_SOURCE_INT | T1_PS_1_1, T1_TICK);

    //  This interrupts signals the end of (or beginning of next) PWM period
    mT1SetIntPriority(1);       // Set Timer 1 group priority level 1
    mT1SetIntSubPriority(1);    // Set Timer 1 subgroup priority level 1
    mT1IntEnable(1);            // Enable T1 interrupts   
}

// Turns off all LED segments
void clr_dsp(void) {
    SEG_CA(0);      
    SEG_CB(0);
    SEG_CC(0);
    SEG_CD(0);
    SEG_CE(0);
    SEG_CF(0);
    SEG_CG(0);
    SEG_DP(0);
}

/** 
  @Function
    * void set_digit(int dsp, int value, int dp);
  @Summary
    * Turns off all LED segments 
  @Description
    * All 7 LEDs are set to zero.
  @Precondition
    * LED IO pins have been initialized as outputs
  @Parameters
    * 1: Display digit
    * 2: Display value
    * 3: decimal point control   
  @Returns
    * None
  @Remarks
    * Digital display anode controls are active low.
    * 
    * Pmod JA Pin 2 is used for timing instrumentation and should be removed 
    * after testing.
 */

void set_digit(int dsp, int value, int dp) {
    dsp_digit(value, dp);   // Set LED cathodes according to 
    switch(dsp)
    {
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

/** 
  @Function
* void dsp_digit(int value);

  @Summary
    *  Single digit LED display
  @Description
    * Turns on specific LEDs segments to display the value in decimal format.
  @Precondition
    * LED IO pins have been initialized as outputs
  @Parameters
    * int - value to be displayed    
  @Returns
    * None
  @Remarks
    * Each value is displayed using a macro that consists of multiple C 
    * statements.
 */
void dsp_digit(int value, int dp)
{
    switch(value)
    {      
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
            SEG_NEG     // generate negative sign
            break;            
        default:
            SEG_OFF    // Blank the display
    }
    SEG_DP(dp);
}

/** 
  @Function
* void led_number(int value);
  @Summary
    * Display 4 digit decimal number
  @Description
    * A value of -999 through 9999 is displayed on the 4 digit 7 segment 
    * display. Leading zero blanking is used.
  @Precondition
    * LED IO pins have been initialized as outputs 
  @Parameters
    * int - value to be displayed in the range -999 to +9999
    * None    
  @Returns
    * None
  @Remarks
    * Each value is displayed using a macro that consists of multiple C 
    * statements. This function must be call frequently to keep the numbers
    * visible to the viewer. 
 */
void led_number(int value)
{
int d1, d2, d3, d4, dz = 0;         // Segment values and display zero flag
    
    if(value<10000)                 // Test for less than maximum positive
    {
        if((value < 0) && ( value > -1000)) // Test maximum negative
        {
            value = -value;         // Negate value to make positive
            d4 = -1;                // Assign digit 4 to negative 1
        }
        else
        {
            d4 = value/1000;        // Isolate 1000's digit
            value = value % 1000;   // Save remainder
        }
        d3 = value/100;             // Isolate 100's digit
        value = value % 100;        // Save remainder
        d2 = value/10;              // Isolate 10's digit
        value = value % 10;         // Save remainder
        d1 = value;                 // Isolate units digit

        if(d4)                      // Test for non zero
        {
            set_digit(3, d4, 0);    // If non zero or minus sign - display digit
            if(d4>0)
                dz = 1;             // Set display zero flag
        }
        else
        {
            set_digit(3, 0xFF, 0);  // If leading zero - blank digit
        }
        usDelay(DIG_DLY);

        if(d3 || dz == 1)           // Display if nonzero or dz flag is set
        {
            set_digit(2, d3, 0);
            dz = 1;                 // Set display zero flag
        }
        else
        {
            set_digit(2, 0xFF, 0);  // If leading zero - blank digit
        }
        usDelay(DIG_DLY);

        if(d2 || dz == 1)           // Display if nonzero or dz flag is set
        {
            set_digit(1, d2, 0);    // Set display zero flag
        }
        else
        {
            set_digit(1, 0xFF, 0);  // If leading zero - blank digit
        }
        usDelay(DIG_DLY);
        set_digit(0, d1, 0);           // Display trailing zeros
        usDelay(DIG_DLY);

/* Blank the display */        
        DIGITS_OFF(); 
    }
}

/** 
  @Function
* void test_7seg_leds(void);
  @Summary
    * A display test sequence for the 4 digit seven segment display
  @Description
    * Each of the 7 LED segments A through F are displayed on digit 0. The the 
    * number "8" is shifted from eft to right on the display 
  @Precondition
    * LED IO pins have been initialized as outputs 
  @Parameters
* int - value to be displayed
    * None    
  @Returns
    * None
  @Remarks
    * Each value is displayed using a macro that consists of multiple C 
    * statements.
 */
void test_7seg_leds(void)
{
    DIG_AN0(0);
    SEG_CA(1);
    msDelay(100);
    SEG_CB(1);
    msDelay(100);
    SEG_CC(1);
    msDelay(100);
    SEG_CD(1);
    msDelay(100);
    SEG_CE(1);
    msDelay(100);
    SEG_CF(1);
    msDelay(100);
    SEG_CG(1);
    msDelay(100);
    SEG_DP(1);
    msDelay(250);
    DIG_AN0(1);
    DIG_AN1(0);
    msDelay(250);
    DIG_AN1(1);
    DIG_AN2(0);
    msDelay(250);
    DIG_AN2(1);
    DIG_AN3(0);
    msDelay(200);
    clr_dsp();
    DIG_AN3(1);   
}

/* T2Interrupt Function Description *****************************************
 * SYNTAX:          void T1Interrupt(void);
 * KEYWORDS:        Timer1 interrupt service routine
 * DESCRIPTION:     Toggles JA4 every Timer 1 interrupt at interrutpt group
 *                  level 1.
 * PARAMETERS:      None
 * RETURN VALUE:    None
 *
 * NOTES:           See initPWM
 * END DESCRIPTION **********************************************************/
void __ISR( _TIMER_1_VECTOR, IPL1SOFT) T1Interrupt(void)
{
static int led_disp = 3;            // Seven-segment LED digit display index
static int led_digit;               // Seven-segment LED digit display value
static int ms_cntr = 10;

    if(--ms_cntr<=0)                // Measure out 10 tenths of a second 
    {
        ms_cntr = 10;
        if(led_flag && (led_value<10000) && (led_value > -1000))
        {
            switch(led_disp)    // Determine the display digit
            {
                case 0:
                    led_digit = abs(led_value) % 10;        // Units digit
                    break;
                case 1:
                    led_digit = (abs(led_value) % 100)/10;  // Tens digit
                    break;
                case 2:
                    led_digit = (abs(led_value) % 1000)/100;    // 100s digit
                    break;
                case 3:
                    if((led_value < 0) && ( led_value > -1000)) // Test maximum neg.
                    {
                        led_digit = -1;           // Assign digit 4 to negative 1
                    }
                    else
                    {
                        led_digit = led_value/1000;   // 1K digit
                    }
            }
            if(led_disp == SET_DP)
            {
                set_digit(led_disp, led_digit, 1);  // Display digit value with dp
            }
            else
            {
                set_digit(led_disp, led_digit, 0);  // Display digit value without dp
            }
            if(--led_disp < 0)                    // Update display index modulo 4
            {
                led_disp = 3;
            }
        }
        else
        {
            DIGITS_OFF();
            clr_dsp();
        }
    }
    
    mT1ClearIntFlag(); 	// clear T2Interrupt flag
} // End of T1Interrupt ISR

/* *****************************************************************************
 End of File
 */
