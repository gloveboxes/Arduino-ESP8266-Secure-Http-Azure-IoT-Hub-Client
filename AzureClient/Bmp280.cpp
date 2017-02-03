#include "Bmp280.h"


void Bmp280::initialise(){
  if (powerOn()) {return;}
  bmp280.begin();
  delay(100);  
  initialised = true;
}

void Bmp280::measure(){
 const int numberOfSamples = 4;

  initialise();
  temperature = pressure = humidity = 0;
   
  for (int c = 0; c < numberOfSamples; c++) {  
    temperature += bmp280.readTemperature(); 
    Serial.print(temperature);
    pressure += (int)((int)( bmp280.readPressure() + 0.5) / 100);
    delay(500);
  }
  
  temperature /= numberOfSamples;
  pressure /= numberOfSamples;  

  powerOff();
}

