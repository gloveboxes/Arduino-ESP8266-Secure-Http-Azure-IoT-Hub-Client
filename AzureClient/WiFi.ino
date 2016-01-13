
void initWifi(){
  const int WifiTimeoutMilliseconds = 60000;  // 60 seconds
  String SSIDs[] = {"NCW", "fred"};
  
  if (WiFi.status() == WL_CONNECTED) {return;}  

  if (wifiConfig.LastWifiTime >  millis() ) {
    delay(500);    
    return;
  }  

  if (wifiConfig.WifiIndex >= wifiConfig.wifiPairs) {wifiConfig.WifiIndex = 0;}

  Serial.println("trying " + String(wifiConfig.ssid[wifiConfig.WifiIndex]));

  WiFi.begin(wifiConfig.ssid[wifiConfig.WifiIndex], wifiConfig.pwd[wifiConfig.WifiIndex]);
  
  wifiConfig.WiFiConnectAttempts++;
  wifiConfig.LastWifiTime = millis() + WifiTimeoutMilliseconds;
  
  wifiConfig.WifiIndex++;  //increment wifi indexready for the next ssid/pwd pair in case the current wifi pair dont connect
}
