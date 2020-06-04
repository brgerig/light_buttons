Code for a simple wireless control device for RGB lights via MQTT and Home Assistant. See: https://imgur.com/a/lUKORLU

Parts list:
* 1x ESP8266 NodeMCU 1.1
* 2x Adafruit 1x4 keypads (https://www.adafruit.com/product/1332)
* 10 jumper wires
* USB cable

Instructions:

Connect the common wires on the keypads to ground pins. Connect the button pins to pins D1-D7, and RX. (D8 is unusable due to lack of an internal pull-up resistor.)

In the Arduino IDE, install the ESP8266WiFi and PubSubClient libraries.
Change the variables in config.h, and if desired, change the colors in the color array.
