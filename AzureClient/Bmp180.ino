#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>

Adafruit_BMP085_Unified bmp180 = Adafruit_BMP085_Unified(10085);

bool bmp180Initialised = false;

void initBmp180(){
  if (bmp180Initialised) { return; }  

  bmp180.begin();
  
  bmp180Initialised = true;

  delay(100);
}


void getBmp180Readings(){
  const int numberOfSamples = 4;
  int sampleCount = 0, sampleAttempts = 0;

  initBmp180();

  data.temperature = data.pressure = data.humidity = 0;
   
  while (sampleCount < numberOfSamples && sampleAttempts < 10) { // no more than 10 attempts to read sensor

    sensors_event_t event;
    bmp180.getEvent(&event);
   
    if (event.pressure) {      
      data.pressure += (int)event.pressure;    
      float temperature;
      bmp180.getTemperature(&temperature);
      data.temperature += temperature;

      sampleCount++;
      delay(100);  
    }
    
    sampleAttempts++;
  }   
  
  data.temperature /= sampleCount;
  data.pressure /= sampleCount;  
  data.humidity /= sampleCount;  
}

