#include "DHT.h"

float dht11Calibration;
bool dht11Initialised = false;


const int DHT11PIN = 2; // GPIO pin 2 = D4 alias on NodeMCU, using gpio pin rather than D4 alias so it compiles against all ESP8266 Development boards.
DHT dht11(DHT11PIN, DHT11);

void initDHT11(){
  if (dht11Initialised) { return; }
  
  dht11.begin();
  dht11Calibration = getTemperatureCalibration();
  device.sensorMode = DhtShieldMode;
  delay(100);  
  
  dht11Initialised = true;
}

void getDht11Readings(){
  int retryCount = 0;
  initDHT11();
  
  do {  
    delay(50);
    data.temperature = dht11.readTemperature(); 
    data.humidity = dht11.readHumidity();
  } while ((isnan(data.temperature) || isnan(data.humidity)) && ++retryCount < 10);
}

