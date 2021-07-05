// <BASE>
  
  #define FSGIMBAL_INVERT_X 1
  #define FSGIMBAL_INVERT_Y 0

  #define XY_FILTERING_ENABLED 0
  
  #define CUSTOM_CYCLIC_SENS 90

  #define PSEUDO_FORCE_TRIM 1
  #define FORCE_TRIM_BUTTON_MODE "HOLD"// "MOMENTARY" or "HOLD" - use "MOMENTARY" for non-centering cyclic
  
  #define RATES_POTS_ENABLED 1 //set to 0 to use RATES knobs as axes

  // CYCLIC/PEDALS RATES
  // You can adjust sensitivity of your stick and pedals with knobs on the rates control panel.
  // The table below shows at which pot value +- RATES_TRESHOLD the corresponding value of the 'sens' column will be set.
  
  #define RATES_KNOB_RESOLUTION 10 //bits - only change that if you want to use RATES pots as axes - otherwise you will need to adjust the table below
  #define RATES_TRESHOLD 30 // points

  #define RATES_POT_AXIS_MODE_STABILIZER_ENABLED 1
  #define RATES_POT_AXIS_MODE_STABILIZER_STEP 5

  // to calibrate rates, Serial.print the stick rate pot values and put them into the table, starting from the bottom-left position.
  #if (defined CYCLIC_BASE)
    const t_struct_cbase_rates g_struct_cbase_rates[] =
    {
    // val   sens
      {0,    80},
      {50,   85},
      {180,  90},
      {323,  95},
      {481,  100},
      {639,  105},
      {804,  110},
      {965,  115},
      {1023, 120},
    };
  #endif

// </BASE>
