#if (defined B206_COLL_SWITCH_PANEL)
  Joystick_ j_b206h(0x18, 0x05, 12, 0, false, false, false, false, false, false, false, false, false, false, false);

  void setup_b206_coll_head() {
    j_b206h.begin();    
    if (DCS_HUEY_IDLE_STOP_COMPAT_MODE_ENABLED == 1) {
      Keyboard.begin();
    }
  }
  
  void poll_b206_coll_head() {
    uint8_t ba;
    uint8_t mod = 0;
      
      Wire.requestFrom(B206_HEAD_I2C_ADDRESS, 1);
      while (Wire.available()) {
        byte b1 = Wire.read(); // receive a byte as character    
        ba = b1;
      }


    if (COLLECTIVE_MODE_SWITCH_ENABLED == 1) {
      if (g_coll_modesw_pos_decimal == MODESW_POS_MIDDLE_DECIMAL_VAL) {
        mod = 0;
      } else if (g_coll_modesw_pos_decimal == MODESW_POS_LEFT_DECIMAL_VAL) {
        mod = 3;
      } else if (g_coll_modesw_pos_decimal == MODESW_POS_RIGHT_DECIMAL_VAL) {
        mod = 6;
      }
    }

  // CALIBRATION
  // Serial.print(ba);
  
    parse_button_array_b206h(ba,mod);

  }

  uint8_t extract_modesw_val(uint8_t b) {
    bool v = (b >> (HUEY_HEAD_MODESWITCH_BTN - 1)) & 1;
    bool v1 = (b >> (HUEY_HEAD_MODESWITCH_BTN)) & 1;

    if ((v == 0) && (v1 == 0)) {
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

  void parse_button_array_b206h(uint8_t b, uint8_t modifier) {
    bool v_llsw0 = (b >> 0) & 1;
    bool v_llsw1 = (b >> 1) & 1;

    if (v_llsw0 == v_llsw1) {
      if (g_b206h_lastButtonState[1] != 1) {
        j_b206h.setButton(1,1);
        g_b206h_lastButtonState[1] = 1;
      } 
    }
    if (g_b206h_lastButtonState[0] != v_llsw0) {
        j_b206h.setButton(0,v_llsw0);
        j_b206h.setButton(1,0);
        g_b206h_lastButtonState[1] = 0;
        g_b206h_lastButtonState[0] = v_llsw0;
    }
    if (g_b206h_lastButtonState[2] != v_llsw1) {
        j_b206h.setButton(2,v_llsw1);
        j_b206h.setButton(1,0);
        g_b206h_lastButtonState[2] = v_llsw1;
        g_b206h_lastButtonState[1] = 0;
    }
       
    for (byte i = 2; i < 6; i++) {
      bool v = (b >> i) & 1;

      if (v != g_b206h_lastButtonState[i + 1 + modifier]) {
        j_b206h.setButton(i + 1 + modifier, v);
        g_b206h_lastButtonState[i + 1 + modifier] = v;
      }
    }
  }


    
#endif
