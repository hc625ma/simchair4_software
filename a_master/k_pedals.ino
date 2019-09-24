#if (defined PEDALS)
  Joystick_ j_pedals(0x40, 0x05, 0, 0, false, false, false, false, false, false, true, false, false, false, false);
  Adafruit_ADS1115 pedals(0x4A);
  
  void setup_pedals() {
    j_pedals.begin();
    //initialize ADS1115 filters
    for (int thisReading = 0; thisReading < g_xy_readings; thisReading++){
      g_buf_rudder[thisReading] = 0;
    }
    if (INVERT_RUDDER == 1) {
      j_pedals.setRudderRange(PEDALS_ADC_RANGE, 0);
    } else {
      j_pedals.setRudderRange(0, PEDALS_ADC_RANGE);
    }
    pedals.begin();
    pedals.setSPS(ADS1115_DR_64SPS);
    pedals.setGain(GAIN_ONE);
  
    g_physical_pedals_center = pedals.readADC_SingleEnded(0) >> (15 - ADS1115_RESOLUTION);
    if (INVERT_RUDDER == 1) {
      g_physical_pedals_center = PEDALS_ADC_RANGE - g_physical_pedals_center;
    }
  
    g_force_trim_rudder = g_physical_pedals_center;
  }
  
  void poll_pedals() {
    int rudder;

      rudder = pedals.readADC_SingleEnded(0) >> (15 - ADS1115_RESOLUTION);
      if (RUDDER_FILTERING_ENABLED == 1) {
        g_EMA_Sp = (g_EMA_a*rudder) + ((1-g_EMA_a)*g_EMA_Sp);    //run the EMA
        rudder = g_EMA_Sp;

      }
      //Serial.println(rudder); //uncomment to calibrate pedals
    if (g_ftcr == 1) {
      g_rudder_diff = 0;
      if (INVERT_RUDDER == 1) {
        g_physical_pedals_center = PEDALS_ADC_RANGE - g_physical_pedals_center;
      }
    }
    if (RATES_POTS_ENABLED == 1) {
      rudder = adjust_sensitivity(rudder, g_rudder_sens);
    } else {
      rudder = adjust_sensitivity(rudder, CUSTOM_RUDDER_SENS);
    }  
    if (g_force_trim_rudder_on == 0) { // button is not pressed 
      if ((g_pedals_force_trim_state == 0) || (g_pedals_force_trim_state == 2)) {
        if (g_force_trim_rudder - (PEDALS_ADC_RANGE /2) - rudder < 0) {
          g_rudder_val = rudder + g_rudder_diff;
        } else if (g_force_trim_rudder + (PEDALS_ADC_RANGE /2) - rudder > 0) {
          g_rudder_val = rudder - g_rudder_diff;
        }
        if (g_rudder_val < 0) {
          g_rudder_val = 0;
        } else if (g_rudder_val > PEDALS_ADC_RANGE) {
          g_rudder_val = PEDALS_ADC_RANGE;
        }
        j_pedals.setRudder(g_rudder_val);
      }

      if (g_pedals_force_trim_state == 1) { // after assigning a new center, wait for the stick to be returned to its mechanical center
        if (FORCE_TRIM_BUTTON_MODE == "HOLD") {
          if (g_force_trim_button_pressed == 0) {
            g_pedals_force_trim_state = 2;
            g_rudder_diff = rudder - g_force_trim_rudder;
          }
        } else {
          if (g_controls_freezed == 0) {
            g_pedals_force_trim_state = 2;
            g_rudder_diff = rudder - g_force_trim_rudder;
          }
        }
      }
    } else if (g_force_trim_rudder_on == 1) { // button is pressed
      g_force_trim_rudder_on = 0;
      if (g_pedals_force_trim_state == 0) { //1st trim button press - assign new center
        g_force_trim_rudder = g_rudder_val;
        g_pedals_force_trim_state = 1;
      }
      if ((g_pedals_force_trim_state == 2)) { //2nd button press - return center to its default position
        g_force_trim_rudder = g_rudder_val;
        g_pedals_force_trim_state = 1;
      }
    }
  }

#endif
