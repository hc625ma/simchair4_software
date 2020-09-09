int parse_hat_sw (int x, int y, byte dirs) {
  if(dirs == 8) {
    if(x < PC10_HATSW_VAL && y > PC90_HATSW_VAL ) {
      //top-left
      return 315;
    } else if (x < PC10_HATSW_VAL && y > PC25_HATSW_VAL && y < PC75_HATSW_VAL ) {
      //left
      return 270;
    } else if (x < PC10_HATSW_VAL && y < PC10_HATSW_VAL ) {
      //bottom-left
      return 225;
    } else if (x > PC25_HATSW_VAL && x < PC75_HATSW_VAL && y > PC90_HATSW_VAL) {
      //top
      return 0;
    } else if (x > PC25_HATSW_VAL && x < PC75_HATSW_VAL && y < PC10_HATSW_VAL ) {
      //bottom
      return 180;
    } else if (x > PC90_HATSW_VAL && y > PC90_HATSW_VAL ) {
      //top-right
      return 45;
    } else if (x > PC90_HATSW_VAL && y > PC25_HATSW_VAL && y < PC75_HATSW_VAL ) {
      //right
      return 90;
    } else if (x > PC90_HATSW_VAL && y < PC10_HATSW_VAL ) {
      //bottom-right
      return 135;
    } else {//if ( x > 90 && x < 150 && y > 90 && y < 150 ) { // or use as else...
      //center 
      return JOYSTICK_HATSWITCH_RELEASE;
    }
  } else { // dir 4
    if (x > PC25_HATSW_VAL && x < PC75_HATSW_VAL && y > PC90_HATSW_VAL) {
      //top
      return 0;
    } else if (x < PC10_HATSW_VAL && y > PC25_HATSW_VAL && y < PC75_HATSW_VAL ) {
      //left
      return 270;
    } else if (x > PC25_HATSW_VAL && x < PC75_HATSW_VAL && y < PC10_HATSW_VAL ) {
      //bottom
      return 180;
    } else if (x > PC90_HATSW_VAL && y > PC25_HATSW_VAL && y < PC75_HATSW_VAL ) {
      //right
      return 90;
    } else {//if ( x > 256 && x < 768 && y > 256 && y < 768 ) { // or use as else...
      //center 
      return JOYSTICK_HATSWITCH_RELEASE;
    }
  }
}

void parse_btn_bytes (bool *arr, uint8_t b, uint8_t start_pos, uint8_t end_pos, uint8_t byte_offset ){
  if (end_pos == 0) {
    end_pos = 8;
  }
  for (byte i = byte_offset; i < end_pos; i++) {
    arr[i + start_pos + 1] = (b >> i) & 1; // to sync out button numbers with joy.cpl, our buttons start with 1
  }
}
   

#if ((defined CYCLIC_BASE) || (defined B8_GRIP) || (defined PEDALS))

  int adjust_sensitivity (long val, int percent) {
   // Serial.println(percent);
    int center = CBASE_ADC_RANGE / 2;
    int adj_range = (CBASE_ADC_RANGE / 100) * percent;
  
    val = ((val - center) * percent / 100) + center; // here we at first substract ADC_RANGE / 2 to move our value to the range with center at 0, then make our calculations, and adjust it back by adding ADC_RANGE/2
    val = constrain(val,0,CBASE_ADC_RANGE);  
    return val;
  }

#endif

#if (defined AB412_COLL_SWITCH_PANEL) // new unified collective button parsers, so far only work with ab412 head, others need to be updated

