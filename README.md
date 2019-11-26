# FinedustMonitorWithGPS
* Fine dust monitoring device powered by NodeMCU with the location tracking capability.

## Platform
* ESP8266 NodeMCU

## SDS011 Dust Sensor
* Soldering required.
* D1 : TX of SDS011

## 0.96" OLED I2C
* Soldering required.
* D3 : Data, D2 : Clock

## GY-NEO6MV2 GPS Sensor
* Soldering required.
* D6 & D7 : TX & RX of GY-NEO6MV2 respectively.

## DS18820 Temperature Sensor (Thermometer)
* Soldering required.
* D4 : Data

## KOKIRI A-PACK FIXIE 5 (KP-LS50) Portable Battery
* Micro-USB : Power
* Dimension : W (62.3 mm) × D (112.0 mm) × H (13.0 mm)
* Capacity : 5000 mAh

## Data Refinement / Synchronization
### ThingSpeak
* ThingSpeak (https://thingspeak.com/channels/920137)
![ThingSpeak Screenshot](ThingSpeak.png)
  
### Plaive
* Plaive (https://plaive.10make.com/)
![Plaive Screenshot](Plaive.png)

## Drivers / References
Please install https://github.com/squix78/esp8266-oled-ssd1306
