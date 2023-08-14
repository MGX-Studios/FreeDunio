#include "FreeD.h"

const char* ssid = "MGX";
const char* password = "MGS444TD";
int udpPort = 7005;

FreeD freeD(ssid, password, udpPort);

void setup() {
  freeD.begin();
}

void loop() {
  float rotation[3] = {1, 2, 3}; // Rotation values (x, y, z) 2 3 1
  float location[3] = {40000, 50000, 6000}; // Location values (x, y, z) 0,005 0,006 0,004
  int zoom = 50;
  int focus = 25;

  freeD.sendPacket(rotation, location, zoom, focus);
  delay(20);
}
