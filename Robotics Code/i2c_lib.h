/* --------------------- Guard Against Multiple Inclusions ------------------- */
#ifndef __I2C_LIB_H__
	#define __I2C_LIB_H__

	/* ----------------- Necessary system include statements ----------------- */
	#include "hardware.h"
	#include "swDelay.h"

	#include <plib.h>
	#include <string.h>
	#include <stdio.h>
	#include <stdint.h>

	/* ----------------- Public Global Variables / Constants ----------------- */
	#define I2C_SPEED_STANDARD		100000
	#define I2C_SPEED_FAST			400000

	/* ---------------------- Public Function Declarations ------------------- */
	I2C_RESULT I2C_Init(I2C_MODULE i2c_port,  int speed);
	I2C_RESULT I2C_Read(I2C_MODULE i2c_port, BYTE DeviceAddress, BYTE *str, int *len);
	I2C_RESULT I2CReadRegs(I2C_MODULE i2c_port, BYTE DeviceAddress, int reg_addr, BYTE *i2cData, int len);
#endif
