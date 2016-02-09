#include "DHT.h"

#define DHTPIN22 2     // what digital pin we're connected to
DHT dht22(DHTPIN22, DHT22);

float dht22Calibration;
bool dht22Initialised = false;

void initDht22(){
  if (dht22Initialised) { return; }
  
  dht22.begin();
  dht22Calibration = getTemperatureCalibration();
  device.sensorMode = DhtShieldMode;
  delay(100);

  dht22Initialised = true;
}

void getDht22Readings(){
  int retryCount = 0;
  initDht22();

  do {
    delay(50);
    data.temperature = dht22.readTemperature(); 
    data.humidity = dht22.readHumidity();
  } while ((isnan(data.temperature) || isnan(data.humidity)) && ++retryCount < 10);
}

