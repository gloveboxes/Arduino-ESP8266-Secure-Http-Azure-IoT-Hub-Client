#include "Arduino.h"
#include "DigitalPin.h"

DigitalPin::DigitalPin(int pin, bool state, BoardType bt)
{
  _pin = pin;
  _state = state;
  _bt = bt;
  
  pinMode(_pin, OUTPUT);
  if (_state) { on(); } else { off(); }
}

void DigitalPin::on()
{
  digitalWrite(_pin, HIGH);
}

void DigitalPin::off()
{
  digitalWrite(_pin, LOW);
}

void DigitalPin::toggle()
{
  _state = !_state;
  if (_state) { on(); } else { off(); }
}

