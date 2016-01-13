enum CloudMode {
  IoTHub,
  EventHub
};

enum Leds {
  NodeMCULed = D4, StatusLed = BUILTIN_LED
};

enum LedState {
  On,
  Off
};

enum SensorMode {
  DemoMode,
  Bmp180Mode,
  Dht11Mode
};

enum BoardType {
  NodeMCU,
  WeMos
};

enum DisplayMode {
  NoDisplay,
  LedMatrix
};

enum LightSensor {
  None,
  Enabled
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

struct WiFiConfig {
  int WifiIndex = 0;
  unsigned long LastWifiTime = 0;
  int WiFiConnectAttempts = 0;
  int wifiPairs = 0;
  const char ** ssid;
  const char **pwd;
};

