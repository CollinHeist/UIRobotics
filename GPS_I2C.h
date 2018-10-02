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
#include <plib.h>
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

    struct gps_time gps;
    
    // *****************************************************************************
    // *****************************************************************************
    // Section: Interface Functions
    // *****************************************************************************
    // *****************************************************************************

    /*  A brief description of a section can be given directly below the section
        banner.
     */

    // *****************************************************************************
    /**
      @Function
        I2C_RESULT GPS_I2C_Init(I2C_MODULE i2c_port); 

      @Summary
        Initializes the I2C port for GPS communications

      @Description
        Full description, explaining the purpose and usage of the function.
        <p>
        Additional description in consecutive paragraphs separated by HTML 
        paragraph breaks, as necessary.
        <p>

      @Precondition
        List and describe any required preconditions. If there are no preconditions,
        enter "None."

      @Parameters
        @param param1 I2C port.
    
        @param param2 communications speed: 100K to 400K bps.

      @Returns
        Result of I2C operation
        <ul>
          <li>1   Indicates an error occurred
          <li>0   Indicates an error did not occur
        </ul>

      @Remarks

      @Example
        @code
            I2C_RESULT I2C_ops;
            I2C_ops =  GPS_I2C_Init(1);
     */
    I2C_RESULT GPS_I2C_Init(I2C_MODULE i2c_port, int speed);
    
    
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
        @param param1 Describe the first parameter to the function.
    
        @param param2 Describe the second parameter to the function.

      @Returns
        List (if feasible) and describe the return values of the function.
        <ul>
          <li>1   Indicates an error occurred
          <li>0   Indicates an error did not occur
        </ul>

      @Remarks
        Describe any special behavior not described above.
        <p>
        Any additional remarks.

      @Example
        @code
            BOOL I2C_result;
            I2C_result =  GPS_I2C_Init(1);
        
     */
    I2C_RESULT GPS_I2C_Read(I2C_MODULE i2c_port, BYTE DeviceAddress, BYTE *str, int *len);
    int GPS_DECODE_RMC(BYTE *str);


#endif /* _GPS_I2c_H */

/* *****************************************************************************
 End of File
 */
