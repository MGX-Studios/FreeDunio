
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
  
  freeD.ID = 82;                 // CameraID
  float rotation[3] = {0, 0, 0}; // Rotation values (x, y, z) Tilt , Pan , Roll
  float location[3] = {0, 0, 0}; // Location values (x, y, z) 1000=1 Meter
  int zoom = 5000;               //Zoom Raw Value
  int focus = 500000;            //Focus Raw Value

  freeD.sendPacket(rotation, location, zoom, focus);
  delay(20);
}
