#if (defined AB412_COLL_SWITCH_PANEL)
  Joystick_ j_ab412h(0x13, 0x05, 59, 2, false, false, false, true, true, false, false, false, false, false, false);

  t_struct_coll_head_attr g_struct_coll_head_attr;

//  typedef struct __attribute__((__packed__)) {
//    uint8_t btn_idle_stop;
//    uint8_t btn_modesw;
//    uint8_t switches_w_middle_btn[]; 
//  } t_struct_coll_head_attr;

  void setup_ab412_coll_head() {
    j_ab412h.begin();
    j_ab412h.setRxAxisRange(AB412_HEAD_LEFT_POT_MIN, AB412_HEAD_LEFT_POT_MAX);
    j_ab412h.setRyAxisRange(AB412_HEAD_RIGHT_POT_MIN, AB412_HEAD_RIGHT_POT_MAX);
    
    if (DCS_HUEY_IDLE_STOP_COMPAT_MODE_ENABLED == 1) {
      Keyboard.begin();
    }
    // now we fill the universal collective head structure
    g_struct_coll_head_attr.btn_idle_stop = AB412_HEAD_IDLE_REL_BTN;
    g_struct_coll_head_attr.btn_modesw = AB412_HEAD_MODESWITCH_BTN;
    g_struct_coll_head_attr.switches_w_middle_btn = AB412_HEAD_SWITCHES_WITH_MIDDLE_BTN;
//    byte sp_sw_arr_size = sizeof(AB412_HEAD_SWITCHES_WITH_MIDDLE_BTN);
//    for (byte i = 0; i<sp_sw_arr_size;i++) {
//      g_struct_ab412_coll_head_attr.switches_w_middle_btn[i] = AB412_HEAD_SWITCHES_WITH_MIDDLE_BTN[i];
//    }
  }
  
  void poll_ab412_coll_head() {
  uint8_t x, y, rx,ry, ba0, ba1, ba2, ba3;
  uint8_t lp = 0;
  uint8_t rp = 0;
  uint8_t mod = 0;
    
    Wire.requestFrom(AB412_HEAD_CONTROLLER_B_I2C_ADDRESS, 7);
  while (Wire.available()) {
    byte b1 = Wire.read(); // receive a byte as character
    byte b2 = Wire.read();
    byte b3 = Wire.read(); // receive a byte as character
    byte b4 = Wire.read();
    byte b5 = Wire.read(); // receive a byte as character
    byte b6 = Wire.read();
    byte b7 = Wire.read();

    x = b2;
    y = b1;
    rx = b4;
    ry = b3;
    lp = b5;
    rp = b6;
    ba0 = b7;
  }

  Wire.requestFrom(AB412_HEAD_CONTROLLER_A_I2C_ADDRESS, 3);
  while (Wire.available()) {
    byte b1 = Wire.read(); // receive a byte as character
    byte b2 = Wire.read();
    byte b3 = Wire.read();

    ba1 = b1;
    ba2 = b2;
    ba3 = b3;
  }

// CALIBRATION

//    Serial.print(rx);
//    Serial.println(" ");
//    Serial.print(ry); // <- turn your throttle to idle stop position to see COMPACT_COLLECTIVE_IDLE_STOP_AXIS_VAL value

    if (AB412_HEAD_POT_STABILIZER_ENABLED) {
      int8_t lpdiff = lp - g_ab412h_lpot_val;
      int8_t rpdiff = rp - g_ab412h_rpot_val;
      if (abs(lpdiff) > AB412_HEAD_POT_STABILIZER_STEP) {
        g_ab412h_lpot_val = lp;
        j_ab412h.setRxAxis(lp);
      }
      if (abs(rpdiff) > AB412_HEAD_POT_STABILIZER_STEP) {
        g_ab412h_rpot_val = rp;
        j_ab412h.setRyAxis(rp);
      }
      lpdiff = lp - g_ab412h_lpot_val;
      rpdiff = rp - g_ab412h_rpot_val;
    } else {
      j_ab412h.setRxAxis(lp);
      j_ab412h.setRyAxis(rp);
    }
    
    int16_t hat0_val = parse_hat_sw(x, y, AB412_COLL_HEAD_LEFT_HAT_DIRECTIONS);
    int16_t hat1_val = parse_hat_sw(rx, ry, AB412_COLL_HEAD_RIGHT_HAT_DIRECTIONS);

    j_ab412h.setHatSwitch(0, hat0_val);
    j_ab412h.setHatSwitch(1, hat1_val);

    bool ab412_btn_state[20]; // we create an array for button values we receive from the head; array size equals physical buttons number + 1
    memset(ab412_btn_state,0,sizeof(ab412_btn_state)); // fill it with 0s as it has random values on creation
    byte sp_sw_arr_size = sizeof(g_struct_coll_head_attr.switches_w_middle_btn);
    bool ab412_special_btn_state[sizeof(g_struct_coll_head_attr.switches_w_middle_btn)]; // another array for special buttons
    memset(ab412_special_btn_state,0,sizeof(ab412_special_btn_state)); // fill it with 0s as it has random values on creation
    
    parse_btn_bytes (ab412_btn_state,ba0,0,2,0); //and then fill it with meaningful values from our bytes
    parse_btn_bytes (ab412_btn_state,ba1,2,0,0);
    parse_btn_bytes (ab412_btn_state,ba2,10,0,0);
    parse_btn_bytes (ab412_btn_state,ba3,18,0,0);

//    BUTTON ARRAY TEST - ALL BUTTON PRESSES SHOULD BE SEEN HERE
//    for (byte i=0;i<sizeof(ab412_btn_state);i++) {
//      
//      Serial.print(i);
//      Serial.print("_");
//      Serial.print(ab412_btn_state[i]);
//      Serial.print(" ");
//    }
//    Serial.println("");
    


    if (COLLECTIVE_MODE_SWITCH_ENABLED == 1) {
      if ((g_coll_mk3_detected == 1) && (g_struct_coll_head_attr.btn_modesw != 0)) {
        extract_modesw_val(ab412_btn_state);
      }
      if (g_coll_modesw_pos_decimal == MODESW_POS_MIDDLE_DECIMAL_VAL) {
        mod = 0;
      } else if (g_coll_modesw_pos_decimal == MODESW_POS_LEFT_DECIMAL_VAL) {
        mod = 19;
      } else if (g_coll_modesw_pos_decimal == MODESW_POS_RIGHT_DECIMAL_VAL) {
        mod = 38;
      }
    }

    // parameters: phys. btn curr state array,phys. btn array size,special btn curr state array, global head btn state arr, mode switch modifier, special btn offset, joystick object
    // parse_btn_array(bool *pb_arr, uint8_t pb_arr_size, bool *sb_arr, bool *b_state_arr, uint8_t modifier, uint8_t sbtn_offset, class Joystick_ &joy)
    
    parse_btn_array(ab412_btn_state,20,ab412_special_btn_state,g_ab412h_lastButtonState,mod,58,j_ab412h);
    
    
      
  }

  

#endif
