// THIS FILE CONTAINS DEFAULT DEVICES CONFIGURATION, TO ADD CUSTOM DEVICES PLACE YOUR OWN FILES AFTER THIS FILE
// FILES MUST BE CALLED c[a-z]_conf_<filename>.ino

// <GLOBAL>
  #define ADS1115_RESOLUTION 15 //bits, min 12, max 15
  
  #define DEVICE_INIT_DELAY 1000
// </GLOBAL>


// <COLLECTIVE LEVERS>

  // <COLLECTIVE_GLOBAL>

    #define COLLECTIVE_MODE_SWITCH_ENABLED 1
  
    #define MODESW_POS_LEFT_DECIMAL_VAL 2 // leftmost position of the switch
    #define MODESW_POS_MIDDLE_DECIMAL_VAL 0
    #define MODESW_POS_RIGHT_DECIMAL_VAL 1

    #define COLL_IDLE_DETENT_SUPPORT 1 // limit throttle axis with idle stop detent mark value

    #define BUTTON_PRESS_ON_THROTTLE_CUTOFF 1 // ENABLE JOYSTICK / KEYBOARD BUTTON PRESS ON THR CUTOFF - MUST BE ENABLED FOR DCS HUEY COMPAT MODE TO WORK

    #define DCS_HUEY_IDLE_STOP_COMPAT_MODE_ENABLED 1 // will press keyboard buttons when closing and opening throttle, respectively to IDLE REL button, ONLY WORKS WITH COLL MODE SW IN MIDDLE POS
    #define HUEY_COMPAT_THR_DOWN_KEY KEY_PAGE_DOWN
    #define HUEY_COMPAT_THR_UP_KEY KEY_PAGE_UP
    #define DCS_HUEY_COMPAT_MODE_BUTTON_HOLD 200

    

    #define THROTTLE_STABILIZER_ENABLED 1
    #define THR_STEP 10
    
  //</COLLECTIVE_GLOBAL>

  // <COMPACT COLLECTIVE>

    #define COMPACT_COLLECTIVE_MIN 1023
    #define COMPACT_COLLECTIVE_MAX 0

    #define COMPACT_COLLECTIVE_THR_MIN 0
    #define COMPACT_COLLECTIVE_THR_MAX 1023

    #define COMPACT_COLLECTIVE_IDLE_DETENT_AXIS_VAL 167 // throttle axis value at the detent mark
    #define COMPACT_COLLECTIVE_IDLE_REL_BTN 5 // IDLE RELEASE joystick button number as seen in joy.cpl, 0 to disable. This button allows moving virtual throttle grip past the detent to close the throttle.
    #define COMPACT_COLLECTIVE_PHYSICAL_LATCH_MOD_JOY_BUTTON 3 // this button will be pressed on throttle cutoff, if IDLE REL button was pressed

    #define COMPACT_COLLECTIVE_HAT_DIRECTIONS 8
    
  // </COMPACT COLLECTIVE>

  // <SINGLE_COLLECTIVE>
    #define PHYSICAL_THROTTLE_LATCH 1 // if your collective has a physical idle stop latch, set this to 1, otherwise set it to 0 for advanced throttle related functions to work

    #define SINGLE_COLLECTIVE_MIN 1022
    #define SINGLE_COLLECTIVE_MAX 19

    #define SINGLE_COLLECTIVE_THR_MIN 30
    #define SINGLE_COLLECTIVE_THR_MAX 1023

    #define SINGLE_COLLECTIVE_IDLE_DETENT_AXIS_VAL 342 // throttle axis value at the detent mark
    
    #define SINGLE_COLLECTIVE_PHYSICAL_LATCH_MOD_JOY_BUTTON 3 // this button will be pressed on throttle cutoff, if IDLE REL button was pressed
    
  // </SINGLE_COLLECTIVE>

    // <SINGLE_COLLECTIVE_MKIII>
    #define SCOLL_MKIII_PHYSICAL_THROTTLE_LATCH 0 // if your collective has a physical idle stop latch, set this to 1, otherwise set it to 0 for advanced throttle related functions to work

    #define SINGLE_COLLECTIVE_MKIII_MIN 930
    #define SINGLE_COLLECTIVE_MKIII_MAX 0

    #define SINGLE_COLLECTIVE_MKIII_THR_MIN 0
    #define SINGLE_COLLECTIVE_MKIII_THR_MAX 1023

    #define SINGLE_COLLECTIVE_MKIII_IDLE_DETENT_AXIS_VAL 134 // throttle axis value at the detent mark
    
    #define SINGLE_COLLECTIVE_MKIII_PHYSICAL_LATCH_MOD_JOY_BUTTON 1 // this button will be pressed on throttle cutoff, if IDLE REL button was pressed
    
  // </SINGLE_COLLECTIVE_MKIII>


  // <TWIN_COLLECTIVE_MKIII>
    #define TCOLL_MKIII_PHYSICAL_THROTTLE_LATCH 0 // if your collective has a physical idle stop latch, set this to 1, otherwise set it to 0 for advanced throttle related functions to work

    #define TWIN_COLLECTIVE_MKIII_MIN 1023
    #define TWIN_COLLECTIVE_MKIII_MAX 0

    uint16_t TWIN_COLLECTIVE_MKIII_THR_MIN[] = {1023,1023};
    uint16_t TWIN_COLLECTIVE_MKIII_THR_MAX[] = {0,0};


    uint16_t TWIN_COLLECTIVE_MKIII_THR_IDLE_DETENT_AXIS_VAL[] = {976,959}; // throttle axis value at the detent mark

    byte TWIN_COLLECTIVE_MKIII_PHYSICAL_LATCH_MOD_JOY_BUTTON[] = {1,2};
    
    //#define SINGLE_COLLECTIVE_MKIII_PHYSICAL_LATCH_MOD_JOY_BUTTON 1 // this button will be pressed on throttle cutoff, if IDLE REL button was pressed
    
  // </TWIN_COLLECTIVE_MKIII>
  

  // <COLLECTIVE BASE - NO THROTTLE >
  
    #define COLLECTIVE_NOTHR_MIN 1023
    #define COLLECTIVE_NOTHR_MAX 0

  // </COLLECTIVE BASE - NO THROTTLE >

  // <COLLECTIVE_HEADS>

    // <AB412_COLL_HEAD>
      #define AB412_HEAD_LEFT_POT_MIN 0
      #define AB412_HEAD_LEFT_POT_MAX 255
      #define AB412_HEAD_RIGHT_POT_MIN 0
      #define AB412_HEAD_RIGHT_POT_MAX 255

      #define AB412_HEAD_POT_STABILIZER_ENABLED 1
      #define AB412_HEAD_POT_STABILIZER_STEP 3

      #define AB412_COLL_HEAD_LEFT_HAT_DIRECTIONS 8
      #define AB412_COLL_HEAD_RIGHT_HAT_DIRECTIONS 4

      #define AB412_HEAD_IDLE_REL_BTN 17
    // </AB412_COLL_HEAD>

    // <HUEY_COLL_HEAD>

      #define HUEY_COLL_HEAD_HAT_DIRECTIONS 4
      #define HUEY_HEAD_IDLE_REL_BTN 12
      #define USE_HUEY_HEAD_MODESWITCH 1
      #define HUEY_HEAD_MODESWITCH_BTN 4 // use lower joy button number of the switch here

    // </HUEY_COLL_HEAD>
  
  //</COLLECTIVE_HEADS>

