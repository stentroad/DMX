#include <SoftwareSerial.h>
#include <DMXSerial.h>

#define rxPin 10
#define txPin 11
SoftwareSerial soft(rxPin, txPin);

void setup() {
    pinMode(13, OUTPUT);
    DMXSerial.init(DMXReceiver);
    DMXSerial.write(1, 80);
    soft.begin(4800);
    soft.println("DMX setup");
}

void loop() {
    if (soft.available()) {
        digitalWrite(13, HIGH);
        soft.read();
    }
}
