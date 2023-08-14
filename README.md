# **FreeD Library**

This library is the FreeD library, a real-time camera tracking protocol that can be used on popular platforms like Atmega328p, ESP8266, and ESP32. With this library, you can transmit position data of your tracking devices or robotics using the FreeD protocol over UDP. These FreeD packets can be read seamlessly by software like Disguise D3 and Unreal Engine. Furthermore, you can intercept and open the FreeD packets from your camera tracking device transmitting through Mosys Stype or another FreeD protocol.

Note: The library includes Wifi and Udp.

## **Usage**

Example to intercept and open FreeD Packets:

```cpp
#include "FreeD.h"

const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";
int udpPort = 7500;

FreeD freed(ssid, password, udpPort);

void setup() {
    freed.begin();
}

void loop() {
    freed.parsePacket();
}
```

Example to send a FreeD Packet:

```cpp
#include <ESP8266WiFi.h>  
#include <WiFiUdp.h>
#include <SPI.h>
#include "FreeD.h"

const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";
int udpPort = 7005;

FreeD freeD(ssid, password, udpPort);

void setup() {
  freeD.begin();
}

void loop() {
  freeD.ID = 82;                 // CameraID
  float rotation[3] = {0, 0, 0}; // Rotation values (x, y, z) Tilt, Pan, Roll
  float location[3] = {0, 0, 0}; // Location values (x, y, z) 1000 = 1 Meter
  int zoom = 5000;               // Zoom Raw Value
  int focus = 500000;            // Focus Raw Value

  freeD.sendPacket(rotation, location, zoom, focus);

  delay(20); //20ms = 50Fps
}
```

## **API**

### **`FreeD(const char* ssid, const char* password, int udpPort)`**

**`ssid`**: SSID of the WiFi network to connect to.

**`password`**: Password of the WiFi network to connect to.

**`udpPort`**: UDP port number to be used to connect to the FreeD system.

### **`void begin()`**

Connects to the WiFi network and initializes the UDP connection.

### **`void sendPacket(float rotation[3], float location[3], int zoom, int focus)`**

Sends a packet containing 3D rotation, location, zoom, and focus data.

### **`void parsePacket()`**

Listens to packets incoming from the FreeD system, processes them appropriately, and then prints them on the Serial Screen.

## **License**

This library is licensed under the license. For more information, please refer to the LICENSE file.

## **Contribution**

If you would like to contribute to this library, please visit our GitHub page and submit a "pull request".
