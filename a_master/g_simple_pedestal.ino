#if (defined SIMPLE_PEDESTAL)
  Joystick_ j_spdstl(0x21, 0x05, 18, 0, false, false, true, false, false, false, false, false, false, false, false);

  void setup_simple_pedestal() {
    j_spdstl.begin();   
    j_spdstl.setZAxisRange(0, 1023);
    Mouse.begin();
  }
  
  void poll_simple_pedestal() {
    uint16_t x,y,z;
    uint8_t ba0,ba1,enc;
  
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
//    Serial.println(enc);

      ministick_to_mouse(x,y);
      parse_button_array(ba0,0);
      parse_button_array(ba1,8);
      j_spdstl.setZAxis(z);
      parse_encoder(enc);

    }
   
  }

    void ministick_to_mouse(uint8_t x, uint8_t y) {
    if (y < Y_CENTER - 3) {
        // UP
//        if (y < Y_CENTER - 110) {
//          Mouse.move(0, -3, 0);
//        }
        if (y < Y_CENTER - 115) {
          Mouse.move(0, -g_ministick_sensitivity, 0);
        }
        else {
          Mouse.move(0, -1, 0);
          delay(15);
        }
      }
      else if (y > Y_CENTER + 3) {
        // DOWN
//        if (y > Y_CENTER + 110) {
//          Mouse.move(0, 3, 0);
//        }
        if (y > Y_CENTER + 115) {
          Mouse.move(0, g_ministick_sensitivity, 0);
        }
        else {
          Mouse.move(0, 1, 0);
          delay(15);
        }
      }
      if (x < X_CENTER - 3) { 
        // LEFT
//        if (x < X_CENTER - 110) {
//          Mouse.move(-3, 0, 0);
//        }
        if (x < X_CENTER - 115) {
          Mouse.move(-g_ministick_sensitivity, 0, 0);
        }
        else {
          Mouse.move(-1, 0, 0);
          delay(15);
        }
      }
      else if (x > X_CENTER + 3) { 
        // RIGHT
//        if (x > X_CENTER + 110) {
//          Mouse.move(+3, 0, 0);
//        }
        if (x > X_CENTER + 115) {
          Mouse.move(g_ministick_sensitivity, 0, 0);
        }
        else {
          Mouse.move(1, 0, 0);
          delay(15);
        }
      }
    }

    void parse_button_array(uint8_t b, uint8_t start_pos) {
      for (byte i = 0; i < 8; i++) {
        bool v = (b >> i) & 1;
        
        if (v != g_spdstl_lastButtonState[i + start_pos]) {
          if (((start_pos + i) !=  MB_LEFT - 1) && ((start_pos + i) !=  MB_RIGHT - 1) && ((start_pos + i) !=  MINISTICK_SENS_SWITCH_BTN - 1)) {
            j_spdstl.setButton(i + start_pos, v);     
          } else if (((start_pos + i) ==  MB_LEFT - 1) && (v == 1)) {
            Mouse.press(MOUSE_LEFT);
          } else if (((start_pos + i) ==  MB_LEFT - 1) && (v == 0)) {
            Mouse.release(MOUSE_LEFT);
          } else if (((start_pos + i) ==  MB_RIGHT - 1) && (v == 1)) {
            Mouse.press(MOUSE_RIGHT);
          } else if (((start_pos + i) ==  MB_RIGHT - 1) && (v == 0)) {
            Mouse.release(MOUSE_RIGHT);
          } else if (((start_pos + i) ==  MINISTICK_SENS_SWITCH_BTN - 1) && (v == 1)) {
            if (g_ministick_sensitivity == MINISTICK_SENSITIVITY_LOW) {
              g_ministick_sensitivity = MINISTICK_SENSITIVITY_HIGH;
            } else {
              g_ministick_sensitivity = MINISTICK_SENSITIVITY_LOW;
            }
          }
        }
        g_spdstl_lastButtonState[i + start_pos] = v;

        
      }
    }

    void parse_encoder(uint8_t e) {
      if (e != g_spdstl_enc_lastVal) {
        if (MOUSEWHEEL_DIR == "NOR") {
          Mouse.move(0,0,g_spdstl_enc_lastVal - e);
        } else {
          Mouse.move(0,0,e - g_spdstl_enc_lastVal);
        }
        g_spdstl_enc_lastVal = e;
      }
    }

#endif
