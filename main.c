/* 
** main.c
** Richard Wall
** 
** rwall@gold.uidaho.edu
**
** November 24, 2012
 * March 3, 2017 - ported to PIC32MX370 for the Basys MX3 
 * October 1, 2018
**
** Test program for the 4 channel Servo Motor using
** the Cerebot PIC32 MXck.
**
** See 
*/

#include "config_bits.h"    // Processor configuration
#include "hardware.h"       // Platform declarations

// XC32 libraries
#include <plib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

// Included files for platform 
#include "LCDlib.h"
#include "led7.h"
#include "comm_lib.h"
#include "swDelay.h"
#include "main.h"
#include "DHT.h"
#include "TMP03.h"
#include "GPS_I2C.h"

float temp1;
float temp2;
float temp3, rh;
float tmp4, tmp5;
BOOL sensor_okay;

float length;
float angle;

/* ----------------------------------------------------------- */
int main(void)
{
char lcdstr[40];	// Local LCD display buffers
//char uartstr[80];	// Local UART input buffer
I2C_RESULT i2cFlag;
//BYTE gps_str[300];
int len = 255;
int pkt_len = 0;

	Hardware_Setup();       // Initialize common hardware
	initLCD();				// Initialize LCD
	uart4_init(38400, NO_PARITY);	// Initialize UART
	initTimer1();			// Start timer interrupt
    init_ad22100();
    i2cFlag = I2C_Init(I2C1, I2C_SPEED_STANDARD);
    if(!i2cFlag)
    {
        printf("I2C setup error\n\r");
    }
    len = 1;
    do
    {
        DelayMs(100);			// Wait a second
        
        i2cFlag = MAG3110_initialize();
        if(!i2cFlag)
        {
            printf("Magnetometer not found\n\r");
        }
    }while(!i2cFlag);
    
//	initRC();				// Initialize RC channel output pins
	DelayMs(2000);			// Wait a second
	clrLCD();				// Clear hello messages
	while(1)
	{
//        length = distance(lat1, lon1, lat2, lon2, 'K');

    	clrLCD();				// Clear hello messages

        sensor_okay = DHT11_read_temp_rh(&temp3, &rh);
    	sprintf(lcdstr,"T3:%d RH:%d", (int) temp3, (int) rh  );        
		putsLCD(lcdstr);	// Post channel settings to LCD line 1			
        printf("\n\r%s \n\r", lcdstr);	// Send channel setting to UART
		gotoLCD(16);			// Blank out LCD line 1    
        read_ad22100(&tmp4, &tmp5);
    	sprintf(lcdstr,"T4:%d T5:%d", (int) tmp4, (int) tmp5 );        
		putsLCD(lcdstr);	// Post channel settings to LCD line 1			
        printf("temp #4 = %d   temp #5 = %d \n\r", (int) tmp4, (int) tmp5 );
        do 
        {
            DelayMs(3);     // Give GPS time to up load new buffer
            len = MAX_PACKET_SIZE;
//            i2cFlag = I2C_Read(I2C1, GPS_DEV_ID, gps_str, &len);
//            gps_str[len] = 0;
//            pkt_len = GPS_DECODE_RMC(gps_str);
//            printf("Time: %d  LAT: %f  LON: %f\n\r", gps.utc_time, gps.lon, gps.lat);
            
        } while(pkt_len == 0);
        
       // i2cFlag = mag_read();
        
    	DelayMs(2000);			// Wait a second
  	}
	return 0;		// The program should never get to this LOC 
}

