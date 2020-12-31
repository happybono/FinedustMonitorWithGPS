//
//    FILE: FinedustMonitorWithGPS.ino
//  AUTHOR: Jaewoong Mun (happybono@outlook.com)
// CREATED: November 19, 2019
//
// Released to the public domain
//

#include <TinyGPS++.h>
#include <SoftwareSerial.h>
TinyGPSPlus gps;
SoftwareSerial ss(12, 13);
SoftwareSerial dust(D1, D0, false, 256);            //RX, TX Communication
#include "RunningMedian.h"

RunningMedian pm25s = RunningMedian(19);
RunningMedian pm10s = RunningMedian(19);

char* ssid = "[Wi-Fi SSID]";
char* password = "[Wi-Fi Password]";
String api_key = "[ThingSpeak Write API Key]";

//#define PLAIVE_SERVER_ENABLE
#define THINGSPEAK_SERVER_ENABLE

boolean wifi_ready;
float map_x, map_y;
String s_map_x, s_map_y, status;
int pm25i, pm10i;

//초기 세팅 (Initialize.)
void setup() {
  Serial.begin(115200);
  dust.begin(9600);
  ss.begin(9600);
  setup_oled();
  wifi_ready = connect_ap(ssid, password);
  
  if (!wifi_ready) nowifi_oled();

  Serial.println("\nFinedust Sensor Box V1.3, 2019/12/25 HappyBono");
}


void got_dust(int pm25, int pm10) {                 //formula for dust sensor just use!!
  pm25 /= 10;
  pm10 /= 10;
  pm25s.add(pm25);
  pm10s.add(pm10);
  do_oled(pm25, pm10);                              //print pm25, pm10 in oled
}


//서버에 데이터 보내기 (Sending collected data to server.)
void do_interval() {
  if (wifi_ready) {
#ifdef PLAIVE_SERVER_ENABLE
    do_server_plaive(api_key, int(pm25s.getMedian()), int(pm10s.getMedian()), get_temperature(), s_map_x, s_map_y);
#else
#ifdef THINGSPEAK_SERVER_ENABLE
    Serial.println("dst: pm25=" + String(int(pm25s.getMedian())) + " / pm10=" + String(int(pm10s.getMedian())) + "/ s=" + String(status));
    do_server_thingspeak(api_key, int(pm25s.getMedian()), int(pm10s.getMedian()), get_temperature(), s_map_x, s_map_y, status);
#else
    do_server_default(api_key, int(pm25s.getMedian()), int(pm10s.getMedian()), get_temperature(), s_map_x, s_map_y);
#endif
#endif
  }                                                    //wifi is ok
}

unsigned long mark = 0;
boolean got_interval = false;

