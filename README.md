# FinedustMonitorWithGPS
Finedust (air pollution) monitoring station. Air pollution monitoring channel updates once per every minutes with PM10.0, PM2.5 readings from Nova PM (SDS011) Sensor. Also includes real time location tracking capabilities with using the GY-NEO6MV2 GPS Sensor.
 
>**[Please note]** <br>
This project requires a GY-NEO6MV2 GPS Sensor. For a project that does not require a GPS module, Please visit the following link : https://github.com/happybono/FinedustMonitor
 
![Final Product](20191126_082913.jpg)

## What's New
### December 16, 2019
[Read GPS data from ThingSpeak and displays it on the Google Maps using ESP12e NodeMCU and GPSNeo6MV2 Module.](https://github.com/happybono/FinedustMonitorWithGPS/blob/master/Maps/GoogleMaps.html "FinedustMonitorWithGPS/Maps/GoogleMaps.html")

### December 25, 2019
[Now supports [Channel Status Updates] in ThingSpeak](https://thingspeak.com/channels/920137) : Updated [FinedustMonitorWithGPS.ino](https://github.com/happybono/FinedustMonitorWithGPS/commit/01367946fd2dc10b39b39dec19309892311e92ea "/FinedustMonitorWithGPS/FinedustMonitorWithGPS.ino"), [server.ino](https://github.com/happybono/FinedustMonitorWithGPS/commit/3c3a89e9d8a4e45f591379dc96e2e7b67e15914d "/FinedustMonitorWithGPS/server.ino")

### December 29, 2019
[Various bugs fixed.](https://github.com/happybono/FinedustMonitorWithGPS/commit/d6d25d7cf4dc4b91f174259ea1e473220ab79385)

## Specifications
### Scenarios
* Measuring the finedust contamination level in the air in a real time and display on the OLED Screen. 
* Measuring the current temperature.
* Track & update devices' location in a realtime.
* Read GPS data (longitude, latitude) from [ThingSpeak](https://thingspeak.com/channels/920137) and displays it on the [Google Maps](https://www.google.com/maps/) using the [Javascript API](https://developers.google.com/maps/documentation/javascript/tutorial).
* Refining data and synchronize with the web-based cloud service. (e.g. [ThingSpeak](https://www.thingspeak.com/), [Plaive](https://plaive.10make.com/)) 

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
* USB Port : Power
* Micro-USB : Charging Port
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
