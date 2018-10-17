/* 
 * File:   main.c
 * Author: Richard Wall
 *
 * Created on October 5, 2018
 */

#include "main.h"
#include "hardware.h"
#include "config_bits.h"
#ifdef WF32
    #include <plib.h>
#endif

#include "swDelay.h"
#include "Uno.h"
#include "UART1.h"
#include "UART4.h"
#include "i2c_lib.h"
#include "GPS_I2C.h"
#include "MAG3110.h"
#include "RC.h"

#include <stdint.h>     /* For uint32_t definition */
#include <stdbool.h>    /* For true/false definition */
#include <xc.h>
#include <stdio.h>
#include <stdlib.h>

extern unsigned int millisec;

/*
 * Private functions 
 */

/* Application entry */
int main(void)
{
I2C_RESULT i2cFlag;
float heading;
int x, y, z;
int r1 = 0, r2 = 25, r3 = 50, r4 = 75;
char ch;
// BOOL u4Flag;
unsigned int t1, t2;

    i2cFlag =  Hardware_Setup();    // Execute only once
    if(i2cFlag == I2C_SUCCESS)
    {
        printf("System configuration completed.\n\r");
        t1 = millisec;          // Reset millisecond counter
        while(1)            // Operation continuously loops
        {
            if(++r1 >99) r1 = 0;
            if(++r2 >99) r2 = 0;
            if(++r3 >99) r3 = 0;
            if(++r4 >99) r4 = 0;
            
             set_rc(r1, r2, r3, r4);

            i2cFlag =   ReportGPS(TRUE);    // Read RMC and send to terminal
            i2cFlag = MAG3110_readMag(&x, &y, &z);
            if(i2cFlag == I2C_SUCCESS)
            {
                heading = MAG3110_readHeading();
                if(heading<0.0)
                {
                    heading += 360.0;
                }    
                printf("Magnetic X: %4d   Y:%4d   Z:%4d  Dir:%f\n\r", x,y,z, heading);
            }
            
            do{
                t2 = millisec;
            }while((t2-t1) < 1000);
            t1 = t2;
            LATDINV = BIT_3; 
            LD1_IO = !LD1_IO;    
        }
        
    }
    else   
    {
        printf("System failed to initialize properly.\n\r");
    }

   return (EXIT_FAILURE);
}


/* *************************  End of main ********************************** */
