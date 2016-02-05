/*
  README

  PROJECT OWNER: Dave Glover | dglover@microsoft.com | @dglover

  COPYRIGHT: Free to use, reuse and modify, no libability accepted

  CREATED: Jan 2016

  SUMMARY:
  This project was implemented and tested on the NodeMCU V2 (also known as V1.0), WeMos D1 Mini and Sparkfun ESP8266 Dev development boards on the ESP8266 platform
  flashed with Arduino core for ESP8266 WiFi chip V2.0 firmware.

  This solution streams sample sensor data securely over HTTPS / REST directly to Azure IoT Hub or Azure Event Hub.

  PLATFORM: ESP8266

  DEVELOPMENT BOARDS:
  NodeMCU: http://nodemcu.com/index_en.html#fr_54747661d775ef1a3600009e (This project will reflash the NodeMCU in to the Arduino platform. IMHO LUA not stable)
  WeMod D1 Mini: http://www.wemos.cc/wiki/doku.php?id=en:d1_mini
  Sparkfun 8266 Thing Dev Board: https://learn.sparkfun.com/tutorials/esp8266-thing-development-board-hookup-guide/introduction
  Other ESP8266 Development Boards - Adafruit, and others

  Sparkfun ESP8266 Dev Board Notes:

  With my limited experience with the Sparkfun ESP8266 Dev board I needed to use the Generic ESP8266 Arduino IDE Board Manager.
  See notes at https://learn.sparkfun.com/tutorials/esp8266-thing-development-board-hookup-guide/setting-up-arduino
  Change to Generic ESP8266 Module, and in the sub menus, make sure the following sub-menu's are also set:
	- Flash Mode: DIO
	- Flash Frequency: 80MHz
	- Upload Using: Serial
	- CPU Frequency: 80MHz
	- Flash Size: 512K (no SPIFFS)
	- Reset Method: nodemcu


  FIRMWARE: Arduino core for ESP8266 WiFi chip V2.0 | https://github.com/esp8266/Arduino

  ARDUINO IDE: 4.6.5 (as at Jan 2016)
  - ESP8266 V2 requires Arduino IDE 4.6.5 (high versions will result is scope compiler errors
  - http://esp8266.github.io/Arduino/versions/2.0.0/doc/installing.html


  REQUIRED LIBRARIES: Install Arduino Libraries from Sketch -> Include Library -> Manage Libraries
  - ArduinoJson
  - Time
  - Adafruit BMP085 (DON’T install the unified version)
  - DHT (DON’T install the unified version)


  CLOUD CONFIGURATION:

  The method initCloudConfig() called in setup has two signatures

  1. initCloudConfig() with no parameters reads prepopulated configuration information from the EEPROM
  2. initCloudConfig("IoT hub device connection string", "WiFi SSID (Case Sensitive)", "WiFi password", "Geo location of the device")

  eg initCloudConfig("HostName=YourIoTHub.azure-devices.net;DeviceId=DeviceID;SharedAccessKey=Device Key", "SSID", "Password", "Sydney");


  If loading from EEPROM then 
  - Open the SetEEPROMConfiguration.ino found in the SetEEPROMConfiguration folder and update the following variables
  - Wi-Fi SSID and password pairs, put in priority order.
  - Wifi - Is the number of WiFi ssid and password pairs
  - Azure IoT Hub or Event Bus Host name eg "MakerDen.azure-devices.net", Device ID, and Key. For IoT Hub get this information from the Device Explorer, for Event Hub, get from Azure Management Portal.
  - Geo location of the device
  - Deploy this app to the NodeMCU to write configuration settings to EEPROM


  DEVICE SAMPLE CONFIGURATION:

  // device configuration in this Arduino Sketch
  void initDeviceConfig(){
    device.boardType = Other;            // BoardType enumeration: NodeMCU, WeMos, SparkfunThing, Other (defaults to Other). This determines pin number of the onboard LED for wifi and publish status. Other means no LED status 
    device.deepSleepSeconds = 0;         // if greater than zero with call ESP8266 deep sleep (default is 0 disabled). GPIO16 needs to be tied to RST to wake from deepSleep. Causes a reset, execution restarts from beginning of sketch
    cloud.cloudMode = IoTHub;            // CloudMode enumeration: IoTHub and EventHub (default is IoTHub)
    cloud.publishRateInSeconds = 90;     // limits publishing rate to specified seconds (default is 90 seconds)
    cloud.sasExpiryDate = 1737504000;    // Expires Wed, 22 Jan 2025 00:00:00 GMT (defaults to Expires Wed, 22 Jan 2025 00:00:00 GMT)
  }


  DRIVERS:
  - NodeMCU - On Windows, Mac and Linux you will need to install the latest CP210x USB to UART Bridge VCP Drivers. (https://www.silabs.com/products/mcu/Pages/USBtoUARTBridgeVCPDrivers.aspx)
  - WeMos - On Windows and Mac install the latest Ch340G drivers. No drivers required for Linux. (http://www.wemos.cc/wiki/doku.php?id=en:ch340g)
  - Sparkfun ESP8266 Thing Dev - No additional drivers required


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

#include <Wire.h>
#include <ESP8266WiFi.h>
#include <Time.h>           // http://playground.arduino.cc/code/time - installed via library manager
#include <ArduinoJson.h>    // https://github.com/bblanchon/ArduinoJson - installed via library manager
#include "globals.h"        // global structures and enums used by the applocation

CloudConfig cloud;
DeviceConfig device;
SensorData data;

void initDeviceConfig() { // Example device configuration
  device.boardType = Other;            // BoardType enumeration: NodeMCU, WeMos, SparkfunThing, Other (defaults to Other). This determines pin number of the onboard LED for wifi and publish status. Other means no LED status 
  device.deepSleepSeconds = 0;         // if greater than zero with call ESP8266 deep sleep (default is 0 disabled). GPIO16 needs to be tied to RST to wake from deepSleep. Causes a reset, execution restarts from beginning of sketch
  cloud.cloudMode = IoTHub;            // CloudMode enumeration: IoTHub and EventHub (default is IoTHub)
  cloud.publishRateInSeconds = 60;     // limits publishing rate to specified seconds (default is 90 seconds)
  cloud.sasExpiryDate = 1737504000;    // Expires Wed, 22 Jan 2025 00:00:00 GMT (defaults to Expires Wed, 22 Jan 2025 00:00:00 GMT)
}



void setup() {
	Serial.begin(9600);
	delay(100);
	Serial.println("");

	initDeviceConfig();
	initCloudConfig("HostName=MakerDen.azure-devices.net;DeviceId=ESP8266;SharedAccessKey=xJFsb2qh3PG73h+1NNxYrCFBhO1iSHHVKB64+yn4ci4=", "NCW", "malolos5459", "Sydney");
}

void loop() {
  int ntpRetryCount = 0;

  measureSensor();

	if (WiFi.status() == WL_CONNECTED) {
		setLedState(On);
    
    ntpRetryCount = 0;
    while (timeStatus() == timeNotSet && ++ntpRetryCount < 10) { // get NTP time
      Serial.println(WiFi.localIP());
      delay(250);
      setSyncProvider(ntpUnixTime);
      setSyncInterval(60 * 60);  
      delay(250);
    }

    publish();  

    if (device.deepSleepSeconds > 0) {
      ESP.deepSleep(1000000 * device.deepSleepSeconds, WAKE_RF_DEFAULT); // GPIO16 needs to be tied to RST to wake from deepSleep. Execute restarts from beginning of sketch
    }
    else {
      delay(cloud.publishRateInSeconds * 1000);  // limit publishing rate
    }
	}
	else {
    setLedState(Off);
    initWifi();
	}
  delay(250);
}

void measureSensor(){  // uncomment sensor, default is getFakeWeatherReadings()
//  getFakeWeatherReadings();
  getDht11Readings();
//  getDht22Readings();
  getBmp180Readings();
//  getBmp280Readings();
}

void publish() {
	unsigned long currentTime = millis();
	if (device.deepSleepSeconds > 0 || cloud.lastPublishTime + (cloud.publishRateInSeconds * 1000) < currentTime) {  // publish rate no more than every 20 secs so not to consume IoT Hub 8k message/day free subscription limit
		cloud.lastPublishTime = currentTime;
    publishToAzure();
	}
}

void getFakeWeatherReadings() {
	data.temperature = 25;
	data.humidity = 50;
	data.pressure = 1000;
}
