#include <EEPROM.h>

void loadConfigFromEEPROM() {
	EEPROM.begin(512);
	char* data;
	int length;

	const int BUFFER_SIZE = JSON_OBJECT_SIZE(4) + JSON_ARRAY_SIZE(0);
	StaticJsonBuffer<1000> jsonBuffer;
	int address = 2;

	length = word(EEPROM.read(0), EEPROM.read(1));
	data = new char[length];

	for (address = 2; address < length + 2; address++) {
		data[address - 2] = EEPROM.read(address);
	}
	data[address - 2] = '\0';


	JsonObject& root = jsonBuffer.parseObject(data);
	if (!root.success())
	{
		Serial.println("parseObject() failed");
		return;
	}

  cloudConfig.host = GetValue(root["host"]);
  cloudConfig.key = (char*)GetValue(root["key"]);
  cloudConfig.id = GetValue(root["id"]);
  cloudConfig.geo = GetValue(root["geo"]);


  deviceConfig.wifiPairs = root["wifi"]; 
  deviceConfig.ssid = new const char*[deviceConfig.wifiPairs];
  deviceConfig.pwd = new const char*[deviceConfig.wifiPairs];

	for (int i = 0; i < deviceConfig.wifiPairs; i++)
	{
    deviceConfig.ssid[i] = GetValue(root["ssid"][i]);
    deviceConfig.pwd[i] = GetValue(root["pwd"][i]);
	}
}

void configIotHubClient(String cs, const char *ssid, const char *pwd, const char *geo){
  deviceConfig.wifiPairs = 1;
  deviceConfig.ssid = new const char*[deviceConfig.wifiPairs];
  deviceConfig.pwd = new const char*[deviceConfig.wifiPairs];
  deviceConfig.ssid[0] = ssid;
  deviceConfig.pwd[0] = pwd;
  cloudConfig.geo = geo;
  
  cloudConfig.host = GetStringValue(splitStringByIndex(splitStringByIndex(cs, ';', 0), '=', 1));
  cloudConfig.id = GetStringValue(splitStringByIndex(splitStringByIndex(cs, ';', 1), '=', 1));
  cloudConfig.key = (char*)GetStringValue(splitStringByIndex(splitStringByIndex(cs, ';', 2), '=', 1));
}


