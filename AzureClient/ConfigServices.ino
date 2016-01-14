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

  cloud.host = GetValue(root["host"]);
  cloud.key = (char*)GetValue(root["key"]);
  cloud.id = GetValue(root["id"]);
  cloud.geo = GetValue(root["geo"]);


  wifiConfig.wifiPairs = root["wifi"]; 
  wifiConfig.ssid = new const char*[wifiConfig.wifiPairs];
  wifiConfig.pwd = new const char*[wifiConfig.wifiPairs];

	for (int i = 0; i < wifiConfig.wifiPairs; i++)
	{
    wifiConfig.ssid[i] = GetValue(root["ssid"][i]);
    wifiConfig.pwd[i] = GetValue(root["pwd"][i]);
	}
}

//void azureConnectionString(String cs){
//  cloud.host = GetStringValue(splitStringByIndex(splitStringByIndex(cs, ';', 0), '=', 1));
//  cloud.id = GetStringValue(splitStringByIndex(splitStringByIndex(cs, ';', 1), '=', 1));
//  cloud.key = (char*)GetStringValue(splitStringByIndex(splitStringByIndex(cs, ';', 2), '=', 1));
//}


