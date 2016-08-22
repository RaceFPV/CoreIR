
//******************************************************************************
// IRremote
// Version 2.0.1 June, 2015
// Copyright 2009 Ken Shirriff
// For details, see http://arcfn.com/2009/08/multi-protocol-infrared-remote-library.html
// Edited by Mitra to add new controller SANYO
//
// Interrupt code based on NECIRrcv by Joe Knapp
// http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1210243556
// Also influenced by http://zovirl.com/2008/11/12/building-a-universal-remote-with-an-arduino/
//
// JVC and Panasonic protocol added by Kristian Lauszus (Thanks to zenwheel and other people at the original blog post)
// LG added by Darryl Smith (based on the JVC protocol)
// Whynter A/C ARC-110WD added by Francesco Meschia
//******************************************************************************

#ifndef IRrem_h
#define IRrem_h
//------------------------------------------------------------------------------
// An enumerated list of all supported formats
// You do NOT need to remove entries from this list when disabling protocols!
//
typedef
  enum {
    UNKNOWN      = -1,
    UNUSED       =  0,
  }
decode_type_t;

//------------------------------------------------------------------------------
// Set DEBUG to 1 for lots of lovely debug output
//
#define DEBUG  0

//------------------------------------------------------------------------------
// Debug directives
//
#if DEBUG
# define DBG_PRINT(...)    Serial.print(__VA_ARGS__)
# define DBG_PRINTLN(...)  Serial.println(__VA_ARGS__)
#else
# define DBG_PRINT(...)
# define DBG_PRINTLN(...)
#endif

//------------------------------------------------------------------------------
// Mark & Space matching functions
//
int  MATCH       (int measured, int desired) ;
int  MATCH_MARK  (int measured_ticks, int desired_us) ;
int  MATCH_SPACE (int measured_ticks, int desired_us) ;

//------------------------------------------------------------------------------
// Results returned from the decoder
//
class decode_results
{
  public:
    decode_type_t          decode_type;  // UNKNOWN, NEC, SONY, RC5, ...
    unsigned int           address;      // Used by Panasonic & Sharp [16-bits]
    unsigned long          value;        // Decoded value [max 32-bits]
    int                    bits;         // Number of bits in decoded value
    volatile unsigned int  *rawbuf;      // Raw intervals in 50uS ticks
    int                    rawlen;       // Number of records in rawbuf
    int                    overflow;     // true iff IR raw code too long
};

//------------------------------------------------------------------------------
// Decoded value for NEC when a repeat code is received
//
#define REPEAT 0xFFFFFFFF


//------------------------------------------------------------------------------
// Main class for sending IR
//
class IRsend
{
  public:
    IRsend () { }

    void  custom_delay_usec (unsigned long uSecs);
    void  enableIROut     (int khz) ;
    void  mark            (unsigned int usec) ;
    void  space           (unsigned int usec) ;
    void  sendRaw         (const unsigned int buf[],  unsigned int len,  unsigned int hz) ;
} ;
#endif
