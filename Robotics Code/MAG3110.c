/******************************************************************************
MAG3110.c
SFE_MAG3110 Library
George Beckstein contracting for SparkFun Electronics
Original Creation Date: 9/11/2016

This file defines the MAG3110 class methods, etc

Development environment specifics:
	IDE: Arduino 1.6.7
	Hardware Platform: Arduino Uno
	MAG3110 Breakout Version 1

This code is beerware; if you see me (or any other SparkFun employee) at the
local, and you've found our code helpful, please buy us a round!

Distributed as-is; no warranty is given.
******************************************************************************/

#include "hardware.h"
#include "i2c_lib.h"
#include "MAG3110.h"
#include <plib.h>
#include <math.h>
#include <STDIO.h>

#define CALIBRATION_TIMEOUT 5000 //timeout in milliseconds
#define DEG_PER_RAD (180.0/3.14159265358979)

static int x_offset;
static int y_offset;

static int x_min;
static int x_max;

static int y_min;
static int y_max;

static int timeLastChange;
  
static BOOL calibrationMode;
static BOOL activeMode;
static BOOL rawMode;
  
static int MAG3110_readAxis(BYTE axis);

MAG3110_MAG3110(void) 
{
	
	//Just some random initial values
	x_offset = 0;
	y_offset = 0;

	x_scale = 0.0f;
	y_scale = 0.0f;
}

BOOL MAG3110_initialize(void) 
{
	//Just some random initial values
	x_offset = 0;
	y_offset = 0;

	x_scale = 0.0f;
	y_scale = 0.0f;

	if(MAG3110_readRegister(MAG3110_WHO_AM_I) != MAG3110_WHO_AM_I_RSP)
    { //Could not find MAG3110
		printf("Could not find MAG3110 connected!\n\r");
		return FALSE;
	}
	else //Successfully initialized
	{
		//Initial values
		MAG3110_reset();
		return TRUE;
	}
}

BYTE MAG3110_readRegister(BYTE address)
{
    I2C_RESULT i2c_result;
    BYTE reg[2] = {0};
    int len = 1;

    reg[0] = address;
    i2c_result = I2C_Write(I2C2, MAG3110_I2C_ADDRESS, reg, &len);
//    DelayMs(2);
    DelayUs(100);
    if(i2c_result == I2C_SUCCESS)
    {
        len = 1;
        i2c_result = I2C_Read(I2C2, MAG3110_I2C_ADDRESS, reg, &len);
        return reg[0];
    }
    else
        return 0;
}

I2C_RESULT MAG3110_writeRegister(BYTE address, BYTE value)
{
    I2C_RESULT i2c_result;
    BYTE reg[2] = {0,0};
    int len = 2;
    
    reg[0] = address;
    reg[1] = value;
    i2c_result = I2C_Write(I2C2, MAG3110_I2C_ADDRESS, reg, &len);
    return i2c_result;
}

BOOL MAG3110_dataReady(void) 
{
    return ((MAG3110_readRegister(MAG3110_DR_STATUS) & 0x8) >> 3);
}

I2C_RESULT MAG3110_readMag(int* x, int* y, int* z)
{
I2C_RESULT i2c_result;
BYTE reg[7] = {0};
int len = 1;
int idx;
unsigned int values[3];
    
    reg[0] = MAG3110_OUT_X_MSB;

	// Start readout at X MSB address
    i2c_result = I2C_Write(I2C2, MAG3110_I2C_ADDRESS, reg, &len);
    if(i2c_result == I2C_SUCCESS)
    {
        DelayMs(5);
	// Read out data using multiple byte read mode
        len = 6;
        i2c_result = I2C_Read(I2C2, MAG3110_I2C_ADDRESS, reg, &len);	

        if(i2c_result == I2C_SUCCESS)
        {
 	// Combine registers
            for(idx = 0; idx <= 2; idx++)
            {
                values[idx]  = reg[idx] << 8;       // MSB
        		values[idx] |= reg[idx + 1];        // LSB
            }

	// Put data into referenced variables
            *x = (int) values[0];
            *y = (int) values[1];
            *z = (int) values[2];
        }
    }
    return i2c_result;
}

I2C_RESULT MAG3110_readMicroTeslas(float* x, float* y, float* z)
{
	// Using internal read function
I2C_RESULT i2c_result;
int x_int, y_int, z_int;

	i2c_result = MAG3110_readMag(&x_int, &y_int, &z_int);

	//Read each axis and scale to Teslas
    if(i2c_result == I2C_SUCCESS)
    {
    	*x = (float) x_int * 0.1f;
        *y = (float) y_int * 0.1f;
    	*z = (float) z_int * 0.1f;	
    }
    return i2c_result;
}

