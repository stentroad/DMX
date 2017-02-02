#include <string.h>
#include <Ethernet.h>
#define UDP_TX_PACKET_MAX_SIZE 800
#include <utility/util.h>
#include <stdint.h>
//#include <DMXSerial.h>

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
//IPAddress ip(239, 255, 0, 1);
IPAddress ip(192, 168, 1, 100);
unsigned int localPort = 5568;

EthernetUDP Udp;

//#define SERIAL_DEBUG 1

#define CHAN(n) packet.rlp.e131_packet.dmp_packet.property_values[n]
#define CHAN_COUNT() packet.rlp.e131_packet.dmp_packet.property_value_count
#define MAX_CHANNELS 512
typedef struct DMP_PACKET {
  uint16_t flags_and_length;
  uint8_t vector;
  uint8_t address_type_and_data_type;
  uint16_t first_property_address;
  uint16_t address_increment;
  uint16_t property_value_count;
  uint8_t property_values[1+MAX_CHANNELS]; // CHAN(0) = DMX512-A START Code, CHAN(1-512) channel data
} DMP_PACKET_T;

typedef struct E131_PACKET {
    uint16_t flags_and_length;
    uint32_t vector;
    uint8_t source_name[64];
    uint8_t priority;
    uint16_t reserved;
    uint8_t sequence_number;
    uint8_t options;
    uint16_t universe;
    DMP_PACKET_T dmp_packet;
} E131_PACKET_T;

typedef struct RLP_PACKET {
    uint16_t preamble_size;
    uint16_t postamble_size;
    uint8_t packet_identifier[12];
    uint16_t flags_and_length;
    uint32_t vector;
    uint8_t component_identifier[16];
    E131_PACKET_T e131_packet;
} RLP_PACKET_T;

typedef union DMX_PACKET {
    char udp_buffer[UDP_TX_PACKET_MAX_SIZE];
    RLP_PACKET_T rlp;
} DMX_PACKET_T;

DMX_PACKET_T packet;
const int RED_LED_PIN = 9;
const int GREEN_LED_PIN = 5;
const int BLUE_LED_PIN = 6;

void setup() {
  pinMode(GREEN_LED_PIN, OUTPUT);
  analogWrite(GREEN_LED_PIN, 0);
  Ethernet.begin(mac,ip);
  Udp.begin(localPort);
#ifdef SERIAL_DEBUG
  Serial.begin(9600);
  Serial.println("Setup complete...");
#endif
  //DMXSerial.init(DMXController);
  //DMXSerial.write(2,255); // Red
  
}

#ifdef SERIAL_DEBUG
void print8d(const char* field_name,uint8_t field) {
    char buf[20];
    Serial.print(field_name);
    sprintf(buf,"%u",field);
    Serial.println(buf);
}

void print16d(const char* field_name,uint16_t field) {
    char buf[20];
    Serial.print(field_name);
    sprintf(buf,"%u",ntohs(field));
    Serial.println(buf);
}

void print16(const char* field_name,uint16_t field) {
    char buf[20];
    Serial.print(field_name);
    sprintf(buf,"%04x",ntohs(field));
    Serial.println(buf);
}

void print32(const char* field_name,uint32_t field) {
                                                     char buf[20];
                                                     Serial.print(field_name);
                                                     sprintf(buf,"%08lx",ntohl(field));
                                                     Serial.println(buf);
                                                     }
#endif

void loop() {
  int packetSize = Udp.parsePacket();
  if(packetSize) {
    //digitalWrite(GREEN_LED_PIN, HIGH);
#ifdef SERIAL_DEBUG
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
    memset(packet.udp_buffer,0,sizeof(DMX_PACKET));
#endif
    // read the packet into packet_buffer
    Udp.read(packet.udp_buffer,UDP_TX_PACKET_MAX_SIZE);
#ifdef SERIAL_DEBUG
    Serial.println("Contents:");
    char buf[20];
    for(int i = 0; i < packetSize; i++) {
        sprintf(buf,"%02x ",(unsigned char)packet.udp_buffer[i]);
        if (i % 16 == 0) {
            Serial.println();
        }
        Serial.print(buf);
    }
    Serial.println();
    Serial.println("RLP-------------------------");
    print16("Preamble size: ",packet.rlp.preamble_size);
    print16("Postamble size: ",packet.rlp.postamble_size);
    print16("Flags and length: ",packet.rlp.flags_and_length);
    print32("Vector: ",packet.rlp.vector);
    Serial.println("E131------------------------");
    print16("Flags and length: ",packet.rlp.e131_packet.flags_and_length);
    print32("Vector: ",packet.rlp.e131_packet.vector);
    Serial.println("================================");
    print16d("DMX channel count: ",CHAN_COUNT());
    print8d("DMX channel 1: ",CHAN(1));
    print8d("DMX channel 2: ",CHAN(2));
    print8d("DMX channel 3: ",CHAN(3));
    print8d("DMX channel 4: ",CHAN(4));
#endif
    // for(int i=1;i<=CHAN_COUNT();i++) {
    //   DMXSerial.write(i,CHAN(i));
    // }
    analogWrite(GREEN_LED_PIN, CHAN(1));
    //digitalWrite(GREEN_LED_PIN, LOW);
  }
  delayMicroseconds(2000);
}
