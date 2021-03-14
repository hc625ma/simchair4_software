int hat_to_btns (int hat_val) {
  uint8_t b = 0b00000000;
  switch (hat_val) {
  case 315:
    b |= (1 << 0);
    return b;
    break;
  case 270:
    b |= (1 << 1);
    return b;
    break;
  case 225:
    b |= (1 << 2);
    return b;
    break;
  case 180:
    b |= (1 << 3);
    return(b);
    break;
  case 45:
    b |= (1 << 4);
    return(b);
    break;
  case 90:
    b |= (1 << 5);
    return(b);
    break;
  case 135:
    b |= (1 << 6);
    return(b);
    break;
  case 0:
    b |= (1 << 7);
    return(b);
    break;
  default:
    return(b);
    break;
  }
}

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

// GENERIC SHARED FUNCTIONS

void printBits(byte myByte){
  for(byte mask = 0x80; mask; mask >>= 1){
    if(mask  & myByte)
      Serial.print('1');
    else
      Serial.print('0');
  }
}

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

uint16_t filteredRead (uint16_t input,uint8_t filter_counter)
{
  uint32_t filter = 0;
  for (uint8_t i=0;i<filter_counter;i++)
  {
    filter+= analogRead(input);
    delay(1);
  }

  uint16_t val = filter/filter_counter;
  return val;
}   

void generic_poll_i2c_device (uint8_t addr, uint8_t *bytes, uint8_t bytes_num) {
  Wire.requestFrom(addr, bytes_num);
    while (Wire.available()) {
      for (byte i=0;i<bytes_num;i++) {
        bytes[i] = Wire.read();
      }
    }
}


  
