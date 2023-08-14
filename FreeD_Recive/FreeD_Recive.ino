#include <ESP8266WiFi.h>  
#include <WiFiUdp.h>
#include <SPI.h>
#include "FreeD.h"

const char* ssid = "MGX";
const char* password = "MGS444TD";
int udpPort = 7005;

FreeD freeD(ssid, password, udpPort);

void setup() {
  freeD.begin();
}

void loop() {
 
  freeD.parsePacket(); // Gelen paketleri yakalamak ve çözmek için bu fonksiyonu çağırın.

  delay(20);
}
