#include <PubSubClient.h>

#include "wifimqtt.h"
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <ThingSpeak.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
extern "C" {
#include "user_interface.h"
}
#define VOLTAGE_MAX 2.991
#define VOLTAGE_MAXCOUNTS 1001

char result[8];
double _sens= 0.003174;
double _fvalue;
int n_muestras = 1000;

//int cont = 0 ;
const int sleepTimeS = 10; //Sleep time in seconds




void setup() {
  Serial.begin ( 115200 );

  //connectWiFi();
  //connectMQTT();
}



void loop() {

  delay(500);
  // if(WiFi.status() == 6 ){
  //   Serial.println("Reconnecting WiFi");
  //   connectWiFi();
  // }
  // if (!client.connected()){
  //   Serial.println("Reconnecting MQTT");
  //   connectMQTT();
  // }
  //client.loop();
  //Reading value from A0 (Nodemcu)
  double _read =0;
  double _value=0;
  for(int i=0;i<n_muestras;i++)
  {
    _read=analogRead(A0)*(VOLTAGE_MAX/ VOLTAGE_MAXCOUNTS);    
    _value=_value+_read/_sens;
  }
  _fvalue = _value/n_muestras;
  Serial.print("Sending Pyranometer value: ");
  Serial.println(_fvalue,5);
  dtostrf(_fvalue, 6, 5, result); //converting FLOAT to CHAR

  // if (client.publish(TOPIC, result)) {
  //   Serial.println("Publish value ok");
  // }
  // else {
  //   Serial.println("Publish value failed");
  // }
  
  //system_deep_sleep_set_option(0);
  //system_deep_sleep(sleepTimeS * 1000000);
}
