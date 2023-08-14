#ifndef FREED_H
#define FREED_H

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

class FreeD {
public:
    FreeD(const char* ssid, const char* password, int udpPort);
    void begin();
    void sendPacket(float rotation[3], float location[3], int zoom, int focus);
	unsigned char ID;

private:
    WiFiUDP _udp;
    const char* _ssid;
    const char* _password;
    int _udpPort;
    unsigned char _buf[29];

    void pack_be24(unsigned char *buf, long r);
    void pack_be24_15(unsigned char *buf, double d);
    void pack_be24_6(unsigned char *buf, double d);
    void check_sum();
};

#endif