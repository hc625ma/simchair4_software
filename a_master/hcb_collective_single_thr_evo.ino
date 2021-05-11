#if (defined COLLECTIVE_STHR_EVO)
  //CREATE JOYSTICK OBJECT AND GLOBAL COLLECTIVE STRUCT
  Joystick_ j_scoll(0x20, 0x05, 7, 0, false, false, true, true, false, true, false, true, false, false, false);
  t_struct_coll_attr g_struct_coll_attr;

  //UNCOMMENT THE NEXT LINE TO SET UP CONFIGURATION VALUES
  //#define CALIBRATE_SCOLL_EVO

  // ADJUST THESE VALUES FOR CALIBRATION
    #define COLL_EVO_ENABLE_MAP_FUNCTION 1 // SET TO 0 TO SEE RAW VALUES
    #define COLL_EVO_PHYS_Z_MIN 0
    #define COLL_EVO_PHYS_Z_MAX 1000
    #define COLL_EVO_PHYS_THR0_MIN 149
    #define COLL_EVO_PHYS_THR0_MAX 703
    #define COLL_EVO_PHYS_THR0_MAX_MINUS_PHYS_THR0_MIN 559 //  PHYSICAL THR MAX MINUS THR MIN

  void setup_single_collective_evo() {

    //POPULATE THE GLOBAL STRUCT WITH VALUES FROM CONFIGURATION HERE
    g_struct_coll_attr.i2c_addr = SINGLE_COLLECTIVE_EVO_I2C_ADDRESS;
    g_struct_coll_attr.i2c_bytes = 5; // we will read them locally but still need that here for compatibility
    g_struct_coll_attr.base_version = 5;
    g_struct_coll_attr.throttles = 1;
    g_struct_coll_attr.lvr_max = SINGLE_COLLECTIVE_EVO_MAX;
    g_struct_coll_attr.lvr_min = SINGLE_COLLECTIVE_EVO_MIN;
    g_struct_coll_attr.thr_max[0] = SINGLE_COLLECTIVE_EVO_THR_MAX;
    g_struct_coll_attr.thr_min[0] = SINGLE_COLLECTIVE_EVO_THR_MIN;
    g_struct_coll_attr.thr_max[1] = SINGLE_COLLECTIVE_EVO_THR_MAX;
    g_struct_coll_attr.thr_min[1] = SINGLE_COLLECTIVE_EVO_THR_MIN;
    g_struct_coll_attr.thr_max[2] = SINGLE_COLLECTIVE_EVO_THR_MAX;
    g_struct_coll_attr.thr_min[2] = SINGLE_COLLECTIVE_EVO_THR_MIN;
    g_struct_coll_attr.idle_detent_axis_val[0] = SINGLE_COLLECTIVE_EVO_IDLE_DETENT_AXIS_VAL;
    g_struct_coll_attr.idle_detent_axis_val[1] = SINGLE_COLLECTIVE_EVO_IDLE_DETENT_AXIS_VAL;
    g_struct_coll_attr.idle_detent_axis_val[2] = SINGLE_COLLECTIVE_EVO_IDLE_DETENT_AXIS_VAL;
    g_struct_coll_attr.phys_thr_latch_joy_btn[0] = SINGLE_COLLECTIVE_EVO_PHYSICAL_LATCH_MOD_JOY_BUTTON_1;
    g_struct_coll_attr.phys_thr_latch_joy_btn[1] = SINGLE_COLLECTIVE_EVO_PHYSICAL_LATCH_MOD_JOY_BUTTON_2;
    g_struct_coll_attr.phys_thr_latch_joy_btn[2] = SINGLE_COLLECTIVE_EVO_PHYSICAL_LATCH_MOD_JOY_BUTTON_3;
    g_struct_coll_attr.phys_thr_latch =  SINGLE_COLLECTIVE_EVO_PHYSICAL_THROTTLE_LATCH;
    g_struct_coll_attr.lastButtonState[10]; // modesw as 2 buttons, idle rel button

    if ((COLLECTIVE_EVO_MODE_SWITCH_THR_ENABLED == 1)) {
      g_struct_coll_attr.throttles = 3;
    }

    //INITIALIZE JOYSTICK
    coll_joy_init(j_scoll);
    //coll_base_usb_pins_init();
    
  }
 
  void poll_single_collective_evo() {
    uint16_t lvr;    // LEVER VALUE
    uint16_t thr[3]; // THROTTLE VALUES ARE ALWAYS STORED IN AN ARRAY FOR UNIFICATION, SINGLE THR IS AN ARRAY OF 1 ELEMENT
    uint8_t ms = 0;  // MODE SWITCH VALUE
    uint8_t ms_thr = 0;  // MODE SWITCH THROTTLE VALUE

    coll_lever_evo_poll(lvr,thr,ms,ms_thr); // POLL THE LEVER
    
    #if (defined CALIBRATE_SCOLL_EVO)
      Serial.print("Lever:");
      Serial.print(" ");
      Serial.print(lvr);
      Serial.print(" ");
      Serial.print("Throttle: ");
      Serial.print(" 1 ");
      Serial.print(thr[0]);
      Serial.print(" 2 ");
      Serial.print(thr[1]);
      Serial.print(" 3 ");
      Serial.print(thr[2]);
      Serial.print(" ");
      Serial.print(ms);
      Serial.print(" ");
      Serial.print(ms_thr);
      Serial.println(" ");
    #endif
  
    coll_lever_evo_process_poll_results(lvr,thr,ms,ms_thr,j_scoll);  //PROCESS VALUES WE GOT FROM THE LEVER (SEE SHARED FUNCTIONS TAB)
  }  


#endif
