
void initWifi(){
  const int WifiTimeoutMilliseconds = 60000;  // 60 seconds
  String SSIDs[] = {"NCW", "fred"};
  
  if (WiFi.status() == WL_CONNECTED) {return;}  

  if (deviceConfig.LastWifiTime >  millis() ) {
    delay(500);    
    return;
  }  

  if (deviceConfig.WifiIndex >= deviceConfig.wifiPairs) {deviceConfig.WifiIndex = 0;}

  Serial.println("trying " + String(deviceConfig.ssid[deviceConfig.WifiIndex]));

  WiFi.begin(deviceConfig.ssid[deviceConfig.WifiIndex], deviceConfig.pwd[deviceConfig.WifiIndex]);
  
  deviceConfig.WiFiConnectAttempts++;
  deviceConfig.LastWifiTime = millis() + WifiTimeoutMilliseconds;
  
  deviceConfig.WifiIndex++;  //increment wifi indexready for the next ssid/pwd pair in case the current wifi pair dont connect
}
