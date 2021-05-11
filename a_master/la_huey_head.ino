#if (defined HUEY_COLL_SWITCH_PANEL)
  Joystick_ j_hh(0x17, 0x05, 48, 2, false, false, false, false, false, false, false, false, false, false, false);

  void setup_huey_coll_head() {
    j_hh.begin();    
    if (DCS_HUEY_IDLE_STOP_COMPAT_MODE_ENABLED == 1) {
      Keyboard.begin();
    }
  }
  
  void poll_huey_coll_head() {
    uint8_t x, y, ba0, ba1,hb;
    uint8_t mod = 0;
    uint8_t hs = 0;
      
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
    Serial.print(x);
    Serial.print(" ");
    Serial.print(y);
    Serial.print(" ");
    Serial.print(ba0);
    Serial.print(" ");
    Serial.println(ba1);

    


    



    if (COLLECTIVE_MODE_SWITCH_ENABLED == 1) {
      if ((g_coll_mk3_detected == 1) && (USE_HUEY_HEAD_MODESWITCH == 1)) {
        ba0 = extract_modesw_val(ba0);
      }
      if (g_coll_modesw_pos_decimal == MODESW_POS_MIDDLE_DECIMAL_VAL) {
        mod = 0;
        hs = 0;
      } else if (g_coll_modesw_pos_decimal == MODESW_POS_LEFT_DECIMAL_VAL) {
        mod = 13;
        if (HUEY_COLL_HEAD_HAT_RESPECTS_MODE_SWITCH) {    
          hs = 1;
        }
      } else if (g_coll_modesw_pos_decimal == MODESW_POS_RIGHT_DECIMAL_VAL) {
        mod = 27;
        if (HUEY_COLL_HEAD_HAT_RESPECTS_MODE_SWITCH) {
          hs = 2;
        }
      }
    }


    int16_t hat_val = parse_hat_sw(x, y, HUEY_COLL_HEAD_HAT_DIRECTIONS);

    if (hs <2) {
      j_hh.setHatSwitch(hs, hat_val);
    } else if (hs == 2) {
      hb = hat_to_btns(hat_val);
      parse_button_array_hh(hb,40,0,0,0,0);
    }
    
    parse_button_array_hh(ba0,0,0,0,mod,0);
    parse_button_array_hh(ba1,8,5,0,mod,1);
  }

//  void printBits(byte myByte){
//   for(byte mask = 0x80; mask; mask >>= 1){
//     if(mask  & myByte)
//         Serial.print('1');
//     else
//         Serial.print('0');
//   }
//  }

  uint8_t extract_modesw_val(uint8_t b) {
    bool v = (b >> (HUEY_HEAD_MODESWITCH_BTN - 1)) & 1;
    bool v1 = (b >> (HUEY_HEAD_MODESWITCH_BTN)) & 1;

    if ((v == 0) && (v1 == 0))
    {
      g_coll_modesw_pos_decimal = MODESW_POS_MIDDLE_DECIMAL_VAL;
    } else if (v == 1){
      g_coll_modesw_pos_decimal = MODESW_POS_LEFT_DECIMAL_VAL;
    } else if (v1 == 1){
      g_coll_modesw_pos_decimal = MODESW_POS_RIGHT_DECIMAL_VAL;
    }

    b &= ~(1 << (HUEY_HEAD_MODESWITCH_BTN - 1));
    b &= ~(1 << HUEY_HEAD_MODESWITCH_BTN);

    return b;    
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
            //      Serial.print("HS "); 
//      Serial.print(i); 
//      Serial.print(" sb "); 
//      Serial.print(start_btn);
//      Serial.print(" v "); 
//      Serial.print(v);
//      Serial.println("");   
          } else {
            j_hh.setButton(i + start_btn + modifier, v);
            g_idle_rel_btn_pressed = v;
            g_idle_rel_btn_pressed_new[0]=v;
            g_idle_rel_btn_pressed_new[1]=v;
            g_idle_rel_btn_pressed_new[2]=v;
            g_tl_idle_rel_btn_pressed[0] = v;
            g_tl_idle_rel_btn_pressed[1] = v;
            
          }
        } else {
          j_hh.setButton(i + start_btn + modifier, v);
        }
      }
      g_hh_lastButtonState[i + start_btn + modifier] = v;
    }
  }

    
#endif
