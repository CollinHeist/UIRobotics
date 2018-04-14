/* ------------ Main project file, runs all the non-library code -------------
  @ Summary
     
  @ Description
     
  @ Notes
     Uses a peripheral bus clock set to 80 MHz
  ---------------------------------------------------------------------------- */

/* -------------------------- Project Include Files -------------------------- */
#include "config_bits.h"
#include "hardware.h"
#include "main.h"
#include <plib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <stdbool.h>

#include "swDelay.h"

#include "uart4.h"  // Monitor UART
#include "uart2.h"  // GPS UART
#include "led7.h"
#include "LCDlib.h"

/* --------------------- Global Variable Declarations ------------------------ */

int gps_message = 0;
extern int16_t led_value;
extern BOOL led_flag;
typedef struct { // A custom structure that holds all relevant position data
	float latitude;
	float longitude;
	float heading;
} Position;

/* ---------------------------- Function Declarations ------------------------ */

int changeHeading(Position currPos, Position desPos, float precision);
int set_gps(void);
int calc_ck_sum(char *str);
int decode_gps_msg(char *str, float *lat, float *lng, unsigned char *hour, unsigned char *min, 
                   unsigned char *sec, unsigned char *year, unsigned char *day, unsigned char *mon);

/* -------------------------------- main() -----------------------------------
  @ Syntax
     Run automatically - No need to call from anywhere
  @ Summary
     
  @ Returns
     The system has failed if the last statement is executed. 
     Any return indicates the system has failed
  ---------------------------------------------------------------------------- */
int main (void) {
    /* --------------------- Variable's used in main ------------------------- */
    char str_buf[GPS_BUFFER_SIZE];
    unsigned int str_idx = 0;                           // GPS sentence buffer
    char ch;                                            // Received character
    int rx_flag;                                        // Character ready
    unsigned char hour, minute, sec, year, day, mon;    // Time and date
    float latitude, longitude;				// Latitude and Longitude
	
    /* ----------------------- Initilizing fuctions -------------------------- */
    Hardware_Setup();               // Initialize common IO
    initLCD();                      // Local real time display
    uart4_init(38400, NO_PARITY);   // Uart Output to the PC
    uart2_init(9600, NO_PARITY);    // GPS 
    seg7_init();                    // Displays seconds only
    led_flag = 1;
	
    /* ------------------------- Main while loop ----------------------------- */
    while (1) {
	/* -------------- Code that deals with all UART output --------------- */
        do {
            rx_flag = getcU2(&ch);  // Poll for character received
        } while (!rx_flag);        
        putcU4(ch);                 // Send received character to monitor
        
        if ((ch == '$') || (str_idx >= GPS_BUFFER_SIZE-1)) { // Detect EOL
            str_buf[str_idx] = 0;   // Add string NULL character
            str_idx = 0;            // Reset index for new line
            invLED0();              // Toggle message marker
        }
	/* ------------------------------------------------------------------- */

        str_buf[str_idx++] = ch;     // Add new character to buffer
        if (ch == '\n') {            // Look for new line character
            if (!set_gps()) {        // Check for change of message
                // Requires 3.5ms to decode and display on LCD
                float lat, lng, currLat, currLng;
                decode_gps_msg(str_buf,  &lat, &lng, &hour, // Decode present message
                               &minute, &sec, &year, &day, &mon);
                
                // Testing gave (4644.006347, 11700.389648)
		// ddmm.mmmm and dddmm.mmmm for lat and long
                currLat = 4644.006347;
                currLng = 11700.389648;
                
	        // Truncates all but the decimal
                int latDegrees = currLat / 100.0; 
                int lngDegrees = currLng / 100.0;
                                
                // First part of the coordinate's minute portion
                int latMin1 = fmod(currLat, 100);
                int lngMin1 = fmod(currLng, 100);
		    
				printf("Lat: %f, Lng: %f\n\r", lat, lng); // UART Output
            }
        }
    }
    return EXIT_FAILURE; // Code execution should never get to this statement 
}

/* ---------------------------- powerMotors() --------------------------------
 @ Syntax
    powerMotors(float leftMotor, float rightMotor, float duration);
 @ Description
    This function powers the left motor to |a|% and the right motor to |b|%
    of their total power.
 @ Parameters
    @ param1 : a float from the range [-100, 100], where (-) values indicate
    	       backwards power (i.e. reverse). Power level of the left motor
    @ param2 : a float from the range [-100, 100], where (-) values indicate
    	       backwards power (i.e. reverse). Power level of the right motor
    @ param3 : a float that represents how long we want the motors to be
               powered for. Represented in seconds.
 @ Return Value
    None
  ---------------------------------------------------------------------------- */
