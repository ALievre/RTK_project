#include "NMEA.h"

RTK_position::RTK_position(/* args */){

}

RTK_position::~RTK_position(){

}

void RTK_position::set_NMEA_data(String data)
{
    this->NMEA_data = data;
}

bool RTK_position::is_GNGGA()
{
    String buffer = this->NMEA_data;
    //Serial.println("");
    Serial.println(buffer);
    uint8_t buffer_size = this->NMEA_data.length();
    buffer.remove(6, buffer_size);
    if (buffer.compareTo("$GNGGA"))
        return false;
    else
        return true;
}

float RTK_position::get_Latitude()
{
    char *val = NULL;
    uint8_t buffer_size = this->NMEA_data.length();
    char buff_char[buffer_size];
    this->NMEA_data.toCharArray(buff_char, buffer_size);
    val = strtok(buff_char, ",");
    for (size_t i = 0; i < Latitude_p; i++)
    {
        val = strtok(NULL, ",");
    }
    float Latitude_conv = atof(val) / 100;
    uint8_t int_part = (int)Latitude_conv;
    Latitude_conv -= int_part;
    Latitude_conv *= 100;
    Latitude_conv /= 60;
    Latitude_conv += int_part;

    Serial.printf("Latitude : %f\n", Latitude_conv);

    this->Latitude = Latitude_conv;
    return Latitude_conv;
}

float RTK_position::get_Longitude()
{
    char *val = NULL;
    uint8_t buffer_size = this->NMEA_data.length();
    char buff_char[buffer_size];
    this->NMEA_data.toCharArray(buff_char, buffer_size);
    val = strtok(buff_char, ",");
    for (size_t i = 0; i < Longitude_p; i++)
    {
        val = strtok(NULL, ",");
    }
    float Longitude_conv = atof(val) / 100;
    uint8_t int_part = (int)Longitude_conv;
    Longitude_conv -= int_part;
    Longitude_conv *= 100;
    Longitude_conv /= 60;
    Longitude_conv += int_part;

    Serial.printf("Longitude : %f\n", Longitude_conv);

    this->Longitude = Longitude_conv;
    return Longitude_conv;
}

float RTK_position::get_Altitude()
{
    char *val = NULL;
    uint8_t buffer_size = this->NMEA_data.length();
    char buff_char[buffer_size];
    this->NMEA_data.toCharArray(buff_char, buffer_size);
    val = strtok(buff_char, ",");
    for (size_t i = 0; i < Altitude_p; i++)
    {
        val = strtok(NULL, ",");
    }
    float Altitude_conv = atof(val) / 100;
    uint8_t int_part = (int)Altitude_conv;
    Altitude_conv -= int_part;
    Altitude_conv *= 100;
    Altitude_conv /= 60;
    Altitude_conv += int_part;

    Serial.printf("Altitude : %f\n", Altitude_conv);

    this->Longitude = Altitude_conv;
    return Altitude_conv;
}

String RTK_position::get_UTC_Time()
{
    char *val = NULL;
    uint8_t buffer_size = this->NMEA_data.length();
    char buff_char[buffer_size];
    this->NMEA_data.toCharArray(buff_char, buffer_size);
    val = strtok(buff_char, ",");
    for (size_t i = 0; i < UTC_Time_p; i++)
    {
        val = strtok(NULL, ",");
    }
    String s_val = (String) val[0]+val[1] + ":" + val[2]+val[3] + ":" + val[4]+val[5]+val[6]+val[7]+val[8]+val[9];
    buffer_size = s_val.length();
    s_val.toCharArray(buff_char, buffer_size+1);
    this->UTC_Time = buff_char;
    //Serial.println(this->UTC_Time);
    //Serial.printf(" UTC_Time %d \n", this->UTC_Time);
    return this->UTC_Time;
}

void RTK_position::collect_data()
{
    if(this->is_GNGGA()){
        this->get_Latitude();
        this->get_Longitude();
        this->get_Altitude();
        this->get_UTC_Time();
    }
}
