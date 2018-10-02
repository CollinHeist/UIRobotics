/* 
** I2Clib.c
** Richard Wall
** August 31, 2014
**
** Notes:   This file performs the low level I2C communications. All functions
 *          require the specification of the I2C port either I2C1 or I2C2.
******************************************************************************/
 
#include <plib.h>
#include "hardware.h"
#include "I2C_lib.h"

static int wait_i2c_xfer(I2C_MODULE i2c_port, BYTE DeviceAddress);

/* I2C_init FUNCTION DESCRIPTION ********************************************
 *
 * SYNTAX:     	void I2C_init(I2C_MODULE i2c_port, unsigned int bitrate);
 * KEYWORDS:	i2c, bit rate generator
 * DESCRIPTION:	Opens I2C channel 1 and sets the i2c bit rate based upon PBCLK
 * Parameter 1:	I2C_MODULE i2c_port: I2C1 or I2C2
 * Parameter 2:	I2C bit rate - must be less than 400000
 * RETURN VALUE: None
 * NOTES:	The desired bit rate. Must be less than	400,000 or the bit rate
 *              of the slowest I2C device on the I2C bus.
END DESCRIPTION **********************************************************/
BOOL I2C_init(I2C_MODULE i2c_port, unsigned int bitrate)
{
int actualClock;
BOOL success;
/* Set Desired Operation Frequency */
    actualClock = I2CSetFrequency(i2c_port,GetPeripheralClock(),bitrate);
    if ( abs(actualClock-bitrate) > bitrate/10 )
    {
        printf("  Clock frequency (%d) error exceeds 10\%\r\n", 
                (unsigned) actualClock);
	success = FALSE;
    }
    else
    {
        printf(" I2C%d clock frequency = %ld Hz\n", i2c_port+1,
                (unsigned) actualClock);
	success = TRUE;
    }
    I2CEnable(i2c_port, TRUE);
    return success;
} /* End of I2C_init */

/* I2C_ReadModbus FUNCTION DESCRIPTION ***************************************
 *
 * SYNTAX:      I2C_RESULT I2C_ReadModbus(MODBUS_DATA_BLOCK blk);
 * KEYWORDS:    Read Device, device address, memory address
 * DESCRIPTION:	Reads a I2C device using the Modbus protocol
 *
 * Parameter 1:	    typedef struct
 *                  {
 *                      I2C_MODULE i2c_channel;
 *                      BYTE dev_id;
 *                      BYTE code;
 *                      BYTE reg_addr;
 *                      BYTE block_size;
 *                      BYTE *data;
 *                      } MODBUS_DATA_BLOCK;
 *
 * RETURN VALUE: I2C_RESULT - i2c transaction results
 * NOTES:	There is no restriction on the number of bytes that can
 * 		be read in a single operation provided that the block size is 
 *		less than the size of an unsigned integer data type.
 *
 *              The read operation receives the following data after sending
 *              the device ID with the READ bit set high:
 *                  READ code - 0x03
 *                  Number of bytes to read - N<10
 *                  [N data bytes]
 *                  2 CRC check bytes
 *              There is a NACK sent after the last CRC byte
END DESCRIPTION **********************************************************/
I2C_RESULT I2C_ReadModbus(MODBUS_DATA_BLOCK  blk)
{
I2C_7_BIT_ADDRESS   SlaveAddress;
BOOL i2c_started;   /* Flag that indicates that I2C start successful */
BYTE * data_ptr;    /* Data buffer pointer */
int count;          /* Byte counter */
I2C_RESULT i2c_ops = I2C_SUCCESS;     /* Status of I2C action */

/* Send device ID with R/W bit set high */
    I2C_FORMAT_7_BIT_ADDRESS(SlaveAddress, blk.dev_id, I2C_READ);
    i2c_started = I2C_StartTransfer(blk.i2c_channel, FALSE); /* No repeated start */
    if(i2c_started)
    {
        if (I2C_TransmitOneByte(blk.i2c_channel, SlaveAddress.byte))
        {
            if(!I2CByteWasAcknowledged(blk.i2c_channel))
            {
                printf("Error: Sent byte was not acknowledged\n");
                i2c_ops = I2C_ERROR;
            }
        }
        else  i2c_ops = I2C_ERROR;  /* Bytes was not sent */
        DelayUs(35);            /* Wait >30us after sending device ID */
    }
    else i2c_ops = I2C_ERROR;   /* I2C was not started */

/* While I2C read is successful, dead all but the last byte with ACK bit set
 * into data buffer */
    count = 0;
    data_ptr = blk.data;
    while((count < blk.block_size + 3) && (i2c_ops == I2C_SUCCESS))
    {
        i2c_ops = I2C_ReceiveOneByte(blk.i2c_channel, data_ptr++, TRUE);
        count++;
    }
/* Read the last byte without ACK bit set */
    if(i2c_ops == I2C_SUCCESS)
    {
        i2c_ops |= I2C_ReceiveOneByte(blk.i2c_channel, data_ptr, FALSE);
    }
    I2C_StopTransfer(blk.i2c_channel); /* Terminate the EEPROM transfer */

    return i2c_ops;
} /* End of I2_CReadModbus */


