#ifndef DigitalPin_h
#define DigitalPin_h


#include "Arduino.h"
#include "Device.h"


class DigitalPin
{
  public:
    DigitalPin(int pin, bool state = false, BoardType bt = Other);
    void on();
    void off();
    void toggle();
    
  private:
    int _pin;
    bool _state;
    BoardType _bt;
};

#endif
