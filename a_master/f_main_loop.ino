void setup() {
  #if defined (DEBUG)
    Serial.begin(115200);
  #endif
  Wire.begin();
  #if (defined SIMPLE_PEDESTAL) 
    setup_simple_pedestal();
  #endif
  #if (defined COMPACT_COLLECTIVE) 
    setup_compact_collective();
  #endif
  #if (defined CYCLIC_BASE) 
    setup_cyclic_base();
  #endif
  #if (defined B8_GRIP) 
    setup_b8grip();
  #endif
  #if (defined PEDALS) 
    setup_pedals();
  #endif
}

void loop() {
  #if (defined SIMPLE_PEDESTAL) 
    poll_simple_pedestal();
  #endif
  #if (defined COMPACT_COLLECTIVE) 
    poll_compact_collective();
  #endif
  #if (defined CYCLIC_BASE) 
    poll_cyclic_base();
  #endif
  #if (defined B8_GRIP) 
    poll_b8grip();
  #endif
  #if (defined PEDALS) 
    poll_pedals();
  #endif
}
