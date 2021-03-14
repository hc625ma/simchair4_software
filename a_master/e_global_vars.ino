// THIS FILE CONTAINS DEFAULT GLOBAL VARS, TO ADD CUSTOM DEVICES PLACE YOUR OWN FILES AFTER THIS FILE
// FILES MUST BE CALLED e[a-z]_loop_<filename>.ino 

// <GLOBAL>
  bool g_operating_mode = 0; //0 - FLY mode, 1 - PRGM mode
  int CBASE_ADC_RANGE = 0.5 + pow(2, ADS1115_RESOLUTION);
  int PEDALS_ADC_RANGE = 0.5 + pow(2, ADS1115_RESOLUTION);
  int COLLECTIVE_ADC_RANGE = 0.5 + pow(2, 10);
  int RATE_POTS_ADC_RANGE = 0.5 + pow(2, RATES_KNOB_RESOLUTION);

  int g_one_percent_cbase_range = CBASE_ADC_RANGE / 100;
  int g_one_percent_pedals_range = CBASE_ADC_RANGE / 100;
  int g_one_percent_coll_range = COLLECTIVE_ADC_RANGE / 100;

  // CYCLIC/PEDALS FILTER
  float g_EMA_a = 0.4;      //initialization of EMA alpha
  int g_EMA_Sx = 0;          //initialization of cyclic x EMA S
  int g_EMA_Sy = 0;          //initialization of cyclic y EMA S
  int g_EMA_Sp = 0;          //initialization of pedals EMA S

  bool g_coll_mk3_detected = 0; // for the legacy MKIII mode switch on collective heads to work

  uint8_t g_coll_modesw_pos_decimal = 0;
  uint8_t g_coll_modesw_thr_pos_decimal = 0;

  bool g_coll_evo_thr_direct_control[2] = {0,0};
  uint16_t g_coll_evo_thr_buf[] = {0,0};
  int16_t g_coll_evo_diff[2] = {0,0};

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
      bool g_ftcr;
    #endif
  // </FORCE_TRIM>

  //<COLLECTIVE THR LATCH>
    bool g_idle_rel_btn_pressed = 0; // because button and the latch can have different states
    bool g_idle_rel_btn_pressed_new[2] = {0,0};
    bool g_tl_idle_rel_btn_pressed[2] = {0,0}; // because button and the latch can have different states  
  //</COLLECTIVE_THR_LATCH>

  //<MASTER_UART>
    #define HWSERIAL Serial1
    unsigned long baud = 57600;
    const int g_uart_reset_pin = 4;
    const int g_uart_led_pin = 13; 
    bool g_started_as_uart = 0;
  //</MASTER_UART>
  
// </GLOBAL>

// <SPDSTL>
  #if (defined SIMPLE_PEDESTAL)
    bool g_spdstl_lastButtonState[33]; // number of buttons + 1
    uint8_t g_spdstl_enc_lastVal = 0;
    uint8_t g_ministick_sensitivity = SIMPLE_PEDESTAL_MINISTICK_SENSITIVITY_LOW;
    int g_spdstl_z_val = 0;
  #endif
// </SPDSTL>

// <VRMAXII PEDESTAL>
  #if (defined VRMAXII_PEDESTAL)
    bool g_vrm2pdstl_lastButtonState[131]; // number of buttons + 1
    bool g_vrm2pdstl_rp_lastButtonState[2];
    uint8_t g_vrm2pdstl_enc_lastVal = 0;
    uint8_t g_ministick_sensitivity = VRMAXII_PEDESTAL_MINISTICK_SENSITIVITY_LOW;
    t_struct_vrm2_enc_lastVal g_vrm2_enc_lastVal;
    int g_vrm2pdstl_z_val = 0;
   
  #endif
// </VRMAXII PEDESTAL>


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
  uint8_t g_cbase_rx_val = 0;
  uint8_t g_cbase_ry_val = 0;

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

// <COMPACT COLLECTIVE>
  #if (defined COMPACT_COLLECTIVE)
    bool g_ccoll_lastButtonState[36];
    uint16_t g_ccoll_thr_val;
