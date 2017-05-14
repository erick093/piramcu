
//internet.h#include <SoftwareSerial.h>
#include "arduino.h"
#include <PubSubClient.h>
#include <ESP8266WiFi.h>


//#define DEBUG FALSE //comment out to remove debug msgs


#define SSID "chuchusmote"
#define PASS "44599544459954"
#define TOPIC "panels1/pyranometer"

#define MQTT_SERVER "10.0.0.50" //IP Addres of MQTT SERVER(Raspberry Pi)

//#define THINGSPEAK_URL "api.thingspeak.com"
//#define THINGSPEAK_IPADDRESS IPAddress(184,106,153,149)
//#define THINGSPEAK_PORT_NUMBER 80
// #define staticIP "10.0.0.40"
// #define gateway "10.0.0.2"
// #define subnet  "255.255.255.0"
IPAddress staticIP(10,0,0,41); //Static IP of NODEMCU (Node 2: Pyranometer)
IPAddress gateway(10,0,0,2);
IPAddress subnet(255,255,255,0);


WiFiClient wificlient;
PubSubClient client(wificlient);



String macToStr(const uint8_t* mac){

  String result;

  for (int i = 0; i < 6; ++i) {
    result += String(mac[i], 16);

    if (i < 5){
      result += ':';
    }
  }

  return result;
}

void  connectWiFi()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin ( SSID, PASS );
  WiFi.config(staticIP, gateway, subnet);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("Ready");
  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
}

void connectMQTT(){

 //MQTT Connection
   if(WiFi.status() == WL_CONNECTED){
  // Loop until we're reconnected to the MQTT server
    while (!client.connected()) {
      Serial.print("Attempting MQTT connection...");
      client.setServer(MQTT_SERVER,1883);
      // Generate client name based on MAC address and last 8 bits of microsecond counter
      String clientName;
      clientName += "esp8266-";
      uint8_t mac[6];
      WiFi.macAddress(mac);
      clientName += macToStr(mac);

      //if connected, subscribe to the topic(s) we want to be notified about
      if (client.connect((char*) clientName.c_str())) {
        Serial.println("\tMQTT Connected to broker");
        client.subscribe(TOPIC);
        
        //Serial.print("Topic is:");
        //Serial.println(TOPIC_V);
          // if (client.publish(TOPIC, "hello from NodeMCU")) {         
          //   Serial.println("Publish ok");
          // }
          // else {
          //   Serial.println("Publish failed");
          // }
      }

      //otherwise print failed for debugging
      else{
        Serial.print("failed, rc=");
        Serial.print(client.state());
        Serial.println(" try again in 5 seconds");
        // Serial.println("\t MQTT connection failed.");
        // Serial.println("Will reset and try again ... ");
        // abort();
      }
    }
  }
}
