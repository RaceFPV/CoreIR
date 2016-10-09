//init some variables, set the freq based on which clock we end up using (based on the chipset)
#if defined(atmega) || defined(micro)
  int khz = 370;
  #define framewidth 24 // pulses per data bit
#else
  int khz = 15;
  #define framewidth 24 // pulses per data bit
#endif

unsigned int outputcode[60];
byte codeLen = 0;
byte bit1;
byte bit2;
byte bit3;
byte bit4;
byte bit5;
byte bit6;


// CRC checksum generation
uint16_t crc_xmodem_update (uint16_t crc, uint8_t data)
{
  int i;

  crc = crc ^ ((uint16_t)data << 8);
  for (i=0; i<8; i++)
  {
    if (crc & 0x8000)
      crc = (crc << 1) ^ 0x1021; //(polynomial = 0x1021)
    else
      crc <<= 1;
  }
  return crc;
}

uint16_t calc_crc(byte *msg,int n)
{
  uint16_t x = 0;

  while(n--)
  {
    x = crc_xmodem_update(x, (uint16_t)*msg++);
  }

  return(x);
}

// Timing code constructor
void makeOutputCode(unsigned long tcode) {
  // clear globals
  memset(outputcode,0,sizeof(outputcode));
  codeLen = 0;
  
  // split ID into digits
  byte tdigit7 = tcode % 10;
  byte tdigit6 = (tcode / 10) % 10;
  byte tdigit5 = (tcode / 100) % 10;
  byte tdigit4 = (tcode / 1000) % 10;
  byte tdigit3 = (tcode / 10000) % 10;
  byte tdigit2 = (tcode / 100000) % 10;
  byte tdigit1 = (tcode / 1000000) % 10;

  // fill in code values
  byte fullcode[6];
  fullcode[0] = 0xf0 + tdigit1;
  fullcode[1] = 0x00;
  fullcode[2] = (tdigit2 << 4) + tdigit3;
  fullcode[3] = (tdigit4 << 4) + tdigit5;
  fullcode[4] = (tdigit6 << 4) + tdigit7;
  fullcode[5] = 0x00;

  #ifdef debug
    Serial.println("Initial code: ");
    Serial.print(fullcode[0], HEX);
    Serial.print(" ");
    Serial.print(fullcode[1], HEX);
    Serial.print(" ");
    Serial.print(fullcode[2], HEX);
    Serial.print(" ");
    Serial.print(fullcode[3], HEX);
    Serial.print(" ");
    Serial.print(fullcode[4], HEX);
    Serial.print(" ");
    Serial.println(fullcode[5], HEX);
  #endif
  
  // generate CRC check bits
  byte CRC1 [4] = { fullcode[4], fullcode[3], fullcode[2], fullcode[0] };
  uint16_t crc_out;
  crc_out = calc_crc(CRC1,4); //Calculate CRC on-the-fly         
 
  // fill in check bits
  fullcode[1] = (crc_out>>8) & 0xff;
  fullcode[5] = crc_out & 0xff;
  
  #ifdef debug
    Serial.println("Feeding bytes into CRC16 XMODEM: ");
    Serial.print(fullcode[4], HEX);
    Serial.print(" ");
    Serial.print(fullcode[3], HEX);
    Serial.print(" ");
    Serial.print(fullcode[2], HEX);
    Serial.print(" ");
    Serial.println(fullcode[0], HEX);
    Serial.println("Got back CRC high byte: ");
    Serial.println(fullcode[1], HEX);
    Serial.println("CRC low byte: ");
    Serial.println(fullcode[5], HEX);
    Serial.println("Full code before inversion: ");
    Serial.print(fullcode[0], HEX);
    Serial.print(" ");
    Serial.print(fullcode[1], HEX);
    Serial.print(" ");
    Serial.print(fullcode[2], HEX);
    Serial.print(" ");
    Serial.print(fullcode[3], HEX);
    Serial.print(" ");
    Serial.print(fullcode[4], HEX);
    Serial.print(" ");
    Serial.println(fullcode[5], HEX);
  #endif
  
  // invert all
  fullcode[0] = ~fullcode[0];
  fullcode[1] = ~fullcode[1];
  fullcode[2] = ~fullcode[2];
  fullcode[3] = ~fullcode[3];
  fullcode[4] = ~fullcode[4];
  fullcode[5] = ~fullcode[5];

  #ifdef debug
    Serial.println("Full code after inversion: ");
    Serial.print("0"); // fix an issue where leading 0 in hex does not appear in serial.print
    Serial.print(fullcode[0], HEX);
    Serial.print(" ");
    Serial.print(fullcode[1], HEX);
    Serial.print(" ");
    Serial.print(fullcode[2], HEX);
    Serial.print(" ");
    Serial.print(fullcode[3], HEX);
    Serial.print(" ");
    Serial.print(fullcode[4], HEX);
    Serial.print(" ");
    Serial.println(fullcode[5], HEX);
  #endif
    // make reversed UART string
    uint32_t UARThigh = 0;
    uint32_t UARTlow = 0;
  
    for (int b = 2; b >= 0; b--) {
      UARTlow = UARTlow << 1;
      UARTlow = UARTlow + 1; // add a 1
      UARTlow = UARTlow << 8;
      UARTlow = UARTlow + fullcode[b];
      UARTlow = UARTlow << 1; // add a zero
    }
    for (int b = 5; b >= 3; b--) {
      UARThigh = UARThigh << 1;
      UARThigh = UARThigh + 1; // add a 1
      UARThigh = UARThigh << 8;
      UARThigh = UARThigh + fullcode[b];
      UARThigh = UARThigh << 1; // add a zero
    }
    #ifdef debug
      Serial.println("Inverted UART format: ");
      Serial.print (UARThigh, BIN);
      Serial.print (".");
      Serial.println (UARTlow, BIN);
    #endif
  
    // count bits to generate timing code
    byte numbits = 0;
    byte lastbit = 0;
  
    #ifdef debug
      Serial.println("Parsing output code: ");
      Serial.println("Count\tSearch\tData: ");
    #endif
    while (UARTlow) {
      if ((UARTlow & B1) != lastbit) {
        outputcode[codeLen] = numbits;
        codeLen++;
        numbits = 0;
        if (lastbit) {
          lastbit = 0;
        } else {
          lastbit = 1;
        }
      } else {
        numbits++;
        UARTlow = UARTlow >> 1;
      }    
  
      #ifdef debug
        Serial.print (numbits);
        Serial.print ("\t");
        Serial.print (lastbit);
        Serial.print ("\t");
        Serial.println (UARTlow, BIN);
      #endif
    }
    Serial.println ("-----");
    while (UARThigh) {
      if ((UARThigh & B1) != lastbit) {
        outputcode[codeLen] = numbits;
        codeLen++;
        numbits = 0;
        if (lastbit) {
          lastbit = 0;
        } else {
          lastbit = 1;
        }
      } else {
        numbits++;
        UARThigh = UARThigh >> 1;
      }    
  
      #ifdef debug
        Serial.print (numbits);
        Serial.print ("\t");
        Serial.print (lastbit);
        Serial.print ("\t");
        Serial.println (UARThigh, BIN);
      #endif
    }
    
    #ifdef debug
      Serial.println("Counted array: ");
      for (int i = 0; i < codeLen; i++) {
        Serial.print(outputcode[i]);
      }
      Serial.print(" (");
      Serial.print(codeLen);
      Serial.println(")");
    #endif
  
    for (int i = 0; i < codeLen; i++) {
      outputcode[i] = outputcode[i] * framewidth;
    }
  
    #ifdef debug
      Serial.println("Final array: ");
      for (int i = 0; i < codeLen; i++) {
        Serial.print(outputcode[i]);
        Serial.print(" ");
      }
      Serial.print(" (");
      Serial.print(codeLen);
      Serial.println(")");  
    #endif
}
