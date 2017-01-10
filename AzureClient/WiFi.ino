
void initWifi(){
  const int WifiTimeoutMilliseconds = 60000;  // 60 seconds
  
  if (WiFi.status() == WL_CONNECTED) {return;}  

  if (device.LastWifiTime >  millis() ) {
    delay(500);    
    return;
  }  

  if (device.WifiIndex >= device.wifiPairs) {device.WifiIndex = 0;}

  Serial.println("trying " + String(device.ssid[device.WifiIndex]));

  WiFi.begin(device.ssid[device.WifiIndex], device.pwd[device.WifiIndex]);
  
  data.WiFiConnectAttempts++;
  device.LastWifiTime = millis() + WifiTimeoutMilliseconds;
  
  device.WifiIndex++;  //increment wifi indexready for the next ssid/pwd pair in case the current wifi pair dont connect
}
