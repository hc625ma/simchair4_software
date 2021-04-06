#if (defined SIMPLE_COLLECTIVE_SE_USB)
  //CREATE JOYSTICK OBJECT AND GLOBAL COLLECTIVE STRUCT
  Joystick_ j_scoll(0x20, 0x05, 17, 2, false, false, true, false, false, true, false, true, false, false, false);
  t_struct_coll_attr g_struct_coll_attr;

  //UNCOMMENT THE NEXT LINE TO SET UP CONFIGURATION VALUES
  //#define CALIBRATE_SIMPLE_COLL_SE_USB

  // ADJUST THESE VALUES FOR CALIBRATION
    #define COLL_EVO_USB_ENABLE_MAP_FUNCTION 1 // SET TO 0 TO SEE RAW VALUES
    #define COLL_EVO_USB_PHYS_Z_MIN 0
    #define COLL_EVO_USB_PHYS_Z_MAX 838
    #define COLL_EVO_USB_PHYS_THR0_MIN 800
    #define COLL_EVO_USB_PHYS_THR0_MAX 224
    #define COLL_EVO_USB_PHYS_THR0_MAX_MINUS_PHYS_THR0_MIN 576 //  PHYSICAL THR MAX MINUS THR MIN

  void setup_simple_collective_se_usb() {

    //POPULATE THE GLOBAL STRUCT WITH VALUES FROM CONFIGURATION HERE
    g_struct_coll_attr.i2c_addr = SINGLE_COLLECTIVE_I2C_ADDRESS;
    g_struct_coll_attr.i2c_bytes = 7; // we will read them locally but still need that here for compatibility
    g_struct_coll_attr.base_version = 5;
    g_struct_coll_attr.throttles = 1;
    g_struct_coll_attr.lvr_max = SINGLE_COLLECTIVE_EVO_USB_MAX;
    g_struct_coll_attr.lvr_min = SINGLE_COLLECTIVE_EVO_USB_MIN;
    g_struct_coll_attr.thr_max[0] = SINGLE_COLLECTIVE_EVO_USB_THR_MAX;
    g_struct_coll_attr.thr_min[0] = SINGLE_COLLECTIVE_EVO_USB_THR_MIN;
    g_struct_coll_attr.thr_max[1] = SINGLE_COLLECTIVE_EVO_USB_THR_MAX;
    g_struct_coll_attr.thr_min[1] = SINGLE_COLLECTIVE_EVO_USB_THR_MIN;
    g_struct_coll_attr.idle_detent_axis_val[0] = SINGLE_COLLECTIVE_EVO_USB_IDLE_DETENT_AXIS_VAL;
    g_struct_coll_attr.idle_detent_axis_val[1] = SINGLE_COLLECTIVE_EVO_USB_IDLE_DETENT_AXIS_VAL;
    g_struct_coll_attr.phys_thr_latch_joy_btn[0] = SINGLE_COLLECTIVE_EVO_USB_PHYSICAL_LATCH_MOD_JOY_BUTTON_1;
    g_struct_coll_attr.phys_thr_latch_joy_btn[1] = SINGLE_COLLECTIVE_EVO_USB_PHYSICAL_LATCH_MOD_JOY_BUTTON_2;
    g_struct_coll_attr.phys_thr_latch =  0;
    g_struct_coll_attr.lastButtonState[10]; // modesw as 2 buttons, idle rel button

    if ((COLLECTIVE_EVO_MODE_SWITCH_THR_ENABLED == 1)) {
      g_struct_coll_attr.throttles = 2;
    }

    //INITIALIZE JOYSTICK
    coll_joy_init(j_scoll);
    coll_base_usb_pins_init();

    // SIMPLE COLLECTIVE SE hat switch
    for (int i = 8; i <= 10; i++)
    {
      pinMode(i, INPUT_PULLUP);
    }
    for (int i = 14; i <= 16; i++)
    {
      pinMode(i, INPUT_PULLUP);
    }
    
  }
 
  void poll_simple_collective_se_usb() {
    uint16_t lvr;    // LEVER VALUE
    uint16_t thr[1]; // THROTTLE VALUES ARE ALWAYS STORED IN AN ARRAY FOR UNIFICATION, SINGLE THR IS AN ARRAY OF 1 ELEMENT
    uint8_t ms = 0;  // MODE SWITCH VALUE
    uint8_t ms_thr = 0;  // MODE SWITCH THROTTLE VALUE
    uint8_t hb = 0;
    uint8_t mod = 0;
    uint8_t hs = 0;

    coll_lever_evo_usb_poll(lvr,thr,ms,ms_thr); // POLL THE LEVER
    hb = read_hat_scoll_se_usb(8);

    if (COLLECTIVE_MODE_SWITCH_ENABLED == 1) {
      if (g_coll_modesw_pos_decimal == MODESW_POS_MIDDLE_DECIMAL_VAL) {
        mod = 0;
        hs = 0;
      } else if (g_coll_modesw_pos_decimal == MODESW_POS_LEFT_DECIMAL_VAL) {
        mod = 6;   
        hs = 1;
      } else if (g_coll_modesw_pos_decimal == MODESW_POS_RIGHT_DECIMAL_VAL) {
        mod = 12;  
        hs = 2;
      }
    }

    uint16_t hat_val = parse_scoll_se_hat_val(hb,hs);

    if (hs <2) {
      j_scoll.setHatSwitch(hs, hat_val);
    } else if (hs == 2) {
      hb = hat_to_btns(hat_val);
      parse_button_array_sc_se(hb,mod);
    }

    
     
    #if (defined CALIBRATE_SIMPLE_COLL_SE_USB)
      Serial.print("Lever:");
      Serial.print(" ");
      Serial.print(lvr);
      Serial.print(" ");
      Serial.print("Throttle:");
      Serial.print(" ");
      Serial.print(thr[0]);
      Serial.print(" ");
      Serial.print(ms);
      Serial.print(" ");
      Serial.print(ms_thr);
      Serial.print(" hat byte ");
      printBits(hb);
      Serial.print(" hat dec ");
      Serial.print(hb);
      Serial.println(" ");
    #endif
  
    coll_lever_evo_process_poll_results(lvr,thr,ms,ms_thr,j_scoll);  //PROCESS VALUES WE GOT FROM THE LEVER (SEE SHARED FUNCTIONS TAB)
    
  }  

  uint8_t read_hat_scoll_se_usb (uint8_t start_pin) {
  uint8_t b = 0b00000000;
  for (int i = 8; i <= (10); i++) { // hat takes 4 pins 
    bool pin = !digitalRead(i);
    if (pin == 1) {
      b |= (1 << ((i - start_pin) % 8)); // forces ith bit of b to be 1.  all other bits left alone.
    }
    else {
      b &= ~(1 << ((i - start_pin) % 8)); // forces ith bit of b to be 0.  all other bits left alone.
    }
  }
  bool pin16 = !digitalRead(16);
  if (pin16 == 1) {
      b |= (1 << 3); // forces ith bit of b to be 1.  all other bits left alone.
    }
    else {
      b &= ~(1 << 3); // forces ith bit of b to be 0.  all other bits left alone.
    }
    bool pin14 = !digitalRead(14);
  if (pin14 == 1) {
      b |= (1 << 4); // forces ith bit of b to be 1.  all other bits left alone.
    }
    else {
      b &= ~(1 << 4); // forces ith bit of b to be 0.  all other bits left alone.
    }
  return b;
}

