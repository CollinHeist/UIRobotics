/*
 * DHT Humidity and Temperature Sensor Library
 * Author:  Richard Wall
 * Date:    August 14, 2015
 *
 * Description:  This library reads the temperature and humidity from the DHT11
 * sensor.  The sensor readings are returnd a floating point values.
 *
 */


#include "hardware.h"
#include <xc.h> 
#include <sys/attribs.h>
#include <plib.h>

#include "DHT.h"

static BOOL DHT11_read_data(UINT32 *data, INT32 count);
static BOOL DHT11_start(void);

/* DTH11_read_temp_rh Function Description ***********************************
 * SYNTAX:          BOOL DT11_read(float *temp, float *rh);
 * KEYWORDS:        DT11, read, temperature, humidity,
 * DESCRIPTION:     Initializes the DT11 sensor to communicate a temperature and
 *                  humidity reading. The two calls to "DT11_read_data" must
 *                  occur in immediate succession to perserve sensor output
 *                  timing.
 * PARAMETER:       None
 * RETURN VALUE:    None
 *
 * NOTES:           This function should not be preempted by an interrupt to
 *                  interfere with the timing operations. The maximum access rate is
 *                  1.7 seconds.  This function requires less than 4ms to
 *                  complete.
 * END DESCRIPTION **********************************************************/
BOOL DHT11_read_temp_rh(float *temp, float *rh)
{
BOOL okay = TRUE;
INT32 data, data1, cksum;

#ifdef TEST_DHT11
    PORTClearBits(IOPORT_C, BIT_2);    /* Reset instrumentation */
#endif
    okay &= DHT11_start();                /* Start DT11 communications */
    okay &= DHT11_read_data(&data, 32);   /* Read 32 data bits */
    okay &= DHT11_read_data(&data1, 8);   /* Read 8 checksum bits */
#ifdef TEST_DHT11
    PORTClearBits(IOPORT_C, BIT_2);    /* Reset instrumentation */
#endif

    *rh = ((float) (data >> 16)) / 256;
    *temp = ((float) (data & 0xffff)) / 256;
    *temp = (*temp * 9.0 / 5) + 32;

    cksum = ((BYTE) (data >> 24)) + ((BYTE) (data >> 16)) +
            ((BYTE) (data >> 8)) + ((BYTE) data);

    if(cksum != data1)                  /* Test for valid reading */
    {
        setLED4(1);
        okay &= FALSE;
    }
    else
        setLED4(0);

    return okay;
}

/* DHT11_read_data Function Description ***************************************
 * SYNTAX:          static BOOL DHT11_read_data(UINT32 *data, INT32 count);
 * KEYWORDS:        DHT11, read, data
 * DESCRIPTION:     Initialzes the DHT11 sensor to communicate the sensor data.
 *                  IOPORT_B:BIT_2 (LEDA) is assigned as the communications IO
 *                  pin. This pin is assigned to be low output or a data input.
 * PARAMETER 1:     pointer to data results - maximum 32 bits
 * PARAMETER 2:     Number of data bits to read
 * RETURN VALUE:    Transaction success status: True = communications successful
 *
 * NOTES:           This function should not be preempted by an interrupt to
 *                  interfere the timing operations. LEDB through LEDD of IOPORT_B
 *                  is assigned to timing instrumentation. The "DT11_start"
 *                  function must be called immediately prior to calling this
 *                  function.
 * END DESCRIPTION **********************************************************/
