#if (defined COLLECTIVE_NOTHR)
  Joystick_ j_coll_nthr(0x20, 0x05, 2, 0, false, false, true, false, false, false, false, false, false, false, false);

  void setup_coll_nothr() {
    j_coll_nthr.begin();
    j_coll_nthr.setZAxisRange(COLLECTIVE_NOTHR_MIN, COLLECTIVE_NOTHR_MAX);
  }
  
  void poll_coll_nothr() {
    uint16_t z;
    uint8_t b;
  
    Wire.requestFrom(COLLECTIVE_NOTHR_I2C_ADDRESS, 3);
    while (Wire.available()) {
      byte b1 = Wire.read(); // receive a byte as character
      byte b2 = Wire.read();
      byte b3 = Wire.read();
  
      z = b1;
      z = (z << 8) | b2; 
      b = b3;
    }    
    //Serial.print(z);

    j_coll_nthr.setZAxis(z);
    parse_button_array_coll_nthr(b,0);

  }  

  void parse_button_array_coll_nthr(uint8_t b, uint8_t start_pos) {
    for (byte i = 0; i < 8; i++) {
      bool v = (b >> i) & 1;
      
      if (v !=  g_coll_nthr_lastButtonState[i + start_pos]) {
        j_coll_nthr.setButton(i + start_pos, v);     
      }
       g_coll_nthr_lastButtonState[i + start_pos] = v;
    }
  }
    
#endif
