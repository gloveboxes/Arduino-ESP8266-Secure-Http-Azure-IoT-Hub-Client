#include "Bmp280.h"


void Bmp280::initialise(){
  if (initialised) { return; }  
  bmp280.begin();
  delay(100);  
  initialised = true;
}

void Bmp280::measure(){
 const int numberOfSamples = 4;

  initialise();
  _data->temperature = _data->pressure = _data->humidity = 0;
   
  for (int c = 0; c < numberOfSamples; c++) {  
    _data->temperature += bmp280.readTemperature(); 
    Serial.print(_data->temperature);
    _data->pressure += (int)((int)( bmp280.readPressure() + 0.5) / 100);
    delay(500);
  }
  
  _data->temperature /= numberOfSamples;
  _data->pressure /= numberOfSamples;  
}

