#include <Ethernet.h>

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(239, 255, 0, 1);
unsigned int localPort = 5568;

// buffers for receiving and sending data
char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; //buffer to hold incoming packet,
char ReplyBuffer[] = "acknowledged";       // a string to send back

EthernetUDP Udp;

void setup() {
  Ethernet.begin(mac,ip);
  Udp.begin(localPort);
  Serial.begin(9600);
  Serial.println("Setup complete...");
}

void loop() {
  int packetSize = Udp.parsePacket();
  if(packetSize) {
    Serial.print("Received packet of size ");
    Serial.println(packetSize);
    Serial.print("From ");
    IPAddress remote = Udp.remoteIP();
    for (int i =0; i < 4; i++) {
      Serial.print(remote[i], DEC);
      if (i < 3) {
        Serial.print(".");
      }
    }
    Serial.print(", port ");
    Serial.println(Udp.remotePort());

    // read the packet into packetBufffer
    Udp.read(packetBuffer,UDP_TX_PACKET_MAX_SIZE);
    Serial.println("Contents:");
    char buf[20];
    for(int i = 0; i < packetSize; i++) {
        sprintf(buf,"%02x ",(unsigned char)packetBuffer[i]);
        if (i % 16 == 0) {
            Serial.println();
        }
        Serial.print(buf);
    }
    Serial.println(packetBuffer);

    // send a reply, to the IP address and port that sent us the packet we received
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.write(ReplyBuffer);
    Udp.endPacket();
  }
  delay(10);
}
