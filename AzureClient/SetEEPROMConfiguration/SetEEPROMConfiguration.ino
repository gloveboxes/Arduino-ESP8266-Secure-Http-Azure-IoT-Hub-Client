#include <ArduinoJson.h>
#include <EEPROM.h>

int wifi = 2;  // number of defined wifi ssid/pwd pairs
const char* ssid[] = { "NCW", "dgTether" };
const char* password[] = { "malolos5459", "VisualStudio2005" };
const char* geo = "-33.8745828,151.2067465";  // Sydney Town Hall


// IOT Hub
const char* host = "MakerDen.azure-devices.net";
const char* id = "WeMos01";
const char* key = "phKNWLHKTVcn0iF+Plb4kGkohPenDzLg/jFSRyX9s/g=";


// Event Hub
//const char* host = "makerdenww-ns.servicebus.windows.net";
//const char* key = "Z2aUC4jbHD3aphn8pIMmOhiqf9/ns7o/SvPSixKKCEY=";
//const char* id = "D1";



const int BufferLen = 510;  // max eprom length, minus 2 for data length
char buffer[BufferLen];
int length;


void setup() {
  Serial.begin(9600);
  EEPROM.begin(512);

  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  
  //root["SSID"] = ssid;
   root["wifi"] = wifi;

  JsonArray& _ssid = root.createNestedArray("ssid");
  _ssid.add(ssid[0]);
  _ssid.add(ssid[1]);

  JsonArray& _password = root.createNestedArray("pwd");
  _password.add(password[0]);
  _password.add(password[1]);

  root["host"] = host;
  root["key"] = key;
  root["id"] = id;
  root["geo"] = geo;
  
  length = root.printTo(buffer, BufferLen);
  root.prettyPrintTo(Serial);
  
  EEPROM.write(0, highByte(length));
  EEPROM.write(1, lowByte(length));
  for (int address = 2; address < length + 2; address++){
    EEPROM.write(address, buffer[address-2]);
  }
  EEPROM.commit();
}

void loop() {

  getconfiguration();
  delay(2000);
  
}

void getconfiguration(){
  StaticJsonBuffer<1000> jsonBuffer;
  int address = 2;
  length = word(EEPROM.read(0), EEPROM.read(1));
  for (address = 2; address < length + 2; address++) {
    buffer[address-2] = EEPROM.read(address);
  }
  buffer[address-2] = '\0';
  Serial.println(buffer);

  JsonObject& root = jsonBuffer.parseObject(buffer);
  if (!root.success())
  {
    Serial.println("parseObject() failed");
    return;
  }

  wifi = root["wifi"];
  ssid[0] = root["ssid"][0];
  password[0] = root["pwd"][0];
  host = root["host"];
  key = root["key"];
  id = root["id"];
  geo = root["geo"];

  Serial.println(wifi);
  Serial.println(ssid[0]);
  Serial.println(password[0]);
  Serial.println(ssid[1]);
  Serial.println(password[1]);
  Serial.println(host);
  Serial.println(key);
  Serial.println(id);
  Serial.println(geo);
}


