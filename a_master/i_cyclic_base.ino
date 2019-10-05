#if (defined CYCLIC_BASE)
  Joystick_ j_cyclic(0x30, 0x04, 0, 0, true, true, false, true, true, false, false, false, false, false, false);
  Adafruit_ADS1115 cyclic;
  
  void setup_cyclic_base(){
    //initialize ADS1115 filters
    
    for (int thisReading = 0; thisReading < g_xy_readings; thisReading++) {
      g_buf_x[thisReading] = 0;
      g_buf_y[thisReading] = 0;
    }
    
    j_cyclic.begin();
    j_cyclic.setXAxisRange(0, CBASE_ADC_RANGE);
    j_cyclic.setYAxisRange(0, CBASE_ADC_RANGE);
    j_cyclic.setRxAxisRange(0, RATE_POTS_ADC_RANGE);
    j_cyclic.setRyAxisRange(0, RATE_POTS_ADC_RANGE);
    cyclic.begin();
    cyclic.setSPS(ADS1115_DR_64SPS);
    cyclic.setGain(GAIN_ONE);
    
    g_physical_cyclic_center_x = cyclic.readADC_SingleEnded(0) >> (15 - ADS1115_RESOLUTION);
    g_physical_cyclic_center_y = cyclic.readADC_SingleEnded(1) >> (15 - ADS1115_RESOLUTION);
    
    if (FSGIMBAL_INVERT_X == 1) {
      g_physical_cyclic_center_x = CBASE_ADC_RANGE - g_physical_cyclic_center_x;
    }
    if (FSGIMBAL_INVERT_Y == 1) {
      g_physical_cyclic_center_y = CBASE_ADC_RANGE - g_physical_cyclic_center_y;
    }
    g_force_trim_x = g_physical_cyclic_center_x;
  }
  
  void poll_cyclic_base()
  {
    uint32_t x, y, rx, ry;
    
      x = cyclic.readADC_SingleEnded(0) >> (15 - ADS1115_RESOLUTION);
      y = cyclic.readADC_SingleEnded(1) >> (15 - ADS1115_RESOLUTION);
      rx = cyclic.readADC_SingleEnded(2) >> (15 - RATES_KNOB_RESOLUTION);
      ry = cyclic.readADC_SingleEnded(3) >> (15 - RATES_KNOB_RESOLUTION);

      if (rx > CBASE_ADC_RANGE) {
        rx = 0;
      }
      if (ry > CBASE_ADC_RANGE) {
        ry = 0;
      }

      g_cyclic_sens = set_rates(rx,g_cyclic_sens);
      g_rudder_sens = set_rates(ry,g_rudder_sens);
      

      if (XY_FILTERING_ENABLED == 1) {
        g_EMA_Sx = (g_EMA_a*x) + ((1-g_EMA_a)*g_EMA_Sx);    //run the EMA
        g_EMA_Sy = (g_EMA_a*y) + ((1-g_EMA_a)*g_EMA_Sy);    //run the EMA
        x = g_EMA_Sx;
        y = g_EMA_Sy;
      }
      //g_ftcr = cyclic.readADC_SingleEnded(2) >> 5;
      if (g_ftcr == 1) {
        g_x_diff = 0;
        g_y_diff = 0;
        g_cyclic_x_adj = 0;
        g_cyclic_y_adj = 0;
        if (FSGIMBAL_INVERT_X == 1) {
          g_physical_cyclic_center_x = CBASE_ADC_RANGE - g_physical_cyclic_center_x;
        }
        if (FSGIMBAL_INVERT_Y == 1) {
          g_physical_cyclic_center_y = CBASE_ADC_RANGE - g_physical_cyclic_center_y;
        }
      }
  
      if (FSGIMBAL_INVERT_X == 1) {
        x = CBASE_ADC_RANGE - x;
      }
      if (FSGIMBAL_INVERT_Y == 1) {
        y = CBASE_ADC_RANGE - y;
      }
  
      // uncomment the following lines for cyclic calibration
//      Serial.print(x);
//      Serial.print(" ");
//      Serial.print(y);
//      Serial.print(" ");
//      Serial.print(rx);
//      Serial.print(" ");
//      Serial.print(ry);
//      Serial.print(" ");
//      Serial.println(g_ftcr);

  
    if (RATES_POTS_ENABLED == 1) {
      x = adjust_sensitivity(x, g_cyclic_sens);
//      g_physical_cyclic_center_x = adjust_sensitivity (g_physical_cyclic_center_x, CUSTOM_CYCLIC_SENS);
      y = adjust_sensitivity(y, g_cyclic_sens);
//      g_physical_cyclic_center_y = adjust_sensitivity (g_physical_cyclic_center_y, CUSTOM_CYCLIC_SENS);              
    } else {
      x = adjust_sensitivity(x, CUSTOM_CYCLIC_SENS);
      y = adjust_sensitivity(y, CUSTOM_CYCLIC_SENS);

      if (RATES_POT_AXIS_MODE_STABILIZER_ENABLED) {
        int8_t rxdiff = rx - g_cbase_rx_val;
        int8_t rydiff = ry - g_cbase_ry_val;
        if (abs(rxdiff) > RATES_POT_AXIS_MODE_STABILIZER_STEP) {
          g_cbase_rx_val = rx;
          j_cyclic.setRxAxis(rx);
        }
        if (abs(rydiff) > RATES_POT_AXIS_MODE_STABILIZER_STEP) {
          g_cbase_ry_val = ry;
          j_cyclic.setRyAxis(ry);
        }
        rxdiff = rx - g_cbase_rx_val;
        rydiff = ry - g_cbase_ry_val;
      } else {
        j_cyclic.setRxAxis(rx);
        j_cyclic.setRyAxis(ry);
    }
      

    }
  
    if (g_force_trim_on == 0) { // button is not pressed 
      if ((g_cyclic_force_trim_state == 0) || (g_cyclic_force_trim_state == 2)) {
        if (g_force_trim_x - (CBASE_ADC_RANGE /2) - x < 0) {
          g_xval = x + g_x_diff + g_cyclic_x_adj;
          g_xval_prev = x;
        }
        else if (g_force_trim_x + (CBASE_ADC_RANGE /2) - x > 0) {
          g_xval = x - g_x_diff + g_cyclic_x_adj;
          g_xval_prev = x;
        }
        if (g_xval < 0) {
          g_xval = 0;
        }
        else if (g_xval > CBASE_ADC_RANGE) {
          g_xval = CBASE_ADC_RANGE;
        }
        if (g_force_trim_y - (CBASE_ADC_RANGE /2) - y < 0) {
          g_yval = y + g_y_diff + g_cyclic_y_adj;
        }
        else if (g_force_trim_y + (CBASE_ADC_RANGE /2) - y > 0) {
           g_yval = y - g_y_diff + g_cyclic_y_adj;
        }
        if (g_yval < 0) {
          g_yval = 0;
        }
        else if (g_yval > CBASE_ADC_RANGE) {
          g_yval = CBASE_ADC_RANGE;
        }

        j_cyclic.setXAxis(g_xval);
        j_cyclic.setYAxis(g_yval);
      }
      if (g_cyclic_force_trim_state == 1) {// after assigning a new center, wait for the stick to be returned to its mechanical center
        if (FORCE_TRIM_BUTTON_MODE == "HOLD") {
          if (g_force_trim_button_pressed == 0) {
            g_cyclic_force_trim_state = 2;
            g_x_diff = x - g_force_trim_x;
            g_y_diff = y - g_force_trim_y;
            g_cyclic_x_adj = 0;
            g_cyclic_y_adj = 0;
          }
        } else {
          if (g_controls_freezed == 0) {
            g_cyclic_force_trim_state = 2;
            g_x_diff = x - g_force_trim_x - g_cyclic_x_adj;
            g_y_diff = y - g_force_trim_y - g_cyclic_y_adj;
            g_cyclic_x_adj = 0;
            g_cyclic_y_adj = 0;
          }
        }
      }

    }
    else if (g_force_trim_on == 1) {// button is pressed
      g_diffy = y - g_force_trim_y;
      g_diffx = g_xval - g_force_trim_x;
      g_force_trim_on = 0;
      
      if (g_cyclic_force_trim_state == 0) {//1st trim button press - assign new center
        g_force_trim_x = g_xval;//(x - ADC_RANGE / 2);//(physical_cyclic_center_x));
        g_force_trim_y = g_yval;//(y - ADC_RANGE / 2);//(physical_cyclic_center_y));
        g_cyclic_force_trim_state = 1;
      }
      if ((g_cyclic_force_trim_state == 2)) {//2nd button press - return center to its default position
        g_force_trim_x = g_xval;
        g_force_trim_y = g_yval;
        g_cyclic_force_trim_state = 1;
      }
    }
  }

  uint8_t set_rates (uint16_t val, uint8_t rate) {
    // device: 0 - cyclic, 1 - pedals
    for (byte i = 0; i < 9; i++) {
      int deviation = val - g_struct_cbase_rates[i].pot_val;
      deviation = abs(deviation);
      
      if (deviation <= RATES_TRESHOLD) {
        rate = g_struct_cbase_rates[i].sens_val;
      }
    }
    return rate;
  }


#endif
