#include <PubSubClient.h>
#include <ESP8266WiFi.h>

bool stopac;
long lastmqttupdate = 0;
long updatet = 0;

//Edit this
const char* ssid     = "TP-LINK";
const char* password = "77777777";

//Hex command to send to serial for close relay
  byte rel1ON[]  = {0xA0, 0x01, 0x01, 0xA2, 0x0d, 0x0a};
  //Hex command to send to serial for open relay
  byte rel1OFF[] = {0xA0, 0x01, 0x00, 0xA1, 0x0d, 0x0a};
  
  //Hex command to send to serial for close relay
  byte rel2ON[]  = {0xA0, 0x02, 0x01, 0xA3, 0x0d, 0x0a};
  //Hex command to send to serial for open relay
  byte rel2OFF[] = {0xA0, 0x02, 0x00, 0xA2, 0x0d, 0x0a};
  
  byte rel3OFF[]  = {0xA0, 0x03, 0x00, 0xA3, 0x0d, 0x0a};
  //Hex command to send to serial for open relay
  byte rel3ON[] = {0xA0, 0x03, 0x01, 0xA4, 0x0d, 0x0a};
  byte rel4ON[]  = {0xA0, 0x04, 0x01, 0xA5, 0x0d, 0x0a};
  //Hex command to send to serial for open relay
  byte rel4OFF[] = {0xA0, 0x04, 0x00, 0xA4, 0x0d, 0x0a};
  
void callback(char* topic, byte* payload, unsigned int length);

//EDIT THESE LINES TO MATCH YOUR SETUP
#define MQTT_SERVER "vpn.safebyte.ro"
#define MQTT_USER "user"
#define MQTT_PASS "parola"
const int mqttPort = 1883;
char const* controlTopic1 = "/solar/AC1/";
char const* controlTopic2 = "/solar/AC2/";

WiFiClient espClient;
PubSubClient client(espClient);

void connect_to_MQTT() {
 
if (client.connect("solar-AC-control",MQTT_USER,MQTT_PASS)) {
    
  Serial.print("\tMQTT Connected");

        client.subscribe(controlTopic1);
        client.subscribe(controlTopic2);
        
        

  } else {
    Serial.println("Could not connect to MQTT");
  }  
  
  }
void setup() {
delay (10);
Serial.begin (115200);
Serial.println();
WiFi.mode(WIFI_STA);
Serial.println(WiFi.macAddress());
Serial.print("Connecting to ");
Serial.println(ssid);
WiFi.begin(ssid, password);
  
while (WiFi.status() != WL_CONNECTED) {
delay(500);
}
Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

client.setServer(MQTT_SERVER, mqttPort);
client.setCallback(callback);
stopac = false;
connect_to_MQTT();

  //wait a bit before starting the main loop
      delay(2000);
}


void loop(){


  
//maintain MQTT connection
  client.loop();
if (! client.connected()) {
    Serial.println("Not connected to MQTT....");
    connect_to_MQTT();
    delay(5000);
  }

updatet = millis();
if ( (updatet - lastmqttupdate > 120000) && (stopac) ) {
   Serial.print("\nPutem inchide ");
stopActuator1();
stopActuator2();
stopac = false;
}
  
}


void callback(char* topic, byte* payload, unsigned int length) {
  String topicStr = topic; 
  //Print out some debugging info
  Serial.println("Callback update.");
  Serial.print("Topic: ");
  Serial.println(topicStr);
  
   if (topicStr == "/solar/AC1/") 
    {

     //turn the switch on if the payload is '1' and publish to the MQTT server a confirmation message
     
     if(payload[0] == '1'){
       stopActuator1();
       extendActuator1();
       Serial.print("Extindem AC1: ");
       client.publish("/solar/AC1Confirm1/", "1");
       lastmqttupdate = millis();
       stopac = true;
       }

      //turn the switch off if the payload is '0' and publish to the MQTT server a confirmation message
     else if (payload[0] == '0'){
       stopActuator1();
       retractActuator1();
       client.publish("/solar/AC1Confirm1/", "0");
       lastmqttupdate = millis();
       stopac = true;
       }
     }

   
     else if (topicStr == "/solar/AC2/") 
     {
     //turn the switch on if the payload is '1' and publish to the MQTT server a confirmation message
     if(payload[0] == '1'){
       stopActuator2();
       extendActuator2();
       client.publish("/solar/AC2Confirm2/", "1");
       lastmqttupdate = millis();
       stopac = true;
       }

      //turn the switch off if the payload is '0' and publish to the MQTT server a confirmation message
     else if (payload[0] == '0'){
       stopActuator2();
       retractActuator2();
       client.publish("/solar/AC2Confirm2/", "0");
       lastmqttupdate = millis();
       stopac = true;
       }
     }

     
     }
 
/////////////////////////////////////////////////////////////



  
void stopActuator1() {
  Serial.write ("\n\r");
Serial.write (rel1OFF, sizeof(rel1OFF)); 
delay (100);
 Serial.write ("\n\r");
Serial.write (rel2OFF, sizeof(rel2OFF)); 
delay (100);
} 

void stopActuator2() {
  Serial.write ("\n\r");
Serial.write (rel3OFF, sizeof(rel3OFF)); 
delay (100);
 Serial.write ("\n\r");
Serial.write (rel4OFF, sizeof(rel4OFF));  
delay (100);
} 

 void extendActuator1() {
  Serial.write ("\n\r"); 
 Serial.write (rel1ON, sizeof(rel1ON)); 
 delay (1000);
  Serial.write ("\n\r"); 
 Serial.write (rel2OFF, sizeof(rel2OFF)); 
 delay (1000);
 } 

 void retractActuator1() { 
   Serial.write ("\n\r"); 
 Serial.write (rel1OFF, sizeof(rel1OFF)); 
 delay (1000);
  Serial.write ("\n\r"); 
 Serial.write (rel2ON, sizeof(rel2ON)); 
 delay (1000);
}

 void extendActuator2() {
   Serial.write ("\n\r"); 
 Serial.write (rel3ON, sizeof(rel3ON)); 
 delay (1000);
  Serial.write ("\n\r"); 
 Serial.write (rel4OFF, sizeof(rel4OFF)); 
 delay (1000);
 } 

 void retractActuator2() { 
   Serial.write ("\n\r"); 
 Serial.write (rel3OFF, sizeof(rel3OFF)); 
 delay (1000);
  Serial.write ("\n\r"); 
 Serial.write (rel4ON, sizeof(rel4ON));
 delay (1000); 
}

 