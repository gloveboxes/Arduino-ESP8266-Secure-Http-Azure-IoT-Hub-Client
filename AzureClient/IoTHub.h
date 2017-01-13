#ifndef IoTHub_h
#define IoTHub_h

#include "Arduino.h"
#include "sha256.h"
#include "Base64.h"
#include <WiFiClientSecure.h>
#include <ESP8266WiFi.h>
#include <TimeLib.h>           // http://playground.arduino.cc/code/time - installed via library manager


class IoT
{
  public:
    String send(String json);
    void setConnectionString(String cs);

    
//    const char* host;
//    char *key;
//    const char *deviceId; 
//    const char* certificateFingerprint;
//    time_t  sasExpiryTime = 0;
//    int sasExpiryPeriodInSeconds = 60 * 15; // Default to 15 minutes
//    String sasUrl;


  time_t  sasExpiryTime = 0;
  int sasExpiryPeriodInSeconds = 60 * 15; // Default to 15 minutes
  const char* host;
  const char* certificateFingerprint;
  char* key;
  const char* deviceId; 
  char* fullSas = new char[0];
  String sasUrl;
  String endPoint;

    
    
  protected:
    String urlEncode(const char* msg);
    const char* GetStringValue(String value);

    

  private:
    void initialiseHub();
    bool connectToAzure();
    bool generateSas();
    String splitStringByIndex(String data, char separator, int index);
    char* format(const char *input, const char *value);
    char* format(const char *input, const char *value1, const char *value2);

    void initialiseAzure();
    String createSas(char* key, String url);
    bool verifyServerFingerprint();
    String buildHttpRequest(String data);
    
    String serializeData();

    bool azureInitialised = false;
    WiFiClientSecure tlsClient;

    const char* TARGET_URL = "/devices/";
    const char* IOT_HUB_END_POINT = "/messages/events?api-version=2015-08-15-preview";
};

#endif
