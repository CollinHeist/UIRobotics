/******************************************************************************
MAG3110.c
SFE_MAG3110 Library
George Beckstein contracting for SparkFun Electronics
Original Creation Date: 9/11/2016
Modified and tested for BASYS MX3 by R. W. Wall, April 11, 2019

Development environment specifics:
	IDE:               MPLAB X v5.05
	Hardware Platform: Digilent BASYS MX3
	MAG3110 Breakout Version 1

    Removed all programming specifics for C++ 

******************************************************************************/

#include "hardware.h"
#include "i2c_lib.h"
#include "MAG3110.h"
#include "led7.h"

#include <plib.h>
#include <math.h>
#include <float.h>
#include <STDIO.h>

#define CALIBRATION_TIMEOUT 10000 //timeout in milliseconds
#define DEG_PER_RAD (180.0/3.14159265358979)

extern int16_t led_value;
extern int angle;
extern int deg;

int16_t x, y, z;

static int16_t x_offset;
static int16_t y_offset;
static int16_t z_offset;
static float x_scale;
static float y_scale;
static float z_scale;
  
static BOOL calibrationMode;
static BOOL activeMode;
static BOOL rawMode;


static int16_t MAG3110_readAxis(BYTE axis);

/* ************************************************************************* */
BOOL MAG3110_initialize(void) 
{
int tempF = 0;
int tempC = 0;
	//Just some random initial values
	x_offset = X_OFFSET;
	y_offset = Y_OFFSET;
	z_offset = Z_OFFSET;

	x_scale = X_GAIN;
	y_scale = Y_GAIN;
    z_scale = Z_GAIN;

	if(MAG3110_readRegister(MAG3110_WHO_AM_I) != MAG3110_WHO_AM_I_RSP)
    { //Could not find MAG3110
		printf("Could not find MAG3110 connected!\r\n");
		return FALSE;
	}
	else //Successfully initialized
	{
		//Initial values
		MAG3110_reset();
        tempC = MAG3110_readRegister(MAG3110_DIE_TEMP);
        tempF = ((tempC * 9) / 5) + 32;
        printf("MAG3110 connected at %dF\r\n", tempF);
		return TRUE;
	}
}

/* ************************************************************************* */
BYTE MAG3110_readRegister(BYTE address)
{
    I2C_RESULT i2c_result;
    BYTE reg[2] = {0};
    int len = 1;

    reg[0] = address;
    i2c_result = I2C_Write(I2C1, MAG3110_I2C_ADDRESS, reg, &len);
    DelayUs(100);
    if(i2c_result == I2C_SUCCESS)
    {
        len = 1;
        i2c_result = I2C_Read(I2C1, MAG3110_I2C_ADDRESS, reg, &len);
        return reg[0];
    }
    else
        return 0;
}

/* ************************************************************************* */
I2C_RESULT MAG3110_writeRegister(BYTE address, BYTE value)
{
    I2C_RESULT i2c_result;
    BYTE reg[2] = {0,0};
    int len = 2;
    
    reg[0] = address;
    reg[1] = value;
    i2c_result = I2C_Write(I2C1, MAG3110_I2C_ADDRESS, reg, &len);
    return i2c_result;
}

/* ************************************************************************* */
BOOL MAG3110_dataReady(void) 
{
unsigned char dr_stat;
    dr_stat = MAG3110_readRegister(MAG3110_DR_STATUS) & 0xf0;
    if(dr_stat == 0xf0)
    {
        return TRUE;
    }
    else
    {
        return FALSE;        
    }
}

