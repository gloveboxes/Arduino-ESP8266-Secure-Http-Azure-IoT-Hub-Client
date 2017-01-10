#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>


Adafruit_BMP280 bmp280; // I2C
bool bmp280Initialised = false;


void initBmp280(){
  if (bmp280Initialised) { return; }  

  bmp280.begin();
  
  bmp280Initialised = true;
}


void getBmp280Readings(){
  const int numberOfSamples = 4;

  initBmp280();
  data.temperature = data.pressure = data.humidity = 0;
   
  for (int c = 0; c < numberOfSamples; c++) {  
    data.temperature += bmp280.readTemperature(); 
    Serial.print(data.temperature);
    data.pressure += (int)((int)( bmp280.readPressure() + 0.5) / 100);
    delay(500);
  }
  
  data.temperature /= numberOfSamples;
  data.pressure /= numberOfSamples;  
}

