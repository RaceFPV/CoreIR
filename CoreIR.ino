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
//Change transponder ID # by setting a different transponder number for tx_id
//WARNING: IDs set by CoreIR-Uplink tool will override these numbers
long tx_id = 4242424;
long tx_alt_id = 8901234;

// Enable debug info on serial output
 //#define debug

//check which arduino board we are using and build accordingly
#if defined(__AVR_ATtiny84__) || defined(__AVR_ATtiny167__) || defined(__AVR_ATtiny85)
  //if using an attiny build with all defaults, don't define anything
#elif defined(__AVR_ATmega32U4__)
  //if using an arduino micro build with eeprom enabled and different LED pin
  #define atmega
  #define micro
#else
  //if using an atmega328p or similar build with eeprom enabled
  #define atmega
#endif

//TODO, set pins for attiny85 boards
#ifdef micro
  // Set up alternate ID jumper bridge, shift pins because micro is special and uses pin 5 for IR output
  #define bridgePinIn 15
  #define bridgePinOut 14
  // Change the status LED location to the proper pin for the atmega32U4
  const int ledPin = 17;
#else
  // Set up alternate ID jumper bridge using normal pins 5 and 6
  #define bridgePinIn 5
  #define bridgePinOut 6
  // Use the normal status LED
  const int ledPin = 13;
#endif

// Include eeprom library for usb connectivity and CoreIR-Uplink ID saving
#ifdef atmega
  #include <EEPROM.h>
  #include "saved.h"
#endif

// Include libraries for IR LED frequency, speed and encoding
#include "IRrem.h"
#include "IRsnd.h"
#include "Encode.h"

// Initialize IR LED send code
IRsend irsend;

// Set up status LED
int ledState = LOW;
const long interval = 100; // Blink LED faster for regular id by default
unsigned long previousMillis = 0; // Allow status LED to blink without sacraficing a timer

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
    const long interval = 1000; // Blink LED slower for alt id
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
  //Send the IR signal, then wait the appropriate amount of time before re-sending
  irsend.sendRaw(outputcode, codeLen, khz);
  delayMicroseconds(2600);

  // -----Status LED blink code start -----
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
    // -----LED blink code end -----
  }
}
