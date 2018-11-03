/* --------------------- Guard Against Multiple Inclusions ------------------- */
#ifndef __LED7_H__
    #define __LED7_H__

    /* ----------------- Necessary system include statements ----------------- */
    #include "hardware.h"
    #include <plib.h>
    #include <stdint.h>

    #include "swDelay.h"
    #include "hardware.h"

    /* -----------------------------------------------------------------------
       This file defines the port and pin assignments for the 7 segment LED 
       displays.  Macros are used extensively to implement abstraction of the
       bit-bang control required because the segment LEDs are not assigned to
       contiguous pins on a single port.
       ----------------------------------------------------------------------- */

    /* --------- Configure the PIC32 Pins for 7-segment LED outputs ---------- */
	#define CAcfg()		TRISGbits.TRISG12 = 0
	#define CBcfg()		TRISAbits.TRISA14 = 0
	#define CCcfg()		TRISDbits.TRISD6  = 0
	#define CDcfg()		TRISGbits.TRISG13 = 0
	#define CEcfg()		TRISGbits.TRISG15 = 0
	#define CFcfg()		TRISDbits.TRISD7  = 0
	#define CGcfg()		TRISDbits.TRISD13 = 0
	#define DPcfg()		TRISGbits.TRISG14 = 0

    /* ---- Macros for setting the PIC32 Pins as Digital outputs for LEDs ---- */
    #define AN0cfg()	(TRISBbits.TRISB12 = 0, ANSELBbits.ANSB12 = 0)	// RB12
    #define AN1cfg()	(TRISBbits.TRISB13 = 0, ANSELBbits.ANSB13 = 0)	// RB13
    #define AN2cfg()	TRISAbits.TRISA9   = 0	// RA9
    #define AN3cfg()	TRISAbits.TRISA10  = 0	// RA10

    #define DIG_DLY		100

    #define Seg7cfg()	(CAcfg(), CBcfg(), CCcfg(), CDcfg(), CEcfg(), CFcfg(),\
						 CGcfg(), DPcfg(), AN0cfg(), AN1cfg(), AN2cfg(),\
						 AN3cfg())

    /* -- Macro controls for the ON/OFF state of the LEDs. Active low LEDs --- */
    #define SEG_CA(b);  {if(b) LATGCLR = BIT_12; else LATGSET = BIT_12;}  //RG12
    #define SEG_CB(b);  {if(b) LATACLR = BIT_14; else LATASET = BIT_14;}  //RA14
    #define SEG_CC(b);  {if(b) LATDCLR = BIT_6;  else LATDSET = BIT_6; }  //RD5
    #define SEG_CD(b);  {if(b) LATGCLR = BIT_13; else LATGSET = BIT_13;}  //RG13
    #define SEG_CE(b);  {if(b) LATGCLR = BIT_15; else LATGSET = BIT_15;}  //RG15
    #define SEG_CF(b);  {if(b) LATDCLR = BIT_7 ; else LATDSET = BIT_7; }  //RD6
    #define SEG_CG(b);  {if(b) LATDCLR = BIT_13; else LATDSET = BIT_13;}  //RD13
    #define SEG_DP(b);  {if(b) LATGCLR = BIT_14; else LATGSET = BIT_14;}  //RG14

    #define DIG_AN0(b); {if(b) LATBbits.LATB12 = 1; else LATBbits.LATB12 = 0;}  //RB12
    #define DIG_AN1(b); {if(b) LATBbits.LATB13 = 1; else LATBbits.LATB13 = 0;}  //RB13
    #define DIG_AN2(b); {if(b) LATAbits.LATA9 = 1;  else LATAbits.LATA9 = 0; }  //RA9
    #define DIG_AN3(b); {if(b) LATAbits.LATA10 = 1; else LATAbits.LATA10 = 0;}  //RA10

    #define SEG_LED0 {SEG_CA(1); SEG_CB(1);SEG_CC(1);SEG_CD(1);SEG_CE(1);SEG_CF(1);SEG_CG(0);SEG_DP(0);}
    #define SEG_LED1 {SEG_CA(0); SEG_CB(1);SEG_CC(1);SEG_CD(0);SEG_CE(0);SEG_CF(0);SEG_CG(0);SEG_DP(0);}
    #define SEG_LED2 {SEG_CA(1); SEG_CB(1);SEG_CC(0);SEG_CD(1);SEG_CE(1);SEG_CF(0);SEG_CG(1);SEG_DP(0);}
    #define SEG_LED3 {SEG_CA(1); SEG_CB(1);SEG_CC(1);SEG_CD(1);SEG_CE(0);SEG_CF(0);SEG_CG(1);SEG_DP(0);}
    #define SEG_LED4 {SEG_CA(0); SEG_CB(1);SEG_CC(1);SEG_CD(0);SEG_CE(0);SEG_CF(1);SEG_CG(1);SEG_DP(0);}
    #define SEG_LED5 {SEG_CA(1); SEG_CB(0);SEG_CC(1);SEG_CD(1);SEG_CE(0);SEG_CF(1);SEG_CG(1);SEG_DP(0);}
    #define SEG_LED6 {SEG_CA(1); SEG_CB(0);SEG_CC(1);SEG_CD(1);SEG_CE(1);SEG_CF(1);SEG_CG(1);SEG_DP(0);}
    #define SEG_LED7 {SEG_CA(1); SEG_CB(1);SEG_CC(1);SEG_CD(0);SEG_CE(0);SEG_CF(0);SEG_CG(0);SEG_DP(0);}
    #define SEG_LED8 {SEG_CA(1); SEG_CB(1);SEG_CC(1);SEG_CD(1);SEG_CE(1);SEG_CF(1);SEG_CG(1);SEG_DP(0);}
    #define SEG_LED9 {SEG_CA(1); SEG_CB(1);SEG_CC(1);SEG_CD(0);SEG_CE(0);SEG_CF(1);SEG_CG(1);SEG_DP(0);}
    #define SEG_NEG  {SEG_CA(0); SEG_CB(0);SEG_CC(0);SEG_CD(0);SEG_CE(0);SEG_CF(0);SEG_CG(1);SEG_DP(0);}
    #define SEG_OFF  {SEG_CA(0); SEG_CB(0);SEG_CC(0);SEG_CD(0);SEG_CE(0);SEG_CF(0);SEG_CG(0);SEG_DP(0);}
    #define DIGITS_OFF(); {DIG_AN0(1); DIG_AN1(1); DIG_AN2(1); DIG_AN3(1);}

    #define DIG_DLY 	100
    #define T1_TICK 	((GetPeripheralClock() / 8000) - 1) // 1 millisecond used by Timer 1

    /* ---------------------- Public function declarations ------------------- */
    void seg7_init(void);
    void clr_dsp(void);
    void set_digit(int dsp, int value);
    void dsp_digit(int dsp);
    void led_number(int value);
    void test_7seg_leds(void);
#endif