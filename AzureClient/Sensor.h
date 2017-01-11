#ifndef Sensor_h
#define Sensor_h

#include "Arduino.h"
#include "Globals.h"

class Sensor
{
  public:
    Sensor(Telemetry* data);
    void measure();

  protected:
    bool initialised;
    Telemetry* _data;  
};

#endif
