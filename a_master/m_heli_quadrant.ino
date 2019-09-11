#if (defined HELI_QUADRANT) 
Joystick_ j_hq(0x50, 0x05, 4, 0, false, false, false, true, true, false, false, false, false, false, false);

  void setup_heli_quadrant() {
    j_hq.begin();
    j_hq.setRxAxisRange(HQ_AXIS1_MIN, HQ_AXIS1_MAX);
    j_hq.setRyAxisRange(HQ_AXIS2_MIN, HQ_AXIS2_MAX);
  }
  
  void poll_heli_quadrant() {
    uint16_t rx,ry;
  
    Wire.requestFrom(HELI_QUADRANT_I2C_ADDRESS, 4);
    while (Wire.available()) {
      byte b1 = Wire.read(); // receive a byte as character
      byte b2 = Wire.read();
      byte b3 = Wire.read();
      byte b4 = Wire.read();
  
      rx = b1;
      rx = (rx << 8) | b2; 
      ry = b3;
      ry = (ry << 8) | b4; 
    }    
//    Serial.print(rx);
//    Serial.print(" ");
//    Serial.println(ry);

    
    if (HQ_AXIS_STABILIZER_ENABLED) {
      int16_t rxdiff = rx - g_hq_rx_val;
      int16_t rydiff = ry - g_hq_ry_val;
      if (abs(rxdiff) > HQ_AXIS_STABILIZER_STEP) {
        g_hq_rx_val = rx;
        if ((rx >= HQ_AXIS1_MIN) && (rx <= HQ_AXIS1_MAX)) {
          j_hq.setRxAxis(rx);
        }
      }
      if (abs(rydiff) > HQ_AXIS_STABILIZER_STEP) {
        g_hq_ry_val = ry;
        if ((ry >= HQ_AXIS2_MIN) && (ry <= HQ_AXIS2_MAX)) {
          j_hq.setRyAxis(ry);
        }
      }
    } else {
      g_hq_rx_val = rx;
      g_hq_ry_val = ry;
      if ((rx >= HQ_AXIS1_MIN) && (rx <= HQ_AXIS1_MAX)) {
        if ((HQ_AXIS1_MIN - rx ) < HQ_AXIS_STABILIZER_STEP) {
          rx = HQ_AXIS1_MIN;
        } else if ((HQ_AXIS1_MAX - rx ) < HQ_AXIS_STABILIZER_STEP) {
          rx = HQ_AXIS1_MAX;
        }
        j_hq.setRxAxis(rx);
      }
      if ((ry >= HQ_AXIS2_MIN) && (ry <= HQ_AXIS2_MAX)) {
        if ((HQ_AXIS2_MIN - ry ) < HQ_AXIS_STABILIZER_STEP) {
          ry = HQ_AXIS2_MIN;
        } else if ((HQ_AXIS2_MAX - ry ) < HQ_AXIS_STABILIZER_STEP) {
          ry = HQ_AXIS2_MAX;
        }
        j_hq.setRyAxis(ry);
      }
    }
    
    

    if (rx < (HQ_AXIS1_DT_MIN + 20)) {
      if (g_hq_lastButtonState[0] != 1) {
        g_hq_lastButtonState[0] = 1;
        j_hq.setButton(0,1);
      }
    } else if (rx > (HQ_AXIS1_DT_MAX - 20)){
      if (g_hq_lastButtonState[1] != 1) {
        g_hq_lastButtonState[1] = 1;
        j_hq.setButton(1,1);
      }      
    } else {
      if (g_hq_lastButtonState[0] != 0) {
        g_hq_lastButtonState[0] = 0;
        j_hq.setButton(0,0);
      }
      if (g_hq_lastButtonState[1] != 0) {
        g_hq_lastButtonState[1] = 0;
        j_hq.setButton(1,0);
      }
    }

    if (ry < (HQ_AXIS2_DT_MIN + 20)) {
      if (g_hq_lastButtonState[2] != 1) {
        g_hq_lastButtonState[2] = 1;
        j_hq.setButton(2,1);
      }
    } else if (ry > (HQ_AXIS2_DT_MAX - 20)){
      if (g_hq_lastButtonState[3] != 1) {
        g_hq_lastButtonState[3] = 1;
        j_hq.setButton(3,1);
      }      
    } else {
      if (g_hq_lastButtonState[2] != 0) {
        g_hq_lastButtonState[2] = 0;
        j_hq.setButton(2,0);
      }
      if (g_hq_lastButtonState[3] != 0) {
        g_hq_lastButtonState[3] = 0;
        j_hq.setButton(3,0);
      }
//      g_hq_lastButtonState[2] = 0;
//      g_hq_lastButtonState[3] = 0;
//      j_hq.setButton(2,0);
//      j_hq.setButton(3,0);
    }

  }  


#endif
