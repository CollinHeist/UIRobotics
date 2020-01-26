/* --------------------- Guard Against Multiple Inclusions ------------------- */
#ifndef __I2C_LIB_H__
	#define __I2C_LIB_H__

	#define CCLK		(80000000)  // (8 * 10) MHz Osc on Basys MX3 
	#define PBCLK	   (CCLK/8)	// Peripheral bus clock

	#include <plib.h>

	typedef struct {
		I2C_MODULE i2c_channel; // I2C channel ? I2C1 or I2C2
		BYTE dev_id;			// I2C device ID
		BYTE reg_addr;		  // Address of register to start
		BYTE block_size;		// Number of bytes to read or write
		BYTE *data;			 // Byte pointer to data array
	} I2C_DATA_BLOCK;

	/* ----------------- Public Global Variables / Constants ----------------- */
	#define I2C_SPEED_STANDARD		100000
	#define I2C_SPEED_FAST			400000

	// Function Prototypes
	I2C_RESULT I2C_Init(I2C_MODULE i2c_port,  int speed);
	I2C_RESULT I2C_Read(I2C_MODULE i2c_port, BYTE DeviceAddress, BYTE *str, int *len);
	I2C_RESULT I2CReadRegs(I2C_MODULE i2c_port, BYTE DeviceAddress, int reg_addr, BYTE *i2cData, int len);
	I2C_RESULT I2C_WriteDev( I2C_DATA_BLOCK blk);
	I2C_RESULT I2C_ReadDev( I2C_DATA_BLOCK  blk);
	void i2c_ackError(int loc);
	static BOOL StartTransfer(I2C_MODULE i2c_port, BOOL restart);
	static BYTE calc_ck_sum(char *str);
	static I2C_RESULT ReceiveOneByte(I2C_MODULE i2c_port, BYTE *data, BOOL ack);
	static BOOL TransmitOneByte(I2C_MODULE i2c_port, BYTE data);
	static void StopTransfer(I2C_MODULE i2c_port);
#endif