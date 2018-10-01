/* 
** Cerebot32MX7.h
**
** Professor Richard Wall
** Department of Electrical and Computer Engineering
** University of Idaho
** Moscow, ID 83844-1023
** rwall@uidaho.edu
**
**	Date:		9/30/2011
**	Modified: 	11/24/2012 - added option for 4 bit LCD control
**
*/


#ifndef __CEREBOT32MX7__
	#define __CEREBOT32MX7__

// Uncomment if using the 4 data bit interface for a 4x20 LCD 
// connected to Pmod connector JC. The files LCD4bit.c and
// LCD4bit.h must be included in the project.
//	#define LCD_4BIT_4LINE 
	
// The following definitions refer to the stepper motor interface PMod
	#define LEDA		BIT_2		// Port B
	#define LEDB		BIT_3		// Port B
	#define LEDC		BIT_4		// Port B
	#define LEDD		BIT_6		// Port B
	#define LEDE		BIT_7		// Port B
	#define LEDF		BIT_8		// Port B
	#define LEDG		BIT_9		// Port B
	#define LEDH		BIT_10		// Port B
	#define SM1			LEDE
	#define SM2			LEDF
	#define SM3			LEDG
	#define SM4			LEDH

	#define RBLEDS	(LEDA | LEDB | LEDC | LEDD | SM1 | SM2 |SM3 | SM4)
	
	#define BTN1		BIT_6		// Port G
	#define BTN2		BIT_7		// Port G
	#define BTN3		BIT_0		// Port A

// Cerebot button sensing macros
	#define BUTTON_1	PORTGbits.RG6
	#define BUTTON_2	PORTGbits.RG7
	#define BUTTON_3	PORTAbits.RA0

// Cerebot board LEDs
	#define LED1		BIT_12		// Port G
	#define LED2		BIT_13		// Port G
	#define LED3		BIT_14		// Port G
	#define LED4		BIT_15		// Port G
	#define RGLEDS		(LED1 | LED2 | LED3 | LED4)

// Cerebot LED control macros
	#define LED_1(a)	LATGbits.LATG12 = a
	#define LED_2(a)	LATGbits.LATG13 = a
	#define LED_3(a)	LATGbits.LATG14 = a
	#define LED_4(a)	LATGbits.LATG15 = a


	// Timer 1
	#define PRESCALE            256
	#define TOGGLES_PER_SEC     1000

// Based upon setting in config_bits.h
	#define XTAL					8000000
	#define FOSC 					(XTAL * 20 / 2)
	#define FPB						(FOSC / 8)

	void Cerebot32mx7_setup();	// Initialize Cerebot LED and Buttons

#endif

