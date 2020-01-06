//
//    FILE: FinedustMonitorWithGPS.ino
//  AUTHOR: Jaewoong Mun (happybono@outlook.com)
// CREATED: November 19, 2019
//
// Released to the public domain
//

#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include "RunningMedian.h"
 
RunningMedian pm25s = RunningMedian(19);
RunningMedian pm10s = RunningMedian(19);

char* ssid = "";   
char* password = "";    
String api_key = "";
//#define PLAIVE_SERVER_ENABLE
#define THINGSPEAK_SERVER_ENABLE

boolean wifi_ready;
float map_x, map_y;
String s_map_x, s_map_y, status;

TinyGPSPlus gps;
SoftwareSerial ss(12, 13);
SoftwareSerial dust(D1, D0, false, 256);

void got_dust(int pm25, int pm10) {                  //formula for dust sensor just use!!
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
    do_server_thingspeak(api_key, int(pm25s.getMedian()), int(pm10s.getMedian()), get_temperature(), s_map_x, s_map_y, status);
#else
    do_server_default(api_key, int(pm25s.getMedian()), int(pm10s.getMedian()), get_temperature(), s_map_x, s_map_y);
#endif
#endif
  }                                                    //wifi is ok
}

unsigned long mark = 0;
boolean got_interval = false;

//초기 세팅 (Initialize.)
void setup() {
  Serial.begin(115200);
  dust.begin(9600);
  ss.begin(9600);
  setup_oled();//oled setting
  wifi_ready = connect_ap(ssid, password);             //wifi 연결 여부 확인 (checking wifi connection.)

  if (!wifi_ready) nowifi_oled();                      //wifi no connection
  delay(5000);
  Serial.println("\nFinedust Sensor Box V1.3, 2019/12/25 HappyBono");
}

