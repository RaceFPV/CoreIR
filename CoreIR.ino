/*
 * 
 */


#include <IRremote.h>

IRsend irsend;
  int khz = 15; // 38kHz carrier frequency for the NEC protocol
  unsigned int rawData[33] = {20, 25, 45, 25, 85, 25, 20, 25, 45, 20, 90, 25, 45, 45, 25, 45, 25, 45, 25, 65, 45, 95, 95, 25, 20, 25, 20, 55, 45, 45, 45, 20, 20};

void setup()
{
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
}

void loop() {
  irsend.sendRaw(rawData, 33, khz); //Note the approach used to automatically calculate the size of the array.
  delayMicroseconds(500);
}
