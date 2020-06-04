#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

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
char* colors[numBtns] = {"off","white","red","yellow","green","blue","darkviolet","fuchsia"};
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

void mqtt_reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    client.setKeepAlive(60);
    if (client.connect(HOSTNAME,MQTT_USER,MQTT_PASS)) {
      Serial.println("connected");
      client.publish(AVAILABILITY_TOPIC, "online");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
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
  if(!client.connected()) {
    mqtt_reconnect();
  }

  for(int i=0;i<numBtns;i++) {
    if(btnPressed[i]) {
      sendColor(i);
    }
  }
}

void sendColor(int btn) {
  Serial.print("Color: ");
  Serial.println(colors[btn]);
  client.publish(STATE_TOPIC,colors[btn]);
  btnPressed[btn] = false;
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
