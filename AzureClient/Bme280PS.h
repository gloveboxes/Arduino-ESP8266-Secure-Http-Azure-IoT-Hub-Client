#ifndef Bme280PS_h
#define Bme280PS_h

/*
 * Power saver version of Bme280 driver. Turns power on and off the sensor before and after measurement
 */

#include "Bme280.h"
#include "DigitalPin.h"


class Bme280PS : public Bme280
{
  public:
    Bme280PS(int pin) {
      _pin = pin;
    }

  private:
    void initialise(){
      powerPin.on();
      delay(100);
      bme280.begin(0x76);  
      delay(100);
    }
    
    int _pin;
    DigitalPin powerPin = DigitalPin(_pin);
};

#endif
