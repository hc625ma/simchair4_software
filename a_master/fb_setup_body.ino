// THIS FILE CONTAINS SETUP FUNCTION BODY, TO ADD CUSTOM DEVICES PLACE YOUR OWN FILES AFTER THIS FILE
// FILES MUST BE CALLED fb[a-z]_<filename>_setup.ino 

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
