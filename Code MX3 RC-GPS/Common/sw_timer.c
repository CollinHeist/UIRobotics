/* 
** sw_timer.c
** Professor Richard Wall
** Department of Electrical and Computer Engineering
** University of Idaho
** Moscow, ID 83844-1023
** rwall@uidaho.edu
**
** Date:		August 18, 2012
*/

#include <plib.h>
#include "sw_timer.h" 

/* START Function Description ***********************************************
SYNTAX:			void DelayMs(unsigned int mSdelay);
PARAMETER1:		mSdelay - the number of milliseconds to delay
KEYWORDS:		delay, ms, milliseconds, software delay
DESCRIPTION:	This is a millisecond delay function that will repeat a
DESCRIPTION:	specified number of times.  It also has nodebug active to
DESCRIPTION:	increase accuracy of measurement.
RETURN VALUE:	None.
END DESCRIPTION ************************************************************/
void DelayMs(unsigned int mSdelay)
{
//	DelayUs(mSdelay*1000);
unsigned int tWait, tStart,tNow;
	tStart = ReadCoreTimer();
	tWait  = tStart + (FOSC/2000UL)*mSdelay;  //Core Oscillator ticks per ms
	do
	{
		tNow = ReadCoreTimer();
	}while(tNow < tWait); // wait for the time to pass

}

/****************************************************************************
  Function:
    void Delay10us( UINT32 tenMicroSecondCounter )
  Description:
    This routine performs a software delay in intervals of 10 microseconds.
  Precondition:    	None
  Parameters:    	unsigned int tenMicroSecondCounter - number of ten microsecond delays
    				to perform at once.
  Returns:		    None
  Remarks:			None
  ***************************************************************************/

void Delay10us(unsigned int uSdelay)
{
//	DelayUs(10);
unsigned int tWait, tStart,tNow;
	tStart = ReadCoreTimer();
	tWait  = tStart + (FOSC/2000000L)*uSdelay;  //Core Oscillator ticks per 10us
	do
	{
		tNow = ReadCoreTimer();
	}while(tNow < tWait); // wait for the time to pass

}

/* START Function Description ***********************************************
SYNTAX:			void DelayUs(unsigned int uSdelay);
PARAMETER1:		uSdelay - the number of microseconds to delay
KEYWORDS:		delay, us, milliseconds, software delay
DESCRIPTION:	This is a millisecond delay function that will repeat a
				specified number of times.  
RETURN VALUE:	None.
END DESCRIPTION ************************************************************/
void DelayUs(unsigned int uSdelay)
{
unsigned int tWait, tStart,tNow;
	tStart = ReadCoreTimer();
	tWait  = tStart + (FOSC/2000000UL)*uSdelay;  //Core Oscillator ticks per us
	do
	{
		tNow = ReadCoreTimer();
	}while(tNow < tWait); // wait for the time to pass
}

