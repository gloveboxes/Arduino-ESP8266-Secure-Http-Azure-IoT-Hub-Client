enum CloudMode {
  IoTHub,
  EventHub
};

enum LedState {
  Off,
  On
};

enum SensorMode {
  None,
  Bmp180Mode,
  Dht11ShieldMode
};

enum BoardType {
  NodeMCU,
  WeMos,
  SparkfunThing,
  Other
};

enum DisplayMode {
  NoDisplay,
  LedMatrix
};

struct SensorData{
  float temperature;
  float humidity;
  int pressure;
  int light;
};

struct CloudConfig {
  const char *host;
  char *key;
  const char *id;
  const char *geo;
  unsigned long lastPublishTime = 0;
};

struct DeviceConfig {
  int WifiIndex = 0;
  unsigned long LastWifiTime = 0;
  int WiFiConnectAttempts = 0;
  int wifiPairs = 0;
  const char ** ssid;
  const char **pwd;
  CloudMode cloudMode = IoTHub;
  BoardType boardType = Other;            // OperationMode enumeration: NodeMCU, WeMos, SparkfunThing, Other
  SensorMode sensorMode = None;           // OperationMode enumeration: DemoMode (no sensors, fakes data), Bmp180Mode, Dht11Mode
  DisplayMode displayMode = NoDisplay;    // DisplayMode enumeration: NoDisplay or LedMatrix
  unsigned int publishRateInSeconds = 60; // defaults to once a minute

  // WARNING EXPIRY SET TO 10 YEARS FROM NOW.  
  // Epoch Timestamp Conversion Tool http://www.epochconverter.com/
  // Expires Wed, 22 Jan 2025 00:00:00 GMT.  Todo: add expiry window - eg now plus 2 days...
  // IOT HUB Devices can be excluded by device id/key - expiry window not so relevant
  // EVENT Hubs Devices can only be excluded by policy so a more sensible expiry should be tried and you'd need to device a moving expiry window
  unsigned int sasExpiryDate = 1737504000;  // Expires Wed, 22 Jan 2025 00:00:00 GMT
};

