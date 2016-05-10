//****CONFIGURATION OPTIONS****
//change transponder ID # by setting a different transponder number
//available values are 1-6

#define transponder1

//****CONFIGURATION END****

//setup for the transponder IR signal
#include <IRremote.h>
IRsend irsend;
int khz = 15;

//setup for blinking the arduino LED
const int ledPin =  13; 
int ledState = LOW;
unsigned long previousMillis = 0;
const long interval = 100;

//transponder defines
#ifdef transponder1
    //raw on/off for code 5196502
    unsigned int rawData[33] = {48, 22, 22, 22, 98, 22, 68, 22, 48, 48, 22, 22, 48, 48, 48, 98, 48, 22, 22, 48, 48, 48, 22, 22, 22, 178, 22, 22, 22, 68, 22, 22, 22};
    int size = 33;
#endif
#ifdef transponder2 
    //2889174
    unsigned int rawData[35] = {22, 22, 22, 48, 98, 22, 22, 22, 22, 22, 48, 48, 22, 22, 22, 68, 22, 68, 22, 22, 48, 68, 22, 48, 22, 22, 22, 48, 22, 22, 68, 48, 48, 103, 83};
    int size = 35;
#endif
#ifdef transponder3 
    //3904041
    unsigned int rawData[29] = {68, 48, 98, 22, 22, 22, 22, 103, 48, 22, 22, 103, 22, 48, 22, 22, 22, 150, 22, 48, 48, 126, 22, 48, 22, 22, 22, 48, 22};
    int size = 29;
#endif
#ifdef transponder4 
    //6491857
    unsigned int rawData[33] = {22, 22, 48, 22, 88, 22, 22, 22, 48, 22, 88, 22, 48, 48, 22, 48, 22, 48, 22, 68, 48, 98, 98, 22, 22, 22, 22, 53, 48, 48, 48, 22, 22};
    int size = 33;
#endif
#ifdef transponder5
    //3852671
    unsigned int rawData[31] = {68, 48, 98, 22, 68, 22, 68, 68, 48, 22, 22, 103, 22, 22, 22, 22, 48, 48, 22, 73, 48, 76, 68, 48, 22, 22, 48, 22, 48, 22, 22};
    int size = 31;
#endif
#ifdef transponder6
    //4742860
    unsigned int rawData[35] = {22, 48, 22, 22, 88, 22, 48, 68, 48, 68, 22, 48, 22, 22, 68, 48, 22, 68, 22, 22, 22, 88, 22, 88, 22, 22, 22, 52, 22, 22, 22, 22, 48, 22, 22};
    int size = 35;
#endif
void setup()
{
  //setup for blinking the arduino LED
  pinMode(ledPin, OUTPUT);    //set transponder raw id
}

void loop() {
  irsend.sendRaw(rawData, size, khz);
  delayMicroseconds(2600);

  //blink the LED if applicable
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }

    // set the LED with the ledState of the variable:
    digitalWrite(ledPin, ledState);
  }
}
