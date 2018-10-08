/* ------------------- Hardware and Common Library Includes ------------------ */
#include "hardware.h"
#include <xc.h> 
#include <sys/attribs.h>
#include <plib.h>
#include "DHT.h"

/* -------------------------- Function Prototyping --------------------------- */
static BOOL DHT11_read_data(UINT32 *data, INT32 count);
static BOOL DHT11_start(void);

/* -------------------------- DHT11_read_temp_rh() ---------------------------
  @ Syntax
     BOOL valid &= DHT11_read_temp_rh(temp, humidity);
  @ Summary
     Changes the values of the temperature and humidity data based off the DHT11
  @ Description
     Initializes the DHT11 sensor to communicate temperature / humidity
     readings. DHT11_read_data() must be called sequentially
  @ Parameters
     param1 : Float pointer that's changed to reflect the latest temperature value
     param2 : Float pointer that's changed to reflect the latest humidity value
  @ Returns
     TRUE  : All data reads were performed correctly with no errors
     FALSE : At least one data read threw an error
  ---------------------------------------------------------------------------- */
BOOL DHT11_read_temp_rh(float *temp, float *rh) {
	BOOL okay = TRUE;
	INT32 data, data1, cksum;

	#ifdef TEST_DHT11
	    PORTClearBits(IOPORT_C, BIT_2);    	// Reset instrumentation
	#endif

    okay &= DHT11_start();             		// Start DT11 communications
    okay &= DHT11_read_data(&data, 32); 	// Read 32 data bits
    okay &= DHT11_read_data(&data1, 8); 	// Read 8 checksum bits

	#ifdef TEST_DHT11
	    PORTClearBits(IOPORT_C, BIT_2);    	// Reset instrumentation
	#endif

	// Assign the values of rh and temp accordingly
    *rh = ((float) (data >> 16)) / 256;
    *temp = ((float) (data & 0xffff)) / 256;
    *temp = (*temp * 9.0 / 5) + 32;

    cksum = ((BYTE) (data >> 24)) + ((BYTE) (data >> 16)) +
            ((BYTE) (data >> 8)) + ((BYTE) data);

    if (cksum != data1) {	// Test for valid reading
        setLED4(1);
        okay &= FALSE;
    }
    else
        setLED4(0);

    return okay;
}

/* ---------------------------- DHT11_read_data() ----------------------------
  @ Syntax
     BOOL valid &= DHT11_read_data(data, count);
  @ Summary
     Read 'count' many bits from the DHT11 sensor, send to 'data'
  @ Description
     Reads from the DHT11 humidity / temperature sensor 'data' many bits
  @ Parameters
     param1 : UINT32 pointer that's filled with the read data
     param2 : INT32 that is how many bits to read of the sensor
  @ Returns
     TRUE  : Commuication was successful
     FALSE : Commuication was unsuccessful
  ---------------------------------------------------------------------------- */
static BOOL DHT11_read_data(UINT32 *data, INT32 count) {
	UINT32  t0 = 0, t1;
	BOOL okay = TRUE;
	INT32 bit_cntr = 0;
	*data = 0;			// Clear the data

	// While we have bits to read, and we haven't errored
    while (bit_cntr < count && okay) { 
		#ifdef TEST_DHT11
	        setLED3(1);      // CLear bit test signal
		#endif

		// Start bit Sync phase
        t1 = ReadCoreTimer();	// Reset timer for bit sync phase
        t0 = 0;
        DelayUs(50);

        while ((PORTReadBits(IOPORT_C, BIT_2) == 0) && (t0 < 55*US2TICKS) && okay) {
            t0 = ReadCoreTimer() - t1;
        }

        if (PORTReadBits(IOPORT_C, BIT_2) == 0) { // Time out
            okay = FALSE;
        }

        t1 = ReadCoreTimer();   // Data cycle
        t0 = 0;
        while ((PORTReadBits(IOPORT_C, BIT_2) == BIT_2) && (t0 < 75*US2TICKS) && okay) {
            t0 = ReadCoreTimer() - t1;
        }
        if (t0 >= 75*US2TICKS) {	// Timing error
            okay = FALSE;
        }
        else {
			#ifdef TEST_DHT11
	            setLED3(1);		// Set bit test signal
			#endif
            *data = *data << 1;

            if (t0 > 50*US2TICKS) {
                *data |= 1;
            }
        }       
        bit_cntr++;
    }
	#ifdef TEST_DHT11
        setLED3(0);      		// Clear bit test signal
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
/* ------------------------------ DHT11_start() ------------------------------
  @ Syntax
     BOOL valid &= DHT11_start();
  @ Summary
     Initialize the DHT11 sensor for bit-stream communication
  @ Description
     IOPORT_B:BIt_2 (LEDA) is assigned as communication IO pin.
  @ Parameters
     None
  @ Returns
     TRUE  : Commuication was successful
     FALSE : Commuication was unsuccessful
  ---------------------------------------------------------------------------- */
static BOOL DHT11_start(void) {
	UINT32  t0, t1;
	BOOL okay = TRUE;

	// Send startup signal from processor
    PORTClearBits(IOPORT_C, BIT_2);				// Set IO pin low
    PORTSetPinsDigitalOut(IOPORT_C, BIT_2);		// Set pin for output
    DelayMs(20);								// Delay at least 18ms
    PORTSetPinsDigitalIn(IOPORT_C, BIT_2);		// Set pin for input

	// From here on the DHT11 has control of the voltage on the IO pin

	/* ---------- Start timing DHT11 response, read sensor (Phase 1) --------- */
    t0 = 0;					// Reset timer for 1st phase
    t1 = ReadCoreTimer();
    DelayUs(10);			// Delay minimum time

	// Keep reading core timer until IO pin goes low
    while ((PORTReadBits(IOPORT_C, BIT_2) == BIT_2) && (t0 < 45*US2TICKS) && okay) {
        t0 = ReadCoreTimer() - t1;              
    }
    if (PORTReadBits(IOPORT_C, BIT_2) == BIT_2)	// Timing error
        okay = FALSE;

	/* --------------------- Read sensor response Phase 2 -------------------- */
    t1 = ReadCoreTimer();	/* Reset timer for 2nd phase */
    t0 = 0;
    DelayUs(80);
	// Keep reading core timer until IO pin goes high
    while ((PORTReadBits(IOPORT_C, BIT_2) == 0) && (t0 < 90*US2TICKS) && okay) {
        t0 = ReadCoreTimer() - t1;
    }
    if (PORTReadBits(IOPORT_C, BIT_2) == 0)	// Timing error
        okay = FALSE;
    
	/* --------------------- Read sensor response Phase 3 -------------------- */
    t1 = ReadCoreTimer();		// Reset timer for 3rd phase
    t0 = 0;
    DelayUs(80);
	// Keep reading core timer until IO pin goes low
    while ((PORTReadBits(IOPORT_C, BIT_2) == BIT_2) && (t0 < 90*US2TICKS) && okay) {
        t0 = ReadCoreTimer() - t1;
    }

    if(PORTReadBits(IOPORT_C, BIT_2) == BIT_2) {	// Timing error
		okay = FALSE;
		setLED5(1);    // Set command complete test signal
    }
    else {
		#ifdef TEST_DHT11
	        if(okay)
	          setLED5(0);    /* Set command complete test signal */
		#endif
    }

    return okay;
}
