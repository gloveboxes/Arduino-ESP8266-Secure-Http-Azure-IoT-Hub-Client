#ifndef LDR_h
#define LDR_h


class Ldr
{
  public:
    Ldr(int pin = A0){ _pin = pin; }
    int measure();
    
  private:
    int _pin;
};

#endif
