/* ************************************************************************** */
/** Descriptive File Name

  @Author
    Richard Wall

  @File Name
    i2c_lib.c

  @Summary
    I2C Driver.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

/* This section lists the other files that are included in this file.
 */

#include "hardware.h"
#include "swDelay.h"
#include "I2C_lib.h"

#ifdef WF32
    #include <plib.h>
#endif
#include <string.h>
#include <stdio.h>
#include <stdint.h>


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */

/*  A brief description of a section can be given directly below the section
    banner.
 */

/* ************************************************************************** */
/** Descriptive Data Item Name

  @Summary
    Brief one-line summary of the data item.
    
  @Description
    Full description, explaining the purpose and usage of data item.
    <p>
    Additional description in consecutive paragraphs separated by HTML 
    paragraph breaks, as necessary.
    <p>
    Type "JavaDoc" in the "How Do I?" IDE toolbar for more information on tags.
    
  @Remarks
    Any additional remarks
 */

static BOOL StartTransfer( I2C_MODULE i2c_port, BOOL restart );
static BYTE calc_ck_sum(char *str);
static  I2C_RESULT ReceiveOneByte( I2C_MODULE i2c_port, BYTE *data, BOOL ack );
static BOOL TransmitOneByte( I2C_MODULE i2c_port, BYTE data );
static void StopTransfer( I2C_MODULE i2c_port );

/* ************************************************************************** */
/* ************************************************************************** */
// Section: Local Functions                                                   */
/* ************************************************************************** */
/* ************************************************************************** */

/*  A brief description of a section can be given directly below the section
    banner.
 */

/* ************************************************************************** */

/* ************************************************************************** */
/* ************************************************************************** */
// Section: Interface Functions                                               */
/* ************************************************************************** */
/* ************************************************************************** */

/*  A brief description of a section can be given directly below the section
    banner.
 */

// *****************************************************************************

/** 
  @Function
    int ExampleInterfaceFunctionName ( int param1, int param2 ) 

  @Summary
    Brief one-line description of the function.

  @Remarks
    Refer to the example_file.h interface header for function usage details.
 */

I2C_RESULT I2C_Init(I2C_MODULE i2c_port,  int speed)
{
BOOL i2cFlag;
int actualClock;
/* Set Desired Operation Frequency */
    actualClock = I2CSetFrequency(i2c_port,GetPeripheralClock(),speed);
    if ( abs(actualClock-speed) > speed/10 )
    {
        printf("I2C setup error\n\r");
        printf("Clock frequency (%d) error exceeds 10\%\r\n",\
               (unsigned int) actualClock);
        i2cFlag = I2C_ERROR;
    }
    else
    {
        printf("I2C%d setup complete\n\r", i2c_port+1);
        printf("I2C clock frequency = %ld Hz\n\r", (unsigned int) actualClock);
        I2CEnable(i2c_port, TRUE);
        i2cFlag = I2C_SUCCESS;
    }   
    return i2cFlag;
}

/* ========================================================================= */
I2C_RESULT I2C_Write(I2C_MODULE i2c_port, BYTE DeviceAddress, BYTE *str, int *len)
{
I2C_7_BIT_ADDRESS   SlaveAddress;
int                 dataIndex = 0;
I2C_RESULT          i2c_result = I2C_SUCCESS;   /* Status of I2C action */
BOOL                okay = TRUE;

    okay = StartTransfer(i2c_port, TRUE); /* TRUE - repeated start */
    if((i2c_result == I2C_SUCCESS) && okay)
    {
        I2C_FORMAT_7_BIT_ADDRESS(SlaveAddress, DeviceAddress, I2C_WRITE);
        if(!TransmitOneByte( i2c_port, SlaveAddress.byte ))
        {
            if(!I2CByteWasAcknowledged(i2c_port))
            {
                printf("Error: Sent byte was not acknowledged\n");
                i2c_result = I2C_ERROR;
            }
        }
        dataIndex = 0;  /* Reset the data array index */
    }
/* Write all bytes */
	while((dataIndex < *len) && (okay))
	{
        okay =  TransmitOneByte(i2c_port, str[dataIndex++]);
	}
    StopTransfer(i2c_port); /* Terminate the EEPROM transfer */
    if(okay)
    {
        i2c_result = I2C_SUCCESS;
    }
    else
    {
        i2c_result = I2C_ERROR;
    }
        
    *len = dataIndex; 
    return i2c_result;
}


