#if (defined COLLECTIVE_STHR_MKIII)
  //CREATE JOYSTICK OBJECT AND GLOBAL COLLECTIVE STRUCT
  Joystick_ j_scoll(0x20, 0x05, 1, 0, false, false, true, false, false, false, false, true, false, false, false);
  t_struct_coll_attr g_struct_coll_attr;

  //UNCOMMENT THE NEXT LINE TO SET UP CONFIGURATION VALUES
  //#define CALIBRATE_SCOLL_MKIII

  void setup_single_collective_mk3() {

    //POPULATE THE GLOBAL STRUCT WITH VALUES FROM CONFIGURATION HERE
    g_struct_coll_attr.i2c_addr = SINGLE_COLLECTIVE_MKIII_I2C_ADDRESS;
    g_struct_coll_attr.i2c_bytes = 4; // see Wire.write call of requestEvent sub in peripheral's firmware
    g_struct_coll_attr.base_version = 3;
    g_struct_coll_attr.throttles = 1;
    g_struct_coll_attr.lvr_max = SINGLE_COLLECTIVE_MKIII_MAX;
    g_struct_coll_attr.lvr_min = SINGLE_COLLECTIVE_MKIII_MIN;
    g_struct_coll_attr.thr_max[0] = SINGLE_COLLECTIVE_MKIII_THR_MAX;
    g_struct_coll_attr.thr_min[0] = SINGLE_COLLECTIVE_MKIII_THR_MIN;
    g_struct_coll_attr.idle_detent_axis_val[0] = SINGLE_COLLECTIVE_MKIII_IDLE_DETENT_AXIS_VAL;
    g_struct_coll_attr.phys_thr_latch_joy_btn[0] = SINGLE_COLLECTIVE_MKIII_PHYSICAL_LATCH_MOD_JOY_BUTTON;
    g_struct_coll_attr.lastButtonState[3]; // modesw as 2 buttons, idle rel button

    //INITIALIZE JOYSTICK
    coll_joy_init(j_scoll);
  }
 
  void poll_single_collective_mk3() {
    uint16_t lvr;    // LEVER VALUE
    uint16_t thr[1]; // THROTTLE VALUES ARE ALWAYS STORED IN AN ARRAY FOR UNIFICATION, SINGLE THR IS AN ARRAY OF 1 ELEMENT
    uint8_t ms = 0;  // MODE SWITCH VALUE

    coll_lever_peripheral_poll(lvr,thr,ms); // POLL THE LEVER
    
    #if (defined CALIBRATE_SCOLL_MKIII)
      Serial.print("Lever:");
      Serial.print(" ");
      Serial.print(lvr);
      Serial.print(" ");
      Serial.print("Throttle:");
      Serial.print(" ");
      Serial.print(thr[0]);
      Serial.println(" ");
    #endif
  
    coll_lever_process_poll_results(lvr,thr,ms,j_scoll);  //PROCESS VALUES WE GOT FROM THE LEVER (SEE SHARE FUNCTIONS TAB)
  }  
#endif