void coll_head_extract_modesw_val( bool *arr) {
    bool v = arr[g_struct_coll_head_attr.btn_modesw];
    bool v1 = arr[g_struct_coll_head_attr.btn_modesw + 1];
    arr[g_struct_coll_head_attr.btn_modesw] = 0;
    arr[g_struct_coll_head_attr.btn_modesw + 1] = 0;

    if ((v == 0) && (v1 == 0))
    {
      g_coll_modesw_pos_decimal = MODESW_POS_MIDDLE_DECIMAL_VAL;
    } else if (v == 1){
      g_coll_modesw_pos_decimal = MODESW_POS_LEFT_DECIMAL_VAL;
    } else if (v1 == 1){
      g_coll_modesw_pos_decimal = MODESW_POS_RIGHT_DECIMAL_VAL;
    }  
  }

  void coll_head_parse_btn_array(bool *pb_arr, uint8_t pb_arr_size, bool *sb_arr, bool *b_state_arr, uint8_t modifier, uint8_t sbtn_offset, class Joystick_ &joy) {
    coll_head_extract_special_btn_values(pb_arr,pb_arr_size,sb_arr);

    for (byte i = 1; i < pb_arr_size; i++) {
      bool v = pb_arr[i];
      
      if (v != b_state_arr[i + modifier]) {
        joy.setButton(i - 1 + modifier, v);
        b_state_arr[i + modifier] = v;
      }
    }
    for (byte i = 0; i < sizeof(g_struct_coll_head_attr.switches_w_middle_btn); i++) {
      bool v = sb_arr[i];
      if (v != b_state_arr[i + sbtn_offset]) {
        joy.setButton(i -1 +sbtn_offset, v);
        b_state_arr[i + sbtn_offset] = v;
      }
    }
  }

  void coll_head_extract_special_btn_values (bool*pba,uint8_t pba_size, bool *sba) {
    //throttle latch feature
    if (BUTTON_PRESS_ON_THROTTLE_CUTOFF == 1) {
      g_idle_rel_btn_pressed = pba[g_struct_coll_head_attr.btn_idle_stop];
      g_tl_idle_rel_btn_pressed[0] = pba[g_struct_coll_head_attr.btn_idle_stop];
      g_tl_idle_rel_btn_pressed[1] = pba[g_struct_coll_head_attr.btn_idle_stop];
      g_idle_rel_btn_pressed_new[0] = pba[g_struct_coll_head_attr.btn_idle_stop];
      g_idle_rel_btn_pressed_new[1] = pba[g_struct_coll_head_attr.btn_idle_stop];
    }
    // switches with middle buttons, we fill the special buttons array
    for (byte i=0;i<sizeof(g_struct_coll_head_attr.switches_w_middle_btn);i++) {
      uint8_t sbtn_num = AB412_HEAD_SWITCHES_WITH_MIDDLE_BTN[i];
      bool sw_btn0_v = pba[sbtn_num];
      bool sw_btn1_v = pba[sbtn_num+1];
      if ((sw_btn0_v == sw_btn1_v) && (g_coll_modesw_pos_decimal == MODESW_POS_MIDDLE_DECIMAL_VAL)) {
        sba[i] = 1;
      } else {
        sba[i] = 0;
      }
    }
  }

#endif

#if (defined COLLECTIVE_STHR_MKIII)

  void coll_lever_process_poll_results (uint16_t &lvr, uint16_t *thr, uint8_t &ms, class Joystick_ &joy) {

    joy.setZAxis(lvr); // SET LEVER AXIS
    
    if (COLLECTIVE_MODE_SWITCH_ENABLED == 1) {
      g_coll_modesw_pos_decimal = ms; // share mode switch position with the rest of the hardware
    } else {
      coll_lever_modesw_to_joy(ms,joy);
    }
    coll_lever_apply_throttle_stabilizer(thr);
    coll_lever_apply_advanced_throttle_features(thr[0],0,j_scoll);
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
        joy.setRxAxis(raw_thr);
      } else {
        joy.setRyAxis(raw_thr);
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
        joy.setThrottleRange(g_struct_coll_attr.idle_detent_axis_val[0], g_struct_coll_attr.thr_max);
      }
    } else {
      if (g_struct_coll_attr.throttles == 2) {
        joy.setRxAxisRange(g_struct_coll_attr.thr_min[0], g_struct_coll_attr.thr_max[0]);
        joy.setRyAxisRange(g_struct_coll_attr.thr_min[1], g_struct_coll_attr.thr_max[1]);
      } else {
        joy.setThrottleRange(g_struct_coll_attr.thr_min, g_struct_coll_attr.thr_max);
      }
    }
    if (DCS_HUEY_IDLE_STOP_COMPAT_MODE_ENABLED == 1) {
      Keyboard.begin();
    }
  }

#endif

// GENERIC SHARED FUNCTIONS

uint16_t generic_read_16bit_axis_from_bytes (uint8_t b1, uint8_t b2) {
  uint16_t axis_val = 0;
  axis_val = b1;
  axis_val = (axis_val << 8) | b2;
  return axis_val;
}

void generic_check_16_bit_axis_val (uint16_t &axis) {
  if (axis > 60000) {
    axis = 0;
  }
}

void generic_poll_i2c_device (uint8_t addr, uint8_t *bytes, uint8_t bytes_num) {
  Wire.requestFrom(addr, bytes_num);
    while (Wire.available()) {
      for (byte i=0;i<bytes_num;i++) {
        bytes[i] = Wire.read();
      }
    }
}


  
