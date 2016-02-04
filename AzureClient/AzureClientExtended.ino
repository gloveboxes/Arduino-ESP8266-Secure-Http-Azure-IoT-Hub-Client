// Azure Client extended with support for BMP180 Temperature and DHT11 Temperature and Humidity, Light Sensor and LED 8x8 Matrix display
// Just replace Azure Sketch


//#include <Time.h>           // http://playground.arduino.cc/code/time - installed via library manager
//#include <ArduinoJson.h>    // https://github.com/bblanchon/ArduinoJson
//#include <ESP8266WiFi.h>
//#include <Wire.h>
//#include "Adafruit_BMP085.h"
//#include "DHT.h"
//#include "names.h"
//
//// cloud configurations
//CloudMode cloudMode = IoTHub;         // CloudMode enumeration: IoTHub or EventHub
//
//// device configuration
//BoardType boardType = SparkfunThing;     // OperationMode enumeration: NodeMCU, WeMos, SparkfunThing, Other
//SensorMode sensorMode = DemoMode;    // OperationMode enumeration: DemoMode (no sensors, fakes data), Bmp180Mode, Dht11Mode
//DisplayMode displayMode = NoDisplay; // DisplayMode enumeration: NoDisplay or LedMatrix
//
//const unsigned int publishRateInSeconds = 20; // limit publishing rate to specified seconds
//
//CloudConfig cloud;
//WiFiConfig device;
//SensorData data;
//
//
//// sensors
//Adafruit_BMP085 bmp;
//
//const int DHTPIN = 2; // GPIO pin 2 = D4 alias on NodeMCU, using gpio pin rather than D4 alias so it compiles against all ESP8266 Development boards.
//#define DHTTYPE DHT11   // DHT 11
//DHT dht(DHTPIN, DHTTYPE);
//
//
//void setup() {  
//  Serial.begin(9600);
//  delay(100);
//  Serial.println();
//
//  loadConfigFromEEPROM();
//  
//  initialiseAzure(cloudMode);
//  
//  initLed(getStatusLed(boardType)); 
//  
//  initDevices();
//}
//
//void loop() {  
//  getLightReading();
//  getWeatherReadings();
//  
//  if (WiFi.status() == WL_CONNECTED) {
//    setLedState(getStatusLed(boardType), On);
//    if (timeStatus() == timeNotSet) {
//      Serial.println(WiFi.localIP());
//      delay(250);
//      setSyncProvider(ntpUnixTime);
//      setSyncInterval(60 * 60);
//    }
//    publishIoTHub();    
//  }
//  else {
//    setLedState(getStatusLed(boardType), Off);
//    initWifi();
//  }
//
//  displayReadings();
//}
//
//void initDevices(){
//  if (displayMode == LedMatrix) { MatrixInit(); }  // initialise the led matrix display
//  
//  switch(sensorMode){
//    case Bmp180Mode:
//      bmp.begin();
//      break;
//    case Dht11Mode:
//      dht.begin();
//      break;
//  }
//}
//
//void publishIoTHub(){
//  unsigned long currentTime = millis(); 
//  if (cloud.lastPublishTime + (publishRateInSeconds * 1000) < currentTime) {  // publish rate no more than every 20 secs so not to consume IoT Hub 8k message/day free subscription limit
//    cloud.lastPublishTime = currentTime;
//    publishData(data, cloud.geo, getStatusLed(boardType));
//  }
//}
//
//void getWeatherReadings() {
//  switch(sensorMode){
//    case DemoMode:
//      data.temperature = 25;
//      data.humidity = 50;
//      data.pressure = 1000;
//      break;
//    case Bmp180Mode:
//      data.temperature = bmp.readTemperature(); 
//      data.humidity = 0;
//      data.pressure = (int)((int)( bmp.readPressure() + 0.5) / 100);
//      break;
//    case Dht11Mode:
//      float temperature = dht.readTemperature() - 2;  // -2 modest recallibrate
//      float humidity = dht.readHumidity();
//      data.pressure = 0;
//      if (isnan(temperature) || isnan(humidity)) {
//            Serial.println("Failed to read from DHT sensor!");
//      }
//      else {
//        data.temperature = temperature;
//        data.humidity = humidity;
//      }
//      break;
//  }
//}
//
//void displayReadings(){   
//  switch(displayMode){
//    case NoDisplay:
//      delay(publishRateInSeconds * 1000); // compensate for no delay from scrolling display
//      break;
//    case LedMatrix:
//      int roundedTemp = (int)(data.temperature + 0.5);  
//      ScrollString(" " + String(roundedTemp) + "C " + String(data.pressure) + "hPa", 71);
//      break;   
//  }
//}
//
//void getLightReading() {
//  if (sensorMode == DemoMode) { return; }
//  
//  int r = analogRead(A0);
//  data.light = (int)((float)r / 10.24f);  // convert to a percentage
//  
//  setDisplayBrightness((byte)data.light);
//}
//
//void setDisplayBrightness(byte lvl) {
//  if (displayMode != LedMatrix) { return; } 
//  lvl = (lvl % 100) / 15;
//  SetBrightness(lvl);
//}
