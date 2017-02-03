#ifndef Sensor_h
#define Sensor_h


#include "Arduino.h"
#include <ArduinoJson.h>    // https://github.com/bblanchon/ArduinoJson - installed via library manager
#include <TimeLib.h>           // http://playground.arduino.cc/code/time - installed via library manager
#include "DigitalPin.h"

class Sensor
{
  public:
    Sensor(){};
    Sensor(DigitalPin *powerPin){
      _powerPin = powerPin;
      usingPowerPin = true;
    }   
    
    void measure();    
    char* toJSON();

    float temperature;
    float humidity;
    int pressure;
    int light;
    int msgId;
    const char *geo;

  protected:
    bool initialised;
    
    bool powerOn(){
      if (usingPowerPin) { 
        _powerPin->on();
        delay(50);
      }
      return initialised && !usingPowerPin;
    }

     void powerOff(){
      if (!usingPowerPin) { return; }
      _powerPin->off();
    }   
   

  private:    
    char buffer[256];
    char isoTime[30];
    char* getISODateTime();
    DigitalPin *_powerPin;
    bool usingPowerPin = false;
};

#endif
