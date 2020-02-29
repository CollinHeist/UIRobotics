#ifndef __SENSOR_DATA_H__
    #define __SENSOR_DATA_H__

    // Structure Definition
    typedef struct {
        int temperatureF, magX, magY, magZ, hour;
        unsigned int relativeHumidity, sec, min, day, month, year;
        float latitude, longitude;
    } SensorData;
#endif