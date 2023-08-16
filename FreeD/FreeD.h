#ifndef FREED_H
#define FREED_H

#include <Ethernet.h>
#include <EthernetUdp.h>

class FreeD {
public:
    FreeD(byte mac[], IPAddress ip, IPAddress gateway, IPAddress subnet, unsigned int udpPort);
    void begin();
    void sendPacket(float rotation[3], float location[3], int zoom, int focus);
    void parsePacket();
    unsigned char ID;

private:
    EthernetUDP _udp;
    byte _mac[6];
    IPAddress _ip;
    IPAddress _gateway;
    IPAddress _subnet;
    unsigned int _udpPort;
    unsigned char _buf[29];

    void pack_be24(unsigned char* buf, long r);
    void pack_be24_15(unsigned char* buf, double d);
    void pack_be24_6(unsigned char* buf, double d);
    void check_sum();

    void decodePacket(unsigned char* packet, int len);
    double unpack_be24_15(unsigned char* buf);
    double unpack_be24_6(unsigned char* buf);
    long unpack_be24(unsigned char* buf);
};

#endif