/* ========================================================================= */
I2C_RESULT I2C_Read(I2C_MODULE i2c_port, BYTE DeviceAddress, BYTE *str, int *len)
{
I2C_7_BIT_ADDRESS   SlaveAddress;
// BYTE                data_byte;
int                 dataIndex = 0;
I2C_RESULT          i2c_result = I2C_SUCCESS;   // Status of I2C action
BOOL                okay;

//    okay = StartTransfer(i2c_port, TRUE); // TRUE - repeated start
    okay = StartTransfer(i2c_port, FALSE);  // FALSE - Not repeated start
    if((i2c_result == I2C_SUCCESS) && okay)
    {
        I2C_FORMAT_7_BIT_ADDRESS(SlaveAddress, DeviceAddress, I2C_READ);
        okay = TransmitOneByte( i2c_port, SlaveAddress.byte );
        if(!okay)
        {
            printf("I2C error - ");
            okay = I2CByteWasAcknowledged(i2c_port);
            if(!okay)
            {
                printf("Sent byte was not acknowledged\n\r");
            }
            else
            {
                printf("\n\r");
            }
            i2c_result = I2C_ERROR;
        }
        dataIndex = 0;  /* Reset the data array index */
    }
/* Read all but the last byte with ACK bit set */
	while((dataIndex < (*len)-1) && (i2c_result == I2C_SUCCESS))
	{
            i2c_result |=  ReceiveOneByte(i2c_port, &str[dataIndex++] , TRUE);
	}
/* Read the last byte without ACK bit set */
    if(i2c_result == I2C_SUCCESS)
    {
        i2c_result |= ReceiveOneByte(i2c_port, &str[dataIndex++], FALSE );
    }
   
    StopTransfer(i2c_port); /* Terminate the EEPROM transfer */
    *len = dataIndex;     
    return i2c_result;
}

/* I2CReadRegs FUNCTION DESCRIPTION ****************************************
 *
 * SYNTAX:      I2C_RESULT I2CReadEEPROM(I2C_MODULE i2c_port, int DeviceAddress,
 *                                 int mem_addr, BYTE *i2cData, int len);
 * KEYWORDS:   Read EEPROM, device address, memory address
 * DESCRIPTION:	Reads the number of specified bytes from the EEPROM starting
 *              at the specified memory address (0x0000 to 0x7FFF).
 * Parameter 1:	I2C_MODULE - i2c port: I2C1 or I2C2
 * Parameter 2:	Device address
 * Parameter 3:	Register starting address
 * Parameter 4:	Array of 8 bit data to be read from the EEPROM
 * Parameter 5:	Number of data bytes to read
 * RETURN VALUE: I2C_RESULT - i2c transaction results 
 * NOTES:   There is no restriction on the number of bytes that can
 *          be read in a single operation
END DESCRIPTION **********************************************************/
I2C_RESULT I2CReadRegs(I2C_MODULE i2c_port, BYTE DeviceAddress,
                  int reg_addr, BYTE *i2cData, int len)
{
BYTE                header[3];
I2C_7_BIT_ADDRESS   SlaveAddress;
int                 headerIndex = 0;
int                 dataIndex = 0;
I2C_RESULT          i2c_result;
BOOL                okay;

/* Set up EEPROM header using write sequence */
    I2C_FORMAT_7_BIT_ADDRESS(SlaveAddress, DeviceAddress, I2C_WRITE);
    header[0] = SlaveAddress.byte;
    header[1] = (BYTE) (reg_addr >> 8);
    headerIndex = 0;

/* Write starting address - this must use a START sequence but not a STOP. A
 * REPEATED START sequence will bother terminate the address write and
 * start the memory read phase. The address write phase polls acknowledgment
 * from slave device to ascertain the device is communicating.
 *
 */
    okay = StartTransfer(i2c_port, FALSE); /* FALSE - no repeated start */
    i2c_result = I2C_SUCCESS;

    while((headerIndex < 2)  && (i2c_result == I2C_SUCCESS) && okay)
    {
        if (TransmitOneByte(i2c_port, header[headerIndex++]))
        {
/* Verify that the byte was acknowledged */
            if(!I2CByteWasAcknowledged(i2c_port))
            {
                printf("Error: Sent byte was not acknowledged\n");
                i2c_result |= I2C_ERROR;
            }
        }
        else
        {
            i2c_result |= I2C_ERROR;
        }
    }

/*The read cycle continues only if successful in setting the starting address*/

    if(i2c_result == I2C_SUCCESS)
    {
        okay = StartTransfer(i2c_port, TRUE); /* TRUE - repeated start */
        if((i2c_result == I2C_SUCCESS) && okay)
        {
            I2C_FORMAT_7_BIT_ADDRESS(SlaveAddress, DeviceAddress, I2C_READ);
            if(!TransmitOneByte( i2c_port, SlaveAddress.byte ))
            {
                if(!I2CByteWasAcknowledged(i2c_port))
                {
                    printf("Error: Sent byte was not acknowledged\n");
                    i2c_result = I2C_ERROR;
                }
            }
            dataIndex = 0;  /* Reset the data array index */
        }
/* Read all but the last byte with ACK bit set */
	while((dataIndex < len-1) && (i2c_result == I2C_SUCCESS))
	{
            i2c_result |=  ReceiveOneByte(i2c_port, &i2cData[dataIndex++],TRUE);
	}
/* Read the last byte without ACK bit set */
        if(i2c_result == I2C_SUCCESS)
        {
            i2c_result |= ReceiveOneByte(i2c_port, &i2cData[dataIndex],FALSE );
        }
    }
    StopTransfer(i2c_port); /* Terminate the EEPROM transfer */
    return i2c_result;
}


