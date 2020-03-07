#ifndef __UART3_H__
    #define __UART3_H__

    // Operational Configurations
    #define SENSOR_STRING_WIDTH     (64)    // How long the buffer for the sensor string is

    // Return encodings
    #define INVALID_STRING          (2)     // Error code for an invalid string
    #define STILL_PARSING           (1)     // Number to denote still parsing
    #define DONE_PARSING            (0)     // Number to denote when parsing is done

    // Message configurations
    #define START_CHARACTER         ('$')   // What character signifies the start of the string

    // Function Prototypes
    unsigned int initializeUART3(const unsigned int baud, const int parity);
    unsigned int putCharacterUART3(const char ch);
    void putStringUART3(const char *s);
    unsigned int getCharacterUART3(char *ch);
    unsigned int getStringUART3(char *s, const unsigned int len);
#endif