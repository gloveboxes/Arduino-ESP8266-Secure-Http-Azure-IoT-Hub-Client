#ifndef Sensor_h
#define Sensor_h

#include "Arduino.h"
#include "Globals.h"

class Sensor
{
  public:
    Sensor(SensorData* data);
    void measure();

  protected:
    bool initialised;
    SensorData* _data;  
};

#endif
