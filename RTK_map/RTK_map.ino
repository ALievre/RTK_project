#include "NMEA.h"
#define RXD2 25 // Pin RX du module LORA
#define TXD2 26 // Pin TX du module LORA


#if defined(ESP32)
#include <WiFi.h>
#include <FirebaseESP32.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#endif

#include "NMEA.h"

//Provide the token generation process info.
#include <addons/TokenHelper.h>

//Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>

/* 1. Define the WiFi credentials */
#define WIFI_SSID "iPhone de Assia"
#define WIFI_PASSWORD "3m6gsz9ycwp9i"

//For the following credentials, see examples/Authentications/SignInAsUser/EmailPassword/EmailPassword.ino

/* 2. Define the API Key */
#define API_KEY "AIzaSyAfZm7f4_eq7XbNIClE8KJR0LaNkCOb-vg"

/* 3. Define the RTDB URL */
#define DATABASE_URL "https://rtk-not-lost-default-rtdb.europe-west1.firebasedatabase.app/" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app
#define DATABASE_SECRET "H2iqBC3B1aPuHfdoQ2L3ZOuvUssXxLc6al7h3bqB"
/* 4. Define the user Email and password that alreadey registerd or added in your project */
#define USER_EMAIL "test@test.com"
#define USER_PASSWORD "123456789"


//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;
void clearBufferArray() ;

unsigned long sendDataPrevMillis = 0;

unsigned long count = 0;

void clearBufferArray() ;
unsigned char buffer[64];     
RTK_position RTK = RTK_position();


void setup()
{

  Serial.begin(115200); //serial monitor 
  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the user sign in credentials */
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h

  //Or use legacy authenticate method
  //config.database_url = DATABASE_URL;
  //config.signer.tokens.legacy_token = "<database secret>";

  //////////////////////////////////////////////////////////////////////////////////////////////
  //Please make sure the device free Heap is not lower than 80 k for ESP32 and 10 k for ESP8266,
  //otherwise the SSL connection will fail.
  //////////////////////////////////////////////////////////////////////////////////////////////

  Firebase.begin(&config, &auth);

  //Comment or pass false value when WiFi reconnection will control by your code or third party library
  Firebase.reconnectWiFi(true);

  Firebase.setDoubleDigits(5);

  /** Timeout options.

  //WiFi reconnect timeout (interval) in ms (10 sec - 5 min) when WiFi disconnected.
  config.timeout.wifiReconnect = 10 * 1000;

  //Socket connection and SSL handshake timeout in ms (1 sec - 1 min).
  config.timeout.socketConnection = 10 * 1000;

  //Server response read timeout in ms (1 sec - 1 min).
  config.timeout.serverResponse = 10 * 1000;

  //RTDB Stream keep-alive timeout in ms (20 sec - 2 min) when no server's keep-alive event data received.
  config.timeout.rtdbKeepAlive = 45 * 1000;

  //RTDB Stream reconnect timeout (interval) in ms (1 sec - 1 min) when RTDB Stream closed and want to resume.
  config.timeout.rtdbStreamReconnect = 1 * 1000;

  //RTDB Stream error notification timeout (interval) in ms (3 sec - 30 sec). It determines how often the readStream
  //will return false (error) when it called repeatedly in loop.
  config.timeout.rtdbStreamError = 3 * 1000;

  Note:
  The function that starting the new TCP session i.e. first time server connection or previous session was closed, the function won't exit until the 
  time of config.timeout.socketConnection.

  You can also set the TCP data sending retry with
  config.tcp_data_sending_retry = 1;

  */
}

