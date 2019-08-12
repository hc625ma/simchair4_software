#if (defined COMPACT_COLLECTIVE)
  Joystick_ j_ccoll(0x20, 0x05, 35, 1, false, false, true, false, false, false, false, true, false, false, false);

  void setup_compact_collective() {
    j_ccoll.begin();
    j_ccoll.setZAxisRange(COMPACT_COLLECTIVE_MIN, COMPACT_COLLECTIVE_MAX);
    
    if (COLL_IDLE_DETENT_SUPPORT) {
     j_ccoll.setThrottleRange(COMPACT_COLLECTIVE_IDLE_DETENT_AXIS_VAL, COMPACT_COLLECTIVE_THR_MAX);
    } else {
      j_ccoll.setThrottleRange(COMPACT_COLLECTIVE_THR_MIN, COMPACT_COLLECTIVE_THR_MAX);
    }
    if (DCS_HUEY_IDLE_STOP_COMPAT_MODE_ENABLED == 1) {
      Keyboard.begin();
    }
  }
  
  void poll_compact_collective() {
    uint16_t z;
    uint16_t throttle;
    uint8_t ba0,ba1,ms;
    uint8_t x,y;
    uint8_t mod = 0;
    
    Wire.requestFrom(COMPACT_COLLECTIVE_I2C_ADDRESS, 5);
    while (Wire.available()) {
      byte b1 = Wire.read(); // receive a byte as character
      byte b2 = Wire.read();
      byte b3 = Wire.read(); // receive a byte as character
      byte b4 = Wire.read();
      byte b5 = Wire.read();
  
      z = b1;
      z = (z << 8) | b2;
      throttle = b3;
      throttle = (throttle << 8) | b4;
      ms = b5;
  
    }

    Wire.requestFrom(COMPACT_COLLECTIVE_HEAD_I2C_ADDRESS, 4);
    while (Wire.available()) {
      byte b1 = Wire.read(); // receive a byte as character
      byte b2 = Wire.read();
      byte b3 = Wire.read();
      byte b4 = Wire.read();

      x = b1;
      y = b2;
      ba0 = b3;
      ba1 = b4;
    }
    
    //Serial.print(z);
    //Serial.print(" ");
    //Serial.print(throttle);
    //Serial.print(" ");
    //Serial.print(ba0);
    //Serial.print(" ");
    //Serial.print(ba1);
    //Serial.print(" ");
//    Serial.print(x);
//    Serial.print(" ");
//    Serial.println(y);
    j_ccoll.setZAxis(z);

     // uncomment the next line and turn your throttle to idle stop position to see COMPACT_COLLECTIVE_IDLE_STOP_AXIS_VAL value
    //Serial.println(throttle);
    if (THROTTLE_STABILIZER_ENABLED) {
      int16_t thrdiff = throttle - g_ccoll_thr_val;
      if (abs(thrdiff) > THR_STEP) {
        apply_advanced_throttle_features(throttle);
        g_ccoll_thr_val = throttle;
      }
    } else {
     apply_advanced_throttle_features(throttle);
    }
    

    int16_t hat_val = parse_hat_sw(x, y, 8);
    j_ccoll.setHatSwitch(0, hat_val);

    if (COLLECTIVE_MODE_SWITCH_ENABLED == 0) {
      parse_button_array_ccoll(ms,0,0,0,0);
    } else {
      //Serial.println(ms); // unsomment to see decimal vals mode switch positions
      g_coll_modesw_pos_decimal = ms; // share mode switch position with the rest of the hardware
      if (ms == MODESW_POS_MIDDLE_DECIMAL_VAL) {
        mod = 0;
      } else if (ms == MODESW_POS_LEFT_DECIMAL_VAL) {
        mod = 11;
      } else if (ms == MODESW_POS_RIGHT_DECIMAL_VAL) {
        mod = 22;
      }
    }
      
    parse_button_array_ccoll(ba0,0,3,mod,1);
    parse_button_array_ccoll(ba1,8,0,mod,0);
  }

  void parse_button_array_ccoll(uint8_t b, uint8_t start_btn,uint8_t byte_offset,uint8_t modifier,bool idle_rel_btn) {
    for (byte i = byte_offset; i < 8; i++) {
      bool v = (b >> i) & 1;
      
      if (v != g_ccoll_lastButtonState[i + start_btn + modifier]) {
        if (BUTTON_PRESS_ON_THROTTLE_CUTOFF == 1) {
          if ((i != COMPACT_COLLECTIVE_IDLE_REL_BTN - 1) && (idle_rel_btn != 1)){
            j_ccoll.setButton(i + start_btn + modifier, v);     
          } else {
            g_idle_rel_btn_pressed = v;
          }
        } else {
          j_ccoll.setButton(i + start_btn + modifier, v);
        }
      }
      g_ccoll_lastButtonState[i + start_btn + modifier] = v;
    }
  }


  void set_thr_latch_state(uint16_t raw_thr) {
    if (g_idle_rel_btn_pressed == 1) {
      g_throttle_latch_pressed = 1;
    }
    if ((raw_thr > COMPACT_COLLECTIVE_IDLE_DETENT_AXIS_VAL + 100) && (g_idle_rel_btn_pressed == 0)) {
      g_throttle_latch_pressed = 0;
    }
  }

  void apply_advanced_throttle_features (uint16_t raw_thr){
    set_thr_latch_state(raw_thr);
    
    if (DCS_HUEY_IDLE_STOP_COMPAT_MODE_ENABLED == 1) {

      coll_head_idle_stop_compat_dcs (raw_thr,COMPACT_COLLECTIVE_IDLE_DETENT_AXIS_VAL);
      j_ccoll.setThrottle(raw_thr);
    } else {
      j_ccoll.setThrottle(raw_thr);
    }
    if (BUTTON_PRESS_ON_THROTTLE_CUTOFF == 1) {
      if ((raw_thr < (COMPACT_COLLECTIVE_THR_MIN + 15)) && (g_throttle_latch_pressed == 1)) {
        if (g_physical_latch_button_state != 1) {
          j_ccoll.setButton(COMPACT_COLLECTIVE_PHYSICAL_LATCH_MOD_JOY_BUTTON - 1, 1);
          g_physical_latch_button_state = 1;
        }
      } else {
        if (g_physical_latch_button_state != 0) {
          j_ccoll.setButton(COMPACT_COLLECTIVE_PHYSICAL_LATCH_MOD_JOY_BUTTON - 1, 0);
          g_physical_latch_button_state = 0;
        }
      }
    }   
  }



  void coll_head_idle_stop_compat_dcs (uint16_t thr,uint16_t idle_detent_val) {
    
    int16_t thrdiff = thr - g_ccoll_thr_val;
    if ((thr < idle_detent_val) && (g_idle_rel_btn_pressed == 1)) {
      if (abs(thrdiff) > THR_STEP)
      {
        Keyboard.press(HUEY_COMPAT_THR_DOWN_KEY);
        delay(DCS_HUEY_COMPAT_MODE_BUTTON_HOLD);
        Keyboard.releaseAll();
      }
    } else if (g_throttle_latch_pressed == 1) {
      if (abs(thrdiff) > THR_STEP) {
        Keyboard.press(HUEY_COMPAT_THR_UP_KEY);
        delay(DCS_HUEY_COMPAT_MODE_BUTTON_HOLD);
        Keyboard.releaseAll();
      }
    }
  }
    
#endif
