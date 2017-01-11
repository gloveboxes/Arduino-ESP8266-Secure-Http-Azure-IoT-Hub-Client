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
    _data->temperature = dht.readTemperature(); 
    _data->humidity = dht.readHumidity();
  } while ((isnan(_data->temperature) || isnan(_data->humidity)) && ++retryCount < 20);
}

