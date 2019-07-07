#if (defined COMPACT_COLLECTIVE)
  Joystick_ j_ccoll(0x20, 0x05, 11, 1, false, false, true, false, false, false, false, true, false, false, false);

  void setup_compact_collective() {
    j_ccoll.begin();
    j_ccoll.setZAxisRange(COMPACT_COLLECTIVE_MIN, COMPACT_COLLECTIVE_MAX);
    j_ccoll.setThrottleRange(COMPACT_COLLECTIVE_THR_MIN, COMPACT_COLLECTIVE_THR_MAX);
  }
  
  void poll_compact_collective() {
    uint16_t z;
    uint16_t throttle;
  
    Wire.requestFrom(COMPACT_COLLECTIVE_I2C_ADDRESS, 4);
    while (Wire.available())
    {
      byte b1 = Wire.read(); // receive a byte as character
      byte b2 = Wire.read();
      byte b3 = Wire.read(); // receive a byte as character
      byte b4 = Wire.read();
  
      z = b1;
      z = (z << 8) | b2;
      throttle = b3;
      throttle = (throttle << 8) | b4;
  
    }
    //Serial.println(z);
    j_ccoll.setZAxis(z);
    j_ccoll.setThrottle(throttle);    
  }
#endif
