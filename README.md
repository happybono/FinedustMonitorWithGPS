# FinedustMonitorWithGPS
Finedust monitoring device powered by NodeMCU with the location tracking capability.
  
![Final Product](20191126_082913.jpg)

## Specifications
### Scenarios
* Measuring the finedust contamination level in the air in a real time and display on the OLED Screen. 
* Measuring the current temperature.
* Track & update devices' location in a realtime.
* Refining data and synchronize with the web-based cloud service. (e.g. ThingSpeak, Plaive) 

### Connections
* 1 x Micro-USB

### Wireless
* IEEE 802.11 b/g/n Wi-Fi technology.

### Battery life 
* Up to 16 hours of typical device usage.

## Apparatus (Equipments)
### Platform
* ESP8266 NodeMCU

### SDS011 Dust Sensor
* Soldering required.
* D1 : TX of SDS011

### 0.96" I2C OLED Display 
* Soldering required.
* D3 : Data, D2 : Clock

### GY-NEO6MV2 GPS Sensor
* Soldering required.
* D6 & D7 : TX & RX of GY-NEO6MV2 respectively.

### DS18820 Temperature Sensor (Thermometer)
* Soldering required.
* D4 : Data

### KOKIRI A-PACK FIXIE 5 (KP-LS50) Portable Battery
* Micro-USB : Power
* Dimension : W (62.3 mm) × D (112.0 mm) × H (13.0 mm)
* Input : DC-5V / 2A
* Output : DC-5V / 2.1A
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

## What's New
### December 16 2019
[Read GPS data from ThingSpeak and displays it on the Google Maps using ESP12e NodeMCU and GPSNeo6M Module.](https://github.com/happybono/FinedustMonitorWithGPS/blob/master/Maps/GoogleMaps.html "FinedustMonitorWithGPS/Maps/GoogleMaps.html")


