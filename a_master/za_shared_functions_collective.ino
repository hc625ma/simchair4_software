#if ((defined COLLECTIVE_STHR_MKIII) || (defined COLLECTIVE_TTHR) || (defined COLLECTIVE_STHR_EVO_USB) || (defined COLLECTIVE_STHR_EVO) || (defined SIMPLE_COLLECTIVE_SE_USB) || (defined SIMPLE_COLLECTIVE_SE_I2C))

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

  void coll_lever_evo_process_poll_results (uint16_t &lvr, uint16_t *thr, uint8_t &ms, uint8_t &ms_thr, class Joystick_ &joy) {

    joy.setZAxis(lvr); // SET LEVER AXIS
    
    if (COLLECTIVE_MODE_SWITCH_ENABLED == 1) {
      g_coll_modesw_pos_decimal = ms; // share mode switch position with the rest of the hardware
    } else {
      coll_lever_evo_modesw_to_joy(ms,0,joy);
    }
    if (COLLECTIVE_EVO_MODE_SWITCH_THR_ENABLED == 1) {
      g_coll_modesw_thr_pos_decimal = ms_thr; // share mode switch position with the rest of the hardware
    } else {
      coll_lever_evo_modesw_to_joy(ms_thr,2,joy);
    }
    coll_lever_apply_throttle_stabilizer(thr);
    coll_lever_apply_advanced_throttle_features(thr[0],0,joy);
    if (g_struct_coll_attr.throttles >= 2) {
      coll_lever_apply_advanced_throttle_features(thr[1],1,joy);
      coll_lever_apply_advanced_throttle_features(thr[2],2,joy);
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

  void coll_lever_apply_throttle_stabilizer (uint16_t *thr) {
    //int16_t thrdiff[g_struct_coll_attr.throttles];
    int16_t thrdiff[3];
    if (THROTTLE_STABILIZER_ENABLED) {  
      if(g_struct_coll_attr.throttles >= 2) {
        thrdiff[0] = thr[0] - g_struct_coll_attr.thr_buf[0];
        thrdiff[1] = thr[1] - g_struct_coll_attr.thr_buf[1];
        thrdiff[2] = thr[2] - g_struct_coll_attr.thr_buf[2];
        if (abs(thrdiff[0]) > THR_STEP) {
          g_struct_coll_attr.thr_buf[0] = thr[0];
          //apply_advanced_throttle_features(throttle0,0);
        }
        if (abs(thrdiff[1]) > THR_STEP) {
          g_struct_coll_attr.thr_buf[1] = thr[1];
          //apply_advanced_throttle_features(throttle1,1);
        }
        if (abs(thrdiff[2]) > THR_STEP) {
          g_struct_coll_attr.thr_buf[2] = thr[2];
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
      if(g_struct_coll_attr.throttles >= 2) {
        g_struct_coll_attr.thr_buf[0] = thr[0];
        g_struct_coll_attr.thr_buf[1] = thr[1];
        g_struct_coll_attr.thr_buf[2] = thr[2];
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

//  void coll_lever_evo_peripheral_poll (uint16_t &lvr,uint16_t *thr, uint8_t &ms, uint8_t &ms_thr) {
//    ms = 0;
//    ms_thr = 0;
//    uint8_t i2c_bytes[g_struct_coll_attr.i2c_bytes];
//    generic_poll_i2c_device(g_struct_coll_attr.i2c_addr,i2c_bytes,g_struct_coll_attr.i2c_bytes);
//    lvr = generic_read_16bit_axis_from_bytes(i2c_bytes[0],i2c_bytes[1]);
//    
//    if (g_struct_coll_attr.throttles == 2) {
//      thr[0] = generic_read_16bit_axis_from_bytes(i2c_bytes[2],i2c_bytes[3]);
//      thr[1] = generic_read_16bit_axis_from_bytes(i2c_bytes[4],i2c_bytes[5]);
//      generic_check_16_bit_axis_val(thr[0]);
//      generic_check_16_bit_axis_val(thr[1]);
//    } else if (g_struct_coll_attr.throttles == 3) {
//      thr[0] = generic_read_16bit_axis_from_bytes(i2c_bytes[2],i2c_bytes[3]);
//      generic_check_16_bit_axis_val(thr[0]);
//      thr[1] = thr[0];
//      thr[2] = thr[0];
//    } else {
//      thr[0] = generic_read_16bit_axis_from_bytes(i2c_bytes[2],i2c_bytes[3]);
//      generic_check_16_bit_axis_val(thr[0]);
//    }
//    
////    if (g_struct_coll_attr.base_version == 4) {
////      if (g_struct_coll_attr.throttles == 1) {
////        ms = i2c_bytes[5];
////      } else {
////        ms = i2c_bytes[6];
////      }
////    }   
//  }

  void coll_lever_evo_modesw_to_joy (uint8_t m, uint8_t btn_offset, class Joystick_ joy) {
    for (int i = 0; i < 2; i++) {
      bool v = (m >> i) & 1;
      if (v != g_struct_coll_attr.lastButtonState[i+btn_offset]) {
        joy.setButton(i+btn_offset, v);
      }
      g_struct_coll_attr.lastButtonState[i+btn_offset] = v;
    }
  }

  void coll_lever_apply_advanced_throttle_features (uint16_t raw_thr, uint8_t thr_num, class Joystick_ &joy){
    coll_lever_set_thr_latch_state(raw_thr, thr_num);
    if (g_struct_coll_attr.throttles >= 2) {
      if (thr_num == 0) {
        //joy.setRxAxis(raw_thr);
        joy.setThrottle(raw_thr);
      } else if (thr_num == 1) {
        //joy.setRyAxis(raw_thr);
        joy.setRzAxis(raw_thr);
      } else if (thr_num == 2) {
        //joy.setRxAxis(raw_thr);
        joy.setRxAxis(raw_thr);
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
      //Serial.println(g_struct_coll_attr.thr_latch_pressed[2]);
      if ((diff < (THR_STEP + 10)) && ((g_struct_coll_attr.thr_latch_pressed[thr_num] == 1) || (g_struct_coll_attr.phys_thr_latch == 1))) {
        if (g_struct_coll_attr.phys_thr_latch_btn_state[thr_num] != 1) {
          
          joy.setButton(g_struct_coll_attr.phys_thr_latch_joy_btn[thr_num] - 1, 1);
          if ((DCS_HUEY_IDLE_STOP_COMPAT_MODE_ENABLED == 1) && (g_coll_modesw_pos_decimal == MODESW_POS_MIDDLE_DECIMAL_VAL) && (thr_num == 0)) {
            Keyboard.press(HUEY_COMPAT_THR_DOWN_KEY);
            delay(DCS_HUEY_COMPAT_MODE_BUTTON_HOLD);
            Keyboard.releaseAll();
          }
          g_struct_coll_attr.phys_thr_latch_btn_state[thr_num] = 1;
        }
      } else {
        if (g_struct_coll_attr.phys_thr_latch_btn_state[thr_num] != 0) {
          joy.setButton(g_struct_coll_attr.phys_thr_latch_joy_btn[thr_num] - 1, 0);
          if ((DCS_HUEY_IDLE_STOP_COMPAT_MODE_ENABLED == 1) && (g_coll_modesw_pos_decimal == MODESW_POS_MIDDLE_DECIMAL_VAL) && (thr_num == 0)) {
            Keyboard.press(HUEY_COMPAT_THR_UP_KEY);
            delay(DCS_HUEY_COMPAT_MODE_BUTTON_HOLD);
            Keyboard.releaseAll();
          }
          g_struct_coll_attr.phys_thr_latch_btn_state[thr_num] = 0;
        }
      }
    }   
  }  

 void coll_lever_set_thr_latch_state(uint16_t raw_thr, uint8_t thr_num) {
  //Serial.println(thr_num);
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
      if (g_struct_coll_attr.throttles >= 2) {
//        joy.setRxAxisRange(g_struct_coll_attr.idle_detent_axis_val[0], g_struct_coll_attr.thr_max[0]);
//        joy.setRyAxisRange(g_struct_coll_attr.idle_detent_axis_val[1], g_struct_coll_attr.thr_max[1]);
          joy.setThrottleRange(g_struct_coll_attr.idle_detent_axis_val[0], g_struct_coll_attr.thr_max[0]);
          joy.setRzAxisRange(g_struct_coll_attr.idle_detent_axis_val[1], g_struct_coll_attr.thr_max[1]);
          joy.setRxAxisRange(g_struct_coll_attr.idle_detent_axis_val[2], g_struct_coll_attr.thr_max[2]);
          joy.setThrottle(g_struct_coll_attr.idle_detent_axis_val[0]);
          joy.setRzAxis(g_struct_coll_attr.idle_detent_axis_val[1]);
          joy.setRxAxis(g_struct_coll_attr.idle_detent_axis_val[2]);
          g_coll_evo_thr_buf[0] = g_struct_coll_attr.idle_detent_axis_val[0];
          g_coll_evo_thr_buf[1] = g_struct_coll_attr.idle_detent_axis_val[1];
          g_coll_evo_thr_buf[2] = g_struct_coll_attr.idle_detent_axis_val[2];
      } else {
        joy.setThrottleRange(g_struct_coll_attr.idle_detent_axis_val[0], g_struct_coll_attr.thr_max[0]);
        g_struct_coll_attr.thr_buf[0] = g_struct_coll_attr.idle_detent_axis_val[0];
        joy.setThrottle(g_struct_coll_attr.idle_detent_axis_val[0]);
      }
    } else {
      if (g_struct_coll_attr.throttles >= 2) {
        //joy.setRxAxisRange(g_struct_coll_attr.thr_min[0], g_struct_coll_attr.thr_max[0]);
        //joy.setRyAxisRange(g_struct_coll_attr.thr_min[1], g_struct_coll_attr.thr_max[1]);
        joy.setThrottleRange(g_struct_coll_attr.thr_min[0], g_struct_coll_attr.thr_max[0]);
        joy.setRzAxisRange(g_struct_coll_attr.thr_min[1], g_struct_coll_attr.thr_max[1]);
        joy.setRxAxisRange(g_struct_coll_attr.thr_min[2], g_struct_coll_attr.thr_max[2]);
        g_struct_coll_attr.thr_buf[0] = g_struct_coll_attr.thr_min[0];
        g_struct_coll_attr.thr_buf[1] = g_struct_coll_attr.thr_min[1];
        g_struct_coll_attr.thr_buf[2] = g_struct_coll_attr.thr_min[2];
        joy.setThrottle(g_struct_coll_attr.thr_min[0]);
        joy.setRzAxis(g_struct_coll_attr.thr_min[1]);
        joy.setRxAxis(g_struct_coll_attr.thr_min[2]);
      } else {
        joy.setThrottleRange(g_struct_coll_attr.thr_min[0], g_struct_coll_attr.thr_max[0]);
        g_struct_coll_attr.thr_buf[0] = g_struct_coll_attr.thr_min[0];
        joy.setThrottle(g_struct_coll_attr.thr_min[0]);
      }
    }
    if (DCS_HUEY_IDLE_STOP_COMPAT_MODE_ENABLED == 1) {
      Keyboard.begin();
    }
    
  }
#endif

#if ((defined COLLECTIVE_STHR_EVO) || (defined SIMPLE_COLLECTIVE_SE) || (defined SIMPLE_COLLECTIVE_SE_I2C))

  uint8_t coll_evo_read_modeswitch(uint8_t b) {
    
    b &= ~(1 << 2);
    b &= ~(1 << 3); 
    uint8_t ms = b;
    //printBits(b);
    return ms;
  }
  uint8_t coll_evo_read_modeswitch_thr(uint8_t b) {
    uint8_t ms_thr = b >> 2;
    //printBits(ms_thr);
    return ms_thr;
  }
  

  void coll_lever_evo_poll (uint16_t &lvr,uint16_t *thr, uint8_t &ms, uint8_t &ms_thr) {
    //ms = 0;
    //ms_thr = 0;
    uint8_t i2c_bytes[5];//[g_struct_coll_attr.i2c_bytes];
    uint16_t raw_thr0,raw_thr1,raw_thr2;
    
    //coll_evo_usb_fill_i2c_bytes_arr(i2c_bytes); 
    generic_poll_i2c_device(g_struct_coll_attr.i2c_addr,i2c_bytes,g_struct_coll_attr.i2c_bytes);
    lvr = generic_read_16bit_axis_from_bytes(i2c_bytes[0],i2c_bytes[1]);
    ms = coll_evo_read_modeswitch(i2c_bytes[4]);
    ms_thr = coll_evo_read_modeswitch_thr(i2c_bytes[4]);
    if ((COLLECTIVE_EVO_MODE_SWITCH_THR_ENABLED == 1)) {
      //g_struct_coll_attr.throttles = 2;
      if (g_coll_modesw_thr_pos_decimal == MODESW_THR_POS_UP_DECIMAL_VAL) {
        g_coll_evo_thr_direct_control[2] = 0;
        if ((g_coll_evo_thr_direct_control[0] == 0)) {// && (g_coll_evo_usb_init_counter > 0)) {
          raw_thr0 = generic_read_16bit_axis_from_bytes(i2c_bytes[2],i2c_bytes[3]);
          g_coll_evo_diff[0] = raw_thr0 - g_coll_evo_thr_buf[0];
          g_coll_evo_diff[0] = abs(g_coll_evo_diff[0]);
          if (g_coll_evo_diff[0] < (THR_STEP)) {
            thr[0] = raw_thr0;
            g_coll_evo_thr_buf[0] = thr[0];
            g_coll_evo_thr_direct_control[0] = 1;
          } else {
            thr[0] = g_coll_evo_thr_buf[0];
          }
        } else {
          thr[0] = generic_read_16bit_axis_from_bytes(i2c_bytes[2],i2c_bytes[3]);
          g_coll_evo_thr_buf[0] = thr[0];
        }
        generic_check_16_bit_axis_val(thr[0]);
        
      } else if (g_coll_modesw_thr_pos_decimal == MODESW_THR_POS_MIDDLE_DECIMAL_VAL) {
        g_coll_evo_thr_direct_control[0] = 0;
        g_coll_evo_thr_direct_control[1] = 0;

        if ((g_coll_evo_thr_direct_control[2] == 0)) {// && (g_coll_evo_usb_init_counter > 0)) {
          raw_thr2 = generic_read_16bit_axis_from_bytes(i2c_bytes[2],i2c_bytes[3]);
          g_coll_evo_diff[2] = raw_thr2 - g_coll_evo_thr_buf[2];
          g_coll_evo_diff[2] = abs(g_coll_evo_diff[2]);
          if (g_coll_evo_diff[2] < (THR_STEP)) {
            thr[2] = raw_thr2;
            g_coll_evo_thr_buf[2] = thr[2];
            g_coll_evo_thr_direct_control[2] = 1;
          } else {
            thr[2] = g_coll_evo_thr_buf[2];
          }
        } else {
          thr[2] = generic_read_16bit_axis_from_bytes(i2c_bytes[2],i2c_bytes[3]);
          g_coll_evo_thr_buf[2] = thr[2];
        }
        generic_check_16_bit_axis_val(thr[2]);
        
        //Serial.println("DC OFF");
      } else if (g_coll_modesw_thr_pos_decimal == MODESW_THR_POS_DOWN_DECIMAL_VAL) { 
        g_coll_evo_thr_direct_control[2] = 0;
        if ((g_coll_evo_thr_direct_control[1] == 0)) { // && (g_coll_evo_usb_init_counter > 0)) {   
          raw_thr1 = generic_read_16bit_axis_from_bytes(i2c_bytes[2],i2c_bytes[3]);
          g_coll_evo_diff[1] = raw_thr1 - g_coll_evo_thr_buf[1];
          g_coll_evo_diff[1] = abs(g_coll_evo_diff[1]);
          if (g_coll_evo_diff[1] < (THR_STEP)) {
            //Serial.println("DC UP");
            thr[1] = raw_thr1;
            g_coll_evo_thr_buf[1] = thr[1];
            g_coll_evo_thr_direct_control[1] = 1;
          } else {
            thr[1] = g_coll_evo_thr_buf[1];
          }
        } else {
          thr[1] = generic_read_16bit_axis_from_bytes(i2c_bytes[2],i2c_bytes[3]);
          g_coll_evo_thr_buf[1] = thr[1];
        }
        generic_check_16_bit_axis_val(thr[1]);
      }
    } else  {
      if (g_struct_coll_attr.throttles >= 2) {
        thr[0] = generic_read_16bit_axis_from_bytes(i2c_bytes[2],i2c_bytes[3]);
        thr[1] = generic_read_16bit_axis_from_bytes(i2c_bytes[4],i2c_bytes[5]);
        generic_check_16_bit_axis_val(thr[0]);
        generic_check_16_bit_axis_val(thr[1]);
      } else {
        thr[0] = generic_read_16bit_axis_from_bytes(i2c_bytes[2],i2c_bytes[3]);
        generic_check_16_bit_axis_val(thr[0]);
      }
    }       
  }

//  void coll_evo_fill_i2c_bytes_arr (uint8_t *bytes) {
//    uint16_t z,thr0,thr1;
//    uint8_t ms = 0;
//    uint8_t ms_thr = 0;
////    z = filteredRead(A0,g_coll_evo_usb_filter_counter_z);
////    thr0 = filteredRead(A1,g_coll_evo_usb_filter_counter_thr[0]);
////    if (g_struct_coll_attr.throttles >= 2) {
////      thr1 = filteredRead(A2,g_coll_evo_usb_filter_counter_thr[1]);
////    }
////    ms = coll_evo_usb_read_modeswitch(4);
////    ms_thr = coll_evo_usb_read_modeswitch(6);
//
////    z = map(z,1000,0,0,1023);
////    throttle = map(thr0,149,703,559,0);
////    if (COLL_EVO_U_ENABLE_MAP_FUNCTION == 1) {
////      z = map(z,COLL_EVO_USB_PHYS_Z_MAX,COLL_EVO_USB_PHYS_Z_MIN,0,1023);
////      if (COLL_EVO_USB_PHYS_THR0_MIN < COLL_EVO_USB_PHYS_THR0_MAX) {
////        thr0 = map(thr0, COLL_EVO_USB_PHYS_THR0_MIN,COLL_EVO_USB_PHYS_THR0_MAX,COLL_EVO_USB_PHYS_THR0_MAX_MINUS_PHYS_THR0_MIN,0);
////      } else {
////        thr0 = map(thr0, COLL_EVO_USB_PHYS_THR0_MAX,COLL_EVO_USB_PHYS_THR0_MIN,COLL_EVO_USB_PHYS_THR0_MAX_MINUS_PHYS_THR0_MIN,0);
////      }
////      if (g_struct_coll_attr.throttles >= 2) {
////        if (COLL_EVO_USB_PHYS_THR0_MIN < COLL_EVO_USB_PHYS_THR0_MAX) {
////          thr1 = map(thr1,COLL_EVO_USB_PHYS_THR1_MIN,COLL_EVO_USB_PHYS_THR1_MAX,COLL_EVO_USB_PHYS_THR1_MAX_MINUS_PHYS_THR1_MIN,0);
////        } else {
////          thr1 = map(thr1,COLL_EVO_USB_PHYS_THR1_MAX,COLL_EVO_USB_PHYS_THR1_MIN,COLL_EVO_USB_PHYS_THR1_MAX_MINUS_PHYS_THR1_MIN,0);        
////        }
////      }
////    }
//    
//    bytes[0] = (byte) (z >> 8);
//    bytes[1] = (byte) (z);
//    bytes[2] = (byte) (thr0 >> 8);
//    bytes[3] = (byte) (thr0);
//    if (g_struct_coll_attr.throttles >= 2) {
//      bytes[4] = (byte) (thr1 >> 8);
//      bytes[5] = (byte) (thr1);
//      bytes[6] = ms;
//      bytes[7] = ms_thr;    
//    } else {
//      bytes[4] = ms;
//      bytes[5] = ms_thr;    
//    }
//
//  }

  #endif

#if ((defined COLLECTIVE_STHR_EVO_USB) || (defined SIMPLE_COLLECTIVE_SE_USB))

  void coll_lever_evo_usb_poll (uint16_t &lvr,uint16_t *thr, uint8_t &ms, uint8_t &ms_thr) {
    //ms = 0;
    //ms_thr = 0;
    uint8_t i2c_bytes[g_struct_coll_attr.i2c_bytes];
    uint16_t raw_thr0,raw_thr1,raw_thr2;
    coll_evo_usb_fill_i2c_bytes_arr(i2c_bytes); 
    lvr = generic_read_16bit_axis_from_bytes(i2c_bytes[0],i2c_bytes[1]);
    g_coll_evo_usb_raw_thr = generic_read_16bit_axis_from_bytes(i2c_bytes[2],i2c_bytes[3]);
    if ((COLLECTIVE_EVO_MODE_SWITCH_THR_ENABLED == 1)) {
      //g_struct_coll_attr.throttles = 2;
      if (g_coll_modesw_thr_pos_decimal == MODESW_THR_POS_UP_DECIMAL_VAL) {
        g_coll_evo_thr_direct_control[2] = 0;
        if ((g_coll_evo_thr_direct_control[0] == 0)) {// && (g_coll_evo_usb_init_counter > 0)) {
          raw_thr0 = generic_read_16bit_axis_from_bytes(i2c_bytes[2],i2c_bytes[3]); 
          g_coll_evo_diff[0] = raw_thr0 - g_coll_evo_thr_buf[0];
          g_coll_evo_diff[0] = abs(g_coll_evo_diff[0]);
//          Serial.print("diff ");
//          Serial.print(g_coll_evo_diff[0]);
//          Serial.print(" raw ");
          
//          Serial.print(" buf ");
//          Serial.print(g_struct_coll_attr.thr_buf[0]);
//          Serial.print(" dc ");
//          Serial.print(g_coll_evo_thr_direct_control[0]);
//          Serial.print(" THR_STEP ");
//          Serial.print(THR_STEP);
//          Serial.println();
          if (g_coll_evo_diff[0] < (THR_STEP)) {
            thr[0] = raw_thr0;
            g_coll_evo_thr_buf[0] = thr[0];
            g_coll_evo_thr_direct_control[0] = 1;
          } else {
            thr[0] = g_coll_evo_thr_buf[0];
          }
        } else {
          thr[0] = generic_read_16bit_axis_from_bytes(i2c_bytes[2],i2c_bytes[3]);
          g_coll_evo_thr_buf[0] = thr[0];
        }
        generic_check_16_bit_axis_val(thr[0]);
        
      } else if (g_coll_modesw_thr_pos_decimal == MODESW_THR_POS_MIDDLE_DECIMAL_VAL) {
        g_coll_evo_thr_direct_control[0] = 0;
        g_coll_evo_thr_direct_control[1] = 0;

        if ((g_coll_evo_thr_direct_control[2] == 0)) {// && (g_coll_evo_usb_init_counter > 0)) {
          raw_thr2 = generic_read_16bit_axis_from_bytes(i2c_bytes[2],i2c_bytes[3]);
          g_coll_evo_diff[2] = raw_thr2 - g_coll_evo_thr_buf[2];
          g_coll_evo_diff[2] = abs(g_coll_evo_diff[2]);
//          Serial.print("diff ");
//          Serial.print(g_coll_evo_diff[0]);
//          Serial.print(" raw ");
//          Serial.print(raw_thr0);
//          Serial.print(" buf ");
//          Serial.print(g_struct_coll_attr.thr_buf[0]);
//          Serial.print(" dc ");
//          Serial.print(g_coll_evo_thr_direct_control[0]);
//          Serial.print(" THR_STEP ");
//          Serial.print(THR_STEP);
//          Serial.println();
          if (g_coll_evo_diff[2] < (THR_STEP)) {
            thr[2] = raw_thr2;
            g_coll_evo_thr_buf[2] = thr[2];
            g_coll_evo_thr_direct_control[2] = 1;
          } else {
            thr[2] = g_coll_evo_thr_buf[2];
          }
        } else {
          thr[2] = generic_read_16bit_axis_from_bytes(i2c_bytes[2],i2c_bytes[3]);
          g_coll_evo_thr_buf[2] = thr[2];
        }
        generic_check_16_bit_axis_val(thr[2]);
        
        //Serial.println("DC OFF");
      } else if (g_coll_modesw_thr_pos_decimal == MODESW_THR_POS_DOWN_DECIMAL_VAL) { 
        g_coll_evo_thr_direct_control[2] = 0;
        if ((g_coll_evo_thr_direct_control[1] == 0)) { // && (g_coll_evo_usb_init_counter > 0)) {   
          raw_thr1 = generic_read_16bit_axis_from_bytes(i2c_bytes[2],i2c_bytes[3]);
          g_coll_evo_diff[1] = raw_thr1 - g_coll_evo_thr_buf[1];
          g_coll_evo_diff[1] = abs(g_coll_evo_diff[1]);
//          Serial.print("diff ");
//          Serial.print(g_coll_evo_diff[1]);
//          Serial.print(" raw ");
//          Serial.print(raw_thr1);
//          Serial.print(" buf ");
//          Serial.print(g_coll_evo_thr_buf[1]);
//          Serial.print(" dc ");
//          Serial.print(g_coll_evo_thr_direct_control[1]);
//          Serial.print(" THR_STEP ");
//          Serial.print(THR_STEP);
//          Serial.println();
          if (g_coll_evo_diff[1] < (THR_STEP)) {
            //Serial.println("DC UP");
            thr[1] = raw_thr1;
            g_coll_evo_thr_buf[1] = thr[1];
            g_coll_evo_thr_direct_control[1] = 1;
          } else {
            thr[1] = g_coll_evo_thr_buf[1];
          }
        } else {
          thr[1] = generic_read_16bit_axis_from_bytes(i2c_bytes[2],i2c_bytes[3]);
          g_coll_evo_thr_buf[1] = thr[1];
        }
        generic_check_16_bit_axis_val(thr[1]);
      }
    } else  {
      if (g_struct_coll_attr.throttles >= 2) {
        thr[0] = generic_read_16bit_axis_from_bytes(i2c_bytes[2],i2c_bytes[3]);
        thr[1] = generic_read_16bit_axis_from_bytes(i2c_bytes[4],i2c_bytes[5]);
        generic_check_16_bit_axis_val(thr[0]);
        generic_check_16_bit_axis_val(thr[1]);
      } else {
        thr[0] = generic_read_16bit_axis_from_bytes(i2c_bytes[2],i2c_bytes[3]);
        generic_check_16_bit_axis_val(thr[0]);
      }
    }
    if (g_struct_coll_attr.throttles == 1) {
      ms = i2c_bytes[4];
      ms_thr = i2c_bytes[5];
    } else {
      ms = i2c_bytes[6];
      ms_thr = i2c_bytes[7];
    }    
      
  }
  



  void coll_evo_usb_fill_i2c_bytes_arr (uint8_t *bytes) {
    uint16_t z,thr0,thr1;
    uint8_t ms = 0;
    uint8_t ms_thr = 0;
    z = filteredRead(A0,g_coll_evo_usb_filter_counter_z);
    thr0 = filteredRead(A1,g_coll_evo_usb_filter_counter_thr[0]);
    if (g_struct_coll_attr.throttles >= 2) {
      thr1 = filteredRead(A2,g_coll_evo_usb_filter_counter_thr[1]);
    }
    ms = coll_evo_usb_read_modeswitch(4);
    ms_thr = coll_evo_usb_read_modeswitch(6);

//    z = map(z,1000,0,0,1023);
//    throttle = map(thr0,149,703,559,0);
    if (COLL_EVO_USB_ENABLE_MAP_FUNCTION == 1) {
      z = map(z,COLL_EVO_USB_PHYS_Z_MAX,COLL_EVO_USB_PHYS_Z_MIN,0,1023);
      if (COLL_EVO_USB_PHYS_THR0_MIN < COLL_EVO_USB_PHYS_THR0_MAX) {
        thr0 = map(thr0, COLL_EVO_USB_PHYS_THR0_MIN,COLL_EVO_USB_PHYS_THR0_MAX,COLL_EVO_USB_PHYS_THR0_MAX_MINUS_PHYS_THR0_MIN,0);
      } else {
        thr0 = map(thr0, COLL_EVO_USB_PHYS_THR0_MAX,COLL_EVO_USB_PHYS_THR0_MIN,COLL_EVO_USB_PHYS_THR0_MAX_MINUS_PHYS_THR0_MIN,0);
      }
      if (g_struct_coll_attr.throttles >= 2) {
        if (COLL_EVO_USB_PHYS_THR0_MIN < COLL_EVO_USB_PHYS_THR0_MAX) {
          thr1 = map(thr1,COLL_EVO_USB_PHYS_THR1_MIN,COLL_EVO_USB_PHYS_THR1_MAX,COLL_EVO_USB_PHYS_THR1_MAX_MINUS_PHYS_THR1_MIN,0);
        } else {
          thr1 = map(thr1,COLL_EVO_USB_PHYS_THR1_MAX,COLL_EVO_USB_PHYS_THR1_MIN,COLL_EVO_USB_PHYS_THR1_MAX_MINUS_PHYS_THR1_MIN,0);        
        }
      }
    }
    
    bytes[0] = (byte) (z >> 8);
    bytes[1] = (byte) (z);
    bytes[2] = (byte) (thr0 >> 8);
    bytes[3] = (byte) (thr0);
    if (g_struct_coll_attr.throttles >= 2) {
      bytes[4] = (byte) (thr1 >> 8);
      bytes[5] = (byte) (thr1);
      bytes[6] = ms;
      bytes[7] = ms_thr;    
    } else {
      bytes[4] = ms;
      bytes[5] = ms_thr;    
    }

  }

  void coll_base_usb_pins_init() {
    for (byte i=4;i<=7;i++) {
      pinMode(i, INPUT_PULLUP);
    }
  }

  uint8_t coll_evo_usb_read_modeswitch (uint8_t pn) {
    uint8_t ms = 0b00000000;
    for (int i = pn; i <=pn+1 ; i++) {
      bool pin = !digitalRead(i);
      if (pin == 1) {
        ms |= (1 << (i - pn));
      } else {
        ms &= ~(1 << (i - pn));
      }
    }
    //printBits(ms);
    //Serial.println();
    return ms;
  }





#endif