/* ************************************************************************* */
I2C_RESULT MAG3110_readMag(int16_t* x, int16_t* y, int16_t* z)
{
I2C_RESULT i2c_result;
BYTE reg[7] = {0};
int len = 1;
int idx;
uint16_t values[3];

    reg[0] = MAG3110_OUT_X_MSB;

	// Start readout at X MSB address
    i2c_result = I2C_Write(I2C1, MAG3110_I2C_ADDRESS, reg, &len);
    if(i2c_result == I2C_SUCCESS)
    {
        DelayMs(5);
	// Read out data using multiple byte read mode
        len = 6;
        i2c_result = I2C_Read(I2C1, MAG3110_I2C_ADDRESS, reg, &len);	

        if(i2c_result == I2C_SUCCESS)
        {
 	// Combine registers
            for(idx = 0; idx <= 2; idx++)
            {
                values[idx]  = reg[idx*2] << 8;       // MSB
        		values[idx] |= reg[idx*2 + 1];        // LSB
            }

	// Put data into referenced variables
            *x = (values[0]);
            *y = (values[1]);
            *z = (values[2]);
        }
    }
    if(i2c_result != I2C_SUCCESS)
    {
        printf("I2C Failure\n\r");
    }
    return i2c_result;
}

/* ************************************************************************* */
I2C_RESULT MAG3110_readMicroTeslas(float* xf, float* yf, float* zf)
{
	// Using internal read function
I2C_RESULT i2c_result;
int16_t x_int, y_int, z_int;

	i2c_result = MAG3110_readMag(&x, &y, &z);

	//Read each axis and scale to Teslas
    if(i2c_result == I2C_SUCCESS)
    {
    	*xf = (float) x * 0.1f;
        *yf = (float) y * 0.1f;
    	*zf = (float) z * 0.1f;	
    }
    return i2c_result;
}

/* ************************************************************************* */
//Note: Must be calibrated to use readHeading!!!
I2C_RESULT MAG3110_readHeading(float *heading)
{
I2C_RESULT i2c_result;	
int16_t x,y,z;
float xf = 0;
float yf = 0;

    i2c_result = MAG3110_rawData(TRUE);
	i2c_result |= MAG3110_readMag(&x, &y, &z);
    if(i2c_result == I2C_SUCCESS)
    {
    	xf = ((float) (x - x_offset))*x_scale;
    	yf = ((float) (y - y_offset))*y_scale;
        if((xf != 0.0) && (yf != 0.0))
            *heading = (atan2f(xf, yf) * RAD2DEG) + MAG_DECLINATION ;    
        else
            *heading = 0.0;
        printf("X:%6d / %1.6f   Y: %6d / %1.6f  -> %8.2f\n\r",x, xf, y, yf, *heading);                
	}
    else
    {
        *heading = 0;
         printf("Bad heading measurement\n\r");                   
    }
        
	return ( i2c_result );
}

/* ************************************************************************* */
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

/* ************************************************************************* */
I2C_RESULT MAG3110_triggerMeasurement()
{
I2C_RESULT i2c_result = I2C_SUCCESS;	
BYTE current;
    current = MAG3110_readRegister(MAG3110_CTRL_REG1);
	i2c_result = MAG3110_writeRegister(MAG3110_CTRL_REG1, (current |  0x02));
    return i2c_result;
}

/* ************************************************************************* */
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

/* ************************************************************************* */
//If you look at the data sheet, the offset registers are kind of strange
//The offset is stored in the most significant 15 bits.
//Bit 0 of the LSB register is always 0 for some reason...
//So we have to left shift the values by 1
//Ask me how confused I was...
I2C_RESULT MAG3110_setOffset(BYTE axis, int16_t offset)
{	
I2C_RESULT i2c_result = I2C_SUCCESS;	

    offset = (offset << 1) & 0xfffe;
	i2c_result |= MAG3110_writeRegister(axis, (BYTE)((offset >> 8) & 0xFF));
	DelayMs(5);
    i2c_result |= MAG3110_writeRegister((axis+1), (BYTE) (offset & 0xFE));
	DelayMs(5);
    return i2c_result;
}

/* ************************************************************************* */
//See above
int16_t MAG3110_readOffset(BYTE axis)
{
	return (MAG3110_readAxis(axis) >> 1);
}

/* ************************************************************************* */
I2C_RESULT MAG3110_start() 
{
I2C_RESULT i2c_result = I2C_SUCCESS;	
	i2c_result = MAG3110_exitStandby();
}

