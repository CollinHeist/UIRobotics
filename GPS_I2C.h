/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.h

  @Summary
    XA1110 I2C Communications
 
  @Description
    Interface for the Sparkfun GPS_14414 breakout board hosting the XA1110 
    Quiic Titan X1 Ver 10 I2C GPS with I2C communications.
 */
/* ************************************************************************** */

#ifndef _GPS_I2c_H    /* Guard against multiple inclusion */
#define _GPS_I2c_H

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

/* This section lists the other files that are included in this file.
 */

#include "hardware.h"
#ifndef WF32
    #include <plib.h>
#endif

#include "I2C_lib.h"

    /* ************************************************************************** */
    /* ************************************************************************** */
    /* Section: Constants                                                         */
    /* ************************************************************************** */
    /* ************************************************************************** */


    /* ************************************************************************** */
    /** Descriptive Constant Name

      @Summary
        GPS conversion constants
    
      @Description
        Linear distance
     */

    #define GPS_VERSION 13 // software version of this library
    #define GPS_MPH_PER_KNOT 1.15077945
    #define GPS_MPS_PER_KNOT 0.51444444
    #define GPS_KMPH_PER_KNOT 1.852
    #define GPS_MILES_PER_METER 0.00062137112
    #define GPS_KM_PER_METER 0.001

    #define LF  0x0A
    #define GPS_DEV_ID  0x10    // I2C device address
    
    #define MAX_PACKET_SIZE 255


    // *****************************************************************************
    // *****************************************************************************
    // Section: Data Types
    // *****************************************************************************
    // *****************************************************************************

    /*  A brief description of a section can be given directly below the section
        banner.
     */

    // *****************************************************************************

     // *****************************************************************************

    /** Descriptive Data Type Name

      @Description
        NMEA RMC Sentence elements
     
      @Remarks

     */
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
    
// different commands to set the update rate from once a second (1 Hz) to 10 times a second (10Hz)
// Note that these only control the rate at which the position is echoed, to actually speed up the
// position fix you must also send one of the position fix rate commands below too.
    #define PMTK_SET_NMEA_UPDATE_100_MILLIHERTZ  "$PMTK220,10000*2F\r\n" // Once every 10 seconds, 100 millihertz.
    #define PMTK_SET_NMEA_UPDATE_200_MILLIHERTZ  "$PMTK220,5000*1B\r\n"  // Once every 5 seconds, 200 millihertz.
    #define PMTK_SET_NMEA_UPDATE_1HZ  "$PMTK220,1000*1F\r\n"
    #define PMTK_SET_NMEA_UPDATE_2HZ  "$PMTK220,500*2B\r\n"
    #define PMTK_SET_NMEA_UPDATE_5HZ  "$PMTK220,200*2C\r\n"
    #define PMTK_SET_NMEA_UPDATE_10HZ "$PMTK220,100*2F\r\n"
// Position fix update rate commands.
    #define PMTK_API_SET_FIX_CTL_100_MILLIHERTZ  "$PMTK300,10000,0,0,0,0*2C\r\n" // Once every 10 seconds, 100 millihertz.
    #define PMTK_API_SET_FIX_CTL_200_MILLIHERTZ  "$PMTK300,5000,0,0,0,0*18\r\n"  // Once every 5 seconds, 200 millihertz.
    #define PMTK_API_SET_FIX_CTL_1HZ  "$PMTK300,1000,0,0,0,0*1C\r\n"
    #define PMTK_API_SET_FIX_CTL_5HZ  "$PMTK300,200,0,0,0,0*2F\r\n"
// Can't fix position faster than 5 times a second!

    #define PMTK_SET_BAUD_57600 "$PMTK251,57600*2C\r\n"
    #define PMTK_SET_BAUD_9600 "$PMTK251,9600*17\r\n"

// turn on only the second sentence (GPRMC)
    #define PMTK_SET_NMEA_OUTPUT_RMCONLY "$PMTK314,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29\r\n"
// turn on GPRMC and GGA
    #define PMTK_SET_NMEA_OUTPUT_RMCGGA "$PMTK314,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28\r\n"
// turn on ALL THE DATA
    #define PMTK_SET_NMEA_OUTPUT_ALLDATA "$PMTK314,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0*28\r\n"
