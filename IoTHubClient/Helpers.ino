void initLed(Leds l){
  pinMode(l, OUTPUT);
  digitalWrite(l, HIGH);
}

void initWifi(){
  const int WifiTimeoutMilliseconds = 60000;  // 60 seconds
  
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

//http://hardwarefun.com/tutorials/url-encoding-in-arduino
String urlEncode(const char* msg)
{
    const char *hex = "0123456789abcdef";
    String encodedMsg = "";

    while (*msg!='\0'){
        if( ('a' <= *msg && *msg <= 'z')
                || ('A' <= *msg && *msg <= 'Z')
                || ('0' <= *msg && *msg <= '9') ) {
            encodedMsg += *msg;
        } else {
            encodedMsg += '%';
            encodedMsg += hex[*msg >> 4];
            encodedMsg += hex[*msg & 15];
        }
        msg++;
    }
    return encodedMsg;
}
