// <GLOBAL>
  int CBASE_ADC_RANGE = 0.5 + pow(2, ADS1115_RESOLUTION);
  int PEDALS_ADC_RANGE = 0.5 + pow(2, ADS1115_RESOLUTION);
  int COLLECTIVE_ADC_RANGE = 0.5 + pow(2, 10);

  int g_one_percent_cbase_range = CBASE_ADC_RANGE / 100;
  int g_one_percent_pedals_range = CBASE_ADC_RANGE / 100;
  int g_one_percent_coll_range = COLLECTIVE_ADC_RANGE / 100;

  // <FORCE_TRIM>
    #if ((defined PEDALS) || (defined B8_GRIP) || (defined CYCLIC_BASE))
      byte g_pedals_force_trim_state = 0;
      bool g_controls_freezed = 0;
      byte g_cyclic_sens = 100;
      byte g_rudder_sens = 100;
      int g_physical_cyclic_center_x = 0;
      int g_physical_cyclic_center_y = 0;
      bool g_force_trim_on = 0;
      int g_x_diff = 0;
      int g_y_diff = 0;
      int g_cyclic_x_adj = 0;
      int g_cyclic_y_adj = 0;
      uint16_t g_force_trim_x = CBASE_ADC_RANGE / 2;
      uint16_t g_force_trim_y = CBASE_ADC_RANGE / 2;
      uint16_t g_force_trim_rudder = PEDALS_ADC_RANGE / 2;
      byte g_cyclic_force_trim_state;
      bool g_force_trim_button_pressed = 0;
      int32_t g_xval;
      int32_t g_yval;
      bool g_force_trim_rudder_on = 0;
      bool g_force_trim_rudder_position_set = 0;
      const int g_xy_readings = 6;
      int g_physical_pedals_center = 0;
      uint8_t g_ftcr;
    #endif
  // </FORCE_TRIM>
  
// </GLOBAL>

// <SPDSTL>
  #if (defined SIMPLE_PEDESTAL)
    bool g_spdstl_lastButtonState[17]; // number of buttons + 1
    uint8_t g_spdstl_enc_lastVal = 0;
    uint8_t g_ministick_sensitivity = MINISTICK_SENSITIVITY_LOW;
  #endif
// </SPDSTL>


// <CYCLIC_BASE>
#if (defined CYCLIC_BASE)

  int g_buf_x[g_xy_readings];
  int g_buf_y[g_xy_readings];
  int g_xy_read_index = 0; // the index of the current reading
  long g_total_x = 0;  // the running total
  long g_total_y = 0;
  int g_xval_prev;
  int g_diffy;
  int g_diffx;
#endif
// </CYLIC_BASE>

// <B8_GRIP>
  #if (defined B8_GRIP)
    bool g_b8grip_lastButtonState[] = {0, 0, 0, 0, 0, 0};
    bool g_force_trim_position_set = 0; 
  #endif
// </B8_GRIP>

// <PEDALS>
  #if (defined PEDALS)
    long g_total_rudder = 0;
    int g_buf_rudder[RUDDER_READINGS];
    int g_rudder_read_index = 0;
    int g_rudder_diff = 0;
    int32_t g_rudder_val;
  #endif
// </PEDALS>