//Note: Must be calibrated to use readHeading!!!
float MAG3110_readHeading(void)
{
I2C_RESULT i2c_result;	
int x, y, z;
float xf = 0;
float yf = 0;

	i2c_result = MAG3110_readMag(&x, &y, &z);
	
    if(i2c_result == I2C_SUCCESS)
    {
    	xf = (float) x * 1.0f;
    	yf = (float) y * 1.0f;
	}
	//Calculate the heading
	return (atan2(-y*y_scale, x*x_scale) * DEG_PER_RAD);
}

I2C_RESULT MAG3110_setDR_OS(BYTE DROS)
{
I2C_RESULT i2c_result = I2C_SUCCESS;	
BOOL wasActive = activeMode;
BYTE current;
	
	if(activeMode)
		i2c_result |= MAG3110_enterStandby(); //Must be in standby to modify CTRL_REG1
	
	//If we attempt to write to CTRL_REG1 right after going into standby
	//It might fail to modify the other bits
	DelayMs(100);
	
	 //Get the current control register
	current = MAG3110_readRegister(MAG3110_CTRL_REG1) & 0x07; //And chop off the 5 MSB
	i2c_result |= MAG3110_writeRegister(MAG3110_CTRL_REG1, (current | DROS)); //Write back the register with new DR_OS set
	
	DelayMs(100);
	
	//Start sampling again if we were before
	if(wasActive)
		i2c_result |= MAG3110_exitStandby();
    
    return i2c_result;
}

I2C_RESULT MAG3110_triggerMeasurement()
{
I2C_RESULT i2c_result = I2C_SUCCESS;	
BYTE current;
    current = MAG3110_readRegister(MAG3110_CTRL_REG1);
	i2c_result = MAG3110_writeRegister(MAG3110_CTRL_REG1, (current |  0x02));
    return i2c_result;
}

//Note that AUTO_MRST_EN will always read back as 0
//Therefore we must explicitly set this bit every time we modify CTRL_REG2
I2C_RESULT MAG3110_rawData(BOOL raw)
{
I2C_RESULT i2c_result = I2C_SUCCESS;	

	if(raw) //Turn on raw (non-user corrected) mode
	{
		rawMode = TRUE;
		i2c_result |= MAG3110_writeRegister(MAG3110_CTRL_REG2, MAG3110_AUTO_MRST_EN | (0x01 << 5) );
	}
	else //Turn off raw mode
	{
		rawMode = FALSE;
		i2c_result |= MAG3110_writeRegister(MAG3110_CTRL_REG2, MAG3110_AUTO_MRST_EN & ~(0x01 << 5));
	}
    return i2c_result;
}

//If you look at the datasheet, the offset registers are kind of strange
//The offset is stored in the most significant 15 bits.
//Bit 0 of the LSB register is always 0 for some reason...
//So we have to left shift the values by 1
//Ask me how confused I was...
I2C_RESULT MAG3110_setOffset(BYTE axis, int offset)
{	
I2C_RESULT i2c_result = I2C_SUCCESS;	
offset = offset << 1;
	
BYTE msbAddress = axis + 8;
BYTE lsbAddress = msbAddress + 1;

	i2c_result |= MAG3110_writeRegister(msbAddress, (BYTE)((offset >> 8) & 0xFF));

	DelayMs(15);
	
	i2c_result |= MAG3110_writeRegister(lsbAddress, (BYTE) offset & 0xFF);
    return i2c_result;
}

//See above
int MAG3110_readOffset(BYTE axis)
{
	return (MAG3110_readAxis(axis+8)) >> 1;
}

I2C_RESULT MAG3110_start() 
{
I2C_RESULT i2c_result = I2C_SUCCESS;	
	i2c_result = MAG3110_exitStandby();
}

I2C_RESULT MAG3110_enterStandby(void)
{
I2C_RESULT i2c_result = I2C_SUCCESS;	
BYTE current;
    activeMode = FALSE;
	current = MAG3110_readRegister(MAG3110_CTRL_REG1);
	//Clear bits 0 and 1 to enter low power standby mode
    return i2c_result;
}

I2C_RESULT  MAG3110_exitStandby()
{
I2C_RESULT i2c_result = I2C_SUCCESS;	
BYTE current;
	activeMode = TRUE;
  	current = MAG3110_readRegister(MAG3110_CTRL_REG1);
    DelayMs(1);
    i2c_result |= MAG3110_writeRegister(MAG3110_CTRL_REG1, (current | MAG3110_ACTIVE_MODE));
    DelayMs(1);
    current = MAG3110_readRegister(MAG3110_CTRL_REG1);        
    return i2c_result;	
}

