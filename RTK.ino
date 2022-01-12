#include "NMEA.h"
#define RXD2 25 // Pin RX du module LORA
#define TXD2 26 // Pin TX du module LORA
void clearBufferArray() ;

unsigned char buffer[64];                   // buffer array for data receive over serial port
int count=0;  
RTK_position RTK = RTK_position();

void setup()
{
  Serial.begin(9600);
  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);
}

void loop()
{
 if (Serial2.available())
    {/*
      while(Serial2.available()){
        buffer[count++] = Serial2.read();
        if(count == 64){
          break;
        }
      }
      Serial.write(buffer, count);
      clearBufferArray();
      count = 0;
      */
       String NMEA_data = Serial2.readStringUntil('\n'); 
       Serial.println(NMEA_data);
       //String NMEA_data = "$GNGGA,092535.00,4334.23269,N,00128.01143,E,2,12,1.93,237.8,M,48.5,M,1.0,0000*69";  
       RTK.set_NMEA_data(NMEA_data);
       RTK.collect_data();
    }
/*
    if(Serial.available()){
      Serial2.write(Serial.read());
    }*/
}

void clearBufferArray()                     // function to clear buffer array
{
    for (int i=0; i<count;i++)
    {
        buffer[i]=NULL;
    }                      // clear all index of array with command NULL
}
