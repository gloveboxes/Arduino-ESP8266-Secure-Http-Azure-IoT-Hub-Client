bool ledInitialised = false;

void initLed(int pin){
  if (ledInitialised) { return; }
  
  if (pin == -1) { return; }  // no valid led pin available
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);

  ledInitialised = true;
}

void setLedState(LedState state){
  int pin = getStatusLed(device.boardType);
  if (pin == -1) { return; }  // no valid led pin available

  initLed(pin);
  digitalWrite(pin, (state == Off) ? HIGH : LOW);
}

int getStatusLed(BoardType boardType){
  if (device.boardType == WeMos && device.sensorMode == DhtShieldMode) { return -1; }   // if wemos using the dht11 temperature and humidity shield set invalid pin as the uses pin D4, the same as the onboard led
  
  switch (boardType){
    case NodeMCU:
    case WeMos:
      return BUILTIN_LED;
    case SparkfunThing:
      return 5;  // override BUILTIN as because I think it's not unusual to deploy against this dev board using the generic ESP8266 board manager which maps the wrong built in LED pin
    default:
      return -1; // set pin to -1 to indicate no valid led status pin
  }
}
