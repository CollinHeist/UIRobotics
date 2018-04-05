/* ------------------- Assignment of glboal variables ------------------------ */

#include <math.h>
#include <iostream>
#include "Variableslist.c"  //struct containing all universal, non-temp variables

#ifndef _MAIN_H    // Guard against multiple inclusion 
#define _MAIN_H

    #define GPS_BUFFER_SIZE 256

    // GPS Sentence type
    #define GPRMC           1
    #define GPGGA           2
    #define GPGLL           3

#endif /* _MAIN_H */

// Functions in gps.c

void UpdateAngleToGoal(VariablesList *VariablesList);
/* -------------------------------------------------------------------------
	@ Syntax
		void UpdateAngleToGoal(VariablesList *VariablesList);
	@ Description
		Sends a pointer leading to a struct, which houses all variables
	@ Parameter
		@ param1: ?
	@ Return Value
		Doesn't: updates a variable housed in the struct
	@ Remarks
		The custom type VarialbesList requires the list of variables c file
        to 
  -------------------------------------------------------------------------- */
