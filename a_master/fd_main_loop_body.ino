// THIS FILE CONTAINS MAIN LOOP FUNCTION BODY, TO ADD CUSTOM DEVICES PLACE YOUR OWN FILES AFTER THIS FILE
// FILES MUST BE CALLED fd[a-z]_loop_<filename>.ino 

if (g_operating_mode == 1) {
  // if the operating mode switch was set to "PRGM"
    while (1) {
      run_uart();
      g_operating_mode = !digitalRead(2);
      if (g_operating_mode == 0) {
         // reset the board when we flip the switch back to "FLY" position
         wdt_enable(WDTO_15MS);
         delay(1000);
      }
    }
}
  
  
#if (defined SIMPLE_PEDESTAL) 
  poll_simple_pedestal();
#endif
#if (defined VRMAXII_PEDESTAL) 
  poll_vrmax2_pedestal();
#endif
#if (defined COMPACT_COLLECTIVE) 
  poll_compact_collective();
#endif
#if (defined COLLECTIVE_STHR) 
  poll_single_collective();
#endif
#if (defined SIMPLE_COLLECTIVE) 
  poll_simple_collective();
#endif
#if (defined COLLECTIVE_STHR_MKIII) 
  poll_single_collective_mk3();
#endif
#if (defined COLLECTIVE_TTHR_MKIII) 
  poll_twin_collective_mk3();
#endif
#if (defined COLLECTIVE_TTHR) 
  poll_twin_collective();
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
#if (defined COLLECTIVE_NOTHR) 
  poll_coll_nothr();
#endif
#if (defined AB412_COLL_SWITCH_PANEL) 
  poll_ab412_coll_head();
#endif
#if (defined HUEY_COLL_SWITCH_PANEL) 
  poll_huey_coll_head();
#endif
#if (defined B206_COLL_SWITCH_PANEL) 
  poll_b206_coll_head();
#endif
#if (defined HELI_QUADRANT) 
  poll_heli_quadrant();
#endif
