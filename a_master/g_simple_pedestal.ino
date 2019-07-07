#if (defined SIMPLE_PEDESTAL)
  Joystick_ j_spdstl(0x21, 0x05, 12, 1, false, false, true, false, false, false, false, false, false, false, false);

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
      byte b8 = Wire.read();
      byte b9 = Wire.read();
  
      x = b1;
      x = (x << 8) | b2;
      y = b3;
      y = (y << 8) | b4;
      z = b5;
      z = (z << 8) | b6;
      ba0 = b7;
      ba1 = b8;
      enc = b9;
  
    }

  }
#endif
