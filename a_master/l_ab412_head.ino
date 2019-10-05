#if (defined AB412_COLL_SWITCH_PANEL)
  Joystick_ j_ab412h(0x13, 0x05, 57, 2, false, false, false, true, true, false, false, false, false, false, false);

  void setup_ab412_coll_head() {
    j_ab412h.begin();
    j_ab412h.setRxAxisRange(AB412_HEAD_LEFT_POT_MIN, AB412_HEAD_LEFT_POT_MAX);
    j_ab412h.setRyAxisRange(AB412_HEAD_RIGHT_POT_MIN, AB412_HEAD_RIGHT_POT_MAX);
    
    if (DCS_HUEY_IDLE_STOP_COMPAT_MODE_ENABLED == 1) {
      Keyboard.begin();
    }
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


    if (COLLECTIVE_MODE_SWITCH_ENABLED == 0) {
      parse_button_array_ab412h(g_coll_modesw_pos_decimal,0,0,0,0,0);
    } else {
      //Serial.println(ms); // unsomment to see decimal vals mode switch positions
      if (g_coll_modesw_pos_decimal == MODESW_POS_MIDDLE_DECIMAL_VAL) {
        mod = 0;
      } else if (g_coll_modesw_pos_decimal == MODESW_POS_LEFT_DECIMAL_VAL) {
        mod = 19;
      } else if (g_coll_modesw_pos_decimal == MODESW_POS_RIGHT_DECIMAL_VAL) {
        mod = 38;
      }
    }
      
    parse_button_array_ab412h(ba0,0,2,0,mod,0);
    parse_button_array_ab412h(ba1,2,0,0,mod,0);
    parse_button_array_ab412h(ba2,10,0,0,mod,1);
    parse_button_array_ab412h(ba3,18,0,0,mod,0);
  }

  void parse_button_array_ab412h(uint8_t b, uint8_t start_btn, uint8_t end_btn, uint8_t byte_offset,uint8_t modifier,bool idle_rel_btn) {
    if (end_btn == 0) {
      end_btn = 8;
    }
    for (byte i = byte_offset; i < end_btn; i++) {
      bool v = (b >> i) & 1;
      
      if (v != g_ab412h_lastButtonState[i + start_btn + modifier]) {
        if (BUTTON_PRESS_ON_THROTTLE_CUTOFF == 1) {
          if ((i != AB412_HEAD_IDLE_REL_BTN - 1) && (idle_rel_btn != 1)){
            j_ab412h.setButton(i + start_btn + modifier, v);     
          } else {
            j_ab412h.setButton(i + start_btn + modifier, v);
            g_idle_rel_btn_pressed = v;
            g_tl_idle_rel_btn_pressed[0] = v;
            g_tl_idle_rel_btn_pressed[1] = v;
          }
        } else {
          j_ab412h.setButton(i + start_btn + modifier, v);
        }
      }
      g_ab412h_lastButtonState[i + start_btn + modifier] = v;
    }
  }


//  void set_thr_latch_state(uint16_t raw_thr) {
//    if (g_idle_rel_btn_pressed == 1) {
//      g_throttle_latch_pressed = 1;
//    }
//    if ((raw_thr > COMPACT_COLLECTIVE_IDLE_DETENT_AXIS_VAL + 100) && (g_idle_rel_btn_pressed == 0)) {
//      g_throttle_latch_pressed = 0;
//    }
//  }
//
//  void apply_advanced_throttle_features (uint16_t raw_thr){
//    set_thr_latch_state(raw_thr);
//    j_ccoll.setThrottle(raw_thr);
//    
//    if (BUTTON_PRESS_ON_THROTTLE_CUTOFF == 1) {
//      if ((raw_thr < (COMPACT_COLLECTIVE_THR_MIN + 15)) && (g_throttle_latch_pressed == 1)) {
//        if (g_physical_latch_button_state != 1) {
//          j_ccoll.setButton(COMPACT_COLLECTIVE_PHYSICAL_LATCH_MOD_JOY_BUTTON - 1, 1);
//          if ((DCS_HUEY_IDLE_STOP_COMPAT_MODE_ENABLED == 1) && (g_coll_modesw_pos_decimal == MODESW_POS_MIDDLE_DECIMAL_VAL)) {
//            Keyboard.press(HUEY_COMPAT_THR_DOWN_KEY);
//            delay(DCS_HUEY_COMPAT_MODE_BUTTON_HOLD);
//            Keyboard.releaseAll();
//          }
//          g_physical_latch_button_state = 1;
//        }
//      } else {
//        if (g_physical_latch_button_state != 0) {
//          j_ccoll.setButton(COMPACT_COLLECTIVE_PHYSICAL_LATCH_MOD_JOY_BUTTON - 1, 0);
//          if ((DCS_HUEY_IDLE_STOP_COMPAT_MODE_ENABLED == 1) && (g_coll_modesw_pos_decimal == MODESW_POS_MIDDLE_DECIMAL_VAL)) {
//            Keyboard.press(HUEY_COMPAT_THR_UP_KEY);
//            delay(DCS_HUEY_COMPAT_MODE_BUTTON_HOLD);
//            Keyboard.releaseAll();
//          }
//          g_physical_latch_button_state = 0;
//        }
//      }
//    }   
//  }

    
#endif
