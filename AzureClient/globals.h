#ifndef Globals_h
#define Globals_h


enum LedState {
  Off,
  On
};

enum SensorMode {
  None,
  Bmp180Mode,
  DhtShieldMode
};

enum BoardType {
  NodeMCU,
  WeMos,
  SparkfunThing,
  Other
};

struct SensorData{
  float temperature;
  float humidity;
  int pressure;
  int light;
  int msgId;
  const char *geo; 
  unsigned int WiFiConnectAttempts;
};

struct CloudConfig {
  time_t  sasExpiryTime = 0;
  int sasExpiryPeriodInSeconds = 60 * 15; // Default to 15 minutes
  const char* host;
  const char* certificateFingerprint;
  char* key;
  const char* deviceId; 
  char* fullSas = new char[0];
  String sasUrl;
  String endPoint;
};

struct DeviceConfig {
  int WifiIndex = 0;
  unsigned long LastWifiTime = 0;
  int wifiPairs = 0;
  const char** ssid;
  const char** pwd;
  BoardType boardType = Other;            // OperationMode enumeration: NodeMCU, WeMos, SparkfunThing, Other
  SensorMode sensorMode = None;           // OperationMode enumeration: DemoMode (no sensors, fakes data), Bmp180Mode, Dht11Mode
  unsigned int deepSleepSeconds = 0;      // Number of seconds for the ESP8266 chip to deepsleep for.  GPIO16 needs to be tied to RST to wake from deepSleep http://esp8266.github.io/Arduino/versions/2.0.0/doc/libraries.html
  unsigned int publishRateInSeconds = 60; // defaults to once a minute
};

#endif