// turn off output
    #define PMTK_SET_NMEA_OUTPUT_OFF "$PMTK314,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28\r\n"
//Packet 314: The SET_NMEA_OUTPUT command is very long

// To generate your own sentences, check out the MTK command datasheet and use a checksum calculator
// such as the awesome http://www.hhhh.org/wiml/proj/nmeaxor.html

    #define PMTK_LOCUS_STARTLOG  "$PMTK185,0*22\r\n"
    #define PMTK_LOCUS_STOPLOG "$PMTK185,1*23\r\n"
    #define PMTK_LOCUS_STARTSTOPACK "$PMTK001,185,3*3C\r\n"
    #define PMTK_LOCUS_QUERY_STATUS "$PMTK183*38\r\n"
    #define PMTK_LOCUS_ERASE_FLASH "$PMTK184,1*22\r\n"
    #define LOCUS_OVERLAP 0
    #define LOCUS_FULLSTOP 1

    #define PMTK_ENABLE_SBAS "$PMTK313,1*2E\r\n"
    #define PMTK_ENABLE_WAAS "$PMTK301,2*2E\r\n"

// standby command & boot successful message
    #define PMTK_STANDBY "$PMTK161,0*28\r\n"
    #define PMTK_STANDBY_SUCCESS "$PMTK001,161,3*36\r\n"  // Not needed currently
    #define PMTK_AWAKE "$PMTK010,002*2D\r\n"

// ask for the release and version
    #define PMTK_Q_RELEASE "$PMTK605*31\r\n"

// request for updates on antenna status 
    #define PGCMD_ANTENNA "$PGCMD,33,1*6C\r\n" 
    #define PGCMD_NOANTENNA "$PGCMD,33,0*6D\r\n" 
    
    #define MAXWAITSENTENCE 10
    
    struct gps_time gps;
    BYTE gpsStr[256] = {0};
    
    // *****************************************************************************
    // *****************************************************************************
    // Section: Interface Functions
    // *****************************************************************************
    // *****************************************************************************

    /*  A brief description of a section can be given directly below the section
        banner.
     */

    // *****************************************************************************

    // *****************************************************************************
    /**
      @Function
            I2C_RESULT GPS_I2C_Read(I2C_MODULE i2c_port, char *str, int *num); 

      @Summary
        Reads NMEA packet from GPS

      @Description
        Full description, explaining the purpose and usage of the function.
        <p>
        Additional description in consecutive paragraphs separated by HTML 
        paragraph breaks, as necessary.
        <p>
        Type "JavaDoc" in the "How Do I?" IDE toolbar for more information on tags.

      @Precondition
        List and describe any required preconditions. If there are no preconditions,
        enter "None."

      @Parameters
        @param param1 identifies the I2C port
        @param param2 identifies the I2C device
        @param param2 is the BYTE register number to start reading at.
        @param param2 is the pointer to the BYTE array to store device data.
        @param param2 is the pointer to the variable containing the number of 
                      bytes to read.

      @Returns
        I2C_RESULT
        <ul>
          <li>I2C_ERROR   Indicates an error occurred
          <li>I2C_SUCCESS Indicates all I2C transfers successful
        </ul>

      @Remarks
        The "Len" parameter contains the actual number of bytes of data received 

      @Example
        @code
        #define DEV_ID   0x10
        I2C_RESULT  i2cFlag;;
        BYTE packet[256] = {0};
        int len = 255;
        i2cFlag = GPS_I2C_Read(I2C2, DEV_ID, packet, &len);
     */
    I2C_RESULT GPS_I2C_Read(I2C_MODULE i2c_port, BYTE DeviceAddress, BYTE *str, int *len);
    int GPS_DECODE_RMC(BYTE *str);
    I2C_RESULT ReportGPS(int show);
    I2C_RESULT  sendMTKpacket(char *command);
    I2C_RESULT setGPS_RMC(void);
    BYTE calcCRCforMTK(char *sentence, char *crcStr); //XORs all bytes between $ and *
    

#endif /* _GPS_I2c_H */

/* *****************************************************************************
 End of GPD_I2C.h
 */
