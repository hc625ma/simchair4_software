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
