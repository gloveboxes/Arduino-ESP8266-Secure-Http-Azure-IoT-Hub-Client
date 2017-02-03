#include "Arduino.h"
#include "Ldr.h"


int Ldr::measure()
{
  return map(analogRead(_pin), 0, 1023, 0, 100);  // convert to a percentage
}
