#ifndef _HARDWARE_H_   
	#define	_HARDWARE_H_

	// Suppress plib obsolesce warnings
	#ifndef _SUPPRESS_PLIB_WARNING	
		#define _SUPPRESS_PLIB_WARNING
	#endif

	#ifndef _DISABLE_OPENADC10_CONFIGPORT_WARNING
		#define  _DISABLE_OPENADC10_CONFIGPORT_WARNING
	#endif

	// Macros for error-detection
	#define NO_ERROR            (0)
	#define ERROR               (1)

    // ASCII Control Characters
	#define BACKSPACE		(0x08)
	#define NO_PARITY		(0)
	#define ODD_PARITY		(1)
	#define EVEN_PARITY		(2)

	#define SET_ALL_IO_DIGITAL()    (ANSELA=0, ANSELB=0, ANSELC=0, ANSELD=0, ANSELE=0, ANSELF=0, ANSELG = 0)
	#define SET_MIC_ANALOG()        (ANSELBbits.ANSB4 = 1)
	#define SET_POT_ANALOG()        (ANSELBbits.ANSB2 = 1)

	// Configure slide switches
	#define CONFIG_SW0()            (TRISFbits.TRISF3 = 1)
	#define CONFIG_SW1()            (TRISFbits.TRISF5 = 1)
	#define CONFIG_SW2()            (TRISFbits.TRISF4 = 1)
	#define CONFIG_SW3()            (TRISDbits.TRISD15 = 1)
	#define CONFIG_SW4()            (TRISDbits.TRISD14 = 1)
	#define CONFIG_SW5()            (TRISBbits.TRISB11 = 1)
	#define CONFIG_SW6()            (TRISBbits.TRISB10 = 1)
	#define CONFIG_SW7()            (TRISBbits.TRISB9 = 1)
	#define CONFIG_ALL_SWITCHES()   (CONFIG_SW0(), CONFIG_SW1(), CONFIG_SW2(), CONFIG_SW3(), CONFIG_SW4(), CONFIG_SW5(), CONFIG_SW6(), CONFIG_SW7())

    // The following macro instructions provide for reading the position of the 8 switches.
	#define SW0()		 PORTFbits.RF3
	#define SW1()		 PORTFbits.RF5
	#define SW2()		 PORTFbits.RF4
	#define SW3()		 PORTDbits.RD15
	#define SW4()		 PORTDbits.RD14
	#define SW5()		 PORTBbits.RB11
	#define SW6()		 PORTBbits.RB10
	#define SW7()		 PORTBbits.RB9

	#define Switch2Binary() ((((int) SW0()) << 0) + (((int) SW1()) << 1) + (((int) SW2()) << 2) + (((int) SW3()) << 3) +\
							 (((int) SW4()) << 4) + (((int) SW5()) << 5) + (((int) SW6()) << 6) + (((int) SW7()) << 7))

	#ifndef DEBUG_MODE
		#define Set_BTNL_in()  (TRISBbits.TRISB0 = 1, CNPDBbits.CNPDB0 = 1)
		#define Set_BTNU_in()  (TRISBbits.TRISB1 = 1, CNPDBbits.CNPDB1 = 1) 
	#endif
	#define Set_BTNR_in()   TRISBbits.TRISB8 = 1
	#define Set_BTNR_out()  TRISBbits.TRISB8 = 0	// use for Servo J1
	#define Set_BTND_in()   TRISAbits.TRISA15 = 1
	#define Set_BTND_out()  TRISAbits.TRISA15 = 0   // use for Servo J2
	#define Set_BTNC_in()   TRISFbits.TRISF0 = 1

	#ifndef DEBUG_MODE
		#define Set_All_PBs_Input()     (Set_BTNL_in(), Set_BTNR_in(), Set_BTNU_in(), Set_BTND_in(), Set_BTNC_in())
	#else
		#define Set_All_PBs_Input()     (Set_BTNR_in(),Set_BTND_in(),Set_BTNC_in())
	#endif

