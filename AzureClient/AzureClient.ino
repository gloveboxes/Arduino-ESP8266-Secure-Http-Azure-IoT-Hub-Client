/*
  Air Temperature and Pressure plus Light Sensor Streaming over HTTPS/REST to Azure IoT Hub or Azure Event Hub

  Describe what it does in layman's terms.  Refer to the components
  attached to the various pins.

  The circuit:
  
  NodeMCU v2 ESP8266 Development Board 
  http://tronixlabs.com/wireless/esp8266/nodemcu-v2-lua-based-esp8266-development-kit
  https://github.com/esp8266/Arduino
  see pins_arduino.h in the \variants\nodemcu directory


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
#include <Adafruit_BMP085.h>
#include "names.h"


OperationMode opMode = SensorMode;  // OperationMode enumeration: DemoMode or SensorMode
CloudMode cloudMode = IoTHub;  // ClodeMode enumeration: IoTHub or EventHub

SensorData data;
CloudConfig cloud;
WiFiConfig wifiConfig;

Adafruit_BMP085 bmp;


void setup() {  
	Serial.begin(256000);
	delay(100);
	Serial.println();

  initLed(WifiConnectedLed);
  initLed(PublishingLed);

	loadConfigFromEEPROM();
  
	initialiseAzure(cloudMode);  
  
  if (opMode == SensorMode) {
  	MatrixInit();  // initialise the led matrix display
    bmp.begin();  //initialise the bmp180 temperature and pressure sensor
  }
}

void loop() {  
  getLightReading();
  getTempPressureReading();
  
	if (WiFi.status() == WL_CONNECTED) {
	  digitalWrite(WifiConnectedLed, LOW);
		if (timeStatus() == timeNotSet) {
      Serial.println(WiFi.localIP());
      delay(250);
			setSyncProvider(ntpUnixTime);
			setSyncInterval(60 * 60);
		}
		publishIoTHub();    
  }
	else {
    digitalWrite(WifiConnectedLed, HIGH);
    initWifi();
	}

  displayReadings();
}

void publishIoTHub(){
  unsigned long currentTime = millis(); 
  if (cloud.lastPublishTime + 20000UL < currentTime) {  // publish rate maximum of once every 20 secs so not to consume IoT Hub 8k message/day free subscription limit
    cloud.lastPublishTime = currentTime;
    publishData(data, cloud.geo);
  }
}

void getTempPressureReading() {
  if (opMode == DemoMode) {
    data.temperature = 25;
    data.pressure = 1000;
    return;
  }
  
  data.temperature = bmp.readTemperature(); 
  data.pressure = (int)((int)( bmp.readPressure() + 0.5) / 100);
}

void displayReadings(){   
  if (opMode == DemoMode) {
    delay(5000); // compensate for no delay from scrolling display
    return;
  }
   
  int roundedTemp = (int)(data.temperature + 0.5);  
  ScrollString(" " + String(roundedTemp) + "C " + String(data.pressure) + "hPa", 71); 
}

void getLightReading() {
  if (opMode == DemoMode) {
    data.light = 50;
    return;
  }
    
	int r = analogRead(A0);
	data.light = (int)((float)r / 10.24f);  // convert to a percentage

  setDisplayBrightness((byte)data.light);
}

void setDisplayBrightness(byte lvl) {
	lvl = (lvl % 100) / 15;
	SetBrightness(lvl);
}
