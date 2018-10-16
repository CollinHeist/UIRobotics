/* ************************************************************************** */
/** Descriptive File Name

  @ Author
    Richard Wall
 
  @ Date
    September 27, 2018
  @Revised
	

   @File Name
    GPS_I2C.c

  @Summary
    GPS application

   @Precondition
        "config_bits* must be included in the project

   @Description

 *  */
/* ************************************************************************** */

#include "hardware.h"
#include "swDelay.h"
#include "GPS_I2C.h"
#include "I2C_lib.h"
#include "Uno.h"

#ifdef WF32
    #include <plib.h>
#endif

#include <string.h>
#include <stdio.h>
#include <stdint.h>

I2C_RESULT setGPS_RMC(void)
{
I2C_RESULT i2cFlag;
int dly = 0;
    do {
        i2cFlag = sendMTKpacket(PMTK_SET_NMEA_OUTPUT_RMCONLY);
        DelayMs(MAXWAITSENTENCE);
    } while(dly);
}

int GPS_DECODE_RMC(BYTE *str)
{
char *s2, *pos, *eol;
int len = 0;

    s2 = strstr(str, "$GNRMC" );

    if(s2 != NULL)
    {
        eol = strchr(s2, LF);
        if(eol != NULL)
        {
            len = (int) (eol - s2);
            if(len > 70)    // Test for full message
            {
                s2[len-1] = 0;
                printf("%s --> %d \n\r", s2, len-1);
                sscanf(s2,"$GNRMC,%d.000,%c,%f,%c,%f,%c,%f,%f,%d,,,%c*%x",\
                           &gps.utc_time,&gps.status,&gps.lat,&gps.ns,&gps.lon,\
                           &gps.ew,&gps.speed,&gps.angle,&gps.date,&gps.mode,\
                           &gps.cksum);
                LD4_IO = !LD4_IO;
            }
            else
            {
                printf("%s --> %d \n\r", s2, len-1);
                len = 0;
            }
        }
    }
    return len;
}

I2C_RESULT ReportGPS(int show)
{
I2C_RESULT i2cFlag;
int len = 255;
static int hour, min, sec;
int pkt_len = 0;
    // do 
    {
        DelayMs(5);     // Give GPS time to up load new buffer
        len = MAX_PACKET_SIZE;
        i2cFlag = I2C_Read(I2C2, GPS_DEV_ID, gpsStr, &len);
//        printf("%s\n\r", gpsStr);
        if(i2cFlag == I2C_SUCCESS && (len > 73) && (len < 256));
        {
            LD1_IO = !LD1_IO;
            gpsStr[len] = 0;
            pkt_len = GPS_DECODE_RMC(gpsStr);
            if(show)
            {
                hour = (gps.utc_time / 10000);
                min = (gps.utc_time - hour*10000)/100;
                sec = (gps.utc_time - (hour*10000 + min*100));
                hour -= 7;
                if(hour < 0)
                    hour += 24;        
                printf("Time: %2d:%2d:%2d  LAT:%f  LON:%f\n\r",\
                        hour, min, sec, gps.lon, gps.lat);
            }
        }
    } //while(pkt_len == 0);
        
    return i2cFlag;   
}

I2C_RESULT sendMTKpacket(char *packet)
{
I2C_RESULT i2cFlag = I2C_SUCCESS;
int len;

    len = strlen(packet);
    if (len > 255)
    {
        printf("packet too long!\n\r");
        i2cFlag = I2C_ERROR;
    }
    else
    {
        printf("GPS command: %s", packet );
        i2cFlag = I2C_Write(I2C2, GPS_DEV_ID, packet, &len);
    }
    DelayMs(MAXWAITSENTENCE);
    return i2cFlag;
}

/* End of file GPS-I2C.c */
