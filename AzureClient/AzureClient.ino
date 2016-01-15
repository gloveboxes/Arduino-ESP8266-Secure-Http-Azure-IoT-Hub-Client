/*
  README  

  PROJECT OWNER: Dave Glover | dglover@microsoft.com | @dglover

  COPYRIGHT: Free to use, reuse and modify, no libability accepted

  CREATED: Jan 2016

  SUMMARY: 
  This project was implemented and tested on the NodeMCU V2 (also known as V1.0) and WeMos D1 Mini development boards on the ESP8266 platform 
  flashed with Arduino core for ESP8266 WiFi chip V2.0 firmware. It should work in "DemoMode" across all ESP8266 Development boards.
  
  This solution streams sensor data securely over HTTPS / REST directly to Azure IoT Hub or Azure Event Hub. 
  
  The solution has two modes:-
  1) DemoMode, sensor data is faked, just upload sketch to a standalone ESP8266 Developer Board, no sensors required
  2) Or with sensors attached
      - Bmp180Mode: BMP085 or BMP180 Temperature (Centigrade) and Air Pressure
      - Dht11Mode: DHT11 Temperature and Humidity 
      - With an LDR attached, light will be measured levels 


  Platform: ESP8266

  NodeMCU: http://nodemcu.com/index_en.html#fr_54747661d775ef1a3600009e (This project will reflash the NodeMCU in to the Arduino platform. IMHO LUA not stable)  
  WeMod D1 Mini: http://www.wemos.cc/wiki/doku.php?id=en:d1_mini
  Other ESP8266 Development Boards - Adafruit, Sparkfun and others
  

  FIRMWARE: Arduino core for ESP8266 WiFi chip V2.0 | https://github.com/esp8266/Arduino    

  ARDUINO IDE: 4.6.5 (as at Jan 2016)
  
  - ESP8266 V2 requires Arduino IDE 4.6.5 (high versions will result is scope compiler errors
  - http://esp8266.github.io/Arduino/versions/2.0.0/doc/installing.html
  

  LIBRARIES: Install Arduino Libraries from Sketch -> Include Library -> Manage Libraries
  - ArduinoJson
  - Adafruit BMP085 (DON’T install the unified version)
  - Adafruit LED Backpack
  - Adafruit GFX
  - DHT (DON’T install the unified version)
  - Time
  
  As at Jan 2016 download Zip from github for the Adafruit LED Backpack and GFX libraries as there are issues with the online Library Manager versions
  Arduino IDE Sketch -> Include Library -> Add Zip Library 
  - Adafruit_LEDBackpack : https://github.com/adafruit/Adafruit-LED-Backpack-Library
  - Adafruit_GFX         : https://github.com/adafruit/Adafruit-GFX-Library
  

  SAMPLE CONFIGURATION:

  No sensors required, no display, creates fake temperature, air pressure and humidity
  
  // device configuration
  SensorMode sensorMode = DemoMode;    // OperationMode enumeration: DemoMode, Bmp180Mode, Dht11Mode
  DisplayMode displayMode = NoDisplay;  // DisplayMode enumeration: NoDisplay or LedMatrix 
  

  DRIVERS:
  
  - NodeMCU - On Windows, Mac and Linux you will need to install the latest CP210x USB to UART Bridge VCP Drivers. (https://www.silabs.com/products/mcu/Pages/USBtoUARTBridgeVCPDrivers.aspx)
  - WeMos - On Windows and Mac install the latest Ch340G drivers. No drivers required for Linux. (http://www.wemos.cc/wiki/doku.php?id=en:ch340g)


  ESP8266 ARDUINO IDE SUPPORT:

  1. Install Arduino 1.6.5 from the Arduino website.
  2. Start Arduino and open Preferences window.
  3. Enter http://arduino.esp8266.com/stable/package_esp8266com_index.json into Additional Board Manager URLs field. You can add multiple URLs, separating them with commas.
  4. RESTART Arduino IDE
  5. Open Boards Manager from Tools > Board -> Boards Manager. Search for ESP8266 and install.
  6. Select NodeMUC or WeMos D1 Mini Board: Tools -> Board  -> NodeMCU 1.0 (ESP-12E module) or WeMos D1 Mini
  7. Set Port and Upload Speed: Tools. Note, you may need to try different port speeds to successfully flash the device. 
     - Faster is better as each time you upload the code to your device you are re-flashing the complete ROM not just your code, but faster can be less reliable.
     

  OPTIONAL COMPONENTS:
  
  Circuit Layout at https://github.com/gloveboxes/Arduino-NodeMCU-ESP8266-Secure-Azure-IoT-Hub-Client
  
  - BMP180 Barometric Pressure Sensor
  - Adafruit Mini 8x8 LED Matrix w/I2C Backpack
  - Light Dependent Resistor
  - 10k resistor

  - WeMos Di Mini DHT(11) Shield: http://www.wemos.cc/wiki/doku.php?id=en:dht

*/

#include <Time.h>           // http://playground.arduino.cc/code/time - installed via library manager
#include <ArduinoJson.h>    // https://github.com/bblanchon/ArduinoJson
#include <ESP8266WiFi.h>
#include <Wire.h>
#include "Adafruit_BMP085.h"
#include "DHT.h"
#include "names.h"

// cloud configurations
CloudMode cloudMode = IoTHub;         // CloudMode enumeration: IoTHub or EventHub

// device configuration
SensorMode sensorMode = DemoMode;    // OperationMode enumeration: DemoMode (no sensors, fakes data), Bmp180Mode, Dht11Mode
DisplayMode displayMode = NoDisplay; // DisplayMode enumeration: NoDisplay or LedMatrix


CloudConfig cloud;
WiFiConfig wifiConfig;
SensorData data;


// sensors
Adafruit_BMP085 bmp;

const int DHTPIN = 2; // GPIO pin 2 = D4 alias on NodeMCU, using gpio pin rather than D4 alias so it compiles against all ESP8266 Development boards.
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
  if (sensorMode == DemoMode) { return; }
  
  int r = analogRead(A0);
  data.light = (int)((float)r / 10.24f);  // convert to a percentage
  
  setDisplayBrightness((byte)data.light);
}

void setDisplayBrightness(byte lvl) {
  if (displayMode != LedMatrix) { return; } 
	lvl = (lvl % 100) / 15;
	SetBrightness(lvl);
}
