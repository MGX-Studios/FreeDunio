#ifndef FREED_H
#define FREED_H

#include <Ethernet.h>
#include <EthernetUdp.h>

class FreeD {
public:
    FreeD(IPAddress deviceIP, IPAddress gateway, IPAddress subnet, IPAddress serverIP, int udpPort);
    void begin();
    void sendPacket(float rotation[3], float location[3], int zoom, int focus);
    void receivePacket();
    void parsePacket();
    unsigned char ID;

private:
    EthernetUDP _udp;
    IPAddress _deviceIP;
    IPAddress _gateway;
    IPAddress _subnet;
    IPAddress _serverIP;
    int _udpPort;
    unsigned char _buf[29];
    
    void pack_be24(unsigned char* buf, long r);
    void pack_be24_15(unsigned char* buf, double d);
    void pack_be24_6(unsigned char* buf, double d);
    void check_sum();

    void decodePacket(unsigned char* packet, int len);
    double unpack_be24_15(unsigned char* buf);
    double unpack_be24_6(unsigned char* buf);
    long unpack_be24(unsigned char* buf);
	long toTwosComplement24Bit(double value);
};

#endif