/******************************************************************************
 * Function:    static BOOL TransmitOneByte( I2C_MODULE i2c_port, BYTE data )
 * Summary:     This transmits one byte to the EEPROM.
 * Description: This transmits one byte to the EEPROM, and reports errors for
 *              any bus collisions.
 * Precondition: The transfer must have been previously started.
 *
 * Parameters:  i2c_port    - I2C_MODULE
 *              data        - Data byte to transmit
 *
 * Returns:     TRUE    - Data was sent successfully
 *              FALSE   - A bus collision occurred
 *
 * Remarks:     This is a blocking routine that waits for the transmission to
 *              complete.
  ****************************************************************************/
static BOOL TransmitOneByte( I2C_MODULE i2c_port, BYTE data )
{
/* Wait for the transmitter to be ready */
    while(!I2CTransmitterIsReady(i2c_port));
/* Transmit the byte */
    if(I2CSendByte(i2c_port, data) == I2C_MASTER_BUS_COLLISION)
    {
        printf("Error: I2C Master Bus Collision\n");
        return FALSE;
    }
/* Wait for the transmission to finish */
    while(!I2CTransmissionHasCompleted(i2c_port));
    return TRUE;
}

/******************************************************************************
 * Function:    static I2C_RESULT ReceiveOneByte( I2C_MODULE i2c_port,
 *                                          BYTE *data, BOOL ack );
 * Summary:     This transmits one byte to the EEPROM.
 * Description: This transmits one byte to the EEPROM, and reports errors for
 *              any bus collisions.
 * Precondition: The transfer must have been previously started.
 *
 * Parameters:  i2c_port    - I2C_MODULE
 *              data        - Data byte to transmit
 *              ack         - TRUE - Send ACK
 *                          - FALSE - SEND NO ACK
 *
 * Returns:     TRUE    - Data was sent successfully
 *              FALSE   - A bus collision occurred
 *
 * Remarks:     This is a blocking routine that waits for the transmission to
 *              complete.
  ****************************************************************************/
static  I2C_RESULT ReceiveOneByte( I2C_MODULE i2c_port, BYTE *data, BOOL ack )
 {
 I2C_RESULT i2c_result = I2C_SUCCESS;
    if(I2CReceiverEnable(i2c_port, TRUE) == I2C_RECEIVE_OVERFLOW)
    {
        printf("Error: I2C Receive Overflow\n");
        i2c_result =  I2C_RECEIVE_OVERFLOW;
    }
    else
    {
        while(!I2CReceivedDataIsAvailable(i2c_port));
/* The "ack" parameter determines if the EEPROM read is acknowledged */
        I2CAcknowledgeByte(i2c_port, ack);
        while(!I2CAcknowledgeHasCompleted(i2c_port));
/* Read the received data byte */
        *data = I2CGetByte(i2c_port);
    }
    return i2c_result;
 } /* End of ReceiveOneByte */