static BOOL DHT11_read_data(UINT32 *data, INT32 count)
{
UINT32  t0 = 0, t1;
BOOL okay = TRUE;
INT32 bit_cntr = 0;
    *data = 0;                                  /* clear data */

    while(bit_cntr < count && okay )
    {
#ifdef TEST_DHT11
        setLED3(1);      /* CLear bit test signal */
#endif
/* Start bit Sync phase */
        t1 = ReadCoreTimer();           /* Reset timer for bit sync phase */
        t0 = 0;
        DelayUs(50);
        while((PORTReadBits(IOPORT_C, BIT_2) == 0) && (t0 < 55*US2TICKS) && okay)
        {
            t0 = ReadCoreTimer() - t1;
        }
        if(PORTReadBits(IOPORT_C, BIT_2) == 0) /* Time out */
        {
            okay = FALSE;
        }

        t1 = ReadCoreTimer();   /* Data cycle */
        t0 = 0;
        while((PORTReadBits(IOPORT_C, BIT_2) == BIT_2) && (t0 < 75*US2TICKS) && okay)
        {
            t0 = ReadCoreTimer() - t1;
        }
        if(t0 >= 75*US2TICKS)                       /* Timing error */
        {
            okay = FALSE;
        }
        else
        {
#ifdef TEST_DHT11
             setLED3(1);            /* Set bit test signal */
#endif
            *data = *data << 1;

            if(t0 > 50*US2TICKS)
            {
                *data |= 1;
            }
        }       
        bit_cntr++;
    }
#ifdef TEST_DHT11
        setLED3(0);      /* CLear bit test signal */
#endif
    
    return okay;
}

/* DHT11_start Function Description ***************************************
 * SYNTAX:          static BOOL DHT11_start(void);
 * KEYWORDS:        DT11, start reading,
 * DESCRIPTION:     Initializes the DHT11 sensor for bit stream communications.
 *                  IOPORT_B:BIT_2 (LEDA) is assigned as the communications IO
 *                  pin. This pin is assigned to be low output or a data input.
 * PARAMETER:       None
 * RETURN VALUE:    Transaction success status: True = communications successful
 *
 * NOTES:           This function should not be preempted by an interrupt to
 *                  interfere with the timing operations. LEDB through LEDD of 
 *                  IOPORT_B is assigned to timing instrumentation
 * END DESCRIPTION **********************************************************/
static BOOL DHT11_start(void)
{
UINT32  t0, t1;
BOOL okay = TRUE;

/* Send startup signal from processor */
    PORTClearBits(IOPORT_C, BIT_2);              /* Set IO pin low */
    PORTSetPinsDigitalOut(IOPORT_C, BIT_2);      /* Set pin for output */
    DelayMs(20);                                /* Delay at least 18ms */
    PORTSetPinsDigitalIn(IOPORT_C, BIT_2);       /* Set pin for input */

/* From here on the DHT11 has control of the voltage on the IO pin */

/* Start timing DHT11 response - read sensor response Phase 1 */
    t0 = 0;                                     /* Reset timer for 1st pahse */
    t1 = ReadCoreTimer();
    DelayUs(10);                                /* Delay minimum time */
/* Keep reading core timer until IO pin goes low */
    while((PORTReadBits(IOPORT_C, BIT_2) == BIT_2) && (t0 < 45*US2TICKS) && okay)
    {
        t0 = ReadCoreTimer() - t1;              
    }
    if(PORTReadBits(IOPORT_C, BIT_2) == BIT_2)     /* Timing error */
        okay = FALSE;

/* Read sensor response Phase 2 */
    t1 = ReadCoreTimer();                       /* Reset timer for 2nd phase */
    t0 = 0;
    DelayUs(80);
/* Keep reading core timer until IO pin goes high */
    while((PORTReadBits(IOPORT_C, BIT_2) == 0) && (t0 < 90*US2TICKS) && okay)
    {
        t0 = ReadCoreTimer() - t1;
    }
    if(PORTReadBits(IOPORT_C, BIT_2) == 0)        /* Timing error */
        okay = FALSE;
    
/*  Read sensor response Phase 3 */
    t1 = ReadCoreTimer();                       /* Reset timer for 3rd phase */
    t0 = 0;
    DelayUs(80);
/* Keep reading core timer until IO pin goes low */
    while((PORTReadBits(IOPORT_C, BIT_2) == BIT_2) && (t0 < 90*US2TICKS) && okay)
    {
        t0 = ReadCoreTimer() - t1;
    }

    if(PORTReadBits(IOPORT_C, BIT_2) == BIT_2)     /* Timing error */
    {
        okay = FALSE;
          setLED5(1);    /* Set command complete test signal */
    }
    else
    {
#ifdef TEST_DHT11
        if(okay)
          setLED5(0);    /* Set command complete test signal */
#endif
    }

    return okay;
}
