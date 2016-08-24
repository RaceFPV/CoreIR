/*
*
* I-lap "transponder" compatible IR transmitter for RC timing system
*
* Type in a 7-digit ID and an alternate ID into the define fields before compiling.
* Note that using non-random ID numbers will increase your chances of encountering 
* another unit using the same ID.
* 
* IDs starting with 0 and those with many repeated 0s may not register correctly.
* Test ID code with system before using, if possible.
* 
*/
//CONFIGURABLE SECTION - SET TRANSPONDER ID
//change transponder ID # by setting a different transponder number for tx_id
//WARNING: IDs set by CoreIR-Uplink tool will override these numbers
long tx_id = 4242424;
long tx_alt_id = 8901234;

//if using an attiny
#if defined(__AVR_ATtiny84__) || defined(__AVR_ATtiny167__) || defined(__AVR_ATtiny85)
#else
  #define atmega
#endif

//if using a arduino pro micro
#if defined(__AVR_ATmega32U4__)
  #define micro
#endif

//CONFIGURATION END

#ifdef micro
// Set up alternate ID jumper bridge, shift pins because micro is special
  #define bridgePinIn 15
  #define bridgePinOut 14
#else
// Set up alternate ID jumper bridge
  #define bridgePinIn 5
  #define bridgePinOut 6
#endif
// Enable debug info on serial output
 //#define debug

// Include eeprom library for usb connectivity if applicable
#ifdef atmega
  #include <EEPROM.h>
  #include "saved.h"
#endif

// Include libraries for IR LED frequency and speed and encoding
#include "IRrem.h"
#include "IRsnd.h"
#include "Encode.h"

IRsend irsend;

// Set up status LED
#ifdef micro
  const int ledPin = 17;
#else
  const int ledPin = 13;
#endif

int ledState = LOW;
unsigned long previousMillis = 0;
const long interval = 100;

void setup() {
  #ifdef atmega
    // save the new transponder numbers in eeprom if they are not already there
    if (EEPROMReadlong(0) != tx_id) {
      long tx_id = EEPROMReadlong(0);
    }
    if (EEPROMReadlong(4) != tx_alt_id) {
      long tx_alt_id = EEPROMReadlong(4);
    }
  #endif
  // set up jumper bridge for alternate ID
  pinMode(bridgePinIn, INPUT);
  digitalWrite(bridgePinIn, HIGH);
  pinMode(bridgePinOut, OUTPUT);
  digitalWrite(bridgePinOut, LOW);
  
  #ifdef debug
    Serial.begin(9600);
  #endif

  // Generate timecode
  #ifdef debug
    Serial.print("Building code: ");
  #endif
  if (digitalRead(bridgePinIn)) {
    #ifdef debug
      Serial.print("ALT ");
      Serial.println(tx_alt_id);
    #endif
    int interval = 1000; // Blink LED slower for alt id
    makeOutputCode(tx_alt_id); // use alternate ID if unbridged
  } else {
    #ifdef debug
      Serial.print("STD ");
      Serial.println(tx_id);
    #endif
    makeOutputCode(tx_id); // use standard ID otherwise
  }
  
  // Set up for blinking the status LED
  pinMode(ledPin, OUTPUT);
}

void loop() {
  irsend.sendRaw(outputcode, codeLen, khz);
  delayMicroseconds(2600);

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
