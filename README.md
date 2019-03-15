# esp8266-actuators
This is the code I used for ESP8266 with Arduino IDE to control two actuators using a 4 relay board with an ESP-01 module.

This code can be used (with some minor modifications) to integrate the actuators control into a Home Assistant instance over MQTT protocol.

The relay board used can be found online in places like Aliexpress for approx 7 USD, for instance https://www.aliexpress.com/item/DC-12V-ESP8266-Four-Channel-Relay-Module-ESP-01-WIFI-Module-for-Smart-Home-Intelligent-Furniture/ . 

![relays](relee.jpg?raw=true "Title")

The specifics of this board include an ESP-01 module and a and high performance microprocessor STM8S103. The ESP-01 module has only 2 GPIO ports available by default and can not control 4 relays by its own - thats why the STM8S103 is used. The connection between ESP-01 and STM8S103 is a serial link, and certain sequence of bytes will trigger the relays ON or OFF.

Two relays will be used per actuator, to switch power polarity and extend or retract the actuator. The benefit of this setup is that when both relays are OFF, the actuator will be decoupled from the power supply and there will be no standby consumption and you will obtain a brake effect. My code will cut the power of the actuators after two minutes of innactivity - you can modify this to fit your setup, just remember to define a longer time than is needed for a complete extend or retract (time) for the actuators.

 
![relays_conn](relays_connection.jpg?raw=true "Title")


A sample configuration file "relee.yaml" for Home Assistant is provided, you have to keep in mind to configure a MQTT broker like Mosquitto and integrate it with Home Assistant server before.


![relays_act](actuators.jpg?raw=true "Title")