//아두이노가 반복적으로 작동하는 부분 (Where Arduino works repeatedly.)
void loop() {
  if (ss.available() <= 0) {
    // Serial.println("SIGNAL STATUS : WEAK");
    s_map_x = String(map_x, 6);
    s_map_y = String(map_y, 6);
  }
  else {
    while (ss.available() > 0) {
      //Serial.println("SIGNAL STATUS : GREAT");
      if (gps.encode(ss.read())) {
        //Serial.println("GPS READ");
        Serial.println(ss.read());
        if (gps.location.isValid()) {
          //Serial.println("LOCATION : GREAT");
          map_x = gps.location.lat();
          map_y = gps.location.lng();
          Serial.println(String(map_x, 6));
          Serial.println(String(map_y, 6));
        }
      }
      s_map_x = String(map_x, 6);
      s_map_y = String(map_y, 6);
      yield();
    }
  }
  while (dust.available() > 0) {
    do_dust(dust.read(), got_dust);
    yield();                                          //loop 에서 while 문을 사용하는 경우 yield 를 포함해주어야 합니다.

    //Serial.println(map_x);
    //Serial.print("pm 10 : ");
    //Serial.println(int(pm10s.getMedian()));

    /* AQI (실시간 대기질 지수) 등급 분류를 위한 코드입니다.
       실시간 대기질 기준 수치는 국제 표준인 WHO 대기질 수치 기준으로 계산하였습니다.

       http://www.euro.who.int/__data/assets/pdf_file/0005/78638/E90038.pdf
       https://airnow.gov/index.cfm?action=aqibasics.aqi */


    // 초미세먼지 AQI (실시간 대기질 지수) 등급을 분류합니다.
    //   0 이상   8 이하 : 1
    //   9 이상  16 이하 : 2
    //  17 이상  26 이하 : 3
    //  27 이상  34 이하 : 4
    //  35 이상  43 이하 : 5
    //  44 이상  51 이하 : 6
    //  52 이상  ∞  이하 : 7

    int pm25m = int(pm25s.getMedian());

    if (pm25m < 9) {
      pm25i = 1;
    } else if (pm25m < 17) {
      pm25i = 2;
    } else if (pm25m < 27) {
      pm25i = 3;
    } else if (pm25m < 35) {
      pm25i = 4;
    } else if (pm25m < 44) {
      pm25i = 5;
    } else if (pm25m < 52) {
      pm25i = 6;
    } else {
      pm25i = 7;
    }

    // 미세먼지 AQI (실시간 대기질 지수) 등급을 분류합니다.
    //   0 이상  16 이하 : 1
    //  16 이상  31 이하 : 2
    //  32 이상  51 이하 : 3
    //  52 이상  68 이하 : 4
    //  69 이상  84 이하 : 5
    //  85 이상 101 이하 : 6
    // 102 이상  ∞  이하 : 7

    int pm10m = int(pm10s.getMedian());

    if (pm10m < 16) {
      pm10i = 1;
    } else if (pm10m < 32) {
      pm10i = 2;
    } else if (pm10m < 52) {
      pm10i = 3;
    } else if (pm10m < 69) {
      pm10i = 4;
    } else if (pm10m < 85) {
      pm10i = 5;
    } else if (pm10m < 102) {
      pm10i = 6;
    } else {
      pm10i = 7;
    }
    
    /* ThingSpeak 채널 내 Status Update (상태 업데이트) 영역에 표시되는 문구이므로,
        종합적인 정보 표현을 위해 초미세먼지와 미세먼지 등급을 비교 한 후
        두 가지 중 높은 등급 기준으로 경고 혹은 권고메시지를 표시합니다. */

    // 가령, 분류된 초미세먼지 등급이 미세먼지 등급보다 같거나 높은 경우, 초미세먼지 등급을 기준으로 내용을 표시하기 위하여 아래의 문자열을 status 변수에 저장합니다.

    switch ((pm25i >= pm10i) ? pm25i : pm10i) {
      case 1:
        status = "Excellent (1) : The air quality is excellent. As air pollution poses no threat, conditions are ideal for outdoor activities.";
        break;

      case 2:
        status = "Very Good (2) : The air quality is very good. As air pollution poses little or no risk, conditions very good for outdoor activities.";
        break;

      case 3:
        status = "Moderate (3) : Air quality is acceptable. however, for some pollutants, there may be a moderate health concern for specific people who are unusually sensitive to air pollution.";
        break;

      case 4:
        status = "Satisfactory (4) : Members of sensitive groups may experience health effects, other people should limit spending time outdoors, especially when they experience symptoms such as cough or sore throat.";
        break;

      case 5:
        status = "Bad (5) : Everyone may begin to experience health effects, members of sensitive groups may experience more serious health effects. Not recommended for outdoor activities.";
        break;

      case 6:
        status = "Severe (6) : Everyone may experience more serious health effects. People at risk should be avoided to go outside and should limit outdoor activities to a minimum. Outdoor activities are discouraged.";
        break;

      case 7:
        status = "Hazardous (7) : People at risk should be avoided going outside and should limit outdoor activities to a minimum. Outdoor activities are strongly discouraged.";
        break;
    }

    //Serial.println("PM2.5 = " + String(int(pm25s.getMedian())) + " / " + String(pm25i));
    //Serial.println("PM10.0  = " + String(int(pm10s.getMedian())) + " / " + String(pm10i));
  }

  if (millis() > mark) {                          //one minute (60000) interval
    mark = millis() + 60000;
    got_interval = true;
  }

  if (got_interval) {
    got_interval = false;
    do_interval();
  }
  yield();
}
