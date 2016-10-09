/*
*
* I-lap "transponder" compatible IR transmitter for RC timing system
*
* Type in a 7-digit ID and an alternate ID into the define fields before compiling.
* Note that using non-random ID numbers will increase your chances of encountering 
* another unit using the same ID.
* 
* !!!!IDs with repeated numbers may not register every pass. For best results don't repeat the same digit multiple times in a row!!!!
* Test ID code with system before using, if possible.
* 
*/
//CONFIGURABLE SECTION - SET TRANSPONDER ID
//Change transponder ID # by setting a different transponder number for tx_id
//WARNING: IDs set by CoreIR-Uplink tool will override these numbers
const long tx_id = 8217306;
const long tx_alt_id = 8901234;
const int easylap_id = 2;

// Enable debug info on serial output
//#define debug

// New softserial output ---- TESTING
//#define softout

// EasyRaceLapTimer support ---- TESTING
//  #define easytimer

  
#ifdef easytimer
  const int easylap_on = 0;
#endif

//check which arduino board we are using and build accordingly
#if defined(__AVR_ATtiny84__) || defined(__AVR_ATtiny167__) || defined(__AVR_ATtiny85__)
  //if using an attiny build with all defaults, don't define anything
#elif defined(__AVR_ATmega32U4__)
  //if using an arduino micro build with eeprom enabled and different LED pin
  #define atmega
  #define micro
#elif defined(__AVR_ATmega328p__) || defined(__AVR_ATmega64__) || defined(__AVR_ATmega128__) || defined(__AVR_ATmega328__) || defined(__AVR_ATmega168__)
  //if using an atmega328p or similar build with eeprom enabled
  #define atmega
#else
  //define nothing
#endif

#if defined(softout)
  #include <SoftwareSerial.h>
  SoftwareSerial mySerial(8, 9);
#endif

#if defined(easytimer)
  #include "Easytimer.h"
#endif

//TODO, set pins for attiny85 boards
#ifdef micro
  // Set up alternate ID jumper bridge, shift pins because micro is special and uses pin 5 for IR output
  #define bridgePinIn 14
  #define bridgePinOut 15
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
String incomingString = "";   // for incoming serial data

// Include libraries for IR LED frequency, speed and encoding
#include "IRsnd.h"
#include "Encode.h"

// Initialize IR LED send code
IRsend irsend;

// Set up status LED
int ledState = LOW;
long interval = 0;
unsigned long previousMillis = 0; // Allow status LED to blink without sacraficing a timer

void setup() {
  #ifdef atmega
    // save the new transponder numbers in eeprom if they are not already there
    if (EEPROMReadlong(64) != tx_id) {
      long tx_id = EEPROMReadlong(64);
    }
    if (EEPROMReadlong(70) != easylap_id) {
      long tx_alt_id = EEPROMReadlong(70);
    }
  #endif

  // set up jumper bridge for alternate ID
  pinMode(bridgePinIn, INPUT);
  digitalWrite(bridgePinIn, HIGH);
  pinMode(bridgePinOut, OUTPUT);
  digitalWrite(bridgePinOut, LOW);

  #ifdef atmega
    Serial.begin(9600);
    #if defined(micro) & defined(debug)
      while (!Serial)
    #endif
  #endif

  
  #if defined(softout)
    #if defined(micro)
      mySerial.begin(38400);  // Begin software serial interface at 38400.
      pinMode(5, OUTPUT);     // Set output pin for timer
      TCCR3A = _BV(WGM31); // Set registers for fast PWM.
      TCCR3B = _BV(WGM33) | _BV(CS30); // Read the 328p datasheet for more details on fast PWM.
      OCR3A = 34; // At 16MHz, these values produce roughly 455KHz signal. 16M / 35 = 457K.
      OCR3B = 17; // This should be half of above value. This can be adjusted slightly for taste.
    #else
      mySerial.begin(38400);  // Begin software serial interface at 38400.
      pinMode(3, OUTPUT);     // Set output pin for OCR2B
      TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM21) | _BV(WGM20); // Set registers for fast PWM.
      TCCR2B = _BV(WGM22) | _BV(CS20); // Read the 328p datasheet for more details on fast PWM.
      OCR2A = 34; // At 16MHz, these values produce roughly 455KHz signal. 16M / 35 = 457K.
      OCR2B = 17; // This should be half of above value. This can be adjusted slightly for taste.
    #endif
  #endif

  // Generate timecode
  #ifdef debug
    Serial.print("Building code from: ");
  #endif

    if (digitalRead(bridgePinIn)) {
      #ifdef debug
        Serial.println("Main ID:");
        Serial.println(tx_id);
      #endif
      interval = 200; // Blink LED faster for regular id by default
      makeOutputCode(tx_id); // use alternate ID if unbridged
    } else {
      #ifdef debug
        Serial.println("Alternate ID: ");
        Serial.println(tx_alt_id);
      #endif
      interval = 1000; // Blink LED slower for alt id
      makeOutputCode(tx_alt_id); // use standard ID otherwise
    }
  
  #if defined(debug) && defined(softout)
    Serial.println("Sending: ");
    Serial.print("0"); // fix an issue where leading 0 in hex does not appear in serial.print
    Serial.print(bit1, HEX);
    Serial.print(" ");
    Serial.print(bit2, HEX);
    Serial.print(" ");
    Serial.print(bit3, HEX);
    Serial.print(" ");
    Serial.print(bit4, HEX);
    Serial.print(" ");
    Serial.print(bit5, HEX);
    Serial.print(" ");
    Serial.println(bit6, HEX);
  #endif
  
  // Set up for blinking the status LED
  pinMode(ledPin, OUTPUT);
}

void loop() {
  
      //Send the IR signal, then wait the appropriate amount of time before re-sending
      irsend.sendRaw(outputcode, codeLen, khz);

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
  delayMicroseconds(1800);
}
