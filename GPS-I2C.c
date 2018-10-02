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

#include <plib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
    
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
//              printf("%s --> %d \n\r", s2, len-1);
                sscanf(s2,"$GNRMC,%d.000,%c,%f,%c,%f,%c,%f,%f,%d,,,%c*%x",\
                           &gps.utc_time,&gps.status,&gps.lat,&gps.ns,&gps.lon,\
                           &gps.ew,&gps.speed,&gps.angle,&gps.date,&gps.mode,\
                           &gps.cksum);
                invLED7();
            }
            else
                len = 0;
        }
    }
    return len;
}


/* End of file GPS-I2C.c */