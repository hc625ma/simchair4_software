#if (defined COLLECTIVE_STHR_MKIII)
  Joystick_ j_scoll(0x20, 0x05, 1, 1, false, false, true, false, false, false, false, true, false, false, false);

  void setup_single_collective_mk3() {
    g_coll_mk3_detected = 1;
    j_scoll.begin();
    j_scoll.setZAxisRange(SINGLE_COLLECTIVE_MKIII_MIN, SINGLE_COLLECTIVE_MKIII_MAX);
    
    if (COLL_IDLE_DETENT_SUPPORT) {
     j_scoll.setThrottleRange(SINGLE_COLLECTIVE_MKIII_IDLE_DETENT_AXIS_VAL, SINGLE_COLLECTIVE_MKIII_THR_MAX);
    } else {
      j_scoll.setThrottleRange(SINGLE_COLLECTIVE_MKIII_THR_MIN, SINGLE_COLLECTIVE_MKIII_THR_MAX);
    }
    if (DCS_HUEY_IDLE_STOP_COMPAT_MODE_ENABLED == 1) {
      Keyboard.begin();
    }
  }
  
  void poll_single_collective_mk3() {
    uint16_t z;
    uint16_t throttle;
    uint8_t ms = 0;
    uint8_t mod = 0;
    
    Wire.requestFrom(SINGLE_COLLECTIVE_MKIII_I2C_ADDRESS, 4);
    while (Wire.available()) {
      byte b1 = Wire.read(); // receive a byte as character
      byte b2 = Wire.read();
      byte b3 = Wire.read(); // receive a byte as character
      byte b4 = Wire.read();
  
      z = b1;
      z = (z << 8) | b2;
      throttle = b3;
      throttle = (throttle << 8) | b4;
      if (throttle > 60000) {
        throttle = 0;
      }
  
    }


// CALIBRATION
//
//    Serial.print(z);
//    Serial.print(" ");
//    Serial.print(throttle); // <- turn your throttle to idle stop position to see COMPACT_COLLECTIVE_IDLE_STOP_AXIS_VAL value
//    Serial.print(" ");
//    Serial.println(ms); // <- uncomment to see decimal vals for mode switch positions

    j_scoll.setZAxis(z);


    if (THROTTLE_STABILIZER_ENABLED) {
      int16_t thrdiff = throttle - g_scoll_thr_val;
      if (abs(thrdiff) > THR_STEP) {
        g_scoll_thr_val = throttle;
        apply_advanced_throttle_features(throttle);
      }
    } else {
     g_scoll_thr_val = throttle;
     apply_advanced_throttle_features(throttle);
    }

//    if (COLLECTIVE_MODE_SWITCH_ENABLED == 0) {
//      parse_button_array_scoll(ms,0,0);
//    } else {
//      g_coll_modesw_pos_decimal = ms; // share mode switch position with the rest of the hardware
//      if (ms == MODESW_POS_MIDDLE_DECIMAL_VAL) {
//        mod = 0;
//      } else if (ms == MODESW_POS_LEFT_DECIMAL_VAL) {
//        mod = 11;
//      } else if (ms == MODESW_POS_RIGHT_DECIMAL_VAL) {
//        mod = 22;
//      }
//    }
      
  }

  void parse_button_array_scoll(uint8_t b, uint8_t start_btn,uint8_t byte_offset) {
    for (byte i = byte_offset; i < 8; i++) {
      bool v = (b >> i) & 1;
      
      if (v != g_scoll_lastButtonState[i + start_btn]) {
        j_scoll.setButton(i + start_btn, v);
      }
      g_scoll_lastButtonState[i + start_btn] = v;
    }
  }

  void set_thr_latch_state_scoll(uint16_t raw_thr) {
    if (SCOLL_MKIII_PHYSICAL_THROTTLE_LATCH == 1) {
      if (raw_thr > SINGLE_COLLECTIVE_MKIII_IDLE_DETENT_AXIS_VAL) {
        g_throttle_latch_pressed = 0;
      } else if (raw_thr < SINGLE_COLLECTIVE_MKIII_IDLE_DETENT_AXIS_VAL) {
        g_throttle_latch_pressed = 1;
      }
    } else {
      if (g_idle_rel_btn_pressed == 1) {
        g_throttle_latch_pressed = 1;
      }
      if ((raw_thr > SINGLE_COLLECTIVE_MKIII_IDLE_DETENT_AXIS_VAL) && (g_idle_rel_btn_pressed == 0)) {
        g_throttle_latch_pressed = 0;
      }
    } 
  }

  void apply_advanced_throttle_features (uint16_t raw_thr){
    set_thr_latch_state_scoll(raw_thr);
    j_scoll.setThrottle(raw_thr);
    
    if (BUTTON_PRESS_ON_THROTTLE_CUTOFF == 1) {
      if ((raw_thr < (SINGLE_COLLECTIVE_MKIII_THR_MIN + THR_STEP + 2)) && (g_throttle_latch_pressed == 1)) {
        if (g_physical_latch_button_state != 1) {
          j_scoll.setButton(SINGLE_COLLECTIVE_MKIII_PHYSICAL_LATCH_MOD_JOY_BUTTON - 1, 1);
          if ((DCS_HUEY_IDLE_STOP_COMPAT_MODE_ENABLED == 1) && (g_coll_modesw_pos_decimal == MODESW_POS_MIDDLE_DECIMAL_VAL)) {
            Keyboard.press(HUEY_COMPAT_THR_DOWN_KEY);
            delay(DCS_HUEY_COMPAT_MODE_BUTTON_HOLD);
            Keyboard.releaseAll();
          }
          g_physical_latch_button_state = 1;
        }
      } else {
        if (g_physical_latch_button_state != 0) {
          j_scoll.setButton(SINGLE_COLLECTIVE_MKIII_PHYSICAL_LATCH_MOD_JOY_BUTTON - 1, 0);
          if ((DCS_HUEY_IDLE_STOP_COMPAT_MODE_ENABLED == 1) && (g_coll_modesw_pos_decimal == MODESW_POS_MIDDLE_DECIMAL_VAL)) {
            Keyboard.press(HUEY_COMPAT_THR_UP_KEY);
            delay(DCS_HUEY_COMPAT_MODE_BUTTON_HOLD);
            Keyboard.releaseAll();
          }
          g_physical_latch_button_state = 0;
        }
      }
    }   
  }

    
#endif