/* ************************************************************************* */
I2C_RESULT MAG3110_enterStandby(void)
{
I2C_RESULT i2c_result = I2C_SUCCESS;	
BYTE current;
    activeMode = FALSE;
	current = MAG3110_readRegister(MAG3110_CTRL_REG1);
	//Clear bits 0 and 1 to enter low power standby mode
    return i2c_result;
}

/* ************************************************************************* */
I2C_RESULT  MAG3110_exitStandby()
{
I2C_RESULT i2c_result = I2C_SUCCESS;	
BYTE current;
	activeMode = TRUE;
  	current = MAG3110_readRegister(MAG3110_CTRL_REG1);
    DelayMs(10);
    i2c_result |= MAG3110_writeRegister(MAG3110_CTRL_REG1, (current | MAG3110_ACTIVE_MODE));
    DelayMs(10);
    current = MAG3110_readRegister(MAG3110_CTRL_REG1);        
    return i2c_result;	
}

/* ************************************************************************* */
BOOL MAG3110_isActive() 
{
	return activeMode;
}

/* ************************************************************************* */
BOOL MAG3110_isRaw() {
	return rawMode;
}

/* ************************************************************************* */
BOOL MAG3110_isCalibrated() 
{
	return calibrated;
}

/* ************************************************************************* */
BOOL MAG3110_isCalibrating() 
{
	return calibrationMode;
}

/* ************************************************************************* */
BYTE MAG3110_getSysMode() 
{
	return MAG3110_readRegister(MAG3110_SYSMOD);
}

/* ************************************************************************* */
I2C_RESULT MAG3110_enterCalMode()
{
I2C_RESULT i2c_result = I2C_SUCCESS;	

    calibrationMode = TRUE;
	//Starting values for calibration

	//Read raw readings for calibration
	MAG3110_rawData(TRUE);	
	
	calibrated = FALSE;
	
	//Set to active mode, highest DROS for continous readings
	i2c_result |= MAG3110_setDR_OS(MAG3110_DR_OS_80_16);
	if(!activeMode)
		i2c_result |= MAG3110_start();
    return i2c_result;
}
/* ************************************************************************** */
I2C_RESULT MAG3110_calibrate(void)
{
I2C_RESULT i2c_result = I2C_SUCCESS;	
BOOL ready;
int16_t x,y,z;
    do
    {
        ready = MAG3110_dataReady();
    }while(!ready);
	i2c_result = MAG3110_readMag(&x, &y, &z);
    MAG3110_exitCalMode();
    return i2c_result;
}

/* ************************************************************************** */
I2C_RESULT MAG3110_exitCalMode()
{
I2C_RESULT i2c_result = I2C_SUCCESS;
    printf("Hard Xoff:%6d  Yoff:%6d  Zoff:%6d\n\r",
            x_offset,y_offset,z_offset);
	
    printf("Xgain: %1.6f  Ygain: %1.6f  Zgain: %1.6f\n\r",
            x_scale,y_scale,z_scale);
	//Use the offsets (set to normal mode)
	MAG3110_rawData(FALSE);
    DelayMs(10);
	
	calibrationMode = FALSE;
	calibrated = TRUE;
	
	//Enter standby and wait
	//enterStandby();
    return i2c_result;
}

/* ************************************************************************** */
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

/* ************************************************************************** */
//This is private because you must read each axis for the data ready bit to 
// be cleared. It may be confusing for casual users
static int16_t MAG3110_readAxis(BYTE axis)
{
BYTE lsbAddress, msbAddress;
BYTE lsb, msb;
int16_t reg;

	msbAddress = axis;
	lsbAddress = axis+1;
	
	msb = MAG3110_readRegister(msbAddress);
	
	DelayUs(5); //needs at least 1.3us free time between start and stop
	
	lsb = MAG3110_readRegister(lsbAddress);
	
	reg = (lsb | (msb << 8)); //concatenate the MSB and LSB;
    
	return reg;
}

/* End of file */