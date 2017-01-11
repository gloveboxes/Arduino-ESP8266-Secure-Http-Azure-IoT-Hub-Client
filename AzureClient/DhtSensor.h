#ifndef DhtSensor_h
#define DhtSensor_h

#include "Sensor.h"
#include "DHT.h"

enum DhtType {
  dht11 = 11,
  dht22 = 22
};

class DhtSensor : public Sensor {  
  public:
    DhtSensor(Telemetry* data, DeviceConfig* device, DhtType dhtType) : Sensor(data){
      _dhtType = dhtType;
      device->sensorMode = DhtShieldMode;
    };
    void measure();

  private:
    void initialise();
    int _dhtType;
    const int DHT11PIN = 2; // GPIO pin 2 = D4 alias on NodeMCU, using gpio pin rather than D4 alias so it compiles against all ESP8266 Development boards.    
    DHT dht = DHT(DHT11PIN, 0);
};

#endif
