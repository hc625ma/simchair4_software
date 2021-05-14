// <GLOBAL>

  #define DEBUG //enable Serial.prints

  #define ADS1115_RESOLUTION 15 //bits, min 12, max 15

  //adjust your KY-023 board based hat switches values here
  #define PC10_HATSW_VAL 20  // 10% of hat axis
  #define PC25_HATSW_VAL 64  // 25% of hat axis
  #define PC75_HATSW_VAL 190 // 75% of hat axis
  #define PC90_HATSW_VAL 220 // 90% of hat axis
  
  #define DEVICE_INIT_DELAY 1000
  
// </GLOBAL>

// <MASTER USB-UART>
  unsigned long g_uart_baud = 57600;
// </MASTER USB_UART>

// <COLLECTIVE_GLOBAL>

  #define COLLECTIVE_MODE_SWITCH_ENABLED 1
  #define COLLECTIVE_EVO_MODE_SWITCH_THR_ENABLED 1

  #define MODESW_POS_LEFT_DECIMAL_VAL 2 // leftmost position of the switch
  #define MODESW_POS_MIDDLE_DECIMAL_VAL 0
  #define MODESW_POS_RIGHT_DECIMAL_VAL 1

  #define MODESW_THR_POS_UP_DECIMAL_VAL 2 // leftmost position of the switch
  #define MODESW_THR_POS_MIDDLE_DECIMAL_VAL 0
  #define MODESW_THR_POS_DOWN_DECIMAL_VAL 1

  #define COLLECTIVE_EVO_USB_FILTER_COUNTER_COLL_AXIS 6
  #define COLLECTIVE_EVO_USB_FILTER_COUNTER_THR_AXIS 6

  #define COLL_IDLE_DETENT_SUPPORT 1 // limit throttle axis with idle stop detent mark value

  #define BUTTON_PRESS_ON_THROTTLE_CUTOFF 1 // ENABLE JOYSTICK / KEYBOARD BUTTON PRESS ON THR CUTOFF - MUST BE ENABLED FOR DCS HUEY COMPAT MODE TO WORK

  #define DCS_HUEY_IDLE_STOP_COMPAT_MODE_ENABLED 1 // will press keyboard buttons when closing and opening throttle, respectively to IDLE REL button, ONLY WORKS WITH COLL MODE SW IN MIDDLE POS
  #define HUEY_COMPAT_THR_DOWN_KEY KEY_PAGE_DOWN
  #define HUEY_COMPAT_THR_UP_KEY KEY_PAGE_UP
  #define DCS_HUEY_COMPAT_MODE_BUTTON_HOLD 1000

  

  #define THROTTLE_STABILIZER_ENABLED 1
  #define THR_STEP 10
    
//</COLLECTIVE_GLOBAL>