/* Macro instructions to read the button position values. 1 = button pressed */ 
	#ifndef DEBUG_MODE
/* Include BTNL and BTNU only if NOT in debug mode */
		#define BTNL()	PORTBbits.RB0  
		#define BTNU()	PORTBbits.RB1  
	#endif
	#define BTNR()          (PORTBbits.RB8)
	#define BTND()          (PORTAbits.RA15) 
	#define BTNC()          (PORTFbits.RF0)

/* Macros to define the PIC pin values for the board LEDs */
	#define LED0            (0x01)
	#define LED1            (0x02)
	#define LED2            (0x04)
	#define LED3            (0x08)
	#define LED4            (0x10)
	#define LED5            (0x20)
	#define LED6            (0x40)
    #define LED7            (0x80)
	#define ALL_LEDs        (0xFF)

/* Macro instruction to configure all 8 LED pins for outputs */
	#define SET_All_LEDs_Output()  TRISACLR = ALL_LEDs

/* Macros to set board each LED on (1) or off (0) */
	#define setLED0(a);  {if(a) LATASET = LED0; else LATACLR = LED0;}
	#define setLED1(a);  {if(a) LATASET = LED1; else LATACLR = LED1;}
	#define setLED2(a);  {if(a) LATASET = LED2; else LATACLR = LED2;}
	#define setLED3(a);  {if(a) LATASET = LED3; else LATACLR = LED3;}
	#define setLED4(a);  {if(a) LATASET = LED4; else LATACLR = LED4;}
	#define setLED5(a);  {if(a) LATASET = LED5; else LATACLR = LED5;}
	#define setLED6(a);  {if(a) LATASET = LED6; else LATACLR = LED6;}
	#define setLED7(a);  {if(a) LATASET = LED7; else LATACLR = LED7;}
	#define SET_All_LEDs_On()   LATASET = ALL_LEDs  // Set all LEDs on
	#define SET_All_LEDs_Off()  LATACLR = ALL_LEDs  // Set all LEDs off

/* Macros to invert the output to the board LEDs */
	#define invLED0()   LATAINV = LED0  
	#define invLED1()   LATAINV = LED1
	#define invLED2()   LATAINV = LED2
	#define invLED3()   LATAINV = LED3
	#define invLED4()   LATAINV = LED4
	#define invLED5()   LATAINV = LED5
	#define invLED6()   LATAINV = LED6
	#define invLED7()   LATAINV = LED7

/* Configure RGB LED outputs */
	#define Set_LED8_R_Out()        (TRISDbits.TRISD2 = 0)	
	#define Set_LED8_G_Out()        (TRISDbits.TRISD12 = 0)
	#define Set_LED8_B_Out()        (TRISDbits.TRISD3 = 0)
	#define Set_RGB_Output()        (Set_LED8_R_Out(), Set_LED8_G_Out(), Set_LED8_B_Out())

    #define Set_LED8_R(a)           (LATDbits.LATD2 = a)
	#define Set_LED8_G(a)           (LATDbits.LATD12 = a)
	#define Set_LED8_B(a)           (LATDbits.LATD3 = a)

	#define Set_LED8_RGB(a)         (LATDbits.LATD2 = a, LATDbits.LATD12 = a, LATDbits.LATD3 = a)

	#define XTAL					(8000000UL)				// 8 MHz Xtal on Basys MX3A
	#define GetSystemClock()		(80000000UL)			// Instruction frequency
	#define SYSTEM_FREQ				(GetSystemClock())
	#define GetCoreClock()			(GetSystemClock()/2)	// Core clock frequency
	#define GetPeripheralClock()	(GetSystemClock()/8)	// PCLK set for 80 MHz

	#define TIMER1_TICK				(100)
	#define TIMER1_MS_COUNT			(100)

	// Function Prototypes
	unsigned int initializeHardware(void);
    inline void clearBuffer(char* buffer, const unsigned int length);
	static void initializeTimer1(void);
#endif