/* I2C_WrtiteModbus FUNCTION DESCRIPTION *************************************
 *
 * SYNTAX:      I2C_RESULT I2C_WrtiteModbus( MODBUS_DATA_BLOCK);
 * KEYWORDS:	Write, Modbus, acknowledgement, write cycle complete
 * DESCRIPTION:	Writes a block of data to an I2C device using the 
 *              Modbus protocol
 *
 * Parameter 1:	    typedef struct
 *                  {
 *                      I2C_MODULE i2c_channel;
 *                      BYTE dev_id;
 *                      BYTE code;
 *                      BYTE reg_addr;
 *                      BYTE block_size;
 *                      BYTE *data; // optional
 *                      } MODBUS_DATA_BLOCK;
 *
 * RETURN VALUE: I2C_RESULT - i2c transaction results 
 * NOTES:		This write algorithm writes data in 64 byte boundries that
 *				is compatible with I2C EEPROM devices.
 * END DESCRIPTION **********************************************************/
I2C_RESULT I2C_WrtiteModbus(MODBUS_DATA_BLOCK blk)
{
BYTE i2c_packet[14];
I2C_7_BIT_ADDRESS   SlaveAddress;
int count;
I2C_RESULT i2c_ops;     /* Status of I2C action */
BOOL i2c_started;       /* Flag that indicates that I2C start successful */

/* Calculate the number of pages needed and round up always */
    i2c_ops = I2C_SUCCESS;

    I2C_FORMAT_7_BIT_ADDRESS(SlaveAddress, blk.dev_id, I2C_WRITE);

    i2c_packet[0] = SlaveAddress.byte;
    i2c_packet[1] = blk.code;
    i2c_packet[2] = blk.reg_addr;
    i2c_packet[3] = blk.block_size;
    if(blk.code == HUM_WRITE_CODE)  /* command code indicates write data */
    {
        for(count = 0; count<(blk.block_size-4); count++)
        {
            i2c_packet[count+4] = *blk.data++;
        }
    }
    count = 0;
/* Send device and memory address */
    i2c_started = I2C_StartTransfer(blk.i2c_channel, FALSE); /* no repeated start */
    if(i2c_started)
    {
        while((count < blk.block_size)  && (i2c_ops == I2C_SUCCESS))
        {
            if (I2C_TransmitOneByte(blk.i2c_channel, i2c_packet[count++]))
            {
/* Verify that the byte was acknowledged */
                if(!I2CByteWasAcknowledged(blk.i2c_channel))
                {
                    printf("Error: Sent byte was not acknowledged\n");
                    i2c_ops = I2C_ERROR;
                }
            }
            else i2c_ops = I2C_ERROR;   /* Byte was not sent */
            if(count == 1)
                DelayUs(35);    /* Delay >30us after device ID */
        }
        I2C_StopTransfer( blk.i2c_channel );
    }
    else i2c_ops = I2C_ERROR;   /* I2C not started */

    return i2c_ops;
} /* End of I2C_WrtiteModbus */

/* I2C_StartTransfer *********************************************************
 * Function:    BOOL I2C_StartTransfer( I2C_MODULE i2c_port, BOOL restart );
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
 *              FALSE   - If a collision occurred during Start signaling
 * Remarks:     This is a blocking routine that waits for the bus to be idle
 *              and the Start (or Restart) signal to complete.
  ***************************************************************************/

BOOL I2C_StartTransfer( I2C_MODULE i2c_port, BOOL restart )
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
} /* End of I2C_StartTransfer */

/* I2C_TransmitOneByte *******************************************************
 *
 * Function:    BOOL I2C_TransmitOneByte( I2C_MODULE i2c_port, BYTE data )
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
  ***************************************************************************/
BOOL I2C_TransmitOneByte( I2C_MODULE i2c_port, BYTE data )
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
} /* End of I2C_TransmitOneByte */

/* I2C_StopTransfer ***********************************************************
 *
 * Function:    void void I2C_StopTransfer( I2C_MODULE i2c_port );
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

void I2C_StopTransfer( I2C_MODULE i2c_port )
{
I2C_STATUS  i2c_status;

/* Send the STOP sequence */
    I2CStop(i2c_port);

/* Wait for the STOP sequence to complete */
    do
    {
        i2c_status = I2CGetStatus(i2c_port);

    } while ( !(i2c_status & I2C_STOP) );
} /* End of I2C_StopTransfer */

/* I2C_ReceiveOneByte *******************************************************
 *
 * Function:    I2C_RESULT I2C_ReceiveOneByte( I2C_MODULE i2c_port,
 *                                              BYTE *data, BOOL ack );
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
  ***************************************************************************/
 I2C_RESULT I2C_ReceiveOneByte( I2C_MODULE i2c_port, BYTE *data, BOOL ack )
 {
 I2C_RESULT i2c_ops = I2C_SUCCESS;
    if(I2CReceiverEnable(i2c_port, TRUE) == I2C_RECEIVE_OVERFLOW)
    {
        printf("Error: I2C Receive Overflow\n");
        i2c_ops =  I2C_RECEIVE_OVERFLOW;
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
    return i2c_ops;
 } /* End of I2C_ReceiveOneByte */

/* End of i2clib.c */

