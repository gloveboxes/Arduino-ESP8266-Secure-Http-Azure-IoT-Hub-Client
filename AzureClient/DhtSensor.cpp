#include "DhtSensor.h"
   
void DhtSensor::initialise(){
  if (initialised) { return; } 

  dht = DHT(DHT11PIN, _dhtType);
 
  dht.begin();
  delay(100);
  initialised = true;
}

void DhtSensor::measure(){
  int retryCount = 0;
  initialise();
  
  do {  
    delay(100);
    temperature = dht.readTemperature(); 
    humidity = dht.readHumidity();
  } while ((isnan(temperature) || isnan(humidity)) && ++retryCount < 20);
}