void powerMotors(float leftMotor, float rightMotor, float duration) {
    /* Needs Implementation */
}

/* --------------------------- changeHeading() -------------------------------
 @ Syntax
     int a = changeHeading(Position currPos, Position desPos, float precision);
 @ Description
    This function turns the boat towards the desired latitude and longitude.
    This should be continually called until it returns a 1, indicating we're
    within the passed parameter of precision
 @ Parameters
    @ param1 : a Position structure that contains the current latitude,
			   longitude, and heading
    @ param2 : a Position structure that contains the desired latitude,
	 	       longitude, and heading
    @ param3 : a float representing how precise we want our heading to be.
	       A degree value (i.e. how far off our current heading and angle
               should be)
 @ Return Value
    1 : Successfully rotated the boat towards the desired location
    0 : The boat is still not at its desired heading, continue rotating 
  ---------------------------------------------------------------------------- */
int changeHeading(Position currPos, Position desPos, float precision) {
	float deltaLat  = desPos.latitude  - currPos.latitude;
	float deltaLong = desPos.longitude - currPos.longitude;
	
	float angleToRotate = -90; // Initialize to -90 to account for possible 0 denom.
	if (deltaLong != 0)
		angleToRotate = 180 / 3.141592654 * atan(deltaLat / deltaLong);
	
	/* Shift the rotation angle to the GPS scale */
	if (deltaLat > 0 && deltaLong > 0)
		angleToRotate = -(90 - angleToRotate);
	else if (deltaLat > 0 && deltaLong < 0)
		angleToRotate = -angleToRotate;
	else if (deltaLat < 0 && deltaLong > 0)
		angleToRotate = angleToRotate - 90;
	else
		angleToRotate = angleToRotate + 90;
	
	/* Account for other denominator of zero cases */
	angleToRotate = (deltaLong == 0 && deltaLat > 0) ? -90 : (deltaLong == 0 && deltaLat < 0) ? 90 : angleToRotate;
	
	// If we're within the margin of our desired precision, exit
	if (abs(currPos.heading - angleToRotate) < precision)
		return 1;
	else if ((currPos.heading - angleToRotate) < 0) // Turn left
		powerMotors(-100, 100, 3);
	else // Turn right
		powerMotors(100, -100, 3);
	
	return 0; // Return 0 so long as we're not at the proper heading
}

/* ------------------------------ set_gps() ----------------------------------
 @ Syntax
    set_gps(void);
 @ Description
    This function changes the NMEA sentence that the GPS reports based upon
    which on of the three buttons is pressed.
 @ Parameters
    None
 @ Return Value
    0 : No new message has been recieved
    1 : A new message has been received
  ---------------------------------------------------------------------------- */
int set_gps () {
    int new_msg = 0;    // Flag message has been set
    int cksum;
    char gps_msg1[GPS_BUFFER_SIZE];
    char gps_msg2[GPS_BUFFER_SIZE];
    if (BTNR()) { // Set RMC
        gps_message = GPRMC;
        strcpy(gps_msg1, "$PMTK314,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*");
        cksum = calc_ck_sum(gps_msg1);
        sprintf(gps_msg2,"%s%X\r\n", gps_msg1, cksum);
        // printf("\n\r%s", gps_msg2);
        putsU2(gps_msg2);
	// Debounce push button switch
        do { msDelay(10); } while (BTNR());
        new_msg = 1;
    }
    if (BTNC()) { // Set GGA
        gps_message = GPGGA;
        strcpy(gps_msg1, "$PMTK314,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*");
        cksum = calc_ck_sum(gps_msg1);
        sprintf(gps_msg2,"%s%X\r\n", gps_msg1, cksum);
        // printf("\n\r%s", gps_msg2);
        putsU2(gps_msg2);
	// Debounce push button switch
        do { msDelay(10); } while (BTNC());
        new_msg = 1;
    }
    if (BTND()) { // Set GLL
        gps_message = GPGLL;
        strcpy(gps_msg1, "$PMTK314,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*");
        cksum = calc_ck_sum(gps_msg1);
        sprintf(gps_msg2,"%s%X\r\n", gps_msg1, cksum);
        // printf("\n\r%s", gps_msg2);
        putsU2(gps_msg2);
	// Debounce push button switch
        do { msDelay(10); } while (BTND());
        new_msg = 1;
    }
    return new_msg;
} // End of set_gps 

