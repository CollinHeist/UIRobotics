/* ************************************************************************** */
/** Descriptive File Name

  @Author
    Richard Wall

  @File Name
    GPS.c

  @Summary
    Convert set of GPS reading to distance in meters.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

/* This section lists the other files that are included in this file.
 */

#include "GPS.h"
#include <math.h>
#include <stdio.h>
#include <stdint.h>

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */

/*  A brief description of a section can be given directly below the section
    banner.
 */

/* ************************************************************************** */
/** Descriptive Data Item Name

  @Summary
    Brief one-line summary of the data item.
    
  @Description
    Full description, explaining the purpose and usage of data item.
    <p>
    Additional description in consecutive paragraphs separated by HTML 
    paragraph breaks, as necessary.
    <p>
    Type "JavaDoc" in the "How Do I?" IDE toolbar for more information on tags.
    
  @Remarks
    Any additional remarks
 */

/* ************************************************************************** */
/* ************************************************************************** */
// Section: Local Functions                                                   */
/* ************************************************************************** */
/* ************************************************************************** */

/*  A brief description of a section can be given directly below the section
    banner.
 */

/* ************************************************************************** */

/** 
  @Function
    int ExampleLocalFunctionName ( int param1, int param2 ) 

  @Summary
    Brief one-line description of the function.

  @Description
    Full description, explaining the purpose and usage of the function.
    <p>
    Additional description in consecutive paragraphs separated by HTML 
    paragraph breaks, as necessary.
    <p>
    Type "JavaDoc" in the "How Do I?" IDE toolbar for more information on tags.

  @Precondition
    List and describe any required preconditions. If there are no preconditions,
    enter "None."

  @Parameters
    @param param1 Describe the first parameter to the function.
    
    @param param2 Describe the second parameter to the function.

  @Returns
    List (if feasible) and describe the return values of the function.
    <ul>
      <li>1   Indicates an error occurred
      <li>0   Indicates an error did not occur
    </ul>

  @Remarks
    Describe any special behavior not described above.
    <p>
    Any additional remarks.

  @Example
    @code
    if(ExampleFunctionName(1, 2) == 0)
    {
        return 3;
    }
 */
static int ExampleLocalFunction(int param1, int param2) {
    return 0;
}


/* ************************************************************************** */
/* ************************************************************************** */
// Section: Interface Functions                                               */
/* ************************************************************************** */
/* ************************************************************************** */

/*  A brief description of a section can be given directly below the section
    banner.
 */

// *****************************************************************************

/** 
  @Function
    float distance(float flat, float flon, float x2lat, float x2lon) 

 * Param1 : flat = our current latitude. flat is from the gps data.
 * Param2 : flon1 = our current longitude. flon is from the gps data.
 * Param3 : latitude of destination waypoint 
 * Param4 : longitude of destination waypoint
 * 
  @Summary
    Brief one-line description of the function.

  @Remarks
   flat = our current latitude. flat is from the gps data.
 */
double distance(double lat1, double lon1, double lat2, double lon2, char unit) {
  double theta, dist;
  
  theta = lon1 - lon2;
  dist = sin(deg2rad(lat1)) * sin(deg2rad(lat2)) + cos(deg2rad(lat1)) * cos(deg2rad(lat2)) * cos(deg2rad(theta));
  dist = acos(dist);
  dist = rad2deg(dist);
  dist = dist * 60 * 1.1515;
  switch(unit) {
    case 'M':
      break;
    case 'K':
      dist = dist * 1609.344;
      break;
    case 'N':
      dist = dist * 0.8684;
      break;
  }
  return (dist);
}

double angleFromCoordinate(double lat1, double lon1, double lat2, double lon2) 
{
double dLon, y, x, fbrng;
int brng;
    dLon = (lon2 - lon1);

    y = sin(dLon) * cos(lat2);
    x = cos(lat1) * sin(lat2) - sin(lat1) * cos(lat2) * cos(dLon);

    fbrng = atan2(y, x);

    brng = (int) rad2deg(fbrng);
    brng = (brng + 360) % 360;
    brng = 360 - brng; // count degrees counter-clockwise - remove to make clockwise

    return brng;
}
 
/* *****************************************************************************
 End of File
 */
