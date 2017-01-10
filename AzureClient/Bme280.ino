#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>


Adafruit_BME280  bme280; // I2C
bool bme280Initialised = false;

void initBme280(){
  if (bme280Initialised) { return; }  

  bme280.begin(0x76);  

  delay(100);
  
  bme280Initialised = true;
}


void getBme280Readings(){
  const int numberOfSamples = 4;

  initBme280();
  data.temperature = data.pressure = data.humidity = 0;
   
  for (int c = 0; c < numberOfSamples; c++) {  
    data.temperature += bme280.readTemperature(); 
    data.pressure += (int)((int)( bme280.readPressure() + 0.5) / 100);
    data.humidity += bme280.readHumidity();
    delay(500);
  }
  
  data.temperature /= numberOfSamples;
  data.pressure /= numberOfSamples;  
  data.humidity /= numberOfSamples;

//  Serial.println(data.temperature);
//  Serial.println(data.pressure);
//  Serial.println(data.humidity);
  
}

