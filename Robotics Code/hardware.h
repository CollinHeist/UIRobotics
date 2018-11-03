/* ----- Guard against multiple-inclusion | Define a few macro functions ----- */
#define DEBUG
#define RC

// This definition stops unnecessary PLIB warnings from flooding the console
#ifndef _SUPPRESS_PLIB_WARNING
	#define _SUPPRESS_PLIB_WARNING
#endif

#ifndef _DISABLE_OPENADC10_CONFIGPORT_WARNING
	#define  _DISABLE_OPENADC10_CONFIGPORT_WARNING
#endif

#ifndef __HARDWARE_H__
	#define __HARDWARE_H__

    /* ----------------- Necessary system include statements ----------------- */
	#include <plib.h>
    #include <stdint.h>
    #include <stdbool.h>
    #include <xc.h>
    #include <stdio.h>
    #include <stdlib.h>

    #include "swDelay.h"
    #include "i2c_lib.h"
    #include "GPS_I2C.h"
    #include "MAG3110.h"
    #include "RC.h"

	/* ------ Macros to configure PIC pins as inputs for button sensors ------ */
	#define BTNL_bit	BIT_0   // RB0  - only functional in stand alone mode
	#define BTNR_bit	BIT_8   // RB8  - RC1
	#define BTND_bit	BIT_15  // RA15 - RC2
	#define BTNU_bit	BIT_1   // RB1  - only functional in stand alone mode
	#define BTNC_bit	BIT_0   // RF0

	#define BNTL()		PORTBbits.RB0  // PORTReadBits(IOPORT_B, BTNL_bit)
	#ifndef RC
		#define BNTR()	PORTBbits.RB8  // PORTReadBits(IOPORT_B, BTNR_bit) - RC1
		#define BNTD()	PORTAbits.RA15 // PORTReadBits(IOPORT_A, BTND_bit) - RC2
	#endif
	#define BNTU()		PORTBbits.RB1  // PORTReadBits(IOPORT_B, BTNU_bit)
	#define BNTC()		PORTFbits.RF0  // PORTReadBits(IOPORT_F, BTNC_bit)

	#ifdef RC
		#define BTNRcfgOut()	PORTSetPinsDigitalOut(IOPORT_B, BTNR_bit)	// Analog Input pin
		#define BTNDcfgOut()	PORTSetPinsDigitalOut(IOPORT_A, BTND_bit)	// RA5
	#else
		#define BTNRcfgIn()		PORTSetPinsDigitalIn(IOPORT_B, BTNR_bit)	// Analog Input pin
		#define BTNDcfgIn()		PORTSetPinsDigitalIn(IOPORT_A, BTND_bit)	// RA5
	#endif
	#define BTNCcfgIn()			PORTSetPinsDigitalIn(IOPORT_F, BTNC_bit)	// RF0 

	#define BTNUcfgIn()			(PORTSetPinsDigitalIn(IOPORT_B, BTNU_bit), CNPDBbits.CNPDB1 = 1) // RB0 With pulldown//  - Analog Input pin
		#define BTNLcfgIn()		(PORTSetPinsDigitalIn(IOPORT_B, BTNL_bit), CNPDBbits.CNPDB0 = 1) // RB1 With pulldown//  - Analog Input pin
	#ifdef DEBUG
		#define BTNcfgIn()		(BTNRcfgIn(), BTNDcfgIn(), BTNCcfgIn())
	#else
		#define BTNcfgIn()		(BTNLcfgIn(), BTNRcfgIn(), BTNUcfgIn(), BTNDcfgIn(), BTNCcfgIn())
	#endif

	/* ------------------------ Configure slide switches --------------------- */
	#define SW0cfg()	TRISFbits.TRISF3   = 1 
	#define SW1cfg()	TRISFbits.TRISF5   = 1 
	#define SW2cfg()	TRISFbits.TRISF4   = 1 
	#define SW3cfg()	TRISDbits.TRISD15  = 1
	#define SW4cfg()	TRISDbits.TRISD14  = 1
	#define SW5cfg()	(TRISBbits.TRISB11 = 1, ANSELBbits.ANSB11 = 0)
	#define SW6cfg()	(TRISBbits.TRISB10 = 1, ANSELBbits.ANSB10 = 0)
	#define SW7cfg()	(TRISBbits.TRISB9  = 1, ANSELBbits.ANSB9  = 0)
	#define SWcfg()		(SW0cfg(), SW1cfg(), SW2cfg(), SW3cfg(), SW4cfg(), SW5cfg(), SW6cfg(), SW7cfg())

	#define SW0()		PORTFbits.RF3
	#define SW1()		PORTFbits.RF5
	#define SW2()		PORTFbits.RF4
	#define SW3()		PORTDbits.RD15
	#define SW4()		PORTDbits.RD14
	#define SW5()		PORTBbits.RB11
	#define SW6()		PORTBbits.RB10
	#define SW7()		PORTBbits.RB9

	#define Switch2Binary() (SW0() + (SW1() << 1) + (SW2() << 2) + (SW3() << 3) +\
							(SW4() << 4) + (SW5() << 5) + (SW6() << 6) + (SW7() << 7))

	/* -------------------------- RGB Led Control ---------------------------- */
	#define RedLED_bit		BIT_2   // RD2
	#define BluLED_bit		BIT_3   // RD3
	#define GrnLED_bit		BIT_12  // RD12
	#define RBG_LEDcfg()	(TRISDCLR = RedLED_bit | BluLED_bit | GrnLED_bit, LATDCLR = RedLED_bit | BluLED_bit | GrnLED_bit)
	#define setRedLED(a);	{ if(a) LATDSET = RedLED_bit; else LATDCLR = RedLED_bit; } 
	#define setBluLED(a);	{ if(a) LATDSET = BluLED_bit; else LATDCLR = BluLED_bit; } 
	#define setGrnLED(a);	{ if(a) LATDSET = GrnLED_bit; else LATDCLR = GrnLED_bit; } 

	/* ---------------------- Set motor control drive inputs ----------------- */
	#define AIN1cfg()		(TRISBbits.TRISB3 = 0, ANSELBbits.ANSB3 = 0, LATBbits.LATB3 = 0)
	#define AIN2cfg()		(TRISEbits.TRISE8 = 0, LATEbits.LATE8   = 0)
	#define BIN1cfg()		(TRISEbits.TRISE9 = 0, LATEbits.LATE9   = 0)
	#define BIN2cfg()		(TRISBbits.TRISB5 = 0, ANSELBbits.ANSB5 = 0, LATBbits.LATB5 = 0)
	#define MCMODEcfg()		(TRISFbits.TRISF1 = 0, LATFbits.LATF1   = 0)
	#define MCInit()		(AIN1cfg(), AIN2cfg(), BIN1cfg(), BIN2cfg(), MCMODEcfg())

	#define AIN1(a);		{ if(a) LATBbits.LATB3 = 1; else LATBbits.LATB3 = 0; }
	#define AIN2(a);		{ if(a) LATEbits.LATE8 = 1; else LATEbits.LATE8 = 0; }
	#define BIN1(a);		{ if(a) LATEbits.LATE9 = 1; else LATEbits.LATE9 = 0; }
	#define BIN2(a);		{ if(a) LATBbits.LATB5 = 1; else LATBbits.LATB5 = 0; }
	#define MCMODE(a);		 {if(a) LATFbits.LATF1 = 1; else LATFbits.LATF1 = 0; }
	
	
	/* ----- Macros to configure PIC pins as outputs for LED indications ----- */
	#define LED0_bit		BIT_0	// RA0
	#define LED1_bit		BIT_1 	// RA1
	#define LED2_bit		BIT_2 	// RA2
	#define LED3_bit		BIT_3 	// RA3
	#define LED4_bit		BIT_4	// RA4
	#define LED5_bit		BIT_5 	// RA5
	#define LED6_bit		BIT_6	// RA6
	#define LED7_bit		BIT_7	// RA7
	#define LED_all_bits	0xff

	#define LED0cfg()		TRISAbits.TRISA0 = 0 	// RA0
	#define LED1cfg()		TRISAbits.TRISA1 = 0 	// RA1
	#define LED2cfg()		TRISAbits.TRISA2 = 0 	// RA2
	#define LED3cfg()		TRISAbits.TRISA3 = 0 	// RA3
	#define LED4cfg()		TRISAbits.TRISA4 = 0 	// RA4
	#define LED5cfg()		TRISAbits.TRISA5 = 0 	// RA5
	#define LED6cfg()		TRISAbits.TRISA6 = 0 	// RA6
	#define LED7cfg()		TRISAbits.TRISA7 = 0 	// RA7
	#define LEDcfg()		(TRISACLR = LED_all_bits, LATACLR = LED_all_bits)

	#define setLED0(a);         { if(a) LATASET = LED0_bit; else LATACLR = LED0_bit; }	//PORTSetBits(IOPORT_A, LED0_bit)
	#define setLED1(a);			{ if(a) LATASET = LED1_bit; else LATACLR = LED1_bit; }	//PORTSetBits(IOPORT_A, LED1_bit)
	#define setLED2(a);			{ if(a) LATASET = LED2_bit; else LATACLR = LED2_bit; }	//PORTSetBits(IOPORT_A, LED2_bit)
	#define setLED3(a);			{ if(a) LATASET = LED3_bit; else LATACLR = LED3_bit; }	//PORTSetBits(IOPORT_A, LED3_bit)
	#define setLED4(a);			{ if(a) LATASET = LED4_bit; else LATACLR = LED4_bit; }	//PORTSetBits(IOPORT_A, LED4_bit)
	#define setLED5(a);			{ if(a) LATASET = LED5_bit; else LATACLR = LED5_bit; }	//PORTSetBits(IOPORT_A, LED5_bit)
	#define setLED6(a);			{ if(a) LATASET = LED6_bit; else LATACLR = LED6_bit; }	//PORTSetBits(IOPORT_A, LED6_bit)
	#define setLED7(a);			{ if(a) LATASET = LED7_bit; else LATACLR = LED7_bit; }	//PORTSetBits(IOPORT_A, LED7_bit)
	#define Set_All_LEDs_On()	PORTSetBits(IOPORT_A, LED_all_bits)
	#define Set_All_LEDs_Off()	PORTClearBits(IOPORT_A, LED_all_bits)

	#define invLED0()			PORTToggleBits(IOPORT_A, LED0_bit)
	#define invLED1()			PORTToggleBits(IOPORT_A, LED1_bit)
	#define invLED2()			PORTToggleBits(IOPORT_A, LED2_bit)
	#define invLED3()			PORTToggleBits(IOPORT_A, LED3_bit)
	#define invLED4()			PORTToggleBits(IOPORT_A, LED4_bit)
	#define invLED5()			PORTToggleBits(IOPORT_A, LED5_bit)
	#define invLED6()			PORTToggleBits(IOPORT_A, LED6_bit)
	#define invLED7()			PORTToggleBits(IOPORT_A, LED7_bit)
	
    /* -----------------------------------------------------------------------
	   Based upon settings in config_bits.h, these influence timed events
	   using the Tick module. These are used for UART I2C, and SPI Baud
	   rate generation
	   ----------------------------------------------------------------------- */
	#define XTAL                    (8000000UL) /* 8 MHz Xtal */
	#define GetSystemClock()        (80000000UL)
	#define GetCoreClock()          (GetSystemClock()/2)
	#define GetPeripheralClock()    (GetSystemClock()/8)
	#define SYSTEM_FREQ             GetSystemClock()

	#define CORE_MS_TICK_RATE       (unsigned int)(GetCoreClock() / 1000UL)

	extern unsigned int millisec;
    
    /* ---------------------- Public Function Declarations ------------------- */
    I2C_RESULT Hardware_Setup(void);
    unsigned int millis(void);
    static void initTimer1(void);
#endif    