# esp8266-actuators
This is a sample code I used for ESP8266 with Arduino IDE to control two actuators using a 4 relay board with an ESP-01 module

This code can be used (with some minor modifications) to integrate the actuators control into a Home Assistant instance over MQTT protocol.

The relay board used can be found online in places like Aliexpress for approx 7 USD, for instance https://www.aliexpress.com/item/DC-12V-ESP8266-Four-Channel-Relay-Module-ESP-01-WIFI-Module-for-Smart-Home-Intelligent-Furniture/ . The specifics of this board include an ESP-01 module and a and high performance microprocessor STM8S103. The ESP-01 module has only 2 GPIO ports available by default and can not control 4 relays by its own - thats why the STM8S103 is used. The connection between ESP-01 and STM8S103 is a serial link, and certain sequence of bytes will trigger the relays ON or OFF.

