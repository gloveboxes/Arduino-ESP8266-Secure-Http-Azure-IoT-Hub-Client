#ifndef Eventhub_h
#define Eventhub_h

#include "Arduino.h"
#include "sha256.h"
#include "Base64.h"
#include "IoTHub.h"
#include <WiFiClientSecure.h>


class Eventhub : public IoT
{
  public:
    String createSas(char* key, String url);
    void initialiseHub();
    

  private:
    // Azure Event Hub settings
    const char* EVENT_HUB_END_POINT = "/ehdevices/publishers/nodemcu/messages";

};

#endif
