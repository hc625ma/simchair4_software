#if ((defined COLLECTIVE_STHR_MKIII) || (defined COLLECTIVE_TTHR))

  void coll_lever_process_poll_results (uint16_t &lvr, uint16_t *thr, uint8_t &ms, class Joystick_ &joy) {

    joy.setZAxis(lvr); // SET LEVER AXIS
    
    if (COLLECTIVE_MODE_SWITCH_ENABLED == 1) {
      g_coll_modesw_pos_decimal = ms; // share mode switch position with the rest of the hardware
    } else {
      coll_lever_modesw_to_joy(ms,joy);
    }
    coll_lever_apply_throttle_stabilizer(thr);
    coll_lever_apply_advanced_throttle_features(thr[0],0,joy);
    if (g_struct_coll_attr.throttles == 2) {
      coll_lever_apply_advanced_throttle_features(thr[1],1,joy);
    }
  }

  void coll_lever_apply_throttle_stabilizer (uint16_t *thr) {
    int16_t thrdiff[g_struct_coll_attr.throttles];
    if (THROTTLE_STABILIZER_ENABLED) {  
      if(g_struct_coll_attr.throttles == 2) {
        thrdiff[0] = thr[0] - g_struct_coll_attr.thr_buf[0];
        thrdiff[1] = thr[1] - g_struct_coll_attr.thr_buf[1];
        if (abs(thrdiff[0]) > THR_STEP) {
          g_struct_coll_attr.thr_buf[0] = thr[0];
          //apply_advanced_throttle_features(throttle0,0);
        }
        if (abs(thrdiff[1]) > THR_STEP) {
          g_struct_coll_attr.thr_buf[1] = thr[1];
          //apply_advanced_throttle_features(throttle1,1);
        }
      } else {
        thrdiff[0] = thr[0] - g_struct_coll_attr.thr_buf[0];
        if (abs(thrdiff[0]) > THR_STEP) {
          g_struct_coll_attr.thr_buf[0] = thr[0];
          //apply_advanced_throttle_features(throttle0,0);
        }
      } 
    } else {
      if(g_struct_coll_attr.throttles == 2) {
        g_struct_coll_attr.thr_buf[0] = thr[0];
        g_struct_coll_attr.thr_buf[1] = thr[1];
      } else {
        g_struct_coll_attr.thr_buf[0] = thr[0]; // FIXME THIS SEEMS TO BE UNNEEDED
      }
    }
  }

  void coll_lever_peripheral_poll (uint16_t &lvr,uint16_t *thr, uint8_t &ms) {
    ms = 0;
    uint8_t i2c_bytes[g_struct_coll_attr.i2c_bytes];
    generic_poll_i2c_device(g_struct_coll_attr.i2c_addr,i2c_bytes,g_struct_coll_attr.i2c_bytes);
    lvr = generic_read_16bit_axis_from_bytes(i2c_bytes[0],i2c_bytes[1]);
    if (g_struct_coll_attr.throttles == 2) {
      thr[0] = generic_read_16bit_axis_from_bytes(i2c_bytes[2],i2c_bytes[3]);
      thr[1] = generic_read_16bit_axis_from_bytes(i2c_bytes[4],i2c_bytes[5]);
      generic_check_16_bit_axis_val(thr[0]);
      generic_check_16_bit_axis_val(thr[1]);
    } else {
      thr[0] = generic_read_16bit_axis_from_bytes(i2c_bytes[2],i2c_bytes[3]);
      generic_check_16_bit_axis_val(thr[0]);
    }
    if (g_struct_coll_attr.base_version == 4) {
      if (g_struct_coll_attr.throttles == 1) {
        ms = i2c_bytes[5];
      } else {
        ms = i2c_bytes[6];
      }
    }        
  }

  void coll_lever_modesw_to_joy (uint8_t ms, class Joystick_ joy) {
    for (byte i = 0; i < 2; i++) {
      bool v = (ms >> i) & 1;      
      if (v != g_struct_coll_attr.lastButtonState[i]) {
        joy.setButton(i, v);
      }
      g_struct_coll_attr.lastButtonState[i] = v;
    }
  }

  void coll_lever_apply_advanced_throttle_features (uint16_t raw_thr, bool thr_num, class Joystick_ &joy){
    coll_lever_set_thr_latch_state(raw_thr, thr_num);
    if (g_struct_coll_attr.throttles == 2) {
      if (thr_num == 0) {
        //joy.setRxAxis(raw_thr);
        joy.setThrottle(raw_thr);
      } else {
        //joy.setRyAxis(raw_thr);
        joy.setRzAxis(raw_thr);
      }
    } else {
      joy.setThrottle(raw_thr);
    }
    if (BUTTON_PRESS_ON_THROTTLE_CUTOFF == 1) {
      uint16_t diff;
      if (raw_thr > g_struct_coll_attr.thr_min[thr_num]) {
        diff = raw_thr - g_struct_coll_attr.thr_min[thr_num];
      } else {
        diff = g_struct_coll_attr.thr_min[thr_num] - raw_thr;
      }
      if ((diff < (THR_STEP + 10)) && (g_struct_coll_attr.thr_latch_pressed[thr_num] == 1)) {
        if (g_struct_coll_attr.phys_thr_latch_btn_state[thr_num] != 1) {
          joy.setButton(g_struct_coll_attr.phys_thr_latch_joy_btn[thr_num] - 1, 1);
          if ((DCS_HUEY_IDLE_STOP_COMPAT_MODE_ENABLED == 1) && (g_coll_modesw_pos_decimal == MODESW_POS_MIDDLE_DECIMAL_VAL)) {
            Keyboard.press(HUEY_COMPAT_THR_DOWN_KEY);
            delay(DCS_HUEY_COMPAT_MODE_BUTTON_HOLD);
            Keyboard.releaseAll();
          }
          g_struct_coll_attr.phys_thr_latch_btn_state[thr_num] = 1;
        }
      } else {
        if (g_struct_coll_attr.phys_thr_latch_btn_state[thr_num] != 0) {
          joy.setButton(g_struct_coll_attr.phys_thr_latch_joy_btn[thr_num] - 1, 0);
          if ((DCS_HUEY_IDLE_STOP_COMPAT_MODE_ENABLED == 1) && (g_coll_modesw_pos_decimal == MODESW_POS_MIDDLE_DECIMAL_VAL)) {
            Keyboard.press(HUEY_COMPAT_THR_UP_KEY);
            delay(DCS_HUEY_COMPAT_MODE_BUTTON_HOLD);
            Keyboard.releaseAll();
          }
          g_struct_coll_attr.phys_thr_latch_btn_state[thr_num] = 0;
        }
      }
    }   
  }  

 void coll_lever_set_thr_latch_state(uint16_t raw_thr, bool thr_num) {
    if (g_idle_rel_btn_pressed_new[thr_num] == 1) {
      g_struct_coll_attr.thr_latch_pressed[thr_num] = 1;
    }
    if (g_struct_coll_attr.thr_min[thr_num] < 500) { // throttle axis is not inverted
      if ((raw_thr > g_struct_coll_attr.idle_detent_axis_val[thr_num]) && (g_idle_rel_btn_pressed_new[thr_num] == 0)) {
        g_struct_coll_attr.thr_latch_pressed[thr_num] = 0;
        if ((raw_thr > g_struct_coll_attr.idle_detent_axis_val[thr_num]) && (g_idle_rel_btn_pressed_new[thr_num] == 0)) {
          g_struct_coll_attr.thr_latch_pressed[thr_num] = 0;
        }
      }    
    } else { // throttle axis is inverted
      if ((raw_thr < g_struct_coll_attr.idle_detent_axis_val[thr_num]) && (g_idle_rel_btn_pressed_new[thr_num] == 0)) {
        g_struct_coll_attr.thr_latch_pressed[thr_num] = 0;
        if ((raw_thr < g_struct_coll_attr.idle_detent_axis_val[thr_num]) && (g_idle_rel_btn_pressed_new[thr_num] == 0)) {
          g_struct_coll_attr.thr_latch_pressed[thr_num] = 0;
        }
      }    
    }  
  }

  void coll_joy_init (class Joystick_ &joy) {
    if (g_struct_coll_attr.base_version == 3) {
      g_coll_mk3_detected = 1;
    }
    joy.begin();
    joy.setZAxisRange(g_struct_coll_attr.lvr_min, g_struct_coll_attr.lvr_max);
        
    if (COLL_IDLE_DETENT_SUPPORT) {
      if (g_struct_coll_attr.throttles == 2) {
        joy.setRxAxisRange(g_struct_coll_attr.idle_detent_axis_val[0], g_struct_coll_attr.thr_max[0]);
        joy.setRyAxisRange(g_struct_coll_attr.idle_detent_axis_val[1], g_struct_coll_attr.thr_max[1]);
      } else {
        joy.setThrottleRange(g_struct_coll_attr.idle_detent_axis_val[0], g_struct_coll_attr.thr_max[0]);
      }
    } else {
      if (g_struct_coll_attr.throttles == 2) {
        joy.setRxAxisRange(g_struct_coll_attr.thr_min[0], g_struct_coll_attr.thr_max[0]);
        joy.setRyAxisRange(g_struct_coll_attr.thr_min[1], g_struct_coll_attr.thr_max[1]);
      } else {
        joy.setThrottleRange(g_struct_coll_attr.thr_min[0], g_struct_coll_attr.thr_max[0]);
      }
    }
    if (DCS_HUEY_IDLE_STOP_COMPAT_MODE_ENABLED == 1) {
      Keyboard.begin();
    }
  }

#endif
