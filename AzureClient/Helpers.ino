void initLed(int pin){
  if (pin == -1) { return; }  // no valid led pin available
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
}

void setLedState(int pin, LedState state){
  if (pin == -1) { return; }  // no valid led pin available
  digitalWrite(pin, (state == Off) ? HIGH : LOW);
}

int getStatusLed(BoardType boardType){
  if (deviceConfig.boardType == WeMos && deviceConfig.sensorMode == Dht11ShieldMode) { return -1; }   // if wemos using the dht11 temperature and humidity shield set invalid pin as the uses pin D4, the same as the onboard led
  
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

//http://hardwarefun.com/tutorials/url-encoding-in-arduino
String urlEncode(const char* msg)
{
    const char *hex = "0123456789abcdef";
    String encodedMsg = "";

    while (*msg!='\0'){
        if( ('a' <= *msg && *msg <= 'z')
                || ('A' <= *msg && *msg <= 'Z')
                || ('0' <= *msg && *msg <= '9') ) {
            encodedMsg += *msg;
        } else {
            encodedMsg += '%';
            encodedMsg += hex[*msg >> 4];
            encodedMsg += hex[*msg & 15];
        }
        msg++;
    }
    return encodedMsg;
}

// http://arduino.stackexchange.com/questions/1013/how-do-i-split-an-incoming-string
String splitStringByIndex(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = { 0, -1  };  
  int maxIndex = data.length()-1;
  
  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
      found++;
      strIndex[0] = strIndex[1]+1;
      strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }
  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}


const char *GetValue(const char* value){
  char *temp = new char[strlen(value) + 1];
  strcpy(temp, value);
  return temp;
}

const char *GetStringValue(String value){
  int len = value.length() + 1;
  char *temp = new char[len];
  value.toCharArray(temp, len);
  return temp;
}
