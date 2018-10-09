/* ------------------- Hardware and Common Library Includes ------------------ */
#include "hardware.h"
#include "swDelay.h"
#include "GPS_I2C.h"
#include "I2C_lib.h"

#include <plib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

/* ----------------------------- GPS_DECODE_RMC ------------------------------
  @ Syntax
     int msgLength = GPS_DECODE_RMC(str);
  @ Summary
     Reads the $GNRMC message of the GPS
  @ Description
     The struct in GPS_I2C.h has values filled upon function call
  @ Parameters
     param1 : String read from the GPS that is to be parsed
  @ Returns
     int : How long the parsed communication was
  ---------------------------------------------------------------------------- */
int GPS_DECODE_RMC(BYTE *str) {
	char *s2, *pos, *eol;
	int len = 0;

	// Ignore all substrings that do not have $GNRMC in them
    s2 = strstr(str, "$GNRMC");
    if (s2 != NULL) {
        eol = strchr(s2, LF);
        if (eol != NULL) {
            len = (int) (eol - s2);
            if (len > 70) {	// Test for full message
                s2[len-1] = 0;
				// printf("%s --> %d \n\r", s2, len-1);

				// Read the serial communication, store results in corresponding gps struct
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
