/* ************************************************************************** */
/** Descriptive File Name
  @ Author
    Richard Wall
 
  @ Date
    October 6, 2018

   @File Name
    hardware.c

  @Summary
    Definition of constants and macro routines, and global functions 

  @Description
    The #define statements and macro C code provide high level access to the 
    "Trainer" boards switches, push buttons, and LEDs.
    
 */
/* ************************************************************************** */

/* This included file provides access to the peripheral library functions and
   must be installed after the XC32 compiler. See
http://ww1.microchip.com/downloads/en/DeviceDoc/32bitPeripheralLibraryGuide.pdf
http://www.microchip.com/SWLibraryWeb/product.aspx?product=PIC32%20Peripheral%20Library
 */

#include "hardware.h"
#include <plib.h>
#include <stdint.h>     /* For uint32_t definition */
#include <stdbool.h>    /* For true/false definition */
#include <xc.h>
#include <stdio.h>
#include <stdlib.h>

#include "swDelay.h"
#include "i2c_lib.h"
#include "Uno.h"
#include "UART1.h"
#include "i2c_lib.h"
#include "GPS_I2C.h"
#include "MAG3110.h"
#include "RC.h"

// *****************************************************************************
/**
    @Function
        void Hardware_Setup(void);

    @Summary
        Initializes PIC32 pins commonly used for IO on the Trainer processor
        board.

    @Description
        Initializes PIC32 digital IO pins to provide functionality for the 
        slide switches, push buttons, and LEDs

    @Precondition
        "config_bits* must be included in the project

    @Parameters
        None
        @param param2: pointer to y in range of 0b0000 to 0b1111 ( 0 - 15d )

    @Returns
        None

    @Remarks
    */
unsigned int millisec;;                 // Global millisecond counter

static void initTimer1(void);
static I2C_RESULT WF32_Setup(void);     // Initialize the WF32 and UUno Shield
static void blinkLED(void);             // Blink all hardware LEDs

I2C_RESULT Hardware_Setup(void)
{
I2C_RESULT i2cFlag;
int len;
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Statement configure cache, wait states and peripheral bus clock
 * Configure the device for maximum performance but do not change the PBDIV
 * Given the options, this function will change the flash wait states, RAM
 * wait state and enable prefetch cache but will not change the PBDIV.
 * The PBDIV value is already set via the pragma FPBDIV option above..
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    SYSTEMConfig(GetSystemClock(), SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE);
    DDPCONbits.JTAGEN = 0;  // Statement is required to use Pin RA0 as IO
    initRC();
#ifdef WF32
    i2cFlag = WF32_Setup();
#endif
    initTimer1();
//    RCInit();
   i2cFlag = I2C_Init(I2C2, I2C_SPEED_FAST); //I2C_SPEED_STANDARD);
    if(i2cFlag == I2C_SUCCESS)
    {
// Reset GPS
        PORTSetPinsDigitalOut(IOPORT_A, BIT_14);
        LATAbits.LATA14 = 1;
        DelayMs(100);
        LATAbits.LATA14 = 0;
        DelayMs(100);
        LATAbits.LATA14 = 1;
        DelayMs(100);
        
        len = 255;
        i2cFlag = I2C_Read(I2C2, GPS_DEV_ID, gpsStr, &len);
        if(i2cFlag == I2C_SUCCESS)
        {
            printf("GPS on line\n\r");
            
            i2cFlag = setGPS_RMC();
            
            if(i2cFlag == I2C_SUCCESS)
            {
                printf("GPS RMC sentence set successfully\n\r");
            }
            else
            {
                printf("GPS RMC sentence failed\n\r");
            }
             
        }    
        else
        {
            printf("GPS not available\n\r");    
        }
    }
    DelayMs(100);
    printf("Magnetometer is calibrating\n\r");

    MAG3110_enterCalMode();
    while(MAG3110_isCalibrating())
    {
        MAG3110_calibrate();
    }
    if(MAG3110_isCalibrated())
    {
        printf("Magnetometer is calibrated\n\r");
    }
    return i2cFlag;
} /* End of BAsys MX3A Setup */

/* START FUNCTION DESCRIPTION ********************************************

SYNTAX:  		static void initTimer1{void);
KEYWORDS:		initialization system timer 1
DESCRIPTION:	Sets timer 1 for an 10 us interrupt
RETURN VALUE:   none
NOTES:			None
END DESCRIPTION **********************************************************/

