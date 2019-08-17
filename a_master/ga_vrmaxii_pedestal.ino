#if (defined VRMAXII_PEDESTAL)
  Joystick_ j_vrm2pdstl(0x21, 0x05, 130, 0, false, false, true, false, false, false, false, false, false, false, false);

  void setup_vrmax2_pedestal() {
    j_vrm2pdstl.begin();   
    j_vrm2pdstl.setZAxisRange(0, 1023);
    Mouse.begin();
  }
  
  void poll_vrmax2_pedestal() {
    uint16_t x,y,z;
    uint8_t ba0,ba1,rp_b,enc,rp_s,rp_enc0,rp_enc1,rp_enc2,rp_enc3;
    uint8_t mod = 0;
  
    Wire.requestFrom(VRMAXII_PEDESTAL_A_I2C_ADDRESS, 8);
    while (Wire.available()) {
      byte b1 = Wire.read(); // receive a byte as character
      byte b2 = Wire.read();
      byte b3 = Wire.read(); // receive a byte as character
      byte b4 = Wire.read();
      byte b5 = Wire.read();
      byte b6 = Wire.read();
      byte b7 = Wire.read();
      byte b8 = Wire.read();

  
      x = b1;
      y = b2;
      z = b3;
      z = (z << 8) | b4;
      ba0 = b5;
      ba1 = b6;
      rp_b = b7;
      enc = b8;
      
//    Serial.print("test");
//    Serial.print(" ");
//    Serial.print(x);
//    Serial.print(" ");
//    Serial.print(y);
//    Serial.print(" ");
//    Serial.print(enc);
//    Serial.print(" ");
//    Serial.print(ba0);
//    Serial.print(" ");
//    Serial.println(ba1);

      if (COLLECTIVE_MODE_SWITCH_ENABLED == 1) {
        if (g_coll_modesw_pos_decimal == MODESW_POS_MIDDLE_DECIMAL_VAL) {
          mod = 0;
        } else if (g_coll_modesw_pos_decimal == MODESW_POS_LEFT_DECIMAL_VAL) {
          mod = 12;
        } else if (g_coll_modesw_pos_decimal == MODESW_POS_RIGHT_DECIMAL_VAL) {
          mod = 24;
        }
      }
      ministick_to_mouse(x,y);
      parse_button_array(ba0,0,0,mod);
      parse_button_array(ba1,8,0,mod);
      if (VRMAXII_PEDESTAL_ZOOM_STABILIZER_ENABLED) {
        int16_t zdiff = z - g_vrm2pdstl_z_val;
        if (abs(zdiff) > VRMAXII_PEDESTAL_ZOOM_STEP) {
          j_vrm2pdstl.setZAxis(z);
          g_vrm2pdstl_z_val = z;
        }
      } else {
        j_vrm2pdstl.setZAxis(z);
      }
      parse_encoder(enc);

    }

    Wire.requestFrom(VRMAXII_PEDESTAL_B_I2C_ADDRESS, 5);
    while (Wire.available()) {
      byte b1 = Wire.read(); // receive a byte as character
      byte b2 = Wire.read();
      byte b3 = Wire.read(); // receive a byte as character
      byte b4 = Wire.read();
      byte b5 = Wire.read();

      rp_s = b1;
      rp_enc0 = b2;
      rp_enc1 = b3;
      rp_enc2 = b4;
      rp_enc3 = b5;

//      Serial.println(rp_s); // uncomment to view decimal values for rotary switch positions 

      parse_radio_panel(0, rp_enc0, rp_s, rp_b);
      parse_radio_panel(1, rp_enc1, rp_s, rp_b);
      parse_radio_panel(2, rp_enc2, rp_s, rp_b);
      parse_radio_panel(3, rp_enc3, rp_s, rp_b);

    }

   
  }

    void ministick_to_mouse(uint8_t x, uint8_t y) {
    if (y < VRMAXII_PEDESTAL_Y_CENTER - 3) {
        // UP
//        if (y < Y_CENTER - 110) {
//          Mouse.move(0, -3, 0);
//        }
        if (y < VRMAXII_PEDESTAL_Y_CENTER - 115) {
          Mouse.move(0, -g_ministick_sensitivity, 0);
        }
        else {
          Mouse.move(0, -1, 0);
          delay(15);
        }
      }
      else if (y > VRMAXII_PEDESTAL_Y_CENTER + 3) {
        // DOWN
//        if (y > Y_CENTER + 110) {
//          Mouse.move(0, 3, 0);
//        }
        if (y > VRMAXII_PEDESTAL_Y_CENTER + 115) {
          Mouse.move(0, g_ministick_sensitivity, 0);
        }
        else {
          Mouse.move(0, 1, 0);
          delay(15);
        }
      }
      if (x < VRMAXII_PEDESTAL_X_CENTER - 3) { 
        // LEFT
//        if (x < X_CENTER - 110) {
//          Mouse.move(-3, 0, 0);
//        }
        if (x < VRMAXII_PEDESTAL_X_CENTER - 115) {
          Mouse.move(-g_ministick_sensitivity, 0, 0);
        }
        else {
          Mouse.move(-1, 0, 0);
          delay(15);
        }
      }
      else if (x > VRMAXII_PEDESTAL_X_CENTER + 3) { 
        // RIGHT
//        if (x > X_CENTER + 110) {
//          Mouse.move(+3, 0, 0);
//        }
        if (x > VRMAXII_PEDESTAL_X_CENTER + 115) {
          Mouse.move(g_ministick_sensitivity, 0, 0);
        }
        else {
          Mouse.move(1, 0, 0);
          delay(15);
        }
      }
    }

    void parse_button_array(uint8_t b, uint8_t start_pos,uint8_t end_pos, uint8_t modifier) {
      if (end_pos == 0) {
        end_pos = 8;
      }
      for (byte i = 0; i < end_pos; i++) {
        bool v = (b >> i) & 1;
        
        if (v != g_vrm2pdstl_lastButtonState[i + start_pos + modifier]) {
          if (((start_pos + i) !=  VRMAXII_PEDESTAL_MB_LEFT - 1) && ((start_pos + i) !=  VRMAXII_PEDESTAL_MB_RIGHT - 1) && ((start_pos + i) !=  VRMAXII_PEDESTAL_MINISTICK_SENS_SWITCH_BTN - 1)) {
            j_vrm2pdstl.setButton(i + start_pos + modifier, v);     
          } else if (((start_pos + i) ==  VRMAXII_PEDESTAL_MB_LEFT - 1) && (v == 1)) {
            Mouse.press(MOUSE_LEFT);
          } else if (((start_pos + i) ==  VRMAXII_PEDESTAL_MB_LEFT - 1) && (v == 0)) {
            Mouse.release(MOUSE_LEFT);
          } else if (((start_pos + i) ==  VRMAXII_PEDESTAL_MB_RIGHT - 1) && (v == 1)) {
            Mouse.press(MOUSE_RIGHT);
          } else if (((start_pos + i) ==  VRMAXII_PEDESTAL_MB_RIGHT - 1) && (v == 0)) {
            Mouse.release(MOUSE_RIGHT);
          } else if (((start_pos + i) ==  VRMAXII_PEDESTAL_MINISTICK_SENS_SWITCH_BTN - 1) && (v == 1)) {
            if (g_ministick_sensitivity == VRMAXII_PEDESTAL_MINISTICK_SENSITIVITY_LOW) {
              g_ministick_sensitivity = VRMAXII_PEDESTAL_MINISTICK_SENSITIVITY_HIGH;
            } else {
              g_ministick_sensitivity = VRMAXII_PEDESTAL_MINISTICK_SENSITIVITY_LOW;
            }
          }
        }
        g_vrm2pdstl_lastButtonState[i + start_pos + modifier] = v;

        
      }
    }

    void parse_encoder(uint8_t e) {
      if (e != g_vrm2pdstl_enc_lastVal) {
        if (VRMAXII_PEDESTAL_MOUSEWHEEL_DIR == "NOR") {
          Mouse.move(0,0,g_vrm2pdstl_enc_lastVal - e);
        } else {
          Mouse.move(0,0,e - g_vrm2pdstl_enc_lastVal);
        }
        g_vrm2pdstl_enc_lastVal = e;
      }
    }

    void parse_radio_panel(uint8_t enc, uint8_t val, uint8_t s, uint8_t b) {
      uint8_t mod;
      if (s == SELECTOR_POS_0_DECIMAL_VAL) {
        mod = 40;
      } else if (s == SELECTOR_POS_1_DECIMAL_VAL){
        mod = 50;
      } else if (s == SELECTOR_POS_2_DECIMAL_VAL){
        mod = 60;
      } else if (s == SELECTOR_POS_2_DECIMAL_VAL){
        mod = 70;
      } else if (s == SELECTOR_POS_3_DECIMAL_VAL){
        mod = 80;
      } else if (s == SELECTOR_POS_4_DECIMAL_VAL){
        mod = 90;
      } else if (s == SELECTOR_POS_5_DECIMAL_VAL){
        mod = 100;
      } else if (s == SELECTOR_POS_6_DECIMAL_VAL){
        mod = 110;
      } else if (s == SELECTOR_POS_7_DECIMAL_VAL){
        mod = 120;
      }

      for (byte i = 0; i < 2; i++) {
        bool v = (b >> i) & 1;

        if (v != g_vrm2pdstl_rp_lastButtonState[i]) {
            j_vrm2pdstl.setButton(i + mod + 8, v);     
        }
        g_vrm2pdstl_rp_lastButtonState[i] = v;
      }
    
      if (enc == 0) {
        if (val != g_vrm2_enc_lastVal.enc0) {
          int16_t val_diff = val - g_vrm2_enc_lastVal.enc0;
          if (abs(val_diff) > 100) {
            g_vrm2_enc_lastVal.enc0 = val;
          }
          if (val < g_vrm2_enc_lastVal.enc0) {
            j_vrm2pdstl.setButton(0 + mod,1);
            delay(20);
            j_vrm2pdstl.setButton(0 + mod,0);
          } else {
            j_vrm2pdstl.setButton(1 + mod,1);
            delay(20);
            j_vrm2pdstl.setButton(1 + mod,0);
          }      
          g_vrm2_enc_lastVal.enc0 = val;
        }
      } else if (enc == 1) {
        if (val != g_vrm2_enc_lastVal.enc1) {
          int16_t val_diff = val - g_vrm2_enc_lastVal.enc1;
          if (abs(val_diff) > 100) {
            g_vrm2_enc_lastVal.enc1 = val;
          }
          if (val < g_vrm2_enc_lastVal.enc1) {
            j_vrm2pdstl.setButton(2 + mod,1);
            delay(20);
            j_vrm2pdstl.setButton(2 + mod,0);
          } else {
            j_vrm2pdstl.setButton(3 + mod,1);
            delay(20);
            j_vrm2pdstl.setButton(3 + mod,0);
          }      
          g_vrm2_enc_lastVal.enc1 = val;
        }
      } else if (enc == 2) {
        if (val != g_vrm2_enc_lastVal.enc2) {
          int16_t val_diff = val - g_vrm2_enc_lastVal.enc2;
          if (abs(val_diff) > 100) {
            g_vrm2_enc_lastVal.enc2 = val;
          }
          if (val < g_vrm2_enc_lastVal.enc2) {
            j_vrm2pdstl.setButton(4 + mod,1);
            delay(20);
            j_vrm2pdstl.setButton(4 + mod,0);
          } else {
            j_vrm2pdstl.setButton(5 + mod,1);
            delay(20);
            j_vrm2pdstl.setButton(5 + mod,0);
          }      
          g_vrm2_enc_lastVal.enc2 = val;
        }
      } else if (enc == 3) {
        if (val != g_vrm2_enc_lastVal.enc3) {
          int16_t val_diff = val - g_vrm2_enc_lastVal.enc3;
          if (abs(val_diff) > 100) {
            g_vrm2_enc_lastVal.enc3 = val;
          }
          if (val < g_vrm2_enc_lastVal.enc3) {
            j_vrm2pdstl.setButton(6 + mod,1);
            delay(20);
            j_vrm2pdstl.setButton(6 + mod,0);
          } else {
            j_vrm2pdstl.setButton(7 + mod,1);
            delay(20);
            j_vrm2pdstl.setButton(7 + mod,0);
          }      
          g_vrm2_enc_lastVal.enc3 = val;
        }
      }
    }

   

#endif
