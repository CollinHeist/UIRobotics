#ifndef __MAIN_H__
    #define __MAIN_H__

	#define GPS_BUFFER_SIZE 256

	// These GPS signals are not used in this application
	#define FIX_3d			PORTCbits.RC3	   // 3D fix indication
	#define PPS1			PORTGbits.RG9	   // 1 PPS flag
	#define FIX_3D_INPUT	TRISCbits.TRISC3
	#define PPS1_INPUT		TRISGbits.TRISG9

	// GPS Sentence type
	#define GPRMC			1
	#define GPGGA			2
	#define GPGLL			3
#endif