/* ---------------------------- calc_ck_sum ----------------------------------
  @ Syntax
     int a = calc_ck_sum(char *str);
  @ Description
     This function computes the checksum for an NMEA sentence 
     between the "$" and "*" characters. Called only from set_gps()
  @ Parameter
    @ param1: string containing the GPS sentence
  @ Return Value
     checksum of BYTE type
  ---------------------------------------------------------------------------- */
int calc_ck_sum(char *str) {
    int cksum = 0;
    int start_flag = 0;
    int stop_flag = 0;

    do {  // Search for "$" character 
        if (start_flag == 0) {
            if (*str == 0x24) {  // Search for '$' character
                start_flag = 1;
            }
        }
        else { // Search for "*" character signifying the end of sentence 
            if (*str == '*') {
                stop_flag = 1;   // Mark end of GPS sentence 
            }
            else {
                cksum ^= (unsigned char) (*str);   //  Update check sum 
            }
        }
        str++;
    } while ((*str != 0x00) && (stop_flag == 0)); // End of string or "*" 
    return cksum;
} // End of calc_ck_sum

/* --------------------------- decode_gps_msg() ------------------------------
  @ Syntax
     int a = decode_gps_msg(char *str, BYTE *hour, BYTE *min, BYTE *sec,
                            BYTE *year, BYTE *day, BYTE *mon);
  @ Description
     This function parses one of three different GPS sentences
  @ Parameter
    @ param1 : string containing the GPS sentence
    @ param2 : int reference to hours
    @ param3 : int reference to minutes
    @ param4 : int reference to seconds
    @ param5 : int reference to year (units and tens digits only)
    @ param6 : int reference to day
    @ param7 : int reference to month
  @ Return Value
     Number of variables successfully decoded
     The real return is the changed values of the parameters (passed by ref.)
  ---------------------------------------------------------------------------- */