// </COLLECTIVE LEVERS>

// <SIMPLE PEDESTAL>

  // Ministick
  #define SIMPLE_PEDESTAL_X_CENTER 128
  #define SIMPLE_PEDESTAL_Y_CENTER 128

  // next 3 values control mouse pointer speed, adjust depending on how many devices are connected
  #define SIMPLE_PEDESTAL_MINISTICK_SENSITIVITY_LOW 2 // pixels per tick at LOW speed
  #define SIMPLE_PEDESTAL_MINISTICK_SENSITIVITY_HIGH 6 // pixels per tick at HIGH speed
  #define SIMPLE_PEDESTAL_POINTER_SPEED_MODIFIER 15 // how often ticks happen

  // Buttons to act as mouse buttons as seen in joy.cpl, 0 to disable
  #define SIMPLE_PEDESTAL_MB_LEFT 4
  #define SIMPLE_PEDESTAL_MB_RIGHT 0

  #define SIMPLE_PEDESTAL_MOUSEWHEEL_DIR "REV" // NOR or REV - adjust so knobs in your sim will turn in the right direction
  #define SIMPLE_PEDESTAL_MINISTICK_SENS_SWITCH_BTN 3 // 0 to disable

  #define SIMPLE_PEDESTAL_ZOOM_STABILIZER_ENABLED 1
  #define SIMPLE_PEDESTAL_ZOOM_STEP 10

// </SIMPLE_PEDESTAL>

