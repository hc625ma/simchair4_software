#if (defined HUEY_COLL_SWITCH_PANEL)
  Joystick_ j_hh(0x17, 0x05, 42, 1, false, false, false, false, false, false, false, false, false, false, false);

  void setup_huey_coll_head() {
    j_hh.begin();    
    if (DCS_HUEY_IDLE_STOP_COMPAT_MODE_ENABLED == 1) {
      Keyboard.begin();
    }
  }
  
  void poll_huey_coll_head() {
    uint8_t x, y, ba0, ba1;
    uint8_t mod = 0;
      
      Wire.requestFrom(HUEY_HEAD_I2C_ADDRESS, 4);
      while (Wire.available()) {
        byte b1 = Wire.read(); // receive a byte as character
        byte b2 = Wire.read();
        byte b3 = Wire.read(); // receive a byte as character
        byte b4 = Wire.read();
    
        y = b1;
        x = b2;
        ba0 = b3;
        ba1 = b4;
      }

// CALIBRATION
//    Serial.print(x);
//    Serial.print(" ");
//    Serial.print(y);
//    Serial.print(" ");
//    Serial.print(ba0);
//    Serial.print(" ");
//    Serial.println(ba1);
    
    int16_t hat0_val = parse_hat_sw(x, y, HUEY_COLL_HEAD_HAT_DIRECTIONS);


    j_hh.setHatSwitch(0, hat0_val);


    if (COLLECTIVE_MODE_SWITCH_ENABLED == 0) {
      parse_button_array_hh(g_coll_modesw_pos_decimal,0,0,0,0,0);
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
      
    parse_button_array_hh(ba0,0,0,0,mod,0);
    parse_button_array_hh(ba1,8,0,0,mod,1);
  }

  void parse_button_array_hh(uint8_t b, uint8_t start_btn, uint8_t end_btn, uint8_t byte_offset,uint8_t modifier,bool idle_rel_btn) {
    if (end_btn == 0) {
      end_btn = 8;
    }
    for (byte i = byte_offset; i < end_btn; i++) {
      bool v = (b >> i) & 1;
      
      if (v != g_hh_lastButtonState[i + start_btn + modifier]) {
        if (BUTTON_PRESS_ON_THROTTLE_CUTOFF == 1) {
          if ((i != HUEY_HEAD_IDLE_REL_BTN - 1) && (idle_rel_btn != 1)){
            j_hh.setButton(i + start_btn + modifier, v);     
          } else {
            j_hh.setButton(i + start_btn + modifier, v);
            g_idle_rel_btn_pressed = v;
          }
        } else {
          j_hh.setButton(i + start_btn + modifier, v);
        }
      }
      g_hh_lastButtonState[i + start_btn + modifier] = v;
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
