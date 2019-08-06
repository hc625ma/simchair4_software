// THIS FILE CONTAINS MAIN LOOP FUNCTION BODY, TO ADD CUSTOM DEVICES PLACE YOUR OWN FILES AFTER THIS FILE
// FILES MUST BE CALLED fd[a-z]_loop_<filename>.ino 

#if (defined SIMPLE_PEDESTAL) 
  poll_simple_pedestal();
#endif
#if (defined VRMAXII_PEDESTAL) 
  poll_vrmax2_pedestal();
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
#if (defined COLLECTIVE_NOTHR) 
  poll_coll_nothr();
#endif


