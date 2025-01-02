#ifndef _CONFIG_H_
#define _CONFIG_H_

#define TZ_OFFSET -5

#define GPS_BAUDRATE 115200
#define GPS_REFRESH_RATE 100000               // Refresh rate of data update, in microseconds, default 100ms
#define SCREEN_REFRESH_RATE 10000             // Refresh rate of data update, in microseconds, default 10ms
#define GPS_CLOCK_SYNC 3600                   // System clock GPS time sync intervale, in seconds, default 3600 seconds [1 hour]

#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 240
#define SCREEN_ROTATION 0
#define CHART_POINTS 10

// TFT Display Pins
#define TFT_MOSI 23
#define TFT_SCLK 18
#define TFT_CS   5
#define TFT_DC   19
#define TFT_RST  4

// GPS Module Pins
#define GPS_RX   16
#define GPS_TX   17
#define GPS_PPS  34

// Debug mode configuration
//#define DEBUG    // Comment this line out to disable debug features

// Serial configuration
#ifdef DEBUG
    #define SERIAL_BAUDRATE 115200
    
    // Debug macros
    #define DEBUG_PRINT(x) Serial.print(x)
    #define DEBUG_PRINTLN(x) Serial.println(x)
    #define DEBUG_PRINTF(x, ...) Serial.printf(x, __VA_ARGS__)
#else
    // When debug is disabled, these macros do nothing
    #define DEBUG_PRINT(x)
    #define DEBUG_PRINTLN(x)
    #define DEBUG_PRINTF(x, ...)
#endif

#endif