int pm25i, pm10i;
//아두이노가 반복적으로 작동하는 부분 (Where Arduino works repeatedly.)
void loop() {
  if (ss.available() <= 0) {
    Serial.println("SIGNAL STATUS : WEAK");
    s_map_x = String(map_x, 6);
    s_map_y = String(map_y, 6);
  }
  else {
    while (ss.available() > 0) {
      Serial.println("SIGNAL STATUS : GREAT");
      if (gps.encode(ss.read())) {
        Serial.println("GPS READ");
        Serial.println(ss.read());
        if (gps.location.isValid()) {
          Serial.println("LOCATION : GREAT");
          map_x = gps.location.lat();
          map_y = gps.location.lng();
          Serial.println(String(map_x, 6));
          Serial.println(String(map_y, 6));
          Serial.println(gps.satellites.value());
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

   
    /* AQI (실시간 대기질 지수) 등급 분류를 위한 코드입니다.
       실시간 대기질 기준 수치는 국제 표준인 미국 대기질 수치 기준으로 계산하였습니다.
       https://airnow.gov/index.cfm?action=aqibasics.aqi */
   
    // 초미세먼지 AQI (실시간 대기질 지수) 등급을 분류합니다.
    if (8 >= int(pm25s.getMedian()) && int(pm25s.getMedian()) >= 0) {
      pm25i = 1;
    }
    else if (16 >= int(pm25s.getMedian()) && int(pm25s.getMedian()) >= 9) {
      pm25i = 2;
    }
    else if (26 >= int(pm25s.getMedian()) && int(pm25s.getMedian()) >= 17) {
      pm25i = 3;
    }
    else if (34 >= int(pm25s.getMedian()) && int(pm25s.getMedian()) >= 27) {
      pm25i = 4;
    }
    else if (43 >= int(pm25s.getMedian()) && int(pm25s.getMedian()) >= 35) {
      pm25i = 5;
    }
    else if (51 >= int(pm25s.getMedian()) && int(pm25s.getMedian()) >= 44) {
      pm25i = 6;
    }
    else if (60 >= int(pm25s.getMedian()) && int(pm25s.getMedian()) >= 52) {
      pm25i = 7;
    }
   
    // 미세먼지 AQI (실시간 대기질 지수) 등급을 분류합니다.
    if (8 >= int(pm10s.getMedian()) && int(pm10s.getMedian()) >= 0) {
      pm10i = 1;
    }
    else if (16 >= int(pm10s.getMedian()) && int(pm10s.getMedian()) >= 9) {
      pm10i = 2;
    }
    else if (51 >= int(pm10s.getMedian()) && int(pm10s.getMedian()) >= 17) {
      pm10i = 3;
    }
    else if (68 >= int(pm10s.getMedian()) && int(pm10s.getMedian()) >= 52) {
      pm10i = 4;
    }
    else if (84 >= int(pm10s.getMedian()) && int(pm10s.getMedian()) >= 69) {
      pm10i = 5;
    }
    else if (101 >= int(pm10s.getMedian()) && int(pm10s.getMedian()) >= 85) {
      pm10i = 6;
    }
    else if (120 >= int(pm10s.getMedian()) && int(pm10s.getMedian()) >= 102) {
      pm10i = 7;
    }

   
     /* ThingSpeak 채널 내 Status Update (상태 업데이트) 영역에 표시되는 문구이므로, 
        종합적인 정보 표현을 위해 초미세먼지와 미세먼지 등급을 비교 한 후
        두 가지 중 높은 등급 기준으로 경고 혹은 권고메시지를 표시하도록 구현하였습니다. */
   
     // 분류된 초미세먼지 등급이 미세먼지 등급보다 높거나 같은 경우 아래와 같은 문구를 변수에 저장합니다.
     if (pm25i >= pm10i) {
      if (pm25i == 1) {
        status = "Excellent (1) : The air quality is excellent. The air pollution pose no threat. The conditions ideal for outdoor activities.";
      }

      else if (pm25i == 2) {
        status = "Very Good (2) : Air quality is very good, and air pollution poses little or no risk. Conditions very good for outdoor activities.";
      }

      else if (pm25i == 3) {
        status = "Moderate (3) : Air quality is acceptable. however, for some pollutants there may be a moderate health concern for a very small number of people who are unusually sensitive to air pollution.";
      }

      else if (pm25i == 4) {
        status = "Satisfactory (4) : Members of sensitive groups may experience health effects, Other people should limit spending time outdoors, especially when they experience symptoms such as cough or sore throat.";
      }

      else if (pm25i == 5) {
        status = "Bad (5) : Everyone may begin to experience health effects, members of sensitive groups may experience more serious health effects. People at risk should avoid to go outside. Not recommended for outdoor activities.";
      }

      else if (pm25i == 6) {
        status = "Severe (6) : Air quality is severe. Everyone may experience more serious health effects. People at risk should be avoided to go outside and should limit the outdoor activities to minimum. Outdoor activities are discouraged.";
      }

      else if (pm25i == 7) {
        status = "Hazardous (7) : Health warnings of emergency conditions. People at risk should be avoided to go outside and should limit the outdoor activities to minimum. Outdoor activities are strongly discouraged.";
      }      
      
     // 분류된 미세먼지 등급이 초미세먼지 등급보다 높은 경우 아래와 같은 문구를 변수에 저장합니다.
    } else if (pm25i < pm10i) {
      if (pm10i == 1) {
        status = "Excellent (1) : The air quality is excellent. The air pollution pose no threat. The conditions ideal for outdoor activities.";
      }
      
      else if (pm10i == 2) {
        status = "Very Good (2) : Air quality is very good, and air pollution poses little or no risk. Conditions very good for outdoor activities";
      }

      else if (pm10i == 3) {
        status = "Moderate (3) :  Air quality is acceptable. however, for some pollutants there may be a moderate health concern for a very small number of people who are unusually sensitive to air pollution.";
      }

      else if (pm10i == 4) {
        status = "Satisfactory (4) : Members of sensitive groups may experience health effects, Other people should limit spending time outdoors, especially when they experience symptoms such as cough or sore throat.";
      }

      else if (pm10i == 5) {
        status = "Bad (5) : Everyone may begin to experience health effects, members of sensitive groups may experience more serious health effects. People at risk should avoid to go outside. Not recommended for outdoor activities.";
      }

      else if (pm10i == 6) {
        status = "Severe (6) : Air quality is severe. Everyone may experience more serious health effects. People at risk should be avoided to go outside and should limit the outdoor activities to minimum. Outdoor activities are discouraged.";
      }

      else if (pm10i == 7) {
        status = "Hazardous (7) : Health warnings of emergency conditions. People at risk should be avoided to go outside and should limit the outdoor activities to minimum. Outdoor activities are strongly discouraged.";
      }
    }
  }

  //Serial.println(map_x);
  //Serial.print("pm 25 : ");
  //Serial.println(int(pm25s.getMedian()));

  if (millis() > mark) {//one minute(60000) interval
    mark = millis() + 60000;
    got_interval = true;
  }

  if (got_interval) {
    got_interval = false;
    do_interval();
  }
  yield();
}
