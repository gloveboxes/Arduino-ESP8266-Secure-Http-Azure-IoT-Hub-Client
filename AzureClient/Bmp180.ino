#include "Adafruit_BMP085.h"
#include <Wire.h>

Adafruit_BMP085 bmp;

void initBmp180() {
  bmp.begin();
}

void getBmp180Readings(){
  data.temperature = bmp.readTemperature(); 
  data.humidity = 0;
  data.pressure = (int)((int)( bmp.readPressure() + 0.5) / 100);
}

