#ifndef Device_h
#define Device_h

#include "arduino.h"
//#include <EEPROM.h>
//#include <ArduinoJson.h>    // https://github.com/bblanchon/ArduinoJson - installed via library manager

enum BoardType {
  NodeMCU,
  WeMos,
  SparkfunThing,
  Other
};

enum SensorMode {
  None,
  Bmp180Mode,
  DhtShieldMode
};


class Device {
  public:
    void initCloudConfig(const char *_ssid, const char *_pwd);
    void initCloudConfig();
  
    int WifiIndex = 0;
    int wifiPairs = 0;
    const char **ssid;
    const char **pwd;
    BoardType boardType = Other;            // OperationMode enumeration: NodeMCU, WeMos, SparkfunThing, Other 
    unsigned int deepSleepSeconds = 0;      // Number of seconds for the ESP8266 chip to deepsleep for.  GPIO16 needs to be tied to RST to wake from deepSleep http://esp8266.github.io/Arduino/versions/2.0.0/doc/libraries.html
    unsigned int publishRateInSeconds = 60; // defaults to once a minute
    unsigned long lastPublishTime = 0;
    
    unsigned long LastWifiTime = 0;
    SensorMode sensorMode = None;           // OperationMode enumeration: DemoMode (no sensors, fakes data), Bmp180Mode, Dht11Mode

  private:
    const char *GetValue(const char *value);

};

#endif
