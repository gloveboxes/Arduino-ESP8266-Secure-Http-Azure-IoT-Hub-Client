#ifndef Bme280_h
#define Bme280_h

#include "Sensor.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>


class Bme280 : public Sensor
{
  public:
    void measure();

  protected:
    void initialise();
    Adafruit_BME280  bme280; // I2C
};

#endif