static void initTimer1(void)
{
#define TMR1_TICK	100
    
    PORTSetPinsDigitalOut(IOPORT_D, BIT_3);
    LATDbits.LATD3 = 0;
    millisec = 0;       // Global millisecond counter
    OpenTimer1(T1_ON | T1_SOURCE_INT | T1_PS_1_1, TMR1_TICK);
    
	// Set Timer 1 interrupt with a priority of 2
    ConfigIntTimer1(T1_INT_ON | T1_INT_PRIOR_2);

	// Enable multi-vector interrupts
    INTConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);  /* Do only once */
    INTEnableInterrupts();   /*Do as needed for global interrupt control */
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
void __ISR(_TIMER_1_VECTOR, IPL2SOFT) Timer1Handler(void)
{
static ms = 100;              // millisecond counter
static int onesec = 10000;   // One second counter

    mT1ClearIntFlag();			// clear the interrupt flag
    ms--;                   // iNCREMENT MILLISECOND COUNTER
    if(ms <= 0)
    {
        millisec++;
        ms = 100;
        onesec--;
        if(onesec <= 0)
        {
            onesec = 10000;
            LD3_IO = !LD3_IO;
        }
    }

    rcUpdate();         // High speed periodic tasks;
}

/* START FUNCTION DESCRIPTION ********************************************

SYNTAX:  		unsigned int millis(void);
KEYWORDS:		millisecond, count
DESCRIPTION:	Global millisecond counter
RETURN VALUE:   Millisecond count
NOTES:			None
END DESCRIPTION **********************************************************/

unsigned int millis(void)
{
    return millisec;
}


/* FUNCTION DESCRIPTION ****************************************************
 * SYNTAX:          I2C_RESULT WF32_Setup(void);
 * KEYWORDS:        WF32, initialize, hardware, configure, setup
 * Description:     Initialization of processor board IO
 * Parameters:      None
 * Return:          None
 * Notes:           Specifically for WF32
 * END DESCRIPTION ********************************************************/
#ifdef WF32
static I2C_RESULT WF32_Setup(void)
{
    uno_init();         // Configure UNIO Shield LEDs
   
    LED1cfg();          // Set LED outputs
    LED2cfg();    
    LED3cfg();    
    LED4cfg();    
    blinkLED();         // Test LEDs  
    
    BTN2cfg();          // Set Button inputs
    BTN3cfg();

    VR1cfg();           // Set VR1 input for analog

    uart1_init(38400, NO_PARITY);     // COM4
    uart4_init(9600, NO_PARITY);      // GPS / XBee
    
 // Configure I2C2 pins  
    TRISAbits.TRISA2 = 1;   
    TRISAbits.TRISA3 = 1;
    LATAbits.LATA2 = 1;     
    LATAbits.LATA3 = 1;
    
} /* End of WF32_Setup */
#endif

/* FUNCTION DESCRIPTION ****************************************************
 * SYNTAX:          void blinkLED(void);
 * KEYWORDS:        WF32, UNO Shield, LED
 * Description:     Sequentially turns on all hardware LEDs
 * Parameters:      None
 * Return:          None
 * Notes:           None
 * END DESCRIPTION ********************************************************/
static void blinkLED(void)
{
    
    LED1_IO = 0;        // Reset WF32 LEDs to off
    LED2_IO = 0;
    LED3_IO = 0;
    LED4_IO = 0;
    LD1_IO = 0;         // Reset Uno LEDs to off
    LD2_IO = 0;
    LD3_IO = 0;
    LD4_IO = 0;
// Scan on board LEDs
    LED1_IO = !LED1_IO;
    LD1_IO = !LD1_IO;
    DelayMs(100);
    LED2_IO = !LED2_IO;
    LD2_IO = !LD2_IO;
    DelayMs(100);
    LED3_IO = !LED3_IO;
    LD3_IO = !LD3_IO;
    DelayMs(100);
    LED4_IO = !LED4_IO;
    LD4_IO = !LD4_IO;
    DelayMs(100);
    LED1_IO = 0;        // Reset WF32 LEDs to off
    LED2_IO = 0;
    LED3_IO = 0;
    LED4_IO = 0;
    LD1_IO = 0;         // Reset Uno LEDs to off
    LD2_IO = 0;
    LD3_IO = 0;
    LD4_IO = 0;
} /* End of blink */
