 #include <ESP8266WiFi.h>  
#include <WiFiUdp.h>
#include <SPI.h>


char ssid[] = "id";                         // wifi ssid
char pass[] = "pass";                    // parola
const IPAddress outIp(192, 168, 3, 255); // Göndericeği IP Adressi bu Broadcast'te olabilir
IPAddress staticIP(192, 168, 3, 208);
IPAddress gateway(192, 168, 3, 1);
IPAddress subnet(255,255,255,0);
//const unsigned int outPort = 9989;           // Gönderme Portu



#define BROADCAST_IP IPAddress(192, 168, 3, 255)
#define SEND_PORT 8092

// IMU verisi alınan pinler
//#define PITCH_PIN A0
//#define YAW_PIN A1
//#define POS_Z_PIN A3
//#define POS_X_PIN A4
//#define POS_Y_PIN A5
//#define ZOOM_PIN A6
//#define FOCUS_PIN A7

// WiFi kartının MAC adresi
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

WiFiUDP udp;

void setup() {



  Serial.begin(115200);     



  Serial.println();                         // Wifi Bağlantı aşaması Seri Yazdırma
  Serial.print("Connecting to ");           // Wifi Bağlantı aşaması Seri Yazdırma
  Serial.println(ssid);                     // Wifi Bağlantı aşaması Seri Yazdırma

  WiFi.config(staticIP, gateway, subnet);
  WiFi.begin(ssid, pass);


  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Starting UDP");
  Serial.print("Local port: ");
  Serial.println(WiFi.subnetMask());


 

  
  
  udp.begin(SEND_PORT);
}

void loop() {
  // IMU verilerini oku
float pitch = 10;
float yaw = 11;
float roll = 12;
float posZ = 13;
float posX = 14;
float posY = 15;
int zoom = 16;
int focus = 17;

  // FreeD verisi oluştur
  byte data[29];
  data[0] = 0xD1;  // Identifier
  data[1] = 0xFF;  // ID
  setRotation(pitch, data + 2);  // Pitch
  setRotation(yaw, data + 5);    // Yaw
  setRotation(roll, data + 8);   // Roll
  setPosition(posZ, data + 11);  // PosZ
  setPosition(posX, data + 14);  // PosX
  setPosition(posY, data + 17);  // PosY
  setEncoder(zoom, data + 20);   // Zoom
  setEncoder(focus, data + 23);  // Focus
  setReservedBytes(data + 26);   // Reserved
  data[28] = calculateChecksum(data);  // Checksum

  // Veriyi broadcast yayınla
  udp.beginPacket(BROADCAST_IP, SEND_PORT);
  udp.write(data, sizeof(data));
  Serial.write(data, sizeof(data));
  udp.endPacket();

  delay(100);
}

void setRotation(float rot, byte* data) {
  int rotation = rot * 32768 * 256;
  data[0] = rotation >> 24;
  data[1] = rotation >> 16;
  data[2] = rotation >> 8;
}

void setPosition(float pos, byte* data) {
int position = pos * 64 * 256;
data[0] = position >> 24;
data[1] = position >> 16;
data[2] = position >> 8;
}

void setEncoder(int enc, byte* data) {
data[0] = 0x00;
data[1] = enc >> 16;
data[2] = enc >> 8;
}

void setReservedBytes(byte* data) {
data[0] = 0x00;
data[1] = 0x00;
}

byte calculateChecksum(byte* data) {
int checksum = 0;
for (int i = 0; i < 28; i++) {
checksum += data[i];
}
return checksum;
}
