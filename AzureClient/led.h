#ifndef Led_h
#define Led_h


#include "Arduino.h"
#include "Device.h"


class Led
{
  public:
    Led(int pin, bool state = false, BoardType bt = Other);
    void on();
    void off();
    void toggle();
    
  private:
    int _pin;
    bool _state;
    BoardType _bt;
};

#endif
