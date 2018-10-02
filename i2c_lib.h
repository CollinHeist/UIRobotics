/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.h

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

#ifndef _I2C_LIB_H    /* Guard against multiple inclusion */
#define _I2C_LIB_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

/* This section lists the other files that are included in this file.
 */

/* TODO:  Include other files here if needed. */

    #include "hardware.h"
    #include <plib.h>

    /* ************************************************************************** */
    /* ************************************************************************** */
    /* Section: Constants                                                         */
    /* ************************************************************************** */
    /* ************************************************************************** */

    /*  A brief description of a section can be given directly below the section
        banner.
     */


    /* ************************************************************************** */
    /** Descriptive Constant Name

      @Summary
        Brief one-line summary of the constant.
    
      @Description
        Full description, explaining the purpose and usage of the constant.
        <p>
        Additional description in consecutive paragraphs separated by HTML 
        paragraph breaks, as necessary.
        <p>
        Type "JavaDoc" in the "How Do I?" IDE toolbar for more information on tags.
    
      @Remarks
        Any additional remarks
     */
    #define I2C_SPEED_STANDARD        100000
    #define I2C_SPEED_FAST            400000

    // *****************************************************************************
    // *****************************************************************************
    // Section: Data Types
    // *****************************************************************************
    // *****************************************************************************

    /*  A brief description of a section can be given directly below the section
        banner.
     */


    // *****************************************************************************

    /** Descriptive Data Type Name

      @Summary
        Brief one-line summary of the data type.
    
      @Description
        Full description, explaining the purpose and usage of the data type.
        <p>
        Additional description in consecutive paragraphs separated by HTML 
        paragraph breaks, as necessary.
        <p>
        Type "JavaDoc" in the "How Do I?" IDE toolbar for more information on tags.

      @Remarks
        Any additional remarks
        <p>
        Describe enumeration elements and structure and union members above each 
        element or member.
     */
/*
    typedef struct _example_struct_t {
        // Describe structure member. 
        int some_number;

        // Describe structure member. 
        BOOL some_flag;

    } example_struct_t;
*/

    // *****************************************************************************
    // *****************************************************************************
    // Section: Interface Functions
    // *****************************************************************************
    // *****************************************************************************

    /*  A brief description of a section can be given directly below the section
        banner.
     */

    //***********************************************************************
    /**
      @Function
        I2C_RESULT I2C_Init(I2C_MODULE i2c_port,  int speed); 

      @Summary
        Initializes the specified I2C port for the specified bit rate

      @Description

      @Precondition
        PCLK specified in hardware.c

      @Parameters
        @param param1 I2C port 
   
        @param param2 I2C clock rate: 100000 <= speed <= 400000

      @Returns
        Results of I2C operation
        <ul>
          <li>1   Indicates an error occurred
          <li>0   Indicates an error did not occur
        </ul>

      @Remarks
        Works for all I2C ports

      @Example
        @code
        I2C_RESULT success;
     unsigned int speed = 100000;
        I2C_Init(I2C1,  int speed);
     */
    I2C_RESULT I2C_Init(I2C_MODULE i2c_port,  int speed);

    //***********************************************************************
    /**
      @Function
        I2C_RESULT I2C_Read(I2C_MODULE i2c_port, BYTE DeviceAddress,\
                           BYTE *str, int *len); 

      @Summary
        Reads I2C data

      @Description
        Reads the specified number of bytes of data into a byte array from the
        the specified device using the specified I2C port

      @Precondition
        I2C port has been initialized

      @Parameters
        @param param1 I2C port 
   
        @param param2 I2C device address

        @param param3 pointer to byte array

        @param param4 pointer to parameter that specifies number of bytes to be
                      read and contains the actual number of bytes read when
                      function is complete. 
      @Returns
        Results of I2C operation
        <ul>
          <li>1   Indicates an error occurred
          <li>0   Indicates an error did not occur
        </ul>

      @Remarks
        Works for all I2C ports

      @Example
        @code
        I2C_RESULT success;
        unsigned int len;
        BYTE data[100];
        I2C_Read(I2C1,  DEVICE_ID, str, &len);
     */
    I2C_RESULT I2C_Read(I2C_MODULE i2c_port, BYTE DeviceAddress, BYTE *str, int *len);
    
    //***********************************************************************
    /**
      @Function
    I2C_RESULT I2CReadRegs(I2C_MODULE i2c_port, BYTE DeviceAddress,
                  int reg_addr, BYTE *i2cData, int len);

      @Summary
        Reads series of I2C device registers

      @Description
        Reads the specified number of bytes of data starting at the specified
        register address into a byte array from the
        the specified device using the specified I2C port

      @Precondition
        I2C port has been initialized

      @Parameters
        @param param1 I2C port 
   
        @param param2 I2C device address

        @param param3 Starting register address
     
        @param param4 pointer to byte array

        @param param5 pointer to parameter that specifies number of bytes to be
                      read and contains the actual number of bytes read when
                      function is complete. 
      @Returns
        Results of I2C operation
        <ul>
          <li>1   Indicates an error occurred
          <li>0   Indicates an error did not occur
        </ul>

      @Remarks
        Works for all I2C ports

      @Example
        @code
        I2C_RESULT success;
        unsigned int len;
        BYTE data[100];
        BYTE reg_addr;
        I2C_Read(I2C1,  DEVICE_ID, str, reg_addr, &len);
     */
    I2C_RESULT I2CReadRegs(I2C_MODULE i2c_port, BYTE DeviceAddress,
                  int reg_addr, BYTE *i2cData, int len);

#endif /* _I2C_LIB_H */

/* *****************************************************************************
 End of File
 */
