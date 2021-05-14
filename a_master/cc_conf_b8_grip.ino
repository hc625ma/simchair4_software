// <B8 GRIP>

  #define PSEUDO_FORCE_TRIM_BUTTON 2
  #define HAT_SWITCH_TRIM_RESET 1
  #define HAT_SWITCH_BUTTON 1 // as seen in joy.cpl
  
  //#define B8_POT_MODE "HAT_SW" // HAT_SW or ANALOG, WILL BE REMOVED, NO USE IN KY023 IN ANALOG MODE
  
//    #define SENS_DEVICE "b8_stick"
  

  #define B8_HAT_SWITCH_MODE "FORCE_TRIM" //"ATT_TRIM","HAT", or "BOTH", it will adjust the position of the trimmed cyclic with hat(Bell 407-style), only use "BOTH" with FORCE_TRIM_BUTTON_MODE "MOMENTARY"
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