/******************************************************************************
 * Function:    static BOOL StartTransfer( I2C_MODULE i2c_port, BOOL restart );
 * Summary:     Starts (or restarts) a transfer to/from the EEPROM.
 * Description: This routine starts (or restarts) a transfer to/from the EEPROM,
 *              waiting (in a blocking loop) until the start (or re-start)
 *              condition has completed.
 *
 * Precondition: The I2C module must have been initialized.
 * Parameters:  restart - If FALSE, send a "Start" condition
 *                      - If TRUE, send a "Restart" condition
 *              i2c_port- I2C1 or I2C2
 * 
 * Returns:     TRUE    - If successful
 *              FALSE   - If a collision occurred during Start signalling
 * Remarks:     This is a blocking routine that waits for the bus to be idle
 *              and the Start (or Restart) signal to complete.
  *****************************************************************************/
static BOOL StartTransfer( I2C_MODULE i2c_port, BOOL restart )
{
I2C_STATUS  status;

/* Send the Start (or Restart) sequence */
    if(restart)
    {
        I2CRepeatStart(i2c_port);
    }
    else
    {
/* Wait for the bus to be idle, then start the transfer */
        while( !I2CBusIsIdle(i2c_port) );

        if(I2CStart(i2c_port) != I2C_SUCCESS)
        {
            printf("Error: Bus collision during transfer Start\n");
            return FALSE;
        }
    }

/* Wait for the START or REPEATED START to complete */
    do
    {
        status = I2CGetStatus(i2c_port);
    } while ( !(status & I2C_START) );

    return TRUE;
}

/******************************************************************************
 * Function:    static void StopTransfer( I2C_MODULE i2c_port )
 * Summary:     Stops a transfer to/from the EEPROM.\
 *
 * Description: This routine Stops a transfer to/from the EEPROM, waiting (in a
 *              blocking loop) until the Stop condition has completed.
 *
 * Precondition: The I2C module must have been initialized & a transfer started.
 *
 * Parameters:  i2c_port    - I2C_MODULE
 * Returns:     None.
 * Remarks:     This is a blocking routine that waits for the Stop sequence to
 *              complete.
  ****************************************************************************/

static void StopTransfer( I2C_MODULE i2c_port )
{
I2C_STATUS  i2c_status;

/* Send the STOP sequence */
    I2CStop(i2c_port);

/* Wait for the STOP sequence to complete */
    do
    {
        i2c_status = I2CGetStatus(i2c_port);

    } while ( !(i2c_status & I2C_STOP) );
}


/*FUNCTION DESCRIPTION ******************************************************
 * SYNTAX:          static BYTE calc_ck_sum(char *str);
 *
 * KEYWORDS:        check sum, GPS, sentence
 * DESCRIPTION:     This function computes the checksum for a NMEA sentence
 *                  between the "$" and "*" characters,
 * Parameter 1:     string containing the GPS sentence
 * RETURN VALUE:    check_sum of BYTE type
 * NOTES:           Called from set_gps(void) function.
 * END DESCRIPTION **********************************************************/
static BYTE calc_ck_sum(char *str)
{
BYTE cksum = 0;
BOOL start_flag = FALSE;
BOOL stop_flag = FALSE;

    do
    {   /* Search for "$" character */
        if(start_flag == FALSE )
        {
            if(*str == 0x24)
            {
                start_flag = TRUE;
            }
        }
        else
        {/* Search for "*" character signifying the end of sentence */
            if(*str == '*')
            {
                stop_flag = TRUE;   /* Mark end of GPS sentence */
            }
            else
            {
                cksum ^= (BYTE) ( *str );   /* Update check sum */
            }
        }
        str++;
    } while((*str != 0x00) && (stop_flag == FALSE)); /* End of string or "*" */
    return cksum;
} /* End of calc_ck_sum */
/* *****************************************************************************
 End of File
 */
