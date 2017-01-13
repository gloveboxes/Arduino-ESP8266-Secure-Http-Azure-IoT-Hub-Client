#include "Device.h"

void Device::initCloudConfig(const char *_ssid, const char *_pwd) {
  wifiPairs = 1;
  ssid = new const char*[wifiPairs];
  pwd = new const char*[wifiPairs];
  ssid[0] = _ssid;
  pwd[0] = _pwd;
}

//void Device::initCloudConfig() {
//  EEPROM.begin(512);
//  char* eepromData;
//  int length;
//
//  const int BUFFER_SIZE = JSON_OBJECT_SIZE(4) + JSON_ARRAY_SIZE(0);
//  StaticJsonBuffer<1000> jsonBuffer;
//  int address = 2;
//
//  length = word(EEPROM.read(0), EEPROM.read(1));
//  eepromData = new char[length];
//
//  for (address = 2; address < length + 2; address++) {
//    eepromData[address - 2] = EEPROM.read(address);
//  }
//  eepromData[address - 2] = '\0';
//
//
//  JsonObject& root = jsonBuffer.parseObject(eepromData);
//  if (!root.success())
//  {
//    Serial.println("parseObject() failed");
//    return;
//  }
//
//
////  sensor.geo = GetValue(root["geo"]);
//
//
//  wifiPairs = root["wifi"];
//  ssid = new const char*[wifiPairs];
//  pwd = new const char*[wifiPairs];
//
//  for (int i = 0; i < wifiPairs; i++)
//  {
//    ssid[i] = GetValue(root["ssid"][i]);
//    pwd[i] = GetValue(root["pwd"][i]);
//  }
//}

const char* Device::GetValue(const char *value) {
  char *temp = new char[strlen(value) + 1];
  strcpy(temp, value);
  return temp;
}