// <VRMAXII PEDESTAL>

  // Ministick
  #define VRMAXII_PEDESTAL_X_CENTER 132
  #define VRMAXII_PEDESTAL_Y_CENTER 124

  // next 3 values control mouse pointer speed
  #define VRMAXII_PEDESTAL_MINISTICK_SENSITIVITY_LOW 3 // pixels per tick at LOW speed
  #define VRMAXII_PEDESTAL_MINISTICK_SENSITIVITY_HIGH 8 // pixels per tick at HIGH speed
  #define VRMAX_PEDESTAL_POINTER_SPEED_MODIFIER 15 // how often ticks happen

  // Buttons to act as mouse buttons as seen in joy.cp
  // look values below in serial output after uncommenting Serial.println(rp_s); under
  // ga_varmaxii_pedestal tab, only touch this if the device is wired differently

  #define SELECTOR_POS_0_DECIMAL_VAL 9 //leftmost position of the switch
  #define SELECTOR_POS_1_DECIMAL_VAL 10
  #define SELECTOR_POS_2_DECIMAL_VAL 12
  #define SELECTOR_POS_3_DECIMAL_VAL 8
  #define SELECTOR_POS_4_DECIMAL_VAL 24
  #define SELECTOR_POS_5_DECIMAL_VAL 40 
  #define VRMAXII_PEDESTAL_MB_LEFT 4
  #define VRMAXII_PEDESTAL_MB_RIGHT 0

  #define VRMAXII_PEDESTAL_MOUSEWHEEL_DIR "REV" // NOR or REV - adjust so knobs in your sim will turn in the right direction
  #define VRMAXII_PEDESTAL_MINISTICK_SENS_SWITCH_BTN 3 // 0 to disable

  #define SELECTOR_POS_6_DECIMAL_VAL 72
  #define SELECTOR_POS_7_DECIMAL_VAL 136

  #define VRMAXII_PEDESTAL_ZOOM_STABILIZER_ENABLED 1
  #define VRMAXII_PEDESTAL_ZOOM_STEP 10

// </VRMAXII_PEDESTAL>

// <CYCLIC>

  // <BASE>
  
    #define FSGIMBAL_INVERT_X 0
    #define FSGIMBAL_INVERT_Y 0

    #define XY_FILTERING_ENABLED 1
    
    #define CUSTOM_CYCLIC_SENS 80

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

  // <B8 GRIP>

    #define PSEUDO_FORCE_TRIM_BUTTON 2
    #define HAT_SWITCH_TRIM_RESET 1
    #define HAT_SWITCH_BUTTON 1 // as seen in joy.cpl
    
    #define B8_POT_MODE "HAT_SW" // HAT_SW or ANALOG
    
//    #define SENS_DEVICE "b8_stick"
    

    #define B8_HAT_SWITCH_MODE "ATT_TRIM" //"ATT_TRIM","HAT", or "BOTH", it will adjust the position of the trimmed cyclic with hat(Bell 407-style), only use "BOTH" with FORCE_TRIM_BUTTON_MODE "MOMENTARY"
    #define ATT_TRIM_STEP_Y 0.02 //0.05 //IN PERCENTS OF AXIS LENGHT, 0.05 for non-centering cyclic, 0.02 for a spring-loaded one
    #define ATT_TRIM_STEP_X 0.02 //0.05 //IN PERCENTS OF AXIS LENGHT
    #define INVERT_HAT_TRIM_X 0
    #define INVERT_HAT_TRIM_Y 1

    #define PTT_KEYBOARD_PRESS 0 // set to 1 to enable keyboard combination press instead of a joystick button
    #define PTT_BUTTON 5 // as seen in joy.cpl
    // this should press CTRL + Q
    #define PTT_KEYBOARD_KEY 'q'
    char PTT_KEYBOARD_KEY_MOD = KEY_LEFT_CTRL;

  // </B8 GRIP>


// </CYCLIC>

// <PEDALS>

  #define INVERT_RUDDER 0
  #define RUDDER_FILTERING_ENABLED 1
  #define RUDDER_READINGS 8 // only active when filtering is enabled
  #define CUSTOM_RUDDER_SENS 80 // this value will be used if SENS_SWITCH is disabled

  
// </PEDALS>

// <HELI QUADRANT>

  #define HQ_AXIS_STABILIZER_ENABLED 1
  #define HQ_AXIS_STABILIZER_STEP 10

  #define HQ_AXIS1_DT_MIN 230
  #define HQ_AXIS1_DT_MAX 790
  #define HQ_AXIS2_DT_MIN 230
  #define HQ_AXIS2_DT_MAX 786

  #define HQ_AXIS1_MIN 304
  #define HQ_AXIS1_MAX 704
  #define HQ_AXIS2_MIN 300
  #define HQ_AXIS2_MAX 700

// </HELI QUADRANT>

