#include "Adafruit_BMP085.h"

Adafruit_BMP085 bmp;

//My device calibrations
//WiMos01 -3.4
//WiMos02 -0.8
//NodeMCU02 = -0.5
float bmp180Calibration = -1.1;


void initBmp180() {
  bmp.begin();
  bmp180Calibration = getCalibration();

  Serial.println(bmp180Calibration);
}

void getBmp180Readings(){
  const int numberOfSamples = 10;
  data.temperature = data.pressure = 0;
   
  for (int c = 0; c < numberOfSamples; c++) {  
    data.temperature += bmp.readTemperature() + bmp180Calibration;  
    data.pressure += (int)((int)( bmp.readPressure() + 0.5) / 100);
    delay(1500);
  }
  
  data.temperature /= numberOfSamples;
  data.pressure /= numberOfSamples;
  data.humidity = 0;
}


