/*
  Software serial multple serial test

 Receives from the hardware serial, sends to software serial.
 Receives from software serial, sends to hardware serial.

 The circuit:
 * RX is digital pin 10 (connect to TX of other device)
 * TX is digital pin 11 (connect to RX of other device)

 Note:
 Not all pins on the Mega and Mega 2560 support change interrupts,
 so only the following can be used for RX:
 10, 11, 12, 13, 50, 51, 52, 53, 62, 63, 64, 65, 66, 67, 68, 69

 Nrot all pins on the Leonardo support change interrupts,
 so only the following can be used for RX:
 8, 9, 10, 11, 14 (MISO), 15 (SCK), 16 (MOSI).

 created back in the mists of time
 modified 25 May 2012
 by Tom Igoe
 based on Mikal Hart's example

 This example code is in the public domain.

 */
#include <SoftwareSerial.h>

#define rxPin 10
#define txPin 11
SoftwareSerial soft(rxPin, txPin);

void setup()
{
    pinMode(13, OUTPUT);
    Serial.begin(4800);
    soft.begin(4800);
    Serial.println("Hardware");
    soft.println("Software");
}

void loop() // run over and over
{
    if (soft.available()) {
        digitalWrite(13, HIGH);   // set the LED on
        Serial.write(soft.read());
    }
    if (Serial.available())
        soft.write(Serial.read());
}
