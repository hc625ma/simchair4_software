#if (defined COLLECTIVE_TTHR)
  //CREATE JOYSTICK OBJECT AND GLOBAL COLLECTIVE STRUCT
  Joystick_ j_tcoll(0x20, 0x05, 4, 0, false, false, true, false, false, true, false, true, false, false, false);
  t_struct_coll_attr g_struct_coll_attr;

  //UNCOMMENT THE NEXT LINE TO SET UP CONFIGURATION VALUES
  //#define CALIBRATE_TCOLL

  void setup_twin_collective() {

    //POPULATE THE GLOBAL STRUCT WITH VALUES FROM CONFIGURATION HERE
    g_struct_coll_attr.i2c_addr = TWIN_COLLECTIVE_I2C_ADDRESS;
    g_struct_coll_attr.i2c_bytes = 7; // see Wire.write call of requestEvent sub in peripheral's firmware
    g_struct_coll_attr.base_version = 4;
    g_struct_coll_attr.throttles = 2;
    g_struct_coll_attr.lvr_max = TWIN_COLLECTIVE_MAX;
    g_struct_coll_attr.lvr_min = TWIN_COLLECTIVE_MIN;
    g_struct_coll_attr.thr_max[0] = TWIN_COLLECTIVE_THR1_MAX;
    g_struct_coll_attr.thr_min[0] = TWIN_COLLECTIVE_THR1_MIN;
    g_struct_coll_attr.thr_max[1] = TWIN_COLLECTIVE_THR2_MAX;
    g_struct_coll_attr.thr_min[1] = TWIN_COLLECTIVE_THR2_MIN;
    g_struct_coll_attr.idle_detent_axis_val[0] = TWIN_COLLECTIVE_IDLE_DETENT1_AXIS_VAL;
    g_struct_coll_attr.phys_thr_latch_joy_btn[0] = TWIN_COLLECTIVE_PHYSICAL_LATCH_MOD_JOY_BUTTON1;
    g_struct_coll_attr.idle_detent_axis_val[1] = TWIN_COLLECTIVE_IDLE_DETENT2_AXIS_VAL;
    g_struct_coll_attr.phys_thr_latch_joy_btn[1] = TWIN_COLLECTIVE_PHYSICAL_LATCH_MOD_JOY_BUTTON2;
    g_struct_coll_attr.lastButtonState[4]; // modesw as 2 buttons, 2 idle rel buttons

    //INITIALIZE JOYSTICK
    coll_joy_init(j_tcoll);
  }
 
  void poll_twin_collective() {
    uint16_t lvr;    // LEVER VALUE
    uint16_t thr[2]; // THROTTLE VALUES ARE ALWAYS STORED IN AN ARRAY FOR UNIFICATION, SINGLE THR IS AN ARRAY OF 1 ELEMENT
    uint8_t ms = 0;  // MODE SWITCH VALUE

    coll_lever_peripheral_poll(lvr,thr,ms); // POLL THE LEVER
    
    #if (defined CALIBRATE_TCOLL)
      Serial.print("Lever:");
      Serial.print(" ");
      Serial.print(lvr);
      Serial.print(" ");
      Serial.print("Throttle1:");
      Serial.print(" ");
      Serial.print(thr[0]);
      Serial.print(" ");
      Serial.print("Throttle2:");
      Serial.print(" ");
      Serial.print(thr[1]);
      Serial.println(" ");
    #endif
  
    coll_lever_process_poll_results(lvr,thr,ms,j_tcoll);  //PROCESS VALUES WE GOT FROM THE LEVER (SEE SHARE FUNCTIONS TAB)
  }  
#endif
