#include "Arduino.h"
#include "DigitalPin.h"

void DigitalPin::on()
{
  if (_invert) {
    digitalWrite(_pin, LOW);
  }
  else {
    digitalWrite(_pin, HIGH);
  }
}

void DigitalPin::off()
{
  if (_invert) {
    digitalWrite(_pin, HIGH);
  }
  else {
    digitalWrite(_pin, LOW);
  }
}

void DigitalPin::toggle()
{
  _state = !_state;
  if (_state) { on(); } else { off(); }
}

