#if (defined CYCLIC_BASE)
  Joystick_ j_cyclic(0x30, 0x04, 0, 0, true, true, false, false, false, false, false, false, false, false, false);
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
    cyclic.begin();
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
    uint32_t x, y;
  
    if (XY_FILTERING_ENABLED == 1) {
      g_total_x = g_total_x - g_buf_x[g_xy_read_index];
      g_total_y = g_total_y - g_buf_y[g_xy_read_index];
      g_buf_x[g_xy_read_index] = cyclic.readADC_SingleEnded(0) >> (15 - ADS1115_RESOLUTION);
      g_buf_y[g_xy_read_index] = cyclic.readADC_SingleEnded(1) >> (15 - ADS1115_RESOLUTION);
  
      g_total_x = g_total_x + g_buf_x[g_xy_read_index];
      g_total_y = g_total_y + g_buf_y[g_xy_read_index];
      g_xy_read_index = g_xy_read_index + 1;
  
      if (g_xy_read_index >= g_xy_readings) {
        // ...wrap around to the beginning:
        g_xy_read_index = 0;
      }
      
      x = g_total_x / g_xy_readings;
      y = g_total_y / g_xy_readings;
      
    } else {
      x = cyclic.readADC_SingleEnded(0) >> (15 - ADS1115_RESOLUTION);
      y = cyclic.readADC_SingleEnded(1) >> (15 - ADS1115_RESOLUTION);
      g_ftcr = cyclic.readADC_SingleEnded(2) >> 5;
      if (g_ftcr == 255) {
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
  
      // uncomment next 5 lines for cyclic calibration
//      Serial.print(x);
//      Serial.print(" ");
//      Serial.print(y);
//      Serial.print(" ");
//      Serial.println(g_ftcr);
    }
  
    if (SENS_SWITCH_ENABLED == 1) {
      x = adjust_sensitivity(x, g_cyclic_sens);
      y = adjust_sensitivity(y, g_cyclic_sens);
    } else {
      x = adjust_sensitivity(x, CUSTOM_CYCLIC_SENS);
      y = adjust_sensitivity(y, CUSTOM_CYCLIC_SENS);
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


#endif
