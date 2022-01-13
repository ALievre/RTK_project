#ifndef NMEA_H
#define NMEA_H

#include "stdint.h"
#include <string.h>
#include <Arduino.h>

enum GNGGA {
    Msg_id_p = 0,
    UTC_Time_p = 1,
    Latitude_p = 2,
    NS_indicator_p = 3,
    Longitude_p = 4,
    EW_indicator_p = 5,
    Fix_p = 6,
    DOP_p = 7,
    Altitude_p = 8,
    Correction_p = 10,
    Checksum_p = 13,
};

class RTK_position {
private:
    /* data */
    String NMEA_data = "$GNGGA,092535.00,4334.23269,N,00128.01143,E,2,12,1.93,237.8,M,48.5,M,1.0,0000*69";
    char* Msg_id;
    char* UTC_Time;
    float Latitude;
    char* NS_indicator;
    float Longitude;
    char* EW_indicator;
    int Fix;
    float DOP;
    float Altitude;
    float Correction;
    char* Checksum;

public:
    RTK_position(/* args */);
    ~RTK_position();
    void set_NMEA_data(String NMEA_data);
    bool is_GNGGA();
    float get_Latitude();
    float get_Longitude();
    float get_Altitude();
    float get_Correction();
    String get_UTC_Time();
    void collect_data();
};

#endif
