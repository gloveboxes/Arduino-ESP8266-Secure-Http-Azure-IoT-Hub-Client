#include "DHT.h"
const int calibration = 0;

const int DHTPIN = 2; // GPIO pin 2 = D4 alias on NodeMCU, using gpio pin rather than D4 alias so it compiles against all ESP8266 Development boards.
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);

void initDHT11(){
  dht.begin();
}

void getDht11Readings(){
  float temperature = dht.readTemperature() + calibration;  // -2 modest recallibrate
  float humidity = dht.readHumidity();
  data.pressure = 0;
  if (isnan(temperature) || isnan(humidity)) {
        Serial.println("Failed to read from DHT sensor!");
  }
  else {
    data.temperature = temperature;
    data.humidity = humidity;
  }
}

