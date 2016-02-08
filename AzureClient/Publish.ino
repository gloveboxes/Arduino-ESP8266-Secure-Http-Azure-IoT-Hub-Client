#include <WiFiClientSecure.h>
#include "sha256.h"
#include "Base64.h"

WiFiClientSecure tlsClient;

// Azure IoT Hub Settings
const char* TARGET_URL = "/devices/";
const char* IOT_HUB_END_POINT = "/messages/events?api-version=2015-08-15-preview";

// Azure Event Hub settings
const char* EVENT_HUB_END_POINT = "/ehdevices/publishers/nodemcu/messages";

int sendCount = 0;
char buffer[256];
bool azureInitialised = false;


void initialiseAzure(){
  if (azureInitialised) { return; }
  
  switch(cloud.cloudMode){
    case IoTHub:
      initialiseIotHub();
      break;
    case EventHub:
      initialiseEventHub();
      break;
  }

  azureInitialised = true;
}

void initialiseIotHub(){
  String url = urlEncode(cloud.host) + urlEncode(TARGET_URL) + (String)cloud.id;
  cloud.endPoint = (String)TARGET_URL + (String)cloud.id + (String)IOT_HUB_END_POINT;
  cloud.fullSas =  createIotHubSas(cloud.key, url);
}

void initialiseEventHub() {
  String url = urlEncode("https://")  + urlEncode(cloud.host) + urlEncode(EVENT_HUB_END_POINT);
  cloud.endPoint = EVENT_HUB_END_POINT;
  cloud.fullSas = createEventHubSas(cloud.key, url);
}

void connectToAzure() {
  delay(500); // give network connection a moment to settle
  Serial.print(cloud.id);
  Serial.print(" connecting to ");
  Serial.println(cloud.host);
  if (WiFi.status() != WL_CONNECTED) { return; }
  if (!tlsClient.connect(cloud.host, 443)) {      // Use WiFiClientSecure class to create TLS connection
    Serial.print("Host connection failed.  WiFi IP Address: ");
    Serial.println(WiFi.localIP());

    delay(2000);
  }
  else {
    Serial.println("Host connected");
    yield(); // give firmware some time 
//    delay(250); // give network connection a moment to settle
  }
}

String createIotHubSas(char *key, String url){  
  String stringToSign = url + "\n" + cloud.sasExpiryDate;

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
  return "sr=" + url + "&sig="+ urlEncode(encodedSign) + "&se=" + cloud.sasExpiryDate;
  // END: create SAS  
}

String createEventHubSas(char *key, String url){  
  // START: Create SAS  
  // https://azure.microsoft.com/en-us/documentation/articles/service-bus-sas-overview/
  // Where to get seconds since the epoch: local service, SNTP, RTC

  String stringToSign = url + "\n" + cloud.sasExpiryDate;

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
  return "sr=" + url + "&sig="+ urlEncode(encodedSign) + "&se=" + cloud.sasExpiryDate +"&skn=" + cloud.id;
  // END: create SAS
}
  

String serializeData(SensorData data){
  StaticJsonBuffer<JSON_OBJECT_SIZE(16)> jsonBuffer;  //  allow for a few extra json fields that actually being used at the moment
  JsonObject& root = jsonBuffer.createObject();

  root["Dev"] = cloud.id;
  root["Utc"] = GetISODateTime();
  root["Celsius"] = data.temperature;
  root["Humidity"] = data.humidity;
  root["hPa"] = data.pressure;
  root["Light"] = data.light;
  root["Geo"] = cloud.geo;  

  //instrumentation
  root["WiFi"] = device.WiFiConnectAttempts;
  root["Mem"] = ESP.getFreeHeap();
  root["Id"] = ++sendCount;

  root.printTo(buffer, sizeof(buffer));

  return (String)buffer;
}

String buildHttpRequest(String data){  
  return "POST " + cloud.endPoint + " HTTP/1.1\r\n" +
    "Host: " + cloud.host + "\r\n" +
    "Authorization: SharedAccessSignature " + cloud.fullSas + "\r\n" +
    "Content-Type: application/atom+xml;type=entry;charset=utf-8\r\n" +
    "Content-Length: " + data.length() + "\r\n\r\n" + data;
}

void publishToAzure() {
  int bytesWritten = 0;

  // https://msdn.microsoft.com/en-us/library/azure/dn790664.aspx  

  initialiseAzure();
  
  if (!tlsClient.connected()) { connectToAzure(); }
  if (!tlsClient.connected()) { return; }
  
  setLedState(Off);

  tlsClient.flush();

  bytesWritten = tlsClient.print(buildHttpRequest(serializeData(data))); 
  
  String response = "";
  String chunk = "";
  int limit = 1;
  
  do {
    if (tlsClient.connected()) { 
      yield();
      chunk = tlsClient.readStringUntil('\n');
      response += chunk;
    }
  } while (chunk.length() > 0 && ++limit < 100);  

  Serial.print("Bytes sent ");
  Serial.print(bytesWritten);
  Serial.print(", Memory ");
  Serial.print(ESP.getFreeHeap());
  Serial.print(" Message ");
  Serial.print(sendCount);
  Serial.print(", Response chunks ");
  Serial.print(limit);
  Serial.print(", Response code: ");
  
  if (response.length() > 12) { Serial.println(response.substring(9, 12)); }
  else { Serial.println("unknown"); }

  setLedState(On);
}




