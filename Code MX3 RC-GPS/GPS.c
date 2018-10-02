#include "GPS.h"
#include <math.h>
#include <stdio.h>
#include <stdint.h>

static int ExampleLocalFunction(int param1, int param2) {
    return 0;
}

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

double angleFromCoordinate(double lat1, double lon1, double lat2, double lon2)  {
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