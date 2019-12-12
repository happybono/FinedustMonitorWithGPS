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
String s_map_x, s_map_y;

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
  if (wifi_ready){
#ifdef PLAIVE_SERVER_ENABLE
    do_server_plaive(api_key,int(pm25s.getMedian()), int(pm10s.getMedian()), get_temperature(), s_map_x, s_map_y);
#else
  #ifdef THINGSPEAK_SERVER_ENABLE
    do_server_thingspeak(api_key,int(pm25s.getMedian()), int(pm10s.getMedian()), get_temperature(), s_map_x, s_map_y);;
  #else
    do_server_default(api_key,int(pm25s.getMedian()), int(pm10s.getMedian()), get_temperature(), s_map_x, s_map_y);
  #endif
#endif
  } 
                                                       //wifi is ok 
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
  Serial.println("\nDust Sensor Box V1.2, 2019/11/24 HappyBono");
}

//아두이노가 반복적으로 작동하는 부분 (Where Arduino works repeatedly.)
void loop() {
    if(ss.available()<=0){
    Serial.println("SIGNAL STATUS : WEAK");
    s_map_x = String(map_x,6);
    s_map_y = String(map_y,6);
  }
  else{
    while(ss.available()>0){
      Serial.println("SIGNAL STATUS : GREAT");
      if(gps.encode(ss.read())){
        Serial.println("GPS READ");
        Serial.println(ss.read());
        if (gps.location.isValid()){
          Serial.println("LOCATION : GREAT"); 
          map_x = gps.location.lat(); 
          map_y = gps.location.lng();
         Serial.println(String(map_x,6));
         Serial.println(String(map_y,6));
         Serial.println(gps.satellites.value());
        }
      }
      s_map_x = String(map_x,6);
      s_map_y = String(map_y,6);
      yield(); 
    }
  }
  while (dust.available() > 0) {
    do_dust(dust.read(), got_dust);
    yield();                                          //loop 에서 while 문을 사용하는 경우 yield 를 포함해주어야 합니다.
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
