//
//    FILE: server.ino
//  AUTHOR: Jaewoong Mun (happybono@outlook.com)
// CREATED: November 19, 2019
//
// Released to the public domain
//

const char* host_plaive = "data.plaive.10make.com";  
const char* host_thingspeak = "api.thingspeak.com";
const char* host_default = "finedustapi.10make.com";

const int httpPort = 80;

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
WiFiClient client;
String data;
String contentType;

void do_server_plaive(String api_key,int pm25, int pm10, float temperature, String map_x, String map_y) {

  data = "api_key="+ String(api_key) + "&field1=" + String(pm25) + "&field2=" + String(pm10) + "&field3=" + String(temperature) + "&field4=" + String(map_x) + "&field5=" + String(map_y);
  //contentType= "application/x-www-form-urlencoded";

  //서버 통신 공식 client.println 을 사용하여야 합니다.
  if(client.connect(host_plaive,httpPort)){
    Serial.println("connected");
    client.print("GET /insert.php?");
    client.print(data); 
    client.println(" HTTP/1.1");
    client.println("Host: " + String(host_plaive)); // SERVER ADDRESS HERE TOO
    client.println("Cache-Control: no-cache");
    //client.println("Content-Type: application/xE-www-form-urlencoded"); 
    //client.print("Content-Length: "); 
    //client.println(data.length()); 
    client.println("Connection: close");
    client.println(); 
    //client.print(data); 
  }
    
  //서버 통신이 되지 않으면
  else{
    Serial.println("connection failed: ");
    return;
  }
}

void do_server_thingspeak(String api_key,int pm25, int pm10, float temperature, String map_x, String map_y) {

  data = "api_key="+ String(api_key) + "&field1=" + String(pm25) + "&field2=" + String(pm10) + "&field3=" + String(temperature) + "&field4=" + String(map_x) + "&field5=" + String(map_y);
  //contentType= "application/x-www-form-urlencoded";

  //서버 통신 공식 client.println 을 사용하여야 합니다.
  if(client.connect(host_thingspeak,httpPort)){
    Serial.println("connected");
    client.print("GET /update?");
    client.print(data); 
    client.println(" HTTP/1.1");
    client.println("Host: " + String(host_thingspeak)); // SERVER ADDRESS HERE TOO
    client.println("Cache-Control: no-cache");
    
    //client.println("Content-Type: application/xE-www-form-urlencoded"); 
    //client.print("Content-Length: "); 
    //client.println(data.length()); 
    client.println("Connection: close");
    client.println();
     
    //client.print(data); 
  }
    
  //서버 통신이 되지 않으면
  else{
    Serial.println("connection failed: ");
    return;
  }
}

void do_server_default(String api_key,int pm25, int pm10, float temperature, String map_x, String map_y) {

  data = "api_key="+ String(api_key) + "&pm25=" + String(pm25) + "&pm10=" + String(pm10) + "&temp=" + String(temperature) + "&latitude" + String(map_x) + "&longitude" + String(map_y);
  contentType= "application/x-www-form-urlencoded";

  //서버 통신 공식 client.println 을 사용하여야 니다.
  if(client.connect(host_default,httpPort)){
    Serial.println("connected");
    client.print("GET /insert.php?");
    client.print(data); 
    client.println(" HTTP/1.1");
    client.println("Host: " + String(host_default)); // SERVER ADDRESS HERE TOO
    client.println("Content-Type: application/xE-www-form-urlencoded"); 
    client.print("Content-Length: "); 
    client.println(data.length()); 
    client.println("Connection: close");
    client.println(); 
    //client.print(data); 
  }
    
  //서버 통신이 되지 않으면
  else{
    Serial.println("Connection Failed: ");
    return;
  }
}