BOOL MAG3110_isActive() 
{
	return activeMode;
}

BOOL MAG3110_isRaw() {
	return rawMode;
}

BOOL MAG3110_isCalibrated() 
{
	return calibrated;
}

BOOL MAG3110_isCalibrating() 
{
	return calibrationMode;
}

BYTE MAG3110_getSysMode() 
{
	return MAG3110_readRegister(MAG3110_SYSMOD);
}

I2C_RESULT MAG3110_enterCalMode()
{
I2C_RESULT i2c_result = I2C_SUCCESS;	

    calibrationMode = TRUE;
	//Starting values for calibration
	x_min = 32767;
	x_max = 0x8000;

	y_min = 32767;
	y_max = 0x8000;
	
	//Read raw readings for calibration
	MAG3110_rawData(TRUE);	
	
	calibrated = FALSE;
	
	//Set to active mode, highest DROS for continous readings
	i2c_result |= MAG3110_setDR_OS(MAG3110_DR_OS_80_16);
	if(!activeMode)
		i2c_result |= MAG3110_start();
    return i2c_result;
}

I2C_RESULT MAG3110_calibrate(void)
{
I2C_RESULT i2c_result = I2C_SUCCESS;	
int x, y, z;
BOOL changed;

	i2c_result = MAG3110_readMag(&x, &y, &z);
	if(i2c_result == I2C_SUCCESS)
    {
    	changed = FALSE; //Keep track of if a min/max is updated
        if(x < x_min)
        {
            x_min = x;
            changed = TRUE;
        }
        if(x > x_max) 
        {
            x_max = x;
            changed = TRUE;
        }
        if(y < y_min) 
        {
            y_min = y;
            changed = TRUE;
        }
        if(y > y_max) 
        {
            y_max = y;
            changed = TRUE;
        }
	
    	if(changed)
            timeLastChange = millis(); //Reset timeout counter
        
        if(millis() > 5000 && millis() - timeLastChange > CALIBRATION_TIMEOUT) //If the timeout has been reached, exit calibration
            MAG3110_exitCalMode();
    }
    return i2c_result;
}

I2C_RESULT MAG3110_exitCalMode()
{
I2C_RESULT i2c_result = I2C_SUCCESS;	
	//Calculate offsets
	x_offset = (x_min + x_max)/2;

	y_offset = (y_min + y_max)/2;

	x_scale = 1.0/(x_max - x_min);
	y_scale = 1.0/(y_max - y_min);
	
	MAG3110_setOffset(MAG3110_X_AXIS, x_offset);
	//Set the offsets
	MAG3110_setOffset(MAG3110_Y_AXIS, y_offset);
	
	//Use the offsets (set to normal mode)
	MAG3110_rawData(FALSE);
	
	calibrationMode = FALSE;
	calibrated = TRUE;
	
	//Enter standby and wait
	//enterStandby();
    return i2c_result;
}

I2C_RESULT  MAG3110_reset() 
{
I2C_RESULT i2c_result = I2C_SUCCESS;	

    i2c_result |= MAG3110_enterStandby();
	i2c_result |= MAG3110_writeRegister(MAG3110_CTRL_REG1, 0x00); //Set everything to 0
	i2c_result |= MAG3110_writeRegister(MAG3110_CTRL_REG2, 0x80); //Enable Auto Mag Reset, non-raw mode
	
	calibrationMode = FALSE;
	activeMode = FALSE;
	rawMode = FALSE;
	calibrated = FALSE;
	
	i2c_result |= MAG3110_setOffset(MAG3110_X_AXIS, 0);
	i2c_result |= MAG3110_setOffset(MAG3110_Y_AXIS, 0);
	i2c_result |= MAG3110_setOffset(MAG3110_Z_AXIS, 0);
    
    return i2c_result;
}

//This is private because you must read each axis for the data ready bit to be cleared
//It may be confusing for casual users
int MAG3110_readAxis(BYTE axis)
{
BYTE lsbAddress, msbAddress;
BYTE lsb, msb;
unsigned int out;

	msbAddress = axis;
	lsbAddress = axis+1;
	
	msb = MAG3110_readRegister(msbAddress);
	
	DelayUs(5); //needs at least 1.3us free time between start and stop
	
	lsb = MAG3110_readRegister(lsbAddress);
	
	out = (lsb | (msb << 8)); //concatenate the MSB and LSB;
	return (int)out;
}

/* End of file */