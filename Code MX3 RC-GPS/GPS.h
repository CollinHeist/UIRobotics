#ifndef _GPS_H    /* Guard against multiple inclusion */
  #define _GPS_H
  #ifdef __cplusplus
  extern "C" {
  #endif

  #define PI2         3.1415926535
  #define deg2rad(x)  (x*PI2/180)
  #define rad2deg(x)  (x*180/PI2)

  double distance(double lat1, double lon1, double lat2, double lon2, char unit);
  double angleFromCoordinate(double lat1, double lon1, double lat2, double lon2);

  #ifdef __cplusplus
  }
#endif

#endif