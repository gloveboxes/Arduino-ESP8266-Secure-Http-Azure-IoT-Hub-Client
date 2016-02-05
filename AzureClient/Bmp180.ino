#include "Adafruit_BMP085.h"

Adafruit_BMP085 bmp;
bool bmp180Initialised = false;
float bmp180Calibration = 0;


void initBmp180() {
  if (bmp180Initialised) { return; }  

  if (device.boardType == SparkfunThing) { Wire.begin(2, 14); } // Sparkfun ESP8266 Thing Dev
  bmp.begin();
  bmp180Calibration = getTemperatureCalibration();
  
  bmp180Initialised = true;
}

void getBmp180Readings(){
  const int numberOfSamples = 4;

  initBmp180();
  data.temperature = data.pressure = 0;
   
  for (int c = 0; c < numberOfSamples; c++) {  
    data.temperature += bmp.readTemperature() + bmp180Calibration;  
    data.pressure += (int)((int)( bmp.readPressure() + 0.5) / 100);
    delay(500);
  }
  
  data.temperature /= numberOfSamples;
  data.pressure /= numberOfSamples;
}


