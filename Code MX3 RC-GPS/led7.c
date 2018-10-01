#include "swDelay.h"
#include "led7.h"
#include "hardware.h"
#include <plib.h>
#include <stdint.h>

int16_t led_value = 0;
BOOL led_flag = 0;

/* Provides control of individual digits or write a decimal value to the four
 * LED displays
 */

void seg7_init(void) {
    Seg7cfg();
    DIGITS_OFF();
    clr_dsp();
    test_7seg_leds();
    DIGITS_OFF();
    clr_dsp();
    
//  This interrupts signals when to switch LED digits
// Configure Timer 1 using internal clock, 1:8 pre-scale   
    OpenTimer2(T2_ON | T2_SOURCE_INT | T2_PS_1_8, T2_TICK);
    mT1SetIntPriority( 1);      // Set Timer 1 group priority level 1
    mT1SetIntSubPriority( 1);	// Set Timer 1 subgroup priority level 1 
    mT1IntEnable( 1);           // Enable T1 interrupts 
    
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

/** 
  @Function
    * void clr_dsp(void);
  @Summary
    * Turns off all LED segments 
  @Description
    * All 7 LEDs are set to zero.
  @Precondition
    * LED IO pins have been initialized as outputs
  @Parameters
    * None    
  @Returns
    * None
  @Remarks
    * None
 */
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
    * void set_digit(int dsp, int value);
  @Summary
    * Turns off all LED segments 
  @Description
    * All 7 LEDs are set to zero.
  @Precondition
    * LED IO pins have been initialized as outputs
  @Parameters
    * 1: Display digit
    * 2: Display value   
  @Returns
    * None
  @Remarks
    * Digital display anode controls are active low.
 */
void set_digit(int dsp, int value)
{
    dsp_digit(value);       // Set LED cathodes according to segment number
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
void dsp_digit(int value)
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
            set_digit(3, d4);       // If non zero or minus sign - display digit
            if(d4>0)
                dz = 1;             // Set display zero flag
        }
        else
            set_digit(3, 0xFF);     // If leading zero - blank digit
        DelayUs(DIG_DLY);

        if(d3 || dz == 1)           // Display if nonzero or dz flag is set
        {
            set_digit(2, d3);
            dz = 1;                 // Set display zero flag
        }
        else
            set_digit(2, 0xFF);     // If leading zero - blank digit
        DelayUs(DIG_DLY);

        if(d2 || dz == 1)           // Display if nonzero or dz flag is set
        {
            set_digit(1, d2);       // Set display zero flag
        }
        else
            set_digit(1, 0xFF);     // If leading zero - blank digit
        DelayUs(DIG_DLY);
        set_digit(0, d1);           // Display trailing zeros
        DelayUs(DIG_DLY);

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
    * None    
  @Returns
    * None
  @Remarks
    * Each segment is displayed using a macro that consists of multiple C 
    * statements as defined in led7.h.
 */
void test_7seg_leds(void)
{
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

/* T1Interrupt Function Description *****************************************
 * SYNTAX:          void T1Interrupt(void);
 * KEYWORDS:        Timer1 interrupt service routine
 * DESCRIPTION:     Updates the 4 digit 7 segment display according to the value
 *                  set for the global variable "led_value". The display can be
 *                  controlled by setting led_flag to 0 for off or 1 for on.
 *          
 * PARAMETERS:      None
 * RETURN VALUE:    None
 *
 * NOTES:           Calls function "set_digit" and "clr_dsp".
 * END DESCRIPTION **********************************************************/
void __ISR( _TIMER_2_VECTOR, IPL1SOFT) T2Interrupt(void)
{
static int led_disp = 3;            // Seven-segment LED digit display index
static int led_digit;               // Seven-segment LED digit display value
static int ms_cntr = 2;

    if(--ms_cntr<=0)                // Measure out 2 tenths of a second 
    {
        ms_cntr = 2;
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
                        led_digit = led_value/1000;         // 1K digit
                    }
            }
            set_digit(led_disp, led_digit);       // Display digit value
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
    
    mT2ClearIntFlag(); 	/* clear T2Interrupt flag */
} /* End of T1Interrupt ISR */

/* *****************************************************************************
 End of File
 */
