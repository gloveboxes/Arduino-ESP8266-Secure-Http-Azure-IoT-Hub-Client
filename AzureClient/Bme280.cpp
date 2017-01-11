#include "Bme280.h"


void Bme280::initialise(){
  if (initialised) { return; }  
  bme280.begin(0x76);  
  delay(100);  
  initialised = true;
}

void Bme280::measure(){
  const int numberOfSamples = 4;

  initialise();
  _data->temperature = _data->pressure = _data->humidity = 0;
   
  for (int c = 0; c < numberOfSamples; c++) {  
    _data->temperature += bme280.readTemperature(); 
    _data->pressure += (int)((int)( bme280.readPressure() + 0.5) / 100);
    _data->humidity += bme280.readHumidity();
    delay(500);
  }
  
  _data->temperature /= numberOfSamples;
  _data->pressure /= numberOfSamples;  
  _data->humidity /= numberOfSamples;
}

