#include <WiFiClientSecure.h>
#include "sha256.h"
#include "Base64.h"


// Azure IoT Hub Settings
const char* TARGET_URL = "/devices/";
const char* IOT_HUB_END_POINT = "/messages/events?api-version=2015-08-15-preview";

// Azure Evet Hub settings
const char* EVENT_HUB_END_POINT = "/ehdevices/publishers/nodemcu/messages";

String endPoint;
String fullSas;

int limit = 0; // limiter for response loop
int sendCount = 0;

WiFiClientSecure tlsClient;

const int BufferLen = 256;
char buffer[BufferLen];


void initialiseAzure(CloudMode cm){
  switch(cm){
    case IoTHub:
      initialiseIotHub();
      break;
    case EventHub:
      initialiseEventHub();
      break;
  }
  Serial.print("sas: ");
  Serial.println(fullSas);
}

void initialiseIotHub(){
  String url = urlEncode(cloudConfig.host) + urlEncode(TARGET_URL) + (String)cloudConfig.id;
  endPoint = (String)TARGET_URL + (String)cloudConfig.id + (String)IOT_HUB_END_POINT;

  fullSas =  createIotHubSas(cloudConfig.key, url);
}

void initialiseEventHub() {
  String url = urlEncode("https://")  + urlEncode(cloudConfig.host) + urlEncode(EVENT_HUB_END_POINT);
  endPoint = EVENT_HUB_END_POINT;
  fullSas = createEventHubSas(cloudConfig.key, url);
}

void connectToAzure() {
  delay(500); // give network connection a moment to settle
  Serial.print(cloudConfig.id);
  Serial.print(" connecting to ");
  Serial.println(cloudConfig.host);
  if (WiFi.status() != WL_CONNECTED) { return; }
 	if (!tlsClient.connect(cloudConfig.host, 443)) {      // Use WiFiClientSecure class to create TLS connection
		Serial.println("Host connection failed");
		delay(5000);
	}
	else {
		Serial.println("Host connected");
    delay(250); // give network connection a moment to settle
	}
}

String createIotHubSas(char *key, String url){  
  String stringToSign = url + "\n" + deviceConfig.sasExpiryDate;

  // START: Create signature
  // https://raw.githubusercontent.com/adamvr/arduino-base64/master/examples/base64/base64.ino
  
  int keyLength = strlen(key);
  
  int decodedKeyLength = base64_dec_len(key, keyLength);
  char decodedKey[decodedKeyLength];  //allocate char array big enough for the base64 decoded key
  
  base64_decode(decodedKey, key, keyLength);  //decode key
  
  Sha256.initHmac((const uint8_t*)decodedKey, decodedKeyLength);
  Sha256.print(stringToSign);  
  char* sign = (char*) Sha256.resultHmac();
  // END: Create signature
  
  // START: Get base64 of signature
  int encodedSignLen = base64_enc_len(HASH_LENGTH);
  char encodedSign[encodedSignLen];
  base64_encode(encodedSign, sign, HASH_LENGTH); 
  
  // SharedAccessSignature
  return "sr=" + url + "&sig="+ urlEncode(encodedSign) + "&se=" + deviceConfig.sasExpiryDate;
  // END: create SAS  
}

String createEventHubSas(char *key, String url){  
  // START: Create SAS  
  // https://azure.microsoft.com/en-us/documentation/articles/service-bus-sas-overview/
  // Where to get seconds since the epoch: local service, SNTP, RTC

  String stringToSign = url + "\n" + deviceConfig.sasExpiryDate;

  // START: Create signature
  Sha256.initHmac((const uint8_t*)key, 44);
  Sha256.print(stringToSign);
  
  char* sign = (char*) Sha256.resultHmac();
  int signLen = 32;
  // END: Create signature

  // START: Get base64 of signature
  int encodedSignLen = base64_enc_len(signLen);
  char encodedSign[encodedSignLen];
  base64_encode(encodedSign, sign, signLen);   
  // END: Get base64 of signature

  // SharedAccessSignature
  return "sr=" + url + "&sig="+ urlEncode(encodedSign) + "&se=" + deviceConfig.sasExpiryDate +"&skn=" + cloudConfig.id;
  // END: create SAS
}
  

void publishData(SensorData data, const char *geo, int statusLed){
  int length;
  StaticJsonBuffer<300> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();

  preamble(root);
  
  root["Celsius"] = data.temperature;
  root["Humidity"] = data.humidity;
  root["hPa"] = data.pressure;
  root["Light"] = data.light;

  postamble(root);
  
  length = root.printTo(buffer, BufferLen);

  publishToAzure((String)buffer, statusLed);
}

void preamble(JsonObject& root){
  root["Dev"] = cloudConfig.id;
  root["Utc"] = GetISODateTime();
}

void postamble(JsonObject& root){
  root["Geo"] = cloudConfig.geo;  
  root["WiFi"] = deviceConfig.WiFiConnectAttempts;
  root["Mem"] = ESP.getFreeHeap();
  root["Id"] = ++sendCount;
}

void publishToAzure(String data, int statusLed) {
  // https://msdn.microsoft.com/en-us/library/azure/dn790664.aspx  

  String request = "POST " + endPoint + " HTTP/1.1\r\n" +
    "Host: " + cloudConfig.host + "\r\n" +
    "Authorization: SharedAccessSignature " + fullSas + "\r\n" +
    "Content-Type: application/atom+xml;type=entry;charset=utf-8\r\n" +
    "Content-Length: " + data.length() + "\r\n\r\n" + data;
  
  if (!tlsClient.connected()) { connectToAzure(); }

  if (!tlsClient.connected()) { return; }
  
  setLedState(statusLed, Off);
    
  tlsClient.print(request);
  
  String response = "";
  String chunk = "";
  limit = 0;
  
  do {
    if (tlsClient.connected()) {
      delay(5);  // give esp8266 firmware networking some time
      chunk = tlsClient.readStringUntil('\n');
      response += chunk;
    }
  } while (chunk.length() > 0 && ++limit < 100);
  
  Serial.print(limit);
  Serial.print(" Message ");
  Serial.print(sendCount);
  Serial.print(" Response code: ");
  
  if (response.length() > 12) { Serial.println(response.substring(9, 12)); }
  else { Serial.println("unknown"); }

  setLedState(statusLed, On);
}




