enum CloudMode {
  IoTHub,
  EventHub
};

enum Leds {
  WifiConnectedLed = D4, PublishingLed = BUILTIN_LED
};

enum OperationMode {
  DemoMode,
  SensorMode
};

struct SensorData{
  float temperature;
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

struct WiFiConfig {
  int WifiIndex = 0;
  unsigned long LastWifiTime = 0;
  int WiFiConnectAttempts = 0;
  int wifiPairs = 0;
  const char **ssid;
  const char **pwd;
};

