//******************************************************************************
// IRremote
// Origial code by Ken Shirriff
// Modified by Paul Stoffregen <paul@pjrc.com> to support other boards and timers
// Modified by Michael Rickert <Slacker87> for more accurate IR LED timings
// Interrupt code based on NECIRrcv by Joe Knapp
//******************************************************************************

#ifndef IRrem_h
#define IRrem_h

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
