/*
  Air Temperature and Pressure plus Light Sensor Streaming over HTTPS/REST to Azure IoT Hub or Azure Event Hub

  Describe what it does in layman's terms.  Refer to the components
  attached to the various pins.

  The circuit:
  
  NodeMCU v2 ESP8266 Development Board 
  http://tronixlabs.com/wireless/esp8266/nodemcu-v2-lua-based-esp8266-development-kit
  https://github.com/esp8266/Arduino




  Optionally
  BMP180 Barometric Pressure Sensor
  3.Adafruit Mini 8x8 LED Matrix w/I2C Backpack
  4.1 x Light Dependent Resistor
  5.1 x 10k resistor

  Circuit layout at https://github.com/gloveboxes/Arduino-NodeMCU-ESP8266-Secure-Azure-IoT-Hub-Client
  
  Created Jan 2016
  Dave Glover
  dglover@microsoft.com

*/

#include <Time.h>           // http://playground.arduino.cc/code/time - installed via library manager
#include <ArduinoJson.h>    // https://github.com/bblanchon/ArduinoJson
#include <ESP8266WiFi.h>
#include <Wire.h>
#include "Adafruit_BMP085.h"
#include "DHT.h"
#include "names.h"

// cloud configurations
CloudMode cloudMode = IoTHub;         // ClodeMode enumeration: IoTHub or EventHub

// device configuration
BoardType boardType = WeMos;          // BoardType enumeration: NodeMCU or WeMos
SensorMode sensorMode = DemoMode;    // OperationMode enumeration: DemoMode, Bmp180Mode, Dht11Mode
DisplayMode displayMode = NoDisplay;  // DisplayMode enumeration: NoDisplay or LedMatrix
LightSensor lightSensor = None;       // LightSensor enumeration: None, Enabled


CloudConfig cloud;
WiFiConfig wifiConfig;
SensorData data;


// sensors
Adafruit_BMP085 bmp;

const int DHTPIN = D4;     // DHT11 shield on pin D4
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);


void setup() {  
	Serial.begin(9600);
	delay(100);
	Serial.println();

	loadConfigFromEEPROM();
  
	initialiseAzure(cloudMode);
 
  initLed(StatusLed); 
  initDevices();
}

void loop() {  
  getLightReading();
  getWeatherReadings();
  
	if (WiFi.status() == WL_CONNECTED) {
    setStatusLed(On);
		if (timeStatus() == timeNotSet) {
      Serial.println(WiFi.localIP());
      delay(250);
			setSyncProvider(ntpUnixTime);
			setSyncInterval(60 * 60);
		}
		publishIoTHub();    
  }
	else {
    setStatusLed(Off);
    initWifi();
	}

  displayReadings();
}

void initDevices(){
  if (displayMode == LedMatrix) { MatrixInit(); }  // initialise the led matrix display
  
  switch(sensorMode){
    case Bmp180Mode:
      bmp.begin();
      break;
    case Dht11Mode:
      dht.begin();
      break;
  }
}

void publishIoTHub(){
  unsigned long currentTime = millis(); 
  if (cloud.lastPublishTime + 20000UL < currentTime) {  // publish rate no more than every 20 secs so not to consume IoT Hub 8k message/day free subscription limit
    cloud.lastPublishTime = currentTime;
    publishData(data, cloud.geo);
  }
}

void getWeatherReadings() {
  switch(sensorMode){
    case DemoMode:
      data.temperature = 25;
      data.humidity = 50;
      data.pressure = 1000;
      break;
    case Bmp180Mode:
      data.temperature = bmp.readTemperature(); 
      data.humidity = 0;
      data.pressure = (int)((int)( bmp.readPressure() + 0.5) / 100);
      break;
    case Dht11Mode:
      float temperature = dht.readTemperature() - 2;  // -2 modest recallibrate
      float humidity = dht.readHumidity();
      data.pressure = 0;
      if (isnan(temperature) || isnan(humidity)) {
            Serial.println("Failed to read from DHT sensor!");
      }
      else {
        data.temperature = temperature;
        data.humidity = humidity;
      }
      break;
  }
}

void displayReadings(){   
  switch(displayMode){
    case NoDisplay:
      delay(5000); // compensate for no delay from scrolling display
      break;
    case LedMatrix:
      int roundedTemp = (int)(data.temperature + 0.5);  
      ScrollString(" " + String(roundedTemp) + "C " + String(data.pressure) + "hPa", 71);
      break;   
  }
}

void getLightReading() {
  if (lightSensor == None) { return; }
  
  int r = analogRead(A0);
  data.light = (int)((float)r / 10.24f);  // convert to a percentage
  
  setDisplayBrightness((byte)data.light);
}

void setDisplayBrightness(byte lvl) {
  if (displayMode != LedMatrix) { return; } 
	lvl = (lvl % 100) / 15;
	SetBrightness(lvl);
}