//    bool g_idle_rel_btn_pressed = 0; // because button and the latch can have different states  
    bool g_throttle_latch_pressed = 1; // ^^^
    bool g_physical_latch_button_state = 0; // used for pressing button at 0

  #endif
// </COMPACT_COLLECTIVE>

// <SINGLE COLLECTIVE>
  #if ((defined COLLECTIVE_STHR) || (defined COLLECTIVE_STHR_MKIII))
    bool g_scoll_lastButtonState[3];
    uint16_t g_scoll_thr_val;
      
    bool g_throttle_latch_pressed = 1; // because button and the latch can have different states  
    bool g_physical_latch_button_state = 0; // used for pressing button at 0

  #endif

// </SINGLE_COLLECTIVE>

// <SINGLE COLLECTIVE_EVO_USB>
  #if ((defined COLLECTIVE_STHR_EVO_USB))
    bool g_scoll_lastButtonState[3];
    uint16_t g_scoll_thr_val;
      
    bool g_throttle_latch_pressed = 1; // because button and the latch can have different states  
    bool g_physical_latch_button_state = 0; // used for pressing button at 0
    
    uint8_t g_coll_evo_usb_filter_counter_z = SINGLE_COLLECTIVE_EVO_USB_FILTER_COUNTER_COLL_AXIS;
    uint8_t g_coll_evo_usb_filter_counter_thr[] = {SINGLE_COLLECTIVE_EVO_USB_FILTER_COUNTER_THR_AXIS,};

    uint8_t g_coll_evo_usb_init_counter = 0;
    

    #define COLL_EVO_USB_PHYS_THR1_MIN 0
    #define COLL_EVO_USB_PHYS_THR1_MAX 0
    #define COLL_EVO_USB_PHYS_THR1_MAX_MINUS_PHYS_THR1_MIN 0
  #endif

// </SINGLE_COLLECTIVE_EVO_USB>

// <SIMPLE COLLECTIVE>
  #if (defined SIMPLE_COLLECTIVE)
    bool g_scoll_lastButtonState[20];
    uint16_t g_scoll_thr_val;
      
    bool g_throttle_latch_pressed = 1; // because button and the latch can have different states  
    bool g_physical_latch_button_state = 0; // used for pressing button at 0
   

  #endif

// </SIMPLE_COLLECTIVE>

// <TWIN COLLECTIVE>
  #if ((defined COLLECTIVE_TTHR) || (defined COLLECTIVE_TTHR_MKIII))
    bool g_tcoll_lastButtonState[3];
    uint16_t g_tcoll_thr_val[2];
    
    bool g_tl_throttle_latch_pressed[2] = {0,0}; // ^^^
   bool g_tl_physical_latch_button_state[2] = {0,0}; // used for pressing button at 0

  #endif

// </TWIN_COLLECTIVE>

// <AB412_COLL_HEAD>
  #if (defined AB412_COLL_SWITCH_PANEL)
    uint8_t g_ab412h_lpot_val = 0;
    uint8_t g_ab412h_rpot_val = 0;
    bool g_ab412h_lastButtonState[85];
  #endif
//</AB412_COLL_HEAD>

// <HUEY_COLL_HEAD>
  #if (defined HUEY_COLL_SWITCH_PANEL)
    bool g_hh_lastButtonState[48];
  #endif
// </HUEY_COLL_HEAD>

// <B206_COLL_HEAD>
  #if (defined B206_COLL_SWITCH_PANEL)
    bool g_b206h_lastButtonState[20];
  #endif
// </B206_COLL_HEAD>

// <COLLECTIVE_NO_THROTTLE>
  #if (defined COLLECTIVE_NOTHR)
    bool g_coll_nthr_lastButtonState[8];
  #endif
// </COLLECTIVE_NO_THROTTLE>

// <HELI QUADRANT>
  #if (defined HELI_QUADRANT)
    bool g_hq_lastButtonState[4];
    uint16_t g_hq_rx_val = 0;
    uint16_t g_hq_ry_val = 0;
  #endif
// </HELI QUADRANT>
