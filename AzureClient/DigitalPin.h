#ifndef DigitalPin_h
#define DigitalPin_h


#include "Arduino.h"
#include "Device.h"


class DigitalPin
{
  public:
    DigitalPin(){};
    DigitalPin(int pin, bool state = false, bool invert = false)
    {
      _pin = pin;
      _state = state;
      _invert = invert;
      
      pinMode(_pin, OUTPUT);
      if (_state) { on(); } else { off(); }
    }
    void on();
    void off();
    void toggle();
    int _pin;
    
  private:
    
    bool _state;
    bool _invert;
};

#endif
