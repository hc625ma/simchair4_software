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
}

void loop() {
  #if (defined SIMPLE_PEDESTAL) 
    poll_simple_pedestal();
  #endif
  #if (defined COMPACT_COLLECTIVE) 
    poll_compact_collective();
  #endif
}
