#ifndef Bmp180_h
#define Bmp180_h

#include "Sensor.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>
#include "DigitalPin.h"


class Bmp180 : public Sensor
{
  public:
    Bmp180(){};
    Bmp180(DigitalPin *powerPin):Sensor(powerPin){};
    void measure();

  protected:
    Adafruit_BMP085_Unified bmp180 = Adafruit_BMP085_Unified(10085);

  private:
    void initialise();  
};

#endif
