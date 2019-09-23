#if (defined HUEY_COLL_SWITCH_PANEL)
  Joystick_ j_hh(0x17, 0x05, 40, 1, false, false, false, false, false, false, false, false, false, false, false);

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



    if (COLLECTIVE_MODE_SWITCH_ENABLED == 1) {
      if ((g_coll_mk3_detected == 1) && (USE_HUEY_HEAD_MODESWITCH == 1)) {
        ba0 = extract_modesw_val(ba0);
      }
      if (g_coll_modesw_pos_decimal == MODESW_POS_MIDDLE_DECIMAL_VAL) {
        mod = 0;
      } else if (g_coll_modesw_pos_decimal == MODESW_POS_LEFT_DECIMAL_VAL) {
        mod = 13;
      } else if (g_coll_modesw_pos_decimal == MODESW_POS_RIGHT_DECIMAL_VAL) {
        mod = 27;
      }
    }
     
    parse_button_array_hh(ba0,0,0,0,mod,0);
    parse_button_array_hh(ba1,8,0,0,mod,1);
  }

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

    
#endif