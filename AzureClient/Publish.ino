char buffer[256];

String serializeData(){
  StaticJsonBuffer<JSON_OBJECT_SIZE(16)> jsonBuffer;  //  allow for a few extra json fields that actually being used at the moment
  JsonObject& root = jsonBuffer.createObject();

//  root["Dev"] = cloud.id;
  root["Utc"] = GetISODateTime();
  root["Celsius"] = data.temperature;
  root["Humidity"] = data.humidity;
  root["hPa"] = data.pressure;
  root["Light"] = data.light;
  root["Geo"] = data.geo;  

  //instrumentation
//  root["WiFi"] = device.WiFiConnectAttempts;
  root["Mem"] = ESP.getFreeHeap();
  root["Id"] = ++data.msgId;

  root.printTo(buffer, sizeof(buffer));

  return (String)buffer;
}

void publishToAzure() {
  Serial.print("Publish To Azure: ");

  // https://msdn.microsoft.com/en-us/library/azure/dn790664.aspx  
  
  setLedState(On);

  String response = hub.send(serializeData());

  Serial.print("Memory ");
  Serial.print(ESP.getFreeHeap());
  Serial.print(" Message ");
  Serial.print(data.msgId);
  Serial.print(", Response code: ");
  Serial.println(response);

  setLedState(Off);
}




