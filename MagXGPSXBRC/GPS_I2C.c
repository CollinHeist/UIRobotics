#include "GPS_I2C.h"

 BYTE gpsStr[256]= {0};

/* ------------------------------- setGPS_RMC --------------------------------
  @ Summary
     Configures the GPS to only send the $RMC Packet, reduces I2C parsing needed
  @ Parameters
     None
  @ Returns
     I2C_result : A flag ensuring the communication was successful
  ---------------------------------------------------------------------------- */
I2C_RESULT setGPS_RMC(void) {
    I2C_RESULT i2cFlag;
    int dly = 0;
    do {
        i2cFlag = sendMTKpacket(PMTK_SET_NMEA_OUTPUT_RMCONLY);
        DelayMs(MAXWAITSENTENCE);
    } while (dly);
    
    return i2cFlag;
}

/* ----------------------------- GPS_DECODE_RMC ------------------------------
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
                s2[len - 1] = 0;
				printf("%s --> %d \n\r", s2, len - 1);

				// Read the serial communication, store results in corresponding gps struct
                sscanf(s2,"$GNRMC,%d.000,%c,%f,%c,%f,%c,%f,%f,%d,,,%c*%x",\
                           &gps.utc_time,&gps.status,&gps.lat,&gps.ns,&gps.lon,\
                           &gps.ew,&gps.speed,&gps.angle,&gps.date,&gps.mode,\
                           &gps.cksum);
            }
            else {
				printf("%s --> %d \n\r", s2, len - 1);
                len = 0;
			}
        }
    }
    
    return len;
}

/* -------------------------------- ReportGPS --------------------------------
  @ Summary
     Reads from the GPS, and returns the value as a result
  @ Description
     Function for actually filling a struct with GPS results
  @ Parameters
     @ param1 : Boolean on whether or not to output each GPS read
  @ Returns
     I2C_RESULT : Struct to fill with GPS results
  ---------------------------------------------------------------------------- */
I2C_RESULT ReportGPS (int show) {
	I2C_RESULT i2cFlag;
	int len = 255;
	static int hour, min, sec;
	int pkt_len = 0;

	DelayMs(5);     // Give GPS time to up load new buffer
	len = MAX_PACKET_SIZE;
	i2cFlag = I2C_Read(I2C1, GPS_DEV_ID, gpsStr, &len);

	if (i2cFlag == I2C_SUCCESS && (len > 73) && (len < 256)); {
		gpsStr[len] = 0;
		pkt_len = GPS_DECODE_RMC(gpsStr);
		if (show) {
			hour = (gps.utc_time / 10000);
			min = (gps.utc_time - hour * 10000) / 100;
			sec = (gps.utc_time - (hour * 10000 + min * 100));
			hour -= 7;
			if (hour < 0)
				hour += 24;        
			printf("Time: %2d:%2d:%2d  LAT:%f  LON:%f\n\r",\
					hour, min, sec, gps.lon, gps.lat);
		}
	}
        
	return i2cFlag;   
}

/* ------------------------------ sendMTKpacket ------------------------------
  @ Summary
     Writes a provided char[] packet to the GPS over I2C
  @ Parameters
     @ param1 : Character array to be sent to the GPS - Max of 255 characters
  @ Returns
     I2C_RESULT : Either an error or success flag from the GPS, if write was sucessful
  ---------------------------------------------------------------------------- */
I2C_RESULT sendMTKpacket (char *packet) {
	I2C_RESULT i2cFlag = I2C_SUCCESS;
	int len;

	len = strlen(packet);
	// If the packet is >255 characters, error and return. Otherwise, send to the GPS
	if (len > 255) {
		printf("Packet too long, cannot be sent\n\r");
		i2cFlag = I2C_ERROR;
	}
	else {
		printf("GPS command: %s", packet);
		i2cFlag = I2C_Write(I2C1, GPS_DEV_ID, packet, &len);
	}
	DelayMs(MAXWAITSENTENCE);
    
	return i2cFlag;
}