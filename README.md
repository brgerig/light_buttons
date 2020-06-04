Code for a simple wireless control device for RGB lights via Hue Bridge. See: https://imgur.com/a/lUKORLU

Parts list:
* 1x ESP8266 NodeMCU 1.1
* 2x Adafruit 1x4 keypads (https://www.adafruit.com/product/1332)
* 10 jumper wires
* USB cable

Instructions:

Connect the common wires on the keypads to ground pins. Connect the button pins to pins D1-D7, and RX. (D8 is unusable due to lack of an internal pull-up resistor.)

In the Arduino IDE, make sure the ESP8266WiFi is installed.
Change the variables in config.h, and if desired, change the colors in the color array.

For instructions on creating the API user on your Hue bridge, see: https://www.makeuseof.com/tag/control-philips-hue-lights-arduino-and-motion-sensor/
