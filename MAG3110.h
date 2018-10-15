/******************************************************************************
MAG3110.h
SFE_MAG3110 Library - MAG3110 Register Map
George Beckstein contracting for SparkFun Electronics
Original Creation Date: 9/11/2016

This file declares the MAG3110 class and registers

Development environment specifics:
	IDE: Arduino 1.6.7
	Hardware Platform: Arduino Uno
	MAG3110 Breakout Version 1

This code is beerware; if you see me (or any other SparkFun employee) at the
local, and you've found our code helpful, please buy us a round!

Distributed as-is; no warranty is given.
******************************************************************************/

#ifndef SFE_MAG3110_h
#define SFE_MAG3110_h

#include "hardware.h"
#include "i2c_lib.h"

/////////////////////////////////////////
// MAG3110 I2C Address    			   //
/////////////////////////////////////////

#define MAG3110_I2C_ADDRESS 0x0E

/////////////////////////////////////////
// MAG3110 Magnetometer Registers      //
/////////////////////////////////////////
#define MAG3110_DR_STATUS			0x00
#define MAG3110_OUT_X_MSB			0x01
#define MAG3110_OUT_X_LSB			0x02
#define MAG3110_OUT_Y_MSB			0x03
#define MAG3110_OUT_Y_LSB			0x04
#define MAG3110_OUT_Z_MSB			0x05
#define MAG3110_OUT_Z_LSB			0x06
#define MAG3110_WHO_AM_I			0x07
#define MAG3110_SYSMOD				0x08
#define MAG3110_OFF_X_MSB			0x09
#define MAG3110_OFF_X_LSB			0x0A
#define MAG3110_OFF_Y_MSB			0x0B
#define MAG3110_OFF_Y_LSB			0x0C
#define MAG3110_OFF_Z_MSB			0x0D
#define MAG3110_OFF_Z_LSB			0x0E
#define MAG3110_DIE_TEMP			0x0F
#define MAG3110_CTRL_REG1			0x10
#define MAG3110_CTRL_REG2			0x11

////////////////////////////////
// MAG3110 WHO_AM_I Response  //
////////////////////////////////
#define MAG3110_WHO_AM_I_RSP		0xC4

/////////////////////////////////////////
// MAG3110 Commands and Settings       //
/////////////////////////////////////////

//CTRL_REG1 Settings
//Output Data Rate/Oversample Settings
//DR_OS_80_16 -> Output Data Rate = 80Hz, Oversampling Ratio = 16

#define MAG3110_DR_OS_80_16 		0x00
#define MAG3110_DR_OS_40_32 		0x08
#define MAG3110_DR_OS_20_64 		0x10
#define MAG3110_DR_OS_10_128		0x18
#define MAG3110_DR_OS_40_16			0x20
#define MAG3110_DR_OS_20_32			0x28
#define MAG3110_DR_OS_10_64			0x30
#define MAG3110_DR_OS_5_128			0x38
#define MAG3110_DR_OS_20_16			0x40
#define MAG3110_DR_OS_10_32			0x48
#define MAG3110_DR_OS_5_64			0x50
#define MAG3110_DR_OS_2_5_128		0x58
#define MAG3110_DR_OS_10_16			0x60
#define MAG3110_DR_OS_5_32			0x68
#define MAG3110_DR_OS_2_5_64		0x70
#define MAG3110_DR_OS_1_25_128		0x78
#define MAG3110_DR_OS_5_16			0x80
#define MAG3110_DR_OS_2_5_32		0x88
#define	MAG3110_DR_OS_1_25_64		0x90
#define MAG3110_DR_OS_0_63_128		0x98
#define MAG3110_DR_OS_2_5_16		0xA0
#define MAG3110_DR_OS_1_25_32		0xA8
#define MAG3110_DR_OS_0_63_64		0xB0
#define MAG3110_DR_OS_0_31_128		0xB8
#define MAG3110_DR_OS_1_25_16		0xC0
#define MAG3110_DR_OS_0_63_32		0xC8
#define MAG3110_DR_OS_0_31_64		0xD0
#define MAG3110_DR_OS_0_16_128		0xD8
#define MAG3110_DR_OS_0_63_16		0xE0
#define MAG3110_DR_OS_0_31_32		0xE8
#define MAG3110_DR_OS_0_16_64		0xF0
#define MAG3110_DR_OS_0_08_128		0xF8

//Other CTRL_REG1 Settings
#define MAG3110_FAST_READ 			0x04
#define MAG3110_TRIGGER_MEASUREMENT	0x02
#define MAG3110_ACTIVE_MODE			0x01
#define MAG3110_STANDBY_MODE		0x00

//CTRL_REG2 Settings
#define MAG3110_AUTO_MRST_EN		0x80
#define MAG3110_RAW_MODE			0x20
#define MAG3110_NORMAL_MODE			0x00
#define MAG3110_MAG_RST				0x10

//SYSMOD Readings
#define MAG3110_SYSMOD_STANDBY		0x00
#define MAG3110_SYSMOD_ACTIVE_RAW	0x01
#define	MAG3110_SYSMOD_ACTIVE		0x02

#define MAG3110_X_AXIS 1
#define MAG3110_Y_AXIS 3
#define MAG3110_Z_AXIS 5
  
  BOOL MAG3110_initialize(void);
  
  //Public methods
  BYTE MAG3110_readRegister(BYTE address);
  I2C_RESULT MAG3110_writeRegister(BYTE address, BYTE value);
  
  BOOL MAG3110_dataReady(void);
  
  I2C_RESULT  MAG3110_readMag(int* x, int* y, int* z);
  I2C_RESULT  MAG3110_readMicroTeslas(float* x, float* y, float* z);
  float MAG3110_readHeading(void);
  
  I2C_RESULT  MAG3110_setDR_OS(BYTE DROS);
  I2C_RESULT  MAG3110_triggerMeasurement();
  I2C_RESULT  MAG3110_rawData(BOOL raw);
  
  I2C_RESULT  MAG3110_setOffset(BYTE axis, int offset);
  int MAG3110_readOffset(BYTE axis);

  I2C_RESULT MAG3110_start(void);
  I2C_RESULT MAG3110_enterStandby(void);
  I2C_RESULT MAG3110_exitStandby(void);
  
  BOOL MAG3110_isActive(void);
  BOOL MAG3110_isRaw(void);
  BOOL MAG3110_isCalibrated(void);
  BOOL MAG3110_isCalibrating(void);
  BYTE MAG3110_getSysMode(void);
  
  I2C_RESULT MAG3110_enterCalMode(void);
  I2C_RESULT MAG3110_calibrate(void);
  I2C_RESULT MAG3110_exitCalMode(void);
  
  I2C_RESULT MAG3110_reset(void);
  
  BOOL error;
  
  float x_scale;
  float y_scale;
  
  BOOL calibrated;
 
#endif