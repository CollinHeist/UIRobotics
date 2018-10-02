/* 
** I2Clib.h I2C Library
** Richard Wall
** August 29, 2014
**
*/

#ifndef __I2CLIB_H__
    #define __I2CLIB_H__

    #define CCLK        (80000000) /* 8Mhz Osc on Cerebot MX7cK */
    #define PBCLK       (CCLK/8)

#define MAX_PAGES       64

    typedef struct 
    {
        I2C_MODULE i2c_channel; // I2C channel ? I2C1 or I2C2
        BYTE dev_id;            // I2C device ID
        BYTE code;              // Command code for read or write
        BYTE reg_addr;          // Address of register to start
        BYTE block_size;        // Number of bytes to read or write
        BYTE *data;             // Byte pointer to data array
    } MODBUS_DATA_BLOCK;

#endif

/* Function Prototypes */
BOOL I2C_init( I2C_MODULE i2c_port, unsigned int bitrate);

I2C_RESULT I2C_ReadModbus(MODBUS_DATA_BLOCK blk);
I2C_RESULT I2C_WrtiteModbus(MODBUS_DATA_BLOCK blk);

int I2CReadByte(I2C_MODULE i2c_port, BYTE DeviceAddrress, BYTE *data);
int I2CWriteByte(I2C_MODULE i2c_port, BYTE DeviceAddrress, BYTE data);

I2C_RESULT I2C_ReceiveOneByte( I2C_MODULE i2c_port, BYTE *data, BOOL ack );
BOOL I2C_TransmitOneByte( I2C_MODULE i2c_port, BYTE data );

BOOL I2C_StartTransfer( I2C_MODULE i2c_port, BOOL restart );
void I2C_StopTransfer( I2C_MODULE i2c_port );

/* End of I2C_lib.h */
