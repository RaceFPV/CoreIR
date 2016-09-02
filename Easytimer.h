  #define BIT_SET(a,b) ((a) |= (1<<(b)))
  #define BIT_CLEAR(a,b) ((a) &= ~(1<<(b)))
  #define BIT_FLIP(a,b) ((a) ^= (1<<(b)))
  #define BIT_CHECK(a,b) ((a) & (1<<(b)))
  #define NUM_BITS  9
  #define ZERO 250
  #define ONE 650
  unsigned int buffer[NUM_BITS];
  unsigned int num_one_pulses = 0;
  unsigned int get_pulse_width_for_buffer(int bit){
  if(BIT_CHECK(easylap_id,bit)){
    return ONE;
  }

  return ZERO;
}

unsigned int control_bit(){
  if(num_one_pulses % 2 >= 1){
    return ONE;  
  }else{
    return ZERO;
  }
}



void geteasylapcode() {
  buffer[0] = ZERO;    
  buffer[1] = ZERO;    
  buffer[2] = get_pulse_width_for_buffer(5);
  buffer[3] = get_pulse_width_for_buffer(4);
  buffer[4] = get_pulse_width_for_buffer(3);
  buffer[5] = get_pulse_width_for_buffer(2);
  buffer[6] = get_pulse_width_for_buffer(1);
  buffer[7] = get_pulse_width_for_buffer(0);
  buffer[8] = control_bit();
  #ifdef debug
    for(int i= 0; i < NUM_BITS; i++){
      Serial.println(buffer[i],DEC);
    }
    Serial.println(easylap_id,BIN);
  #endif
}

