//Variable.h

#ifndef __VARIABLE_H__	//Guard against multiple inclusion
#define __VARIABLE_H__

struct VariablesList{
	int GPSMagTimer;					// Timer used to update the GPS & Magnometer
	char uart2_ch, uart4_ch;			// Recieved character
	BOOL uart2_flag, uart4_flag;		// Character ready flags
	int x, y, z;						// GPS coordinates
	float heading;						// GPS Heading
	int timer;							// Time marker to limit output readings
}/*Var = {0}*/;	// Can pre-initialize here - will need to do so for _all_ preceeding variables, though

#endif
