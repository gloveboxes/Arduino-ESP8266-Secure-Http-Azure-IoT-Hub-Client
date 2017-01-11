#include "Sensor.h"

Sensor::Sensor(Telemetry* data)
{ 
  _data = data;
}

void Sensor::measure(){
  _data->temperature = 25;
  _data->humidity = 50;
  _data->pressure = 1000;

  Serial.println(_data->temperature);
  Serial.println(_data->pressure);
  Serial.println(_data->humidity);
}


