#if (defined B407_COLL_SWITCH_PANEL)
  Joystick_ j_b407h(0x18, 0x05, 35, 2, false, false, false, false, false, false, false, false, false, false, false);

  //#define B407_HEAD_DEBUG


  void setup_b407_coll_head() {
    j_b407h.begin();    
    if (DCS_HUEY_IDLE_STOP_COMPAT_MODE_ENABLED == 1) {
      Keyboard.begin();
    }
  }
  
  void poll_b407_coll_head() {
    uint8_t bh,b2,b3;
    uint8_t hs = 0;
    uint8_t mod = 0;
    uint8_t hb = 0;
      
      Wire.requestFrom(B407_HEAD_I2C_ADDRESS, 3);
      while (Wire.available()) {
        bh = Wire.read(); // receive a byte as character  
        b2 = Wire.read();
        b3 = Wire.read();  
      }


    if (COLLECTIVE_MODE_SWITCH_ENABLED == 1) {
      if (g_coll_modesw_pos_decimal == MODESW_POS_MIDDLE_DECIMAL_VAL) {
        mod = 0;
        hs = 0;
      } else if (g_coll_modesw_pos_decimal == MODESW_POS_LEFT_DECIMAL_VAL) {
        mod = 6;
        if (B407_COLL_HEAD_HAT_RESPECTS_MODE_SWITCH) {    
          hs = 1;
        }
      } else if (g_coll_modesw_pos_decimal == MODESW_POS_RIGHT_DECIMAL_VAL) {
        mod = 12;
        if (B407_COLL_HEAD_HAT_RESPECTS_MODE_SWITCH) {    
          hs = 2;
        }
      }
    }
    //join 2 bytes together so working with non-spring-loaded switches will be easier
    uint16_t ba = word (b3,b2);

    #if defined(B407_HEAD_DEBUG)
      printBits16(ba);
      Serial.print(" ");
      Serial.print(bh);
      Serial.println();
    #endif

    uint16_t hat_val = parse_b407h_hat_val(bh,hs);

    if (hs <2) {
      j_b407h.setHatSwitch(hs, hat_val);
    } else if (hs == 2) {
      hb = hat_to_btns(hat_val);
    }
    
    parse_button_array_b407h(ba,hb,mod);
  }

  void parse_button_array_b407h(uint16_t b,uint8_t hb, uint8_t modifier) {
    bool v_llsw0 = (b >> 0) & 1;
    bool v_llsw1 = (b >> 1) & 1;
    bool v_slsw0 = (b >> 6) & 1;
    bool v_slsw1 = (b >> 7) & 1;

    //landing light 3-way switch  
    if (v_llsw0 == v_llsw1) {
      if (g_b407h_lastButtonState[1] != 1) {
        j_b407h.setButton(1,1);
        g_b407h_lastButtonState[1] = 1;
      } 
    }
    if (g_b407h_lastButtonState[0] != v_llsw0) {
        j_b407h.setButton(0,v_llsw0);
        j_b407h.setButton(1,0);
        g_b407h_lastButtonState[1] = 0;
        g_b407h_lastButtonState[0] = v_llsw0;
    }
    if (g_b407h_lastButtonState[2] != v_llsw1) {
        j_b407h.setButton(2,v_llsw1);
        j_b407h.setButton(1,0);
        g_b407h_lastButtonState[2] = v_llsw1;
        g_b407h_lastButtonState[1] = 0;
    }

    //searchlight 3-way switch   
    if (v_slsw0 == v_slsw1) {
      if (g_b407h_lastButtonState[4] != 1) {
        j_b407h.setButton(4,1);
        g_b407h_lastButtonState[4] = 1;
      } 
    }
    if (g_b407h_lastButtonState[3] != v_slsw0) {
        j_b407h.setButton(3,v_slsw0);
        j_b407h.setButton(4,0);
        g_b407h_lastButtonState[4] = 0;
        g_b407h_lastButtonState[3] = v_slsw0;
    }
    if (g_b407h_lastButtonState[5] != v_slsw1) {
        j_b407h.setButton(5,v_slsw1);
        j_b407h.setButton(4,0);
        g_b407h_lastButtonState[5] = v_slsw1;
        g_b407h_lastButtonState[4] = 0;
    }
  
    //1st byte
    for (byte i = 2; i < 6; i++) {
      bool v = (b >> i) & 1;
      if (v != g_b407h_lastButtonState[i + 1 + 6 + modifier]) {
        j_b407h.setButton(i + 1 + 6 + modifier, v);
        g_b407h_lastButtonState[i + 1 + 6 + modifier] = v;
      }
    }
    //2nd byte
    for (byte i = 9; i < 10; i++) {
      // we exclude floats switch
      bool v = (b >> i) & 1;
      if (v != g_b407h_lastButtonState[i - 2 + 1 + 6 + modifier]) {
        j_b407h.setButton(i - 2 + 1 + 6 + modifier, v);
        g_b407h_lastButtonState[i - 2 + 1 + 6 + modifier] = v;
      }   
    }
    // floats switch
    bool v_fl = (b >> 8) & 1;
      if (v_fl != g_b407h_lastButtonState[13]) {
        j_b407h.setButton(13, v_fl);
        g_b407h_lastButtonState[13] = v_fl;
      }
    //hat switch mode 2 parsed to buttons below
    for (byte i = 0; i < 8; i++) {
      bool v = (hb >> i) & 1;
      if (v != g_b407h_lastButtonState[i + 1 + 26]) {
        j_b407h.setButton(i + 1 + 26, v);
        g_b407h_lastButtonState[i + 1 + 26] = v;
      }
    }
  }

  uint16_t parse_b407h_hat_val (uint8_t h, uint8_t hs) {
    if (h == B407H_HAT_LEFT){
      return 270;
    } else if (h == B407H_HAT_LEFT_UP){
      return 315;
    } else if (h == B407H_HAT_LEFT_DOWN){
      return 225;
    } else if (h == B407H_HAT_RIGHT) {
      return 90;
    } else if (h == B407H_HAT_RIGHT_UP) {
      return 45;
    } else if (h == B407H_HAT_RIGHT_DOWN) {
      return 135;
    } else if (h == B407H_HAT_UP) {
      return 0;
    } else if (h == B407H_HAT_DOWN) {
      return 180;
    } else if (h == B407H_HAT_CLICK) {
      j_b407h.setButton(6 + hs,1);
    } else {
      j_b407h.setButton(6 + hs,0);
      return JOYSTICK_HATSWITCH_RELEASE;   
    }
  }
   
#endif
