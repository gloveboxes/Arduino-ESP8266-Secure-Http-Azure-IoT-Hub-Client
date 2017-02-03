#ifndef Device_h
#define Device_h

#include <arduino.h>
//#include <EEPROM.h>
//#include <ArduinoJson.h>    // https://github.com/bblanchon/ArduinoJson - installed via library manager
#include <ESP8266WiFi.h>


enum SensorMode {
  None,
  Bmp180Mode,
  DhtShieldMode
};

class Device {
  public:
    Device(const char* ssid, const char* pwd){
      WiFi.mode(WIFI_OFF);
      WiFi.mode(WIFI_STA);  // Ensure WiFi in Station/Client Mode
      wifiPairs = 1;
      _ssid = new const char*[wifiPairs];
      _pwd = new const char*[wifiPairs];
      _ssid[0] = ssid;
      _pwd[0] = pwd;
    }
//
//    void initialise(const char* ssid, const char* pwd);
//    void initialise(const char **ssid, const char **pwd, int ssidCount, int pwdCount);
    bool connectWifi();
  

    unsigned int deepSleepSeconds = 0;      // Number of seconds for the ESP8266 chip to deepsleep for.  GPIO16 needs to be tied to RST to wake from deepSleep http://esp8266.github.io/Arduino/versions/2.0.0/doc/libraries.html
    unsigned int publishRateInSeconds = 60; // defaults to once a minute
    unsigned long lastPublishTime = 0;
    SensorMode sensorMode = None;           // OperationMode enumeration: DemoMode (no sensors, fakes data), Bmp180Mode, Dht11Mode


  private:  
//    const char *GetValue(const char *value);
    int WifiIndex = 0;
    int wifiPairs = 0;
    const char **_ssid;
    const char **_pwd;
    unsigned long LastWifiTime = 0;

};

#endif
