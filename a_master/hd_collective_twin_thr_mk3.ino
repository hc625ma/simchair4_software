#if (defined COLLECTIVE_TTHR_MKIII)
  Joystick_ j_tcoll(0x20, 0x05, 2, 1, false, false, true, true, true, false, false, false, false, false, false);

  void setup_twin_collective_mk3() {
    g_coll_mk3_detected = 1;
    j_tcoll.begin();
    j_tcoll.setZAxisRange(TWIN_COLLECTIVE_MKIII_MIN, TWIN_COLLECTIVE_MKIII_MAX);
    
    if (COLL_IDLE_DETENT_SUPPORT) {
     j_tcoll.setRxAxisRange(TWIN_COLLECTIVE_MKIII_THR_IDLE_DETENT_AXIS_VAL[0], TWIN_COLLECTIVE_MKIII_THR_MAX[0]);
     j_tcoll.setRyAxisRange(TWIN_COLLECTIVE_MKIII_THR_IDLE_DETENT_AXIS_VAL[1], TWIN_COLLECTIVE_MKIII_THR_MAX[1]);
    } else {
      j_tcoll.setRxAxisRange(TWIN_COLLECTIVE_MKIII_THR_MIN[0], TWIN_COLLECTIVE_MKIII_THR_MAX[0]);
      j_tcoll.setRyAxisRange(TWIN_COLLECTIVE_MKIII_THR_MIN[1], TWIN_COLLECTIVE_MKIII_THR_MAX[1]);
    }
    if (DCS_HUEY_IDLE_STOP_COMPAT_MODE_ENABLED == 1) {
      Keyboard.begin();
    }
  }
  
  void poll_twin_collective_mk3() {
    uint16_t z,rz;
    uint16_t throttle0;
    uint16_t throttle1;
    uint8_t ms = 0;
    uint8_t mod = 0;
    
    Wire.requestFrom(TWIN_COLLECTIVE_MKIII_I2C_ADDRESS, 6);
    while (Wire.available()) {
      byte b1 = Wire.read(); // receive a byte as character
      byte b2 = Wire.read();
      byte b3 = Wire.read(); // receive a byte as character
      byte b4 = Wire.read();
      byte b5 = Wire.read(); // receive a byte as character
      byte b6 = Wire.read();
  
      z = b1;
      z = (z << 8) | b2;
      throttle0 = b3;
      throttle0 = (throttle0 << 8) | b4;
      if (throttle0 > 60000) {
        throttle0 = 0;
      }
      throttle1 = b5;
      throttle1 = (throttle1 << 8) | b6;
      if (throttle1 > 60000) {
        throttle1 = 0;
      }
  
    }


// CALIBRATION
//
//    Serial.print(z);
//    Serial.print(" ");
//    Serial.println(throttle0); // <- turn your throttle to idle stop position to see COMPACT_COLLECTIVE_IDLE_STOP_AXIS_VAL value
//    Serial.print(" ");
//    Serial.print(throttle1);
//    Serial.print(" ");
//    Serial.println(ms); // <- uncomment to see decimal vals for mode switch positions

    j_tcoll.setZAxis(z);

    if (THROTTLE_STABILIZER_ENABLED) {
      int16_t thrdiff0 = throttle0 - g_tcoll_thr_val[0];
      int16_t thrdiff1 = throttle1 - g_tcoll_thr_val[1];
      if (abs(thrdiff0) > THR_STEP) {
        g_tcoll_thr_val[0] = throttle0;
        apply_advanced_throttle_features(throttle0,0);
      }
      if (abs(thrdiff1) > THR_STEP) {
        g_tcoll_thr_val[1] = throttle1;
        apply_advanced_throttle_features(throttle1,1);
      }
    } else {
     g_tcoll_thr_val[0] = throttle0;
     apply_advanced_throttle_features(throttle0,0);
     g_tcoll_thr_val[1] = throttle1;
     apply_advanced_throttle_features(throttle1,1);
    }  
  }

  void parse_button_array_tcoll(uint8_t b, uint8_t start_btn,uint8_t byte_offset) {
    for (byte i = byte_offset; i < 8; i++) {
      bool v = (b >> i) & 1;
      
      if (v != g_tcoll_lastButtonState[i + start_btn]) {
        j_tcoll.setButton(i + start_btn, v);
      }
      g_tcoll_lastButtonState[i + start_btn] = v;
    }
  }

  void set_thr_latch_state_tcoll(uint16_t raw_thr, bool thr_num) {
    
      if (g_tl_idle_rel_btn_pressed[thr_num] == 1) {
        g_tl_throttle_latch_pressed[thr_num] = 1;
      }
      if (TWIN_COLLECTIVE_MKIII_THR_MIN[thr_num] < 500) { // throttle axis is not inverted
        if ((raw_thr > TWIN_COLLECTIVE_MKIII_THR_IDLE_DETENT_AXIS_VAL[thr_num]) && (g_tl_idle_rel_btn_pressed[thr_num] == 0)) {
          g_tl_throttle_latch_pressed[thr_num] = 0;
          if ((raw_thr > TWIN_COLLECTIVE_MKIII_THR_IDLE_DETENT_AXIS_VAL[thr_num]) && (g_tl_idle_rel_btn_pressed[thr_num] == 0)) {
            g_tl_throttle_latch_pressed[thr_num] = 0;
          }
        }    
      } else { // throttle axis is inverted
        if ((raw_thr < TWIN_COLLECTIVE_MKIII_THR_IDLE_DETENT_AXIS_VAL[thr_num]) && (g_tl_idle_rel_btn_pressed[thr_num] == 0)) {
          g_tl_throttle_latch_pressed[thr_num] = 0;
          if ((raw_thr < TWIN_COLLECTIVE_MKIII_THR_IDLE_DETENT_AXIS_VAL[thr_num]) && (g_tl_idle_rel_btn_pressed[thr_num] == 0)) {
            g_tl_throttle_latch_pressed[thr_num] = 0;
          }
        }    
      }  
  }

  void apply_advanced_throttle_features (uint16_t raw_thr, bool thr_num){
    set_thr_latch_state_tcoll(raw_thr,thr_num);

    if (thr_num == 0) {
      j_tcoll.setRxAxis(raw_thr);
    } else {
      j_tcoll.setRyAxis(raw_thr);
    }
    
    
    if (BUTTON_PRESS_ON_THROTTLE_CUTOFF == 1) {
      uint16_t diff = TWIN_COLLECTIVE_MKIII_THR_MIN[thr_num] - raw_thr;
      diff = abs(diff);
      //if ((raw_thr < (TWIN_COLLECTIVE_MKIII_THR0_MIN + THR_STEP + 2)) && (g_tl_throttle_latch_pressed[thr_num] == 1)) {
      if ((diff < (THR_STEP + 2)) && (g_tl_throttle_latch_pressed[thr_num] == 1)) {
        if (g_tl_physical_latch_button_state[thr_num] != 1) {
          j_tcoll.setButton(TWIN_COLLECTIVE_MKIII_PHYSICAL_LATCH_MOD_JOY_BUTTON[thr_num] - 1, 1);
          if ((DCS_HUEY_IDLE_STOP_COMPAT_MODE_ENABLED == 1) && (g_coll_modesw_pos_decimal == MODESW_POS_MIDDLE_DECIMAL_VAL) && (thr_num == 1)) {
            Keyboard.press(HUEY_COMPAT_THR_DOWN_KEY);
            delay(DCS_HUEY_COMPAT_MODE_BUTTON_HOLD);
            Keyboard.releaseAll();
          }
          g_tl_physical_latch_button_state[thr_num] = 1;
        }
      } else {
        if (g_tl_physical_latch_button_state[thr_num] != 0) {
          j_tcoll.setButton(TWIN_COLLECTIVE_MKIII_PHYSICAL_LATCH_MOD_JOY_BUTTON[thr_num] - 1, 0);
          if ((DCS_HUEY_IDLE_STOP_COMPAT_MODE_ENABLED == 1) && (g_coll_modesw_pos_decimal == MODESW_POS_MIDDLE_DECIMAL_VAL) && (thr_num == 1)) {
            Keyboard.press(HUEY_COMPAT_THR_UP_KEY);
            delay(DCS_HUEY_COMPAT_MODE_BUTTON_HOLD);
            Keyboard.releaseAll();
          }
          g_tl_physical_latch_button_state[thr_num] = 0;
        }
      }
    }   
  }

    
#endif
