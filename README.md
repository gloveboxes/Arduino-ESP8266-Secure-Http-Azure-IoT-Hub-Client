# Temperature, Pressure and Light Sensor Streaming over HTTPS to Azure IoT Hub or Event Hubs

## Platform

This project implemented on the [NodeMCU V2 (also known as V1.0)](https://en.wikipedia.org/wiki/NodeMCU) and 
[WeMos D1 Mini](http://www.wemos.cc/wiki/doku.php?id=en:d1_mini) development boards on the [ESP8266](https://en.wikipedia.org/wiki/ESP8266) platform flashed with
[Arduino core for ESP8266 WiFi chip](https://github.com/esp8266/Arduino) V2.0 firmware.  The solution can stream data securely directly to 
[Azure IoT Hub](https://azure.microsoft.com/en-us/documentation/articles/iot-hub-what-is-iot-hub/) or [Azure Event Hub](https://azure.microsoft.com/en-us/services/event-hubs/) over HTTPS calling Azure REST APIs.  




The ESP8266 is a great commodity priced platform that has really come to life with Arduino support.
With "Arduino core for ESP8266 WiFi chip" Version 2.0 there is now Secure Client HTTPS (TLS) support making this a viable platform for secure IoT data streaming.

Thanks to [Štěpán Bechynský](https://microsoft.hackster.io/en-US/stepanb) "[Proof of Concept – NodeMCU, Arduino and 
Azure Event Hub](https://microsoft.hackster.io/en-US/stepanb/proof-of-concept-nodemcu-arduino-and-azure-event-hub-a33043)" project 
I've migrated my "[Arduino NodeMCU ESP8266 MQTT](https://github.com/gloveboxes/Arduino-NodeMCU-ESP82886-Mqtt-Client)" project and added IoT Hub support 
to stream data directly to Azure IoT Hub or Azure Event Hubs over HTTPS.

Check out the [Publish.ino](https://raw.githubusercontent.com/gloveboxes/Arduino-NodeMCU-ESP8266-Secure-Azure-IoT-Hub-Client/master/AzureClient/Publish.ino) sketch, the main bit of magic is to create the [Azure IoT Hub Shared Access Signature](https://azure.microsoft.com/en-us/documentation/articles/iot-hub-devguide/#security) from the [Azure IoT Hub Developer Guide](https://azure.microsoft.com/en-us/documentation/articles/iot-hub-devguide/). See function createIotHubSas.

    String createIotHubSas(char *key, String url){  
        String stringToSign = url + "\n" + expire;

        // START: Create signature
        // https://raw.githubusercontent.com/adamvr/arduino-base64/master/examples/base64/base64.ino
        
        int keyLength = strlen(key);
        
        int decodedKeyLength = base64_dec_len(key, keyLength);
        char decodedKey[decodedKeyLength];  //allocate char array big enough for the base64 decoded key
        
        base64_decode(decodedKey, key, keyLength);  //decode key
        
        Sha256.initHmac((const uint8_t*)decodedKey, decodedKeyLength);
        Sha256.print(stringToSign);  
        char* sign = (char*) Sha256.resultHmac();
        // END: Create signature
        
        // START: Get base64 of signature
        int encodedSignLen = base64_enc_len(HASH_LENGTH);
        char encodedSign[encodedSignLen];
        base64_encode(encodedSign, sign, HASH_LENGTH); 
        
        // SharedAccessSignature
        return "sr=" + url + "&sig="+ urlEncode(encodedSign) + "&se=" + expire;
        // END: create SAS  
    }

##Overview of steps to deploying the solution

1. Setup your Azure IoT Hub. There is a free 8000 message a day subscription to get started.
2. Register your device with Azure IoT Hub.
3. Flash the EEPROM with Wifi and Geolocation data.
4. Update the main AzureClient.ino sketch
    * Add your Azure IoT Hub or Event Hub connection string.
    * Define what platform you are targeting - NodeMCU or WeMos.
    * What sensors you are using, if any - options are DemoMode, bmp180 or dht11 sensors.
5. Deploy the solution to either your NodeMCU or WeMos devices.


##Azure IoT Hub Setup


[Creating an Azure IoT Hub](https://azure.microsoft.com/en-us/documentation/articles/iot-hub-csharp-csharp-getstarted/) (there is a free 8000 message/day limited subscription)

##Register your Device with IoT Hub

Register Devices for your newly created IoT Hub. 

* Use the Device Explorer utility from the [Azure IoT SDKs](https://github.com/Azure/azure-iot-sdks) Tools directory  

![DeviceExplorer](https://raw.githubusercontent.com/gloveboxes/Arduino-NodeMCU-ESP8266-Secure-Azure-IoT-Hub-Client/master/AzureClient/Fritzing/DeviceExplorerUI.jpg)

* or you can create your own utility by following the instructions in the [Create an Azure IoT Hub](https://azure.microsoft.com/en-us/documentation/articles/iot-hub-csharp-csharp-getstarted/) link.


##NodeMCU or WeMos EEPROM Configuration

Before uploading the Azure.ino sketch to your NodeMCU or WeMos you first need to configure the SetEEPROMConfiguration.ino sketch with one or more 
Wifi SSIDs/Passwords and the device geo location. Upload this sketch to burn these settings to the device EEPROM. 
After this you deploy the AzureClient sketch which will read this configuration information from the EEPROM.


1. SetEEPROMConfiguration.ino sets the following 
  * Wi-Fi SSID and password pairs, put in priority order.
  * Wifi - Is the number of WiFi ssid and password pairs
  * Azure IoT Hub or Event Bus Host name eg "MakerDen.azure-devices.net", Device ID, and Key.  For IoT Hub get this information from the Device Explorer, for Event Hub, get from Azure Management Portal.
  * Geo location of the device
  * Deploy this app to the NodeMCU to write configuration settings to EEPROM

##Configure the main AzureClient.ino solution 

You need to configure the following settings in the main section on the AzureClient.ino file.

    // cloud configurations
    CloudMode cloudMode = IoTHub;         // ClodeMode enumeration: IoTHub or EventHub

    // device configuration
    BoardType boardType = WeMos;          // BoardType enumeration: NodeMCU or WeMos
    SensorMode sensorMode = DemoMode;    // OperationMode enumeration: DemoMode (if no sensors attached), Bmp180Mode (bmp085 and bmp180), Dht11Mode
    DisplayMode displayMode = NoDisplay;  // DisplayMode enumeration: NoDisplay or LedMatrix
    LightSensor lightSensor = None;       // LightSensor enumeration: None, Enabled



## Data Schema

The AzureClient sketch streams data in the following JSON format, of course you can change this:)


    {"Dev":"DeviceId","Geo":"2011","Celsius":27,"hPa":1016,"Humidity":50,"Light":99,"Utc":"2015-12-06T23:07:04","Id":103}

##Azure IoT Hub and Azure Event Hub

Ok, so you've read this far and you may be wondering what is Azure IoT Hub and Azure Event Hub.  

IoT Hub is designed to "Connect, monitor, and control millions of IoT assets", Azure Event Hubs is 
designed for internet scale data ingestion. Unlock the value of that data with [Stream Analytics](https://azure.microsoft.com/en-us/services/stream-analytics/), 
[Power Bi](https://powerbi.microsoft.com/en-us/) and preconfigured IoT Hub solutions such as 
[Remote monitoring ](https://azure.microsoft.com/en-us/documentation/articles/iot-suite-remote-monitoring-sample-walkthrough).

##Physical Board


There are a number of ESP8266 based development boards available so be sure to check out this great article 
["Comparison of ESP8266 NodeMCU development boards"](http://frightanic.com/iot/comparison-of-esp8266-nodemcu-development-boards/).

The two dev boards that captured my interest are the NodeMCU V2 and the WeMos D1 Mini and this project supports both.

###NodeMCU V2 Hardware

1. [NodeMCU v2 - Lua based ESP8266 development kit](http://tronixlabs.com/wireless/esp8266/nodemcu-v2-lua-based-esp8266-development-kit)
2. [BMP180 Barometric Pressure Sensor](http://tronixlabs.com/sensors/altitude/bmp180-barometric-pressure-sensor-board/)
3. [Adafruit Mini 8x8 LED Matrix w/I2C Backpack](http://tronixlabs.com/display/led/matrix/adafruit-mini-8x8-led-matrix-w-i2c-backpack-red-australia/)
4. 1 x [Light Dependent Resistor](http://tronixlabs.com/sensors/light/ldr/light-dependent-resistor/)
5. 1 x 10k resistor
6. 1 x [400 Tie Point Interlocking Solderless Breadboard](http://tronixlabs.com/nodebots/400-tie-point-interlocking-solderless-breadboard-australia/)

![schematic](https://raw.githubusercontent.com/gloveboxes/Arduino-NodeMCU-ESP8266-Secure-Azure-IoT-Hub-Client/master/AzureClient/Fritzing/NodeMCU%20MQTT%20Board_bb.jpg)


###WeMos D1 Mini Hardware

1. [WeMos D1 Mini](http://www.wemos.cc/wiki/doku.php?id=en:d1_mini#getting_started)
2. [DHT Shield](http://www.wemos.cc/wiki/doku.php?id=en:dht) or the [DHT Pro Shield](http://www.wemos.cc/wiki/doku.php?id=en:dht_pro).

No wiring required, just solder the supplied header pins for the WeMos and the DHT Sensor shield.

![WeMos D1 Mini](http://www.wemos.cc/wiki/lib/exe/fetch.php?cache=&media=en:d1_mini_esp8266ex_fixled.jpg)


    
## Software Requirements

###Drivers

1. NodeMCU - On Windows, Mac and Linux you will need to install the latest [CP210x USB to UART Bridge VCP Drivers](https://www.silabs.com/products/mcu/Pages/USBtoUARTBridgeVCPDrivers.aspx).
2. WeMos - On Windows and Mac install the latest [Ch340G drivers](http://www.wemos.cc/wiki/doku.php?id=en:ch340g). No drivers required for Linux.

### Arduino IDE

1. [Arduino IDE 1.6.5](https://www.arduino.cc/en/Main/Software) As at Dec, 2015, [Arduino 1.6.6 has several issues, so to stick with 1.6.5](http://esp8266.github.io/Arduino/versions/2.0.0/doc/installing.html)
2. As at Dec 2015, ESP8266 Board Manager 2.0.0 or better required for HTTPS/TLS Secure Client support.

### Visual Studio

There an fantastic plugin for Visual Studio that adds Arduino support from [Visual Micro](http://www.visualmicro.com/).  IntelliSence, auto complete, debugging, it doesn't get much better:)

###Installing ESP8266 support with the Arduino Boards Manager

Starting with 1.6.4, Arduino allows installation of third-party platform packages using Boards Manager. We have packages available for Windows, Mac OS, and Linux (32 and 64 bit).

1. Install Arduino 1.6.5 from the Arduino website.
2. Start Arduino and open Preferences window.
3. Enter  http://arduino.esp8266.com/stable/package_esp8266com_index.json  into Additional Board Manager URLs field. You can add multiple URLs, separating them with commas.
4. Open Boards Manager from Tools > Board menu and install esp8266 platform (and don't forget to select your ESP8266 board from Tools > Board menu after installation).
5. Select NodeMUC or WeMos D1 Mini Board: Tools -> Board -> NodeMCU 1.0 (ESP-12E module) or WeMos D1 Mini
6. Set Port and Upload Speed: Tools.  Note, you may need to try different port speeds to successfully flash the device. Faster is better as each time you upload the code to your device you are re-flashing the complete ROM not just your code.

##ESP8266 Arduino Core Documentation 

Be sure to read the [ESP8266 Arduino Core Documentation](http://esp8266.github.io/Arduino/versions/2.0.0/) - there are some minor gotchas.
