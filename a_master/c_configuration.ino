// <GLOBAL>
  #define ADS1115_RESOLUTION 15 //bits, min 12, max 15
// </GLOBAL>


// <COLLECTIVE LEVERS>

  // <COMPACT COLLECTIVE>

    #define COMPACT_COLLECTIVE_MIN 927
    #define COMPACT_COLLECTIVE_MAX 0

    #define COMPACT_COLLECTIVE_THR_MIN 0
    #define COMPACT_COLLECTIVE_THR_MAX 1023
    
  // </COMPACT COLLECTIVE>

// </COLLECTIVE LEVERS>

// <SIMPLE PEDESTAL>

  // Ministick
  #define X_CENTER 123
  #define Y_CENTER 128
  #define MINISTICK_SENSITIVITY_LOW 5
  #define MINISTICK_SENSITIVITY_HIGH 15

  // Buttons to act as mouse buttons as seen in joy.cpl, 0 to disable
  #define MB_LEFT 9
  #define MB_RIGHT 0

  #define MOUSEWHEEL_DIR "REV" // NOR or REV - adjust so knobs in your sim will turn in the right direction
  #define MINISTICK_SENS_SWITCH_BTN 8 // 0 to disable

// </SIMPLE_PEDESTAL>

// <CYCLIC>

  // <BASE>
    

    #define FSGIMBAL_INVERT_X 0
    #define FSGIMBAL_INVERT_Y 0

    #define XY_FILTERING_ENABLED 0
    
    #define CUSTOM_CYCLIC_SENS 80

    #define PSEUDO_FORCE_TRIM 1
    #define FORCE_TRIM_BUTTON_MODE "HOLD"// "MOMENTARY" or "HOLD" - use "MOMENTARY" for non-centering cyclic
    #define SENS_SWITCH_ENABLED 1

  // </BASE>

  // <B8 GRIP>

    #define PSEUDO_FORCE_TRIM_BUTTON 1
    #define SENS_SWITCH_TRIM_RESET 0 // DEPRECATED - USE FTCR INSTEAD - release the trim when you press SENS SWITCH
    #define B8_POT_MODE "HAT_SW" // HAT_SW or ANALOG
    
    #define SENS_DEVICE "b8_stick"
    #define SENS_SWITCH_BUTTON 0 //0 is the first button

    #define B8_HAT_SWITCH_MODE "ATT_TRIM" //"ATT_TRIM","HAT", or "BOTH", it will adjust the position of the trimmed cyclic with hat(Bell 407-style), only use "BOTH" with FORCE_TRIM_BUTTON_MODE "MOMENTARY"
    #define ATT_TRIM_STEP_Y 0.02 //0.05 //IN PERCENTS OF AXIS LENGHT, 0.05 for non-centering cyclic, 0.02 for a spring-loaded one
    #define ATT_TRIM_STEP_X 0.02 //0.05 //IN PERCENTS OF AXIS LENGHT
    #define INVERT_HAT_TRIM_X 0
    #define INVERT_HAT_TRIM_Y 1

    #define PTT_KEYBOARD_PRESS 0 // set to 1 to enable keyboard combination press instead of a joystick button
    #define PTT_BUTTON 4 //1st button is 0, 4 should be PTT trigger position
    // this should press CTRL + Q
    #define PTT_KEYBOARD_KEY 'q'
    char PTT_KEYBOARD_KEY_MOD = KEY_LEFT_CTRL;

  // </B8 GRIP>


// </CYCLIC>

// <PEDALS>

  #define INVERT_RUDDER 0
  #define RUDDER_FILTERING_ENABLED 0
  #define RUDDER_READINGS 8 // only active when filtering is enabled
  #define CUSTOM_RUDDER_SENS 80 // this value will be used if SENS_SWITCH is disabled
  
// </PEDALS>
