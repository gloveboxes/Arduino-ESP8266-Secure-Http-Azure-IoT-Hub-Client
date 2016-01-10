# Temperature, Pressure and Light Sensor Streaming over HTTPS/REST to Azure IoT Hub or Azure Event Hub

## Platform

This project implemented on the NodeMCU V2.0 (also known as V1.0) on the ESP8266, ESP-12E chip using the Arduino Firmware

## Schema

Streams data in the following JSON formats


1. Azure IoT Hub over HTTPS/REST

    {"Dev":"DeviceId","Geo":"2011","Celsius":27,"hPa":1016,"Light":99,"Utc":"2015-12-06T23:07:04","Id":103}
    
2. Azure Event Hub over HTTPS/REST

    {"Dev":"DeviceId","Geo":"2011","Celsius":27,"hPa":1016,"Light":99,"Utc":"2015-12-06T23:07:04","Id":103}

##Physical Board

1. [NodeMCU v2 - Lua based ESP8266 development kit](http://tronixlabs.com/wireless/esp8266/nodemcu-v2-lua-based-esp8266-development-kit)
2. [BMP180 Barometric Pressure Sensor](http://tronixlabs.com/sensors/altitude/bmp180-barometric-pressure-sensor-board/)
3. [Adafruit Mini 8x8 LED Matrix w/I2C Backpack](http://tronixlabs.com/display/led/matrix/adafruit-mini-8x8-led-matrix-w-i2c-backpack-red-australia/)
4. 1 x [Light Dependent Resistor](http://tronixlabs.com/sensors/light/ldr/light-dependent-resistor/)
5. 1 x 10k resistor

![schematic](https://raw.githubusercontent.com/gloveboxes/Arduino-NodeMCU-ESP8266-Secure-Azure-IoT-Hub-Client/master/IoTHubClient/Fritzing/NodeMCU%20MQTT%20Board_bb.jpg)

    
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



##Project Files

1. SetEEPROMConfiguration.ino.  
Sets 
  * Wi-Fi SSID and password, up to three Wi-Fi/Password pairs must be specified, just repeat Wi-Fi access points if less than three, put in priority order.
  * Device id, Azure IoT Hub Host name, Key, and geo location.  
  * Deploy this app to the NodeMCU to write configuration settings to EPROM
2. IoTHubClient.ino - This is the main application, it will read configuration setting from the EPROM, manages sensor readings and publishing to Azure IoT hub (Event Hub is also supported).
