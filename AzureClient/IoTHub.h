#ifndef IoTHub_h
#define IoTHub_h

#include "Arduino.h"
#include "sha256.h"
#include "Base64.h"
#include "globals.h"
#include <WiFiClientSecure.h>
#include <ESP8266WiFi.h>


class IoT
{
  public:
    IoT(CloudConfig* cloud);
    String send(String json);
    
  protected:
    CloudConfig* _cloud;//    SensorData* _data;
    String urlEncode(const char* msg);

  private:
    void initialiseHub();
    bool connectToAzure();
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
