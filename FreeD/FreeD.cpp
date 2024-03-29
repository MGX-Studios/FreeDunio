#include "FreeD.h"

FreeD::FreeD(const char* ssid, const char* password, int udpPort) : _ssid(ssid), _password(password), _udpPort(udpPort) {}

void FreeD::begin() {
    Serial.begin(115200);
    WiFi.begin(_ssid, _password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

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
















void FreeD::receivePacket() {
    int packetSize = _udp.parsePacket();
    if (packetSize) {
        _udp.read(_buf, packetSize);
        if (_buf[0] == 0xD1 && _buf[1] == ID) { // check if the received packet is a valid FreeD packet
            Serial.print("Rotation (x, y, z): ");
            Serial.print(unpack_be24_15(_buf + 2));
            Serial.print(", ");
            Serial.print(unpack_be24_15(_buf + 5));
            Serial.print(", ");
            Serial.println(unpack_be24_15(_buf + 8));
            Serial.print("Location (x, y, z): ");
            Serial.print(unpack_be24_6(_buf + 11));
            Serial.print(", ");
            Serial.print(unpack_be24_6(_buf + 14));
            Serial.print(", ");
            Serial.println(unpack_be24_6(_buf + 17));
            Serial.print("Zoom: ");
            Serial.println(unpack_be24(_buf + 20));
            Serial.print("Focus: ");
            Serial.println(unpack_be24(_buf + 23));
        }
    }
}

void FreeD::pack_be24(unsigned char* buf, long r) {
    buf[2] = r & 0x00FF;
    r >>= 8;
    buf[1] = r & 0x00FF;
    r >>= 8;
    buf[0] = r & 0x00FF;
}

void FreeD::pack_be24_15(unsigned char* buf, double d) {
    pack_be24(buf, d * 32768.0);
}

void FreeD::pack_be24_6(unsigned char* buf, double d) {
    pack_be24(buf, d * 64.0);
}

void FreeD::check_sum() {
    _buf[28] = 0x40;
    for (int i = 0; i < 28; i++) {
        _buf[28] = _buf[28] - _buf[i];
    }
}



//unpacks




long FreeD::unpack_be24(unsigned char* buf) {
    long value = 0;
    value |= buf[0] << 16;
    value |= buf[1] << 8;
    value |= buf[2];
    return value;
}

double FreeD::unpack_be24_6(unsigned char* buf) {
    long r = unpack_be24(buf);
    if (r & 0x00800000) {
        r |= 0xFF000000;
    }
    return r / 64.0;
}

double FreeD::unpack_be24_15(unsigned char* buf) {
    long r = unpack_be24(buf);
    if (r & 0x00800000) {
        r |= 0xFF000000;
    }
    return r / 32768.0;
}





