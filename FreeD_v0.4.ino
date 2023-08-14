#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

WiFiUDP udp;

const char* ssid = "MGX";
const char* password = "MGS444TD";

int udpPort = 8266;

unsigned char buf[29];

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  while (!udp.begin(udpPort)) {
    delay(250);
  }
}

void loop() {
  PackFreeD();
  IPAddress recipientIP(192, 168, 3, 255); // Replace with the IP address of the recipient
  int port = 7005; // Replace with the recipient's port number
  udp.beginPacket(recipientIP, port);
  udp.write(buf, 29);
  udp.endPacket();
  delay(20);
}

void PackFreeD()
{
  buf[0] = 0xD1; // this is reserved as far as i remember
  buf[1] = 1; // this one is device index, so you can have multiple and differenciate them
 
// this is how axis interpreted in UE
  // rot z
  pack_be24_15(buf + 2, 0);
  // rot y
  pack_be24_15(buf + 5, 0);
  // rot x
  pack_be24_15(buf + 8, 0);

  // loc y
  pack_be24_6(buf + 11, 0);
  // loc x
  pack_be24_6(buf + 14, 0);
  // loc z
  pack_be24_6(buf + 17, 0);

// zoom

pack_be24(buf + 20, 50);
 
// focus
 pack_be24(buf + 23, 25);

  buf[26] = 0;
  buf[27] = 0;

  check_sum(); // this function is calculate checksum. Unreal will work without it calculated right, but will throw a lot of messages in to log
}






static void pack_be24(unsigned char *buf, long r)
{
  buf[2] = r & 0x00FF;
  r >>= 8;
  buf[1] = r & 0x00FF;
  r >>= 8;
  buf[0] = r & 0x00FF;
}

static void pack_be24_15(unsigned char *buf, double d)
{
  pack_be24(buf, d * 32768.0);
}

static void pack_be24_6(unsigned char *buf, double d)
{
  pack_be24(buf, d * 64.0);
}

void check_sum()
{
  buf[28] = 0x40;
  for (int i = 0; i < 28; i++)
  {
    buf[28] = buf[28] - buf[i];
  }
}
