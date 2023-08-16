//Work in Progress

#include "FreeD.h"

FreeD::FreeD(byte mac[], IPAddress ip, IPAddress gateway, IPAddress subnet, unsigned int udpPort) : _ip(ip), _gateway(gateway), _subnet(subnet), _udpPort(udpPort) {
    memcpy(_mac, mac, sizeof(_mac));
}

void FreeD::begin() {
    Serial.begin(115200);

    Ethernet.begin(_mac, _ip, _gateway, _subnet);
    Serial.print("Local IP: ");
    Serial.println(Ethernet.localIP());

    while (!_udp.begin(_udpPort)) {
        delay(250);
    }
}

void FreeD::sendPacket(float rotation[3], float location[3], int zoom, int focus) {
    _buf[0] = 0xD1;
    _buf[1] = ID;

    pack_be24_15(_buf + 2, rotation[1]);
    pack_be24_15(_buf + 5, rotation[0]);
    pack_be24_15(_buf + 8, rotation[2]);

    pack_be24_6(_buf + 11, location[0]);
    pack_be24_6(_buf + 14, location[2]);
    pack_be24_6(_buf + 17, location[1]);

    pack_be24(_buf + 20, zoom);
    pack_be24(_buf + 23, focus);

    _buf[26] = 0;
    _buf[27] = 0;

    check_sum();

    IPAddress recipientIP(192, 168, 3, 255); // broadcast
    int port = 7005;
    _udp.beginPacket(recipientIP, port);
    _udp.write(_buf, 29);
    _udp.endPacket();
}

void FreeD::parsePacket() {
    int packetSize = _udp.parsePacket();
    if (packetSize) {
        Serial.printf("Received %d bytes from %s, port %d\n", packetSize, _udp.remoteIP().toString().c_str(), _udp.remotePort());

        int len = _udp.read(_buf, sizeof(_buf));

        if (len > 0) {
            _buf[len] = 0;
            decodePacket(_buf, len);
        }
    }
}

void FreeD::decodePacket(unsigned char* packet, int len) {
    if (packet[0] == 0xD1) {
        Serial.print("Camera ID: ");
        Serial.println(packet[1]);

        float rotation[3], location[3];
        rotation[1] = unpack_be24_15(packet + 2);
        rotation[0] = unpack_be24_15(packet + 5);
        rotation[2] = unpack_be24_15(packet + 8);

        location[0] = unpack_be24_6(packet + 11);
        location[2] = unpack_be24_6(packet + 14);
        location[1] = unpack_be24_6(packet + 17);

        Serial.print("Rotation (Tilt, Pan, Roll): ");
        Serial.printf("%.2f, %.2f, %.2f\n", rotation[1], rotation[0], rotation[2]);
        Serial.print("Location (X, Z, Y): ");
        Serial.printf("%.2f, %.2f, %.2f\n", location[0], location[2], location[1]);

        int zoom = unpack_be24(packet + 20);
        int focus = unpack_be24(packet + 23);

        Serial.print("Zoom: ");
        Serial.println(zoom);
        Serial.print("Focus: ");
        Serial.println(focus);
    }
}