void parse_button_array_sc_se(uint8_t hb, uint8_t modifier) {    
    //hat switch mode 2 parsed to buttons below
    for (byte i = 0; i < 8; i++) {
      bool v = (hb >> i) & 1;
      if (v != g_scoll_se_lastButtonState[i + 1 + 8]) {
        j_scoll.setButton(i + 1 + 8, v);
        g_scoll_se_lastButtonState[i + 1 + 8] = v;
      }
    }
  }

  uint16_t parse_scoll_se_hat_val (uint8_t h, uint8_t hs) {
    if (h == SCOLL_SE_HAT_LEFT){
      return 270;
    } else if (h == SCOLL_SE_HAT_LEFT_UP){
      return 315;
    } else if (h == SCOLL_SE_HAT_LEFT_DOWN){
      return 225;
    } else if (h == SCOLL_SE_HAT_RIGHT) {
      return 90;
    } else if (h == SCOLL_SE_HAT_RIGHT_UP) {
      return 45;
    } else if (h == SCOLL_SE_HAT_RIGHT_DOWN) {
      return 135;
    } else if (h == SCOLL_SE_HAT_UP) {
      return 0;
    } else if (h == SCOLL_SE_HAT_DOWN) {
      return 180;
    } else if (h == SCOLL_SE_HAT_CLICK) {
      j_scoll.setButton(6 + hs,1);
      if (g_coll_modesw_pos_decimal == MODESW_POS_MIDDLE_DECIMAL_VAL) {
        g_idle_rel_btn_pressed = 1;
        g_idle_rel_btn_pressed_new[0]=1;
        g_idle_rel_btn_pressed_new[1]=1;
        g_tl_idle_rel_btn_pressed[0] = 1;
        g_tl_idle_rel_btn_pressed[1] = 1;
      }
    } else {
      j_scoll.setButton(6 + hs,0);
      g_idle_rel_btn_pressed = 0;
      g_idle_rel_btn_pressed_new[0]=0;
      g_idle_rel_btn_pressed_new[1]=0;
      g_tl_idle_rel_btn_pressed[0] = 01;
      g_tl_idle_rel_btn_pressed[1] = 0;
      return JOYSTICK_HATSWITCH_RELEASE;   
    }
  }

#endif
