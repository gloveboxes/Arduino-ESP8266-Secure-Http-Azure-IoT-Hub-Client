#include "Arduino.h"
#include "Led.h"

Led::Led(int pin, bool state, BoardType bt)
{
  _pin = pin;
  _state = state;
  _bt = bt;
  
  pinMode(_pin, OUTPUT);
  if (_state) { on(); } else { off(); }
}

void Led::on()
{
  digitalWrite(_pin, HIGH);
}

void Led::off()
{
  digitalWrite(_pin, LOW);
}

void Led::toggle()
{
  _state = !_state;
  if (_state) { on(); } else { off(); }
}

