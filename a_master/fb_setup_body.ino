// THIS FILE CONTAINS SETUP FUNCTION BODY, TO ADD CUSTOM DEVICES PLACE YOUR OWN FILES AFTER THIS FILE
// FILES MUST BE CALLED fb[a-z]_setup_<filename>.ino 

#if defined (DEBUG)
  Serial.begin(115200);
#endif
Wire.begin();
#if (defined SIMPLE_PEDESTAL) 
  delay(DEVICE_INIT_DELAY);
  setup_simple_pedestal();
#endif
#if (defined VRMAXII_PEDESTAL) 
  delay(DEVICE_INIT_DELAY);
  setup_vrmax2_pedestal();
#endif
#if (defined COMPACT_COLLECTIVE) 
  delay(DEVICE_INIT_DELAY);
  setup_compact_collective();
#endif
#if (defined COLLECTIVE_STHR) 
  delay(DEVICE_INIT_DELAY);
  setup_single_collective();
#endif
#if (defined CYCLIC_BASE) 
  delay(DEVICE_INIT_DELAY);
  setup_cyclic_base();
#endif
#if (defined B8_GRIP) 
  delay(DEVICE_INIT_DELAY);
  setup_b8grip();
#endif
#if (defined PEDALS) 
  delay(DEVICE_INIT_DELAY);
  setup_pedals();
#endif
#if (defined COLLECTIVE_NOTHR) 
  delay(DEVICE_INIT_DELAY);
  setup_coll_nothr();
#endif
#if (defined AB412_COLL_SWITCH_PANEL) 
  delay(DEVICE_INIT_DELAY);
  setup_ab412_coll_head();
#endif


