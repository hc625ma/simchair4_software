#if (defined COMPACT_COLLECTIVE)
  Joystick_ j_ccoll(0x20, 0x05, 16, 1, false, false, true, false, false, false, false, true, false, false, false);

  void setup_compact_collective() {
    j_ccoll.begin();
    j_ccoll.setZAxisRange(COMPACT_COLLECTIVE_MIN, COMPACT_COLLECTIVE_MAX);
    j_ccoll.setThrottleRange(COMPACT_COLLECTIVE_THR_MIN, COMPACT_COLLECTIVE_THR_MAX);
  }
  
  void poll_compact_collective() {
    uint16_t z;
    uint16_t throttle;
    uint8_t ba0,ba1;
    uint8_t x,y;
  
    Wire.requestFrom(COMPACT_COLLECTIVE_I2C_ADDRESS, 4);
    while (Wire.available()) {
      byte b1 = Wire.read(); // receive a byte as character
      byte b2 = Wire.read();
      byte b3 = Wire.read(); // receive a byte as character
      byte b4 = Wire.read();
  
      z = b1;
      z = (z << 8) | b2;
      throttle = b3;
      throttle = (throttle << 8) | b4;
  
    }

    Wire.requestFrom(COMPACT_COLLECTIVE_HEAD_I2C_ADDRESS, 4);
    while (Wire.available()) {
      byte b1 = Wire.read(); // receive a byte as character
      byte b2 = Wire.read();
      byte b3 = Wire.read();
      byte b4 = Wire.read();

      x = b1;
      y = b2;
      ba0 = b3;
      ba1 = b4;
    }
    
    //Serial.print(z);
    //Serial.print(" ");
    //Serial.print(throttle);
    //Serial.print(" ");
    //Serial.print(ba0);
    //Serial.print(" ");
    //Serial.print(ba1);
    //Serial.print(" ");
//    Serial.print(x);
//    Serial.print(" ");
//    Serial.println(y);
    j_ccoll.setZAxis(z);
    j_ccoll.setThrottle(throttle);

    int16_t hat_val = parse_hat_sw(x, y, 8);
    j_ccoll.setHatSwitch(0, hat_val);

    parse_button_array_ccoll(ba0,0);
    parse_button_array_ccoll(ba1,8);
  }

  void parse_button_array_ccoll(uint8_t b, uint8_t start_pos) {
      for (byte i = 0; i < 8; i++) {
        bool v = (b >> i) & 1;
        
        if (v != g_ccoll_lastButtonState[i + start_pos]) {
            j_ccoll.setButton(i + start_pos, v);     
        }
        g_ccoll_lastButtonState[i + start_pos] = v;

        
      }
    }

    
    
#endif
