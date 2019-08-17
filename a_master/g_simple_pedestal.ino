#if (defined SIMPLE_PEDESTAL)
  Joystick_ j_spdstl(0x21, 0x05, 32, 0, false, false, true, false, false, false, false, false, false, false, false);

  void setup_simple_pedestal() {
    j_spdstl.begin();   
    j_spdstl.setZAxisRange(0, 1023);
    Mouse.begin();
  }
  
  void poll_simple_pedestal() {
    uint16_t x,y,z;
    uint8_t ba0,ba1,enc;
    uint8_t mod = 0;
  
    Wire.requestFrom(SIMPLE_PEDESTAL_I2C_ADDRESS, 7);
    while (Wire.available())
    {
      byte b1 = Wire.read(); // receive a byte as character
      byte b2 = Wire.read();
      byte b3 = Wire.read(); // receive a byte as character
      byte b4 = Wire.read();
      byte b5 = Wire.read();
      byte b6 = Wire.read();
      byte b7 = Wire.read();

  
      x = b1;
      y = b2;
      z = b3;
      z = (z << 8) | b4;
      ba0 = b5;
      ba1 = b6;
      enc = b7;
      
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
      if (SIMPLE_PEDESTAL_ZOOM_STABILIZER_ENABLED) {
        int16_t zdiff = z - g_spdstl_z_val;
        if (abs(zdiff) > SIMPLE_PEDESTAL_ZOOM_STEP) {
          j_spdstl.setZAxis(z);
          g_spdstl_z_val = z;
        }
      } else {
        j_spdstl.setZAxis(z);
      }
      parse_encoder(enc);

    }
   
  }

    void ministick_to_mouse(uint8_t x, uint8_t y) {
    if (y < SIMPLE_PEDESTAL_Y_CENTER - 3) {
        // UP
//        if (y < Y_CENTER - 110) {
//          Mouse.move(0, -3, 0);
//        }
        if (y < SIMPLE_PEDESTAL_Y_CENTER - 115) {
          Mouse.move(0, -g_ministick_sensitivity, 0);
        }
        else {
          Mouse.move(0, -1, 0);
          delay(15);
        }
      }
      else if (y > SIMPLE_PEDESTAL_Y_CENTER + 3) {
        // DOWN
//        if (y > Y_CENTER + 110) {
//          Mouse.move(0, 3, 0);
//        }
        if (y > SIMPLE_PEDESTAL_Y_CENTER + 115) {
          Mouse.move(0, g_ministick_sensitivity, 0);
        }
        else {
          Mouse.move(0, 1, 0);
          delay(15);
        }
      }
      if (x < SIMPLE_PEDESTAL_X_CENTER - 3) { 
        // LEFT
//        if (x < X_CENTER - 110) {
//          Mouse.move(-3, 0, 0);
//        }
        if (x < SIMPLE_PEDESTAL_X_CENTER - 115) {
          Mouse.move(-g_ministick_sensitivity, 0, 0);
        }
        else {
          Mouse.move(-1, 0, 0);
          delay(15);
        }
      }
      else if (x > SIMPLE_PEDESTAL_X_CENTER + 3) { 
        // RIGHT
//        if (x > X_CENTER + 110) {
//          Mouse.move(+3, 0, 0);
//        }
        if (x > SIMPLE_PEDESTAL_X_CENTER + 115) {
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
        
        if (v != g_spdstl_lastButtonState[i + start_pos + modifier]) {
          if (((start_pos + i) !=  SIMPLE_PEDESTAL_MB_LEFT - 1) && ((start_pos + i) !=  SIMPLE_PEDESTAL_MB_RIGHT - 1) && ((start_pos + i) !=  SIMPLE_PEDESTAL_MINISTICK_SENS_SWITCH_BTN - 1)) {
            j_spdstl.setButton(i + start_pos + modifier, v);     
          } else if (((start_pos + i) ==  SIMPLE_PEDESTAL_MB_LEFT - 1) && (v == 1)) {
            Mouse.press(MOUSE_LEFT);
          } else if (((start_pos + i) ==  SIMPLE_PEDESTAL_MB_LEFT - 1) && (v == 0)) {
            Mouse.release(MOUSE_LEFT);
          } else if (((start_pos + i) ==  SIMPLE_PEDESTAL_MB_RIGHT - 1) && (v == 1)) {
            Mouse.press(MOUSE_RIGHT);
          } else if (((start_pos + i) ==  SIMPLE_PEDESTAL_MB_RIGHT - 1) && (v == 0)) {
            Mouse.release(MOUSE_RIGHT);
          } else if (((start_pos + i) ==  SIMPLE_PEDESTAL_MINISTICK_SENS_SWITCH_BTN - 1) && (v == 1)) {
            if (g_ministick_sensitivity == SIMPLE_PEDESTAL_MINISTICK_SENSITIVITY_LOW) {
              g_ministick_sensitivity = SIMPLE_PEDESTAL_MINISTICK_SENSITIVITY_HIGH;
            } else {
              g_ministick_sensitivity = SIMPLE_PEDESTAL_MINISTICK_SENSITIVITY_LOW;
            }
          }
        }
        g_spdstl_lastButtonState[i + start_pos + modifier] = v;

        
      }
    }

    void parse_encoder(uint8_t e) {
      if (e != g_spdstl_enc_lastVal) {
        if (SIMPLE_PEDESTAL_MOUSEWHEEL_DIR == "NOR") {
          Mouse.move(0,0,g_spdstl_enc_lastVal - e);
        } else {
          Mouse.move(0,0,e - g_spdstl_enc_lastVal);
        }
        g_spdstl_enc_lastVal = e;
      }
    }

#endif
