#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>


Adafruit_BMP280 bmp280; // I2C
float bmp280Calibration = 0;


void initBmp280(){
  Wire.begin(2, 14);  // Sparkfun ESP8266 Thing Dev
  bmp280.begin();  
  bmp280Calibration = getCalibration();

    Serial.println(bmp280Calibration);
}


void getBmp280Readings(){
  const int numberOfSamples = 10;
  data.temperature = data.pressure = 0;
   
  for (int c = 0; c < numberOfSamples; c++) {  
    data.temperature += bmp280.readTemperature() + bmp280Calibration; 
    data.pressure += (int)((int)( bmp280.readPressure() + 0.5) / 100);
    delay(1500);
  }
  
  data.temperature /= numberOfSamples;
  data.pressure /= numberOfSamples;
  data.humidity = 0;  
}

