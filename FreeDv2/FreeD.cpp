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

    IPAddress recipientIP(192, 168, 3, 147); //broadcast
    int port = 7005;
    _udp.beginPacket(recipientIP, port);
    _udp.write(_buf, 29);
    _udp.endPacket();
}

void FreeD::pack_be24(unsigned char *buf, long r) {
    buf[2] = r & 0x00FF;
    r >>= 8;
    buf[1] = r & 0x00FF;
    r >>= 8;
    buf[0] = r & 0x00FF;
}

void FreeD::pack_be24_15(unsigned char *buf, double d) {
    pack_be24(buf, d * 32768.0);
}

void FreeD::pack_be24_6(unsigned char *buf, double d) {
    pack_be24(buf, d * 64.0);
}

void FreeD::check_sum() {
    _buf[28] = 0x40;
    for (int i = 0; i < 28; i++) {
        _buf[28] = _buf[28] - _buf[i];
    }
}