int decode_gps_msg(char *str, float * retLat, float * retLng, unsigned char *hour, 
                   unsigned char *min, unsigned char *sec, unsigned char *year, 
                   unsigned char *day, unsigned char *mon) {
    int vars = 0;
    char msg[8] = {' '};
    unsigned int time = 0, date = 0;
    char status=' ', NS=' ', EW=' ', mdir=' ', mode=' ', cksum=0, alt_units=' ', geo_units=' ';
    float utc=0, lat=0, lng=0, speed=0, dir=0, mag_var=0, hdop=0, alt=0, geo_sep=0;
    int pos_fix=0, sats=0;

    unsigned char dow;
    char lcd_str[17];
    char roll_back = 0;

    #ifdef USE_RTCC         // Not available on Basys X3 - requires 32KHz crystal
	rtccTime     tm;    // RTCC time structure
	rtccDate     dt;    // RTCC date structure
    #endif

    if (gps_message == 0) { // Power-up reset - determine GPS message mode 
        if (*str != '$') {
            return 0;       // Incorrect message identifier
        }
        else {
            memcpy(msg, str, 6);    // Determine type of GPS sentence
            if (memcmp(msg, "$GPRMC", 6) == 0) {
                gps_message = GPRMC;
            }
            if (memcmp(msg, "$GPGGA", 6) == 0) {
                gps_message = GPGGA;
            }
            if (memcmp(msg, "$GPGLL", 6) == 0) {
                gps_message = GPGLL;
            }
        }
    }
    
    // Decode the appropriate sentence
    if (gps_message == GPRMC) {
        vars = sscanf(str,"%s,%f,%c,%f,%c,%f,%c,%f,%f,%d,%f,%c,%c*%x",msg,
                    &utc, &status, &lat, &NS, &lng, &EW, &speed, &dir,
                    &date, &mag_var, &mdir, &mode, &cksum  );
	// LATCbits.LATC2 = str[12] & 1;
    }

    if (gps_message == GPGLL) {
        vars = sscanf(str,"%s,%f,%c,%f,%c,%f,%c*%x", msg,
               &lat, &NS, &lng, &EW, &utc, &mode, &cksum  );
    }

    if ((gps_message == GPGGA)) {
        vars = sscanf(str,"%s,%f,%f,%c,%f,%c,%d,%d,%f,%f,%c,%f,%c,,*%x", msg,
                &utc, &lat, &NS, &lng, &EW, &pos_fix, &sats,
                &hdop, &alt, &alt_units, &geo_sep, &geo_units ,&cksum);
    }
    
    // Make a struct of lat and longitude 
    
    
    // Assign data if one of three specific sentences
    if ((memcmp(msg, "$GPRMC", 6) == 0) ||
        (memcmp(msg, "$GPGGA", 6) == 0) ||
        (memcmp(msg, "$GPGLL", 6) == 0)) {
        time  = (INT32) utc; // Type cast float to integer
	// Convert from big integer to composite hour, minute, and seconds 
        *hour = (BYTE) (time / 10000);
        time  = time % 10000;
        *min  = (BYTE) (time / 100 );
        time  = time % 100;
        *sec  = (BYTE) (time);
        led_value = *sec;
              
        // Compensate for 9 hour difference for PST
        if (*hour < 9) {
            *hour += 24;
            roll_back = -1;
        }
        *hour -= 8;
    }
    // Decode date if GPRMC sentence
    if ((memcmp(msg, "$GPRMC", 6) == 0) && (vars >= 10)) {
        // Convert from large integer to days, months, and year
        *day = (BYTE) (date / 10000);
        if (*hour >= 9) 
	    --*day;
        date  = date % 10000;
        *mon  = (BYTE) (date / 100);
        date  = date % 100;
        *year = (BYTE) date;
        
        // Compensate for 9 hour difference for PST
        if (roll_back == -1) { // Between midnight and 9:00 for PST 
            if (*day == 1) {
                switch(*mon) {
                    case 1: // January
                        *mon = 12;
                        *day = 31;
                        *year--;
                        break;
                    case 2: // February
                    case 4: // April
                    case 6: // June 
                    case 8: // August
                    case 9: // September/
                    case 11: // November
                        *day = 31;
                        *mon--;
                        break;
                    case 5: // May
                    case 7: // July
                    case 10: // October
                    case 12: // December
                        *day = 30;
                        *mon--;
                        break;
                    case 3: // March
                        *day = 28;
                        *mon--;
                        break;
                    default:    // Unknown month value
                        *day = 0;
                        *mon = 0;
                        *year = 0;
                }
            }
        }
    }
    // Display time and date, if available, on the LCD 
    clrLCD();
    if ((strcmp(msg, "$GPRMC") == 0) ||
        (strcmp(msg, "$GPGGA") == 0) ||
        (strcmp(msg, "$GPGLL") == 0)) {
        
        // Display sentence type and time on LCD
        sprintf(lcd_str,"%s %2d:%2d:%2d", msg, *hour, *min, *sec);
        putsLCD(lcd_str);

        // Display if date is available for GPRMC message
        if (((status == 'A') && (vars >= 10)) && (gps_message == GPRMC)) {
            gotoLCD(16); // Go to second line of LCD
            sprintf(lcd_str,"%c%3d %2d/%2d/%2d",status, vars, *mon, *day,*year);
            putsLCD(lcd_str);
        }
    }
    else { // No valid GPS sentence received
        sprintf(lcd_str,"%s %d ", msg, vars);
        putsLCD(lcd_str);
    }
    
    * retLat = lat;
    * retLng = lng; 
    
    
#ifdef USE_RTCC
    new_day = (*hour + *min + *sec);    // Check for midnight
    // Update PIC32 RTCC at midnight or reboot
    if ((new_day == 0 ) || (rttc_initialized ==  FALSE )) {
        if (status == 'A' && (vars >= 10)) {
            dow = RtccWeekDay(*year, *mon, *day); // XC32 PLIB function 
            dt.year = bin2bcd( *year);
            dt.mon = bin2bcd( *mon);
            dt.mday = bin2bcd( *day);
            dt.wday = dow;
            RtccSetDate(dt.l);
        }
        tm.hour = bin2bcd(*hour);
        tm.min = bin2bcd(*min);
        tm.sec = bin2bcd(*sec);
        RtccSetTime(tm.l);

        rttc_initialized =  TRUE;
    }
#endif
    return vars;    // Number of variables decoded 
} // End of decode_gps_msg 
