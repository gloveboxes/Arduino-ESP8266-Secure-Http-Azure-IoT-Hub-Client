# Temperature, Pressure and Light Sensor Streaming over HTTPS to Azure IoT Hub or Event Hubs

## Platform

This project implemented on the [NodeMCU V2 (also known as V1.0)](https://en.wikipedia.org/wiki/NodeMCU) and 
[WeMos D1 Mini](http://www.wemos.cc/wiki/doku.php?id=en:d1_mini) on the [ESP8266](https://en.wikipedia.org/wiki/ESP8266) platform running the
[Arduino core for ESP8266 WiFi chip](https://github.com/esp8266/Arduino) V2.0 firmware and can stream data securely directly to Azure IoT Hub or Azure Event Hub over HTTPS/REST.


The ESP8266 is a great commodity priced platform that has really come to life with Arduino support.
With "Arduino core for ESP8266 WiFi chip" Version 2.0 there is now Secure Client HTTPS (TLS) support making this a viable platform for secure IoT data streaming.

Thanks to [Štěpán Bechynský](https://microsoft.hackster.io/en-US/stepanb) "[Proof of Concept – NodeMCU, Arduino and 
Azure Event Hub](https://microsoft.hackster.io/en-US/stepanb/proof-of-concept-nodemcu-arduino-and-azure-event-hub-a33043)" project 
I've migrated my "[Arduino NodeMCU ESP8266 MQTT](https://github.com/gloveboxes/Arduino-NodeMCU-ESP82886-Mqtt-Client)" project and added IoT Hub support 
to stream data directly to Azure IoT Hub or Azure Event Hubs over HTTPS.

Check out the [Publish.ino](https://raw.githubusercontent.com/gloveboxes/Arduino-NodeMCU-ESP8266-Secure-Azure-IoT-Hub-Client/master/AzureClient/Publish.ino) sketch, the main bit of magic is to create the [Azure IoT Hub Shared Access Signature](https://azure.microsoft.com/en-us/documentation/articles/iot-hub-devguide/#security) from the [Azure IoT Hub Developer Guide](https://azure.microsoft.com/en-us/documentation/articles/iot-hub-devguide/). See function createIoTHubSas.

    String createIoTHubSas(char *key, String url){  
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

##Steps to deploying the solution

1. Setup your Azure IoT Hub. There is a free 8000 message a day subscription to get started.
2. Register your device with Azure IoT Hub.
3. Flash the EEPROM with Wifi and Geolocation data.
4. Update the main AzureClient.ino file with your Azure IoT Hub or Event Hub connection string.
    * Define what platform you are targeting - NodeMCU or WeMos.
    * What sensors you are using, if any - options are DemoMode, bmp180 or dht11 sensors.
5. Deploy the solution to either your NodeMCU or WeMos devices.


##Azure IoT Hub Setup

These are the basic steps to setting to running this project

[Create an Azure IoT Hub](https://azure.microsoft.com/en-us/documentation/articles/iot-hub-csharp-csharp-getstarted/) (there is a free 8000 message/day limited subscription)

##Register your Device with IoT Hub

2. Register Devices for your newly created IoT Hub. 

    1. Use the Device Explorer utility from the [Azure IoT SDKs](https://github.com/Azure/azure-iot-sdks) Tools directory  
    2. or you can create your own utility by following the instructions in the [Create an Azure IoT Hub](https://azure.microsoft.com/en-us/documentation/articles/iot-hub-csharp-csharp-getstarted/) link.


##NodeMCU ESP8266 EEPROM Configuration

Before uploading the Azure.ino sketch you first need to configure the SetEEPROMConfiguration.ino sketch with one or more Wifi SSIDs/Passwords, the Azure IoT Hub or Event Hub Host Name, the device Id (or policy id for Event Hubs), and the key. Upload this sketch to burn these settings to the device EEPROM. After this you deploy the AzureClient sketch which will read this configuration information from the EEPROM.


1. SetEEPROMConfiguration.ino sets the following 
  * Wi-Fi SSID and password pairs, put in priority order.
  * Device id, Azure IoT Hub Host name, Key, and geo location.  
  * Deploy this app to the NodeMCU to write configuration settings to EPROM

##Configure the main AzureClient.ino solution 



    // cloud configurations
    // connection string must be in this format and order
    String connectionString = "HostName=MakerDen.azure-devices.net;DeviceId=WeMos01;SharedAccessKey=phABCDEFTVcn0iF+G10veb0xGkohDavezLg/jFSRyX9s/g=";
    CloudMode cloudMode = IoTHub;         // ClodeMode enumeration: IoTHub or EventHub

    // device configuration
    BoardType boardType = WeMos;          // BoardType enumeration: NodeMCU or WeMos
    SensorMode sensorMode = DemoMode;    // OperationMode enumeration: DemoMode (if no sensors attached), Bmp180Mode (bmp085 and bmp180), Dht11Mode
    DisplayMode displayMode = NoDisplay;  // DisplayMode enumeration: NoDisplay or LedMatrix
    LightSensor lightSensor = None;       // LightSensor enumeration: None, Enabled



## Data Schema

The AzureClient sketch streams data in the following JSON format


    {"Dev":"DeviceId","Geo":"2011","Celsius":27,"hPa":1016,"Light":99,"Utc":"2015-12-06T23:07:04","Id":103}


##Physical Board

###NodeMCU Hardware

1. [NodeMCU v2 - Lua based ESP8266 development kit](http://tronixlabs.com/wireless/esp8266/nodemcu-v2-lua-based-esp8266-development-kit)
2. [BMP180 Barometric Pressure Sensor](http://tronixlabs.com/sensors/altitude/bmp180-barometric-pressure-sensor-board/)
3. [Adafruit Mini 8x8 LED Matrix w/I2C Backpack](http://tronixlabs.com/display/led/matrix/adafruit-mini-8x8-led-matrix-w-i2c-backpack-red-australia/)
4. 1 x [Light Dependent Resistor](http://tronixlabs.com/sensors/light/ldr/light-dependent-resistor/)
5. 1 x 10k resistor
6. 1 x [400 Tie Point Interlocking Solderless Breadboard](http://tronixlabs.com/nodebots/400-tie-point-interlocking-solderless-breadboard-australia/)


###WeMos Hardware

2. [WeMos D1 Mini](http://www.wemos.cc/wiki/doku.php?id=en:d1_mini#getting_started)
3. [DHT11 Shield](http://www.wemos.cc/wiki/doku.php?id=en:dht)


![schematic](https://raw.githubusercontent.com/gloveboxes/Arduino-NodeMCU-ESP8266-Secure-Azure-IoT-Hub-Client/master/AzureClient/Fritzing/NodeMCU%20MQTT%20Board_bb.jpg)

    
## Software Requirements

1. On Windows, Mac and Linux you will need to install the latest [CP210x USB to UART Bridge VCP Drivers](https://www.silabs.com/products/mcu/Pages/USBtoUARTBridgeVCPDrivers.aspx).
2. [Arduino IDE 1.6.5](https://www.arduino.cc/en/Main/Software) As at Dec, 2015, [Arduino 1.6.6 has several issues, so to stick with 1.6.5](http://esp8266.github.io/Arduino/versions/2.0.0/doc/installing.html)
3. As at Dec 2015, ESP8266 Board Manager 2.0.0 or better required for HTTPS/TLS Secure Client support.

##Arduino on ESP8266
[Arduino on ESP8266 Project](https://github.com/esp8266/Arduino)

Add NodeMCU to Arduino IDE

1. Add Additional Board Manager URLs: File -> Preferences.  Add http://arduino.esp8266.com/stable/package_esp8266com_index.json 
2. Restart Arduino IDE
3. Add ESP8266 Board: Tools -> Board -> Board Manager -> Search ESP8266 -> Install
4. Select NodeMUC Board: Tools -> Board -> NodeMCU 1.0 (ESP-12E module)
5. Set Port and Upload Speed: Tools.  Note, you may need to try different port speeds to sucessfully flash the device. Faster is better as each time you upload the code to the NodeMCU you are uploading the complete ROM not just your code.