void loop()
{
  
  if (Serial2.available())
    {

      String NMEA_data = Serial2.readStringUntil('\n'); 
      //Serial.println(NMEA_data); 
      //String NMEA_data = "$GNGGA,092535.00,4334.23269,N,00128.01143,E,2,12,1.93,237.8,M,48.5,M,1.0,0000*69"; //Trame de test
      RTK.set_NMEA_data(NMEA_data);
      //RTK.collect_data();
      
      float Latitude= RTK.get_Latitude();
      float Longitude = RTK.get_Longitude();

      String lat_str = RTK.conv_position(Latitude); 
      String long_str = RTK.conv_position(Longitude); 

      if (Firebase.ready() && (millis() - sendDataPrevMillis > 1000 || sendDataPrevMillis == 0)){ //sends of receives data every 2 seconds 
        sendDataPrevMillis = millis();


        Serial.printf("Set int... %s\n", Firebase.setInt(fbdo, F("/RTK_corrected_position/count"), count) ? "ok" : fbdo.errorReason().c_str()); // to have esay access to each value
        
       // Serial.printf("Set float... %s\n", Firebase.setFloat(fbdo, F("/RTK_corrected_position/latitude"+count), Latitude) ? "ok" : fbdo.errorReason().c_str()); //if we want to save every latitude position 
    
        //Serial.printf("Set float... %s\n", Firebase.setFloat(fbdo, F("/RTK_corrected_position/latitude"), Latitude) ? "ok" : fbdo.errorReason().c_str()); //if we want to simply update the position

        //Serial.printf("Set float... %s\n", Firebase.setFloat(fbdo, F("/RTK_corrected_position/longitude"), Longitude) ? "ok" : fbdo.errorReason().c_str());

        Serial.printf("Set float... %s\n", Firebase.setString(fbdo, F("/RTK_corrected_position/longitude"), long_str) ? "ok" : fbdo.errorReason().c_str());

        Serial.printf("Set float... %s\n", Firebase.setString(fbdo, F("/RTK_corrected_position/latitude"), lat_str) ? "ok" : fbdo.errorReason().c_str());

         


        
        
      
        //For the usage of FirebaseJson, see examples/FirebaseJson/BasicUsage/Create_Edit_Parse.ino
//        FirebaseJson json;
//    
//        if (count == 0)
//        {
//          json.set("value/round/" + String(count), F("cool!"));
//          json.set(F("vaue/ts/.sv"), F("timestamp"));
//          Serial.printf("Set json... %s\n", Firebase.set(fbdo, F("/test/json"), json) ? "ok" : fbdo.errorReason().c_str());
//        }
//        else
//        {
//          json.add(String(count), "smart!");
//          Serial.printf("Update node... %s\n", Firebase.updateNode(fbdo, F("/test/json/value/round"), json) ? "ok" : fbdo.errorReason().c_str());
//        }
//    
//        Serial.println();
    
        //For generic set/get functions.
    
        //For generic set, use Firebase.set(fbdo, <path>, <any variable or value>)
    
        //For generic get, use Firebase.get(fbdo, <path>).
        //And check its type with fbdo.dataType() or fbdo.dataTypeEnum() and
        //cast the value from it e.g. fbdo.to<int>(), fbdo.to<std::string>().
    
        //The function, fbdo.dataType() returns types String e.g. string, boolean,
        //int, float, double, json, array, blob, file and null.
    
        //The function, fbdo.dataTypeEnum() returns type enum (number) e.g. fb_esp_rtdb_data_type_null (1),
        //fb_esp_rtdb_data_type_integer, fb_esp_rtdb_data_type_float, fb_esp_rtdb_data_type_double,
        //fb_esp_rtdb_data_type_boolean, fb_esp_rtdb_data_type_string, fb_esp_rtdb_data_type_json,
        //fb_esp_rtdb_data_type_array, fb_esp_rtdb_data_type_blob, and fb_esp_rtdb_data_type_file (10)
        
        //String NMEA_data = "$GNGGA,092535.00,4434.2569,N,00151.01343,E,2,12,1.93,237.8,M,48.5,M,1.0,0000*69";
        count++;
      }
      
      
    }
  
}

void clearBufferArray()                     // function to clear buffer array
{
    for (int i=0; i<count;i++)
    {
        buffer[i]=NULL;
    }     

}
