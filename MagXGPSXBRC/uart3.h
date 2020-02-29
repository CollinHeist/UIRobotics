#ifndef __UART3_H__
    #define __UART3_H__

    #define SENSOR_STRING_WIDTH     (50)    // How long the buffer for the sensor string is
    #define INVALID_STRING          (2)     // Error code for an invalid string
    #define STILL_PARSING           (1)     // Number to denote still parsing
    #define DONE_PARSING            (0)     // Number to denote when parsing is done
    #define START_CHARACTER         ('$')   // What character signifies the start of the string

    // Function Prototypes
    unsigned int initializeUART3(unsigned int baud, int parity);
    int putCharacterUART3(int ch);
    int putStringUART3(const char *s);
    int getCharacterUART3(char *ch);
    int getStringUART3(char *s, unsigned int len);
#endif