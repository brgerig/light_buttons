#include <Arduino.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>

#include "config.h"

#define numBtns 8

void ICACHE_RAM_ATTR b1Pressed(){pressButton(0);}
void ICACHE_RAM_ATTR b2Pressed(){pressButton(1);}
void ICACHE_RAM_ATTR b3Pressed(){pressButton(2);}
void ICACHE_RAM_ATTR b4Pressed(){pressButton(3);}
void ICACHE_RAM_ATTR b5Pressed(){pressButton(4);}
void ICACHE_RAM_ATTR b6Pressed(){pressButton(5);}
void ICACHE_RAM_ATTR b7Pressed(){pressButton(6);}
void ICACHE_RAM_ATTR b8Pressed(){pressButton(7);}

int btns[numBtns] = {D7,3,D5,D6,D2,D1,D4,D3};
bool btnPressed[numBtns];
unsigned long lastPress[numBtns];
//char* colors[numBtns] = {"off","white","red","yellow","green","blue","darkviolet","fuchsia"};
int colors[numBtns][2] = {
  {0,0},  //off
  {0.323, 0.329}, //white
  {0.701, 0.299}, //red
  {0.444, 0.517}, //yellow
  {0.172, 0.747}, //green
  {0.136, 0.040}, //blue
  {0.283, 0.108}, //darkviolet
  {0.385, 0.155}, //fuchsia
}

void (*btnFunctions[numBtns])() {b1Pressed,b2Pressed,b3Pressed,b4Pressed,b5Pressed,b6Pressed,b7Pressed,b8Pressed};

WiFiClient wifiClient;
PubSubClient client(wifiClient);

void wifi_reconnect() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASSWORD);
  //WiFi.begin(SSID);
  Serial.print("Connecting...");

  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(9600);

  for(int i=0;i<numBtns;i++) {
    btnPressed[i] = false;
    lastPress[i] = 0;
    pinMode(btns[i], INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(btns[i]), btnFunctions[i], FALLING);
  }

  wifi_reconnect();
  client.setServer(MQTT_SERVER, 1883);
}

void loop() {
  if(WiFi.status() != WL_CONNECTED) {  
    wifi_reconnect();
  }

  for(int i=0;i<numBtns;i++) {
    if(btnPressed[i]) {
      btnPressed[btn] = false;
      if(i==0) {
        lightOff();
      } else {
        setLight(color[i][0],color[i][1]);
    }
  }
}

void pressButton(int btn) {
  unsigned long interrupt_time = millis();
  if(interrupt_time - lastPress[btn] > 200) {
    Serial.print("Button pressed: ");
    Serial.println(btn);
    btnPressed[btn] = true;
  }
  lastPress[btn] = interrupt_time;
}

void setLight(int x, int y) {
  String put_string;
  put_string = "{\"on\":true,\"bri\":254,\"xy\":[";
  put_string += x;
  put_string += ",";
  put_string += y;
  put_string += "]}";

  sendCommand(put_string);
}

void lightOff() {
  sendCommand("{\"on\":false}");
}

void sendCommand(String cmd) {
  HTTPClient http; 
  String req_string;
  req_string = "http://";
  req_string += HUE_IP;
  req_string += "/api/";
  req_string += HUE_USER;
  req_string += "/lights/";
  req_string += HUE_LIGHT;
  req_string += "/state";
  Serial.println(req_string);
  http.begin(req_string);
  http.addHeader("Content-Type", "text/plain");
  
  Serial.println(cmd);
  int httpResponseCode = http.PUT(cmd);
  
  if(httpResponseCode > 0){
    String response = http.getString();   
    Serial.println(httpResponseCode);
    Serial.println(response);          
   } else {
    Serial.print("Error on sending PUT Request: ");
    Serial.println(httpResponseCode);
   }
   http.end();
}
