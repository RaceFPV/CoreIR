 /*
 * 
 */


#include <IRremote.h>

IRsend irsend;
  int khz = 15; // 38kHz carrier frequency for the NEC protocol
 //6491857 unsigned int rawData[33] = {22, 22, 48, 22, 88, 22, 22, 22, 48, 22, 88, 22, 48, 48, 22, 48, 22, 48, 22, 68, 48, 93, 93, 22, 22, 22, 22, 53, 48, 48, 48, 22, 22};
 unsigned int rawData[35] = {22, 48, 22, 22, 93, 22, 48, 68, 48, 68, 22, 48, 22, 22, 68, 48, 22, 68, 22, 22, 22, 93, 22, 93, 22, 22, 22, 48, 22, 22, 22, 22, 48, 22, 22};

void setup()
{
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
}

void loop() {
  //6491857 irsend.sendRaw(rawData, 33, khz); //Note the approach used to automatically calculate the size of the array.
  irsend.sendRaw(rawData, 35, khz); //Note the approach used to automatically calculate the size of the array.
  delayMicroseconds(480);
}
