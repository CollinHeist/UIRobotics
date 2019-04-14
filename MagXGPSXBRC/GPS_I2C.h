/* --------------------- Guarding against multiple inclusion ----------------- */
#ifndef __GPS_I2C_H__
	#define __GPS_I2C_H__

    /* ----------------- Necessary system include statements ----------------- */
    #include "hardware.h"
    #include <plib.h>
    #include <string.h>
    #include <stdio.h>
    #include <stdint.h>

	#include "i2c_lib.h"
    #include "swDelay.h"
   
	/* ----------------- Public Global Variables / Constants ----------------- */
	#define GPS_MPH_PER_KNOT    1.15077945
	#define GPS_MPS_PER_KNOT    0.51444444
	#define GPS_KMPH_PER_KNOT   1.852
	#define GPS_MILES_PER_METER 0.00062137112
	#define GPS_KM_PER_METER    0.001

	#define LF                  0x0A
	#define GPS_DEV_ID          0x10    // I2C device address

	#define MAX_PACKET_SIZE     255

	/* --------- Structure to hold the information in the GPS Packet --------- */
	struct gps_time {
		unsigned int utc_time;
		char status;
		float lat;
		char ns;
		float lon;
		char ew;
		float speed;
		float angle;
		unsigned int date;
		char mode;
		int cksum;
	};

	/* ------------------- Commands to set the update rate ------------------- */
	/* -------------- These adjust echo rate, not refresh rate --------------- */
	#define PMTK_SET_NMEA_UPDATE_100_MILLIHERTZ  "$PMTK220,10000*2F\r\n"	// .1 Hz
	#define PMTK_SET_NMEA_UPDATE_200_MILLIHERTZ  "$PMTK220,5000*1B\r\n"		// .2 Hz
	#define PMTK_SET_NMEA_UPDATE_1HZ  "$PMTK220,1000*1F\r\n"				// 1 Hz
	#define PMTK_SET_NMEA_UPDATE_2HZ  "$PMTK220,500*2B\r\n"					// 2 Hz
	#define PMTK_SET_NMEA_UPDATE_5HZ  "$PMTK220,200*2C\r\n"					// 5 Hz
	#define PMTK_SET_NMEA_UPDATE_10HZ "$PMTK220,100*2F\r\n"					// 10 Hz, repeat data
	/* ---------------------- These adjust refresh rate ---------------------- */
	#define PMTK_API_SET_FIX_CTL_100_MILLIHERTZ  "$PMTK300,10000,0,0,0,0*2C\r\n"	// .1 Hz
	#define PMTK_API_SET_FIX_CTL_200_MILLIHERTZ  "$PMTK300,5000,0,0,0,0*18\r\n"		// .2 Hz
	#define PMTK_API_SET_FIX_CTL_1HZ  "$PMTK300,1000,0,0,0,0*1C\r\n"				// 1 Hz
	#define PMTK_API_SET_FIX_CTL_5HZ  "$PMTK300,200,0,0,0,0*2F\r\n"					// 5 Hz, max refresh rate

	#define PMTK_SET_BAUD_57600 "$PMTK251,57600*2C\r\n"
	#define PMTK_SET_BAUD_9600  "$PMTK251,9600*17\r\n"


	/* -------- Command to only send certain output structure (GPRMC) -------- */
	#define PMTK_SET_NMEA_OUTPUT_RMCONLY "$PMTK314,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29\r\n"
	/* --------- This one only sends the GPRMC and GPRGGA structures --------- */
	#define PMTK_SET_NMEA_OUTPUT_RMCGGA "$PMTK314,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28\r\n"
	/* ------------------- Sends data from all structures -------------------- */
	#define PMTK_SET_NMEA_OUTPUT_ALLDATA "$PMTK314,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0*28\r\n"
	/* ---------------- Sends data from none of the structures --------------- */
	#define PMTK_SET_NMEA_OUTPUT_OFF "$PMTK314,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28\r\n"

	#define PMTK_LOCUS_STARTLOG     "$PMTK185,0*22\r\n"
	#define PMTK_LOCUS_STOPLOG      "$PMTK185,1*23\r\n"
	#define PMTK_LOCUS_STARTSTOPACK "$PMTK001,185,3*3C\r\n"
	#define PMTK_LOCUS_QUERY_STATUS "$PMTK183*38\r\n"
	#define PMTK_LOCUS_ERASE_FLASH  "$PMTK184,1*22\r\n"
	#define LOCUS_OVERLAP           0
	#define LOCUS_FULLSTOP          1

	#define PMTK_ENABLE_SBAS        "$PMTK313,1*2E\r\n"
	#define PMTK_ENABLE_WAAS        "$PMTK301,2*2E\r\n"

	#define PMTK_STANDBY            "$PMTK161,0*28\r\n"
	#define PMTK_STANDBY_SUCCESS    "$PMTK001,161,3*36\r\n"  // Not needed currently
	#define PMTK_AWAKE              "$PMTK010,002*2D\r\n"

	#define PMTK_Q_RELEASE          "$PMTK605*31\r\n"

	#define PGCMD_ANTENNA           "$PGCMD,33,1*6C\r\n" 
	#define PGCMD_NOANTENNA         "$PGCMD,33,0*6D\r\n" 

	#define MAXWAITSENTENCE         10

	struct gps_time gps;
	BYTE gpsStr[256] = {0};
    
	/* ---------------------- Public Function declarations ------------------- */
	I2C_RESULT GPS_I2C_Read(I2C_MODULE i2c_port, BYTE DeviceAddress, BYTE *str, int *len);
	int GPS_DECODE_RMC(BYTE *str);
	I2C_RESULT ReportGPS(int show);
	I2C_RESULT sendMTKpacket(char *command);
	I2C_RESULT setGPS_RMC(void);
	BYTE calcCRCforMTK(char *sentence, char *crcStr); //XORs all bytes between $ and *
#endif