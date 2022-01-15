#if (defined SIMPLE_COLLECTIVE_SE_I2C)
  //CREATE JOYSTICK OBJECT AND GLOBAL COLLECTIVE STRUCT
  Joystick_ j_scoll(0x20, 0x05, 18, 2, false, false, true, true, false, true, false, true, false, false, false);
  t_struct_coll_attr g_struct_coll_attr;

  void setup_simple_collective_se_i2c() {
    //POPULATE THE GLOBAL STRUCT WITH VALUES FROM CONFIGURATION HERE
    g_struct_coll_attr.i2c_addr = SINGLE_COLLECTIVE_I2C_ADDRESS;
    g_struct_coll_attr.i2c_bytes = 6; // we will read them locally but still need that here for compatibility
    g_struct_coll_attr.base_version = 5;
    g_struct_coll_attr.throttles = 1;
    g_struct_coll_attr.lvr_max = SIMPLE_COLLECTIVE_SE_I2C_MAX;
    g_struct_coll_attr.lvr_min = SIMPLE_COLLECTIVE_SE_I2C_MIN;
    g_struct_coll_attr.thr_max[0] = SIMPLE_COLLECTIVE_SE_I2C_THR_MAX;
    g_struct_coll_attr.thr_min[0] = SIMPLE_COLLECTIVE_SE_I2C_THR_MIN;
    g_struct_coll_attr.thr_max[1] = SIMPLE_COLLECTIVE_SE_I2C_THR_MAX;
    g_struct_coll_attr.thr_min[1] = SIMPLE_COLLECTIVE_SE_I2C_THR_MIN;
    g_struct_coll_attr.thr_max[2] = SIMPLE_COLLECTIVE_SE_I2C_THR_MAX;
    g_struct_coll_attr.thr_min[2] = SIMPLE_COLLECTIVE_SE_I2C_THR_MIN;
    g_struct_coll_attr.idle_detent_axis_val[0] = SIMPLE_COLLECTIVE_SE_I2C_IDLE_DETENT_AXIS_VAL;
    g_struct_coll_attr.idle_detent_axis_val[1] = SIMPLE_COLLECTIVE_SE_I2C_IDLE_DETENT_AXIS_VAL;
    g_struct_coll_attr.idle_detent_axis_val[2] = SIMPLE_COLLECTIVE_SE_I2C_IDLE_DETENT_AXIS_VAL;
    g_struct_coll_attr.phys_thr_latch_joy_btn[0] = SIMPLE_COLLECTIVE_SE_I2C_PHYSICAL_LATCH_MOD_JOY_BUTTON_1;
    g_struct_coll_attr.phys_thr_latch_joy_btn[1] = SIMPLE_COLLECTIVE_SE_I2C_PHYSICAL_LATCH_MOD_JOY_BUTTON_2;
    g_struct_coll_attr.phys_thr_latch_joy_btn[2] = SIMPLE_COLLECTIVE_SE_I2C_PHYSICAL_LATCH_MOD_JOY_BUTTON_3;
    g_struct_coll_attr.phys_thr_latch =  0;
    g_struct_coll_attr.lastButtonState[10]; // modesw as 2 buttons, idle rel button

    if ((COLLECTIVE_EVO_MODE_SWITCH_THR_ENABLED == 1)) {
      g_struct_coll_attr.throttles = 2;
    }

    //INITIALIZE JOYSTICK
    coll_joy_init(j_scoll); 
  }
 
  void poll_simple_collective_se_i2c() {
    uint16_t lvr;    // LEVER VALUE
    uint16_t thr[3]; // THROTTLE VALUES ARE ALWAYS STORED IN AN ARRAY FOR UNIFICATION
    uint8_t ms = 0;  // MODE SWITCH VALUE
    uint8_t ms_thr = 0;  // MODE SWITCH THROTTLE VALUE
    uint8_t hb = 0;
    uint8_t mod = 0;
    uint8_t hs = 0;

    simple_coll_lever_evo_i2c_poll(lvr,thr,ms,ms_thr,hb); // POLL THE LEVER

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

    
     
    #if (defined CALIBRATE_SIMPLE_COLL_SE_I2C)
      Serial.print("Lever:");
      Serial.print(" ");
      Serial.print(lvr);
      Serial.print(" ");
      Serial.print("Throttles: r ");
      Serial.print(g_coll_evo_usb_raw_thr);
      Serial.print(" t1 ");
      Serial.print(thr[0]);
      Serial.print(" t2 ");
      Serial.print(thr[1]);
      Serial.print(" t3 ");
      Serial.print(thr[2]);
      Serial.print(" ms ");
      Serial.print(ms);
      Serial.print(" ms_thr ");
      Serial.print(ms_thr);
      Serial.print(" hat byte ");
      printBits(hb);
      Serial.print(" hat dec ");
      Serial.print(hb);
      Serial.println(" ");
    #endif
  
    coll_lever_evo_process_poll_results(lvr,thr,ms,ms_thr,j_scoll);  //PROCESS VALUES WE GOT FROM THE LEVER (SEE SHARED FUNCTIONS TAB)
    
  }  

void parse_button_array_sc_se(uint8_t hb, uint8_t modifier) {    
    //hat switch mode 2 parsed to buttons below
    for (byte i = 0; i < 8; i++) {
      bool v = (hb >> i) & 1;
      if (v != g_scoll_se_lastButtonState[i + 1 + 9]) {
        j_scoll.setButton(i + 1 + 9, v);
        g_scoll_se_lastButtonState[i + 1 + 9] = v;
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
      j_scoll.setButton(7 + hs,1);
      if (g_coll_modesw_pos_decimal == MODESW_POS_MIDDLE_DECIMAL_VAL) {
        g_idle_rel_btn_pressed = 1;
        //g_tl_idle_rel_btn_pressed[0] = 1;
        //g_tl_idle_rel_btn_pressed[1] = 1;
        if (COLLECTIVE_EVO_MODE_SWITCH_THR_ENABLED == 1) {
          if (g_coll_modesw_thr_pos_decimal == MODESW_THR_POS_UP_DECIMAL_VAL) {
            g_idle_rel_btn_pressed_new[0] = 1;           
          } else if (g_coll_modesw_thr_pos_decimal == MODESW_THR_POS_MIDDLE_DECIMAL_VAL) {
            g_idle_rel_btn_pressed_new[2] = 1;
          } else if (g_coll_modesw_thr_pos_decimal == MODESW_THR_POS_DOWN_DECIMAL_VAL) {
            g_idle_rel_btn_pressed_new[1] = 1;
          }
        } else {
          g_idle_rel_btn_pressed_new[0] = 1;
        }
      }
    } else {
      j_scoll.setButton(7 + hs,0);
      g_idle_rel_btn_pressed = 0;
//      g_tl_idle_rel_btn_pressed[0] = 0;
//      g_tl_idle_rel_btn_pressed[1] = 0;
      if (COLLECTIVE_EVO_MODE_SWITCH_THR_ENABLED == 1) {
          if (g_coll_modesw_thr_pos_decimal == MODESW_THR_POS_UP_DECIMAL_VAL) {
            g_idle_rel_btn_pressed_new[0] = 0;
          } else if (g_coll_modesw_thr_pos_decimal == MODESW_THR_POS_MIDDLE_DECIMAL_VAL) {
            g_idle_rel_btn_pressed_new[2] = 0;
          } else if (g_coll_modesw_thr_pos_decimal == MODESW_THR_POS_DOWN_DECIMAL_VAL) {
            g_idle_rel_btn_pressed_new[1] = 0;
          }
        } else {
          g_idle_rel_btn_pressed_new[0] = 0;
        }
//      g_idle_rel_btn_pressed_new[0]=0;
//      g_idle_rel_btn_pressed_new[1]=0;
//      g_idle_rel_btn_pressed_new[2]=0;
//      g_tl_idle_rel_btn_pressed[0] = 0;
//      g_tl_idle_rel_btn_pressed[1] = 0;
      return JOYSTICK_HATSWITCH_RELEASE;   
    }  
  }

  void simple_coll_lever_evo_i2c_poll (uint16_t &lvr,uint16_t *thr, uint8_t &ms, uint8_t &ms_thr, uint8_t &hb) {
    //ms = 0;
    //ms_thr = 0;
    uint8_t i2c_bytes[6];//[g_struct_coll_attr.i2c_bytes];
    uint16_t raw_thr0,raw_thr1,raw_thr2;
    
    //coll_evo_usb_fill_i2c_bytes_arr(i2c_bytes); 
    generic_poll_i2c_device(g_struct_coll_attr.i2c_addr,i2c_bytes,g_struct_coll_attr.i2c_bytes);
    lvr = generic_read_16bit_axis_from_bytes(i2c_bytes[0],i2c_bytes[1]);
    ms = coll_evo_read_modeswitch(i2c_bytes[4]);
    ms_thr = coll_evo_read_modeswitch_thr(i2c_bytes[4]);
    hb = i2c_bytes[5];
    if ((COLLECTIVE_EVO_MODE_SWITCH_THR_ENABLED == 1)) {
      //g_struct_coll_attr.throttles = 2;
      if (g_coll_modesw_thr_pos_decimal == MODESW_THR_POS_UP_DECIMAL_VAL) {
        g_coll_evo_thr_direct_control[2] = 0;
        if ((g_coll_evo_thr_direct_control[0] == 0)) {// && (g_coll_evo_usb_init_counter > 0)) {
          raw_thr0 = generic_read_16bit_axis_from_bytes(i2c_bytes[2],i2c_bytes[3]);
          g_coll_evo_diff[0] = raw_thr0 - g_coll_evo_thr_buf[0];
          g_coll_evo_diff[0] = abs(g_coll_evo_diff[0]);
          if (g_coll_evo_diff[0] < (THR_STEP)) {
            thr[0] = raw_thr0;
            g_coll_evo_thr_buf[0] = thr[0];
            g_coll_evo_thr_direct_control[0] = 1;
          } else {
            thr[0] = g_coll_evo_thr_buf[0];
          }
        } else {
          thr[0] = generic_read_16bit_axis_from_bytes(i2c_bytes[2],i2c_bytes[3]);
          g_coll_evo_thr_buf[0] = thr[0];
        }
        generic_check_16_bit_axis_val(thr[0]);
        
      } else if (g_coll_modesw_thr_pos_decimal == MODESW_THR_POS_MIDDLE_DECIMAL_VAL) {
        g_coll_evo_thr_direct_control[0] = 0;
        g_coll_evo_thr_direct_control[1] = 0;

        if ((g_coll_evo_thr_direct_control[2] == 0)) {// && (g_coll_evo_usb_init_counter > 0)) {
          raw_thr2 = generic_read_16bit_axis_from_bytes(i2c_bytes[2],i2c_bytes[3]);
          g_coll_evo_diff[2] = raw_thr2 - g_coll_evo_thr_buf[2];
          g_coll_evo_diff[2] = abs(g_coll_evo_diff[2]);
          if (g_coll_evo_diff[2] < (THR_STEP)) {
            thr[2] = raw_thr2;
            g_coll_evo_thr_buf[2] = thr[2];
            g_coll_evo_thr_direct_control[2] = 1;
          } else {
            thr[2] = g_coll_evo_thr_buf[2];
          }
        } else {
          thr[2] = generic_read_16bit_axis_from_bytes(i2c_bytes[2],i2c_bytes[3]);
          g_coll_evo_thr_buf[2] = thr[2];
        }
        generic_check_16_bit_axis_val(thr[2]);
        
        //Serial.println("DC OFF");
      } else if (g_coll_modesw_thr_pos_decimal == MODESW_THR_POS_DOWN_DECIMAL_VAL) { 
        g_coll_evo_thr_direct_control[2] = 0;
        if ((g_coll_evo_thr_direct_control[1] == 0)) { // && (g_coll_evo_usb_init_counter > 0)) {   
          raw_thr1 = generic_read_16bit_axis_from_bytes(i2c_bytes[2],i2c_bytes[3]);
          g_coll_evo_diff[1] = raw_thr1 - g_coll_evo_thr_buf[1];
          g_coll_evo_diff[1] = abs(g_coll_evo_diff[1]);
          if (g_coll_evo_diff[1] < (THR_STEP)) {
            //Serial.println("DC UP");
            thr[1] = raw_thr1;
            g_coll_evo_thr_buf[1] = thr[1];
            g_coll_evo_thr_direct_control[1] = 1;
          } else {
            thr[1] = g_coll_evo_thr_buf[1];
          }
        } else {
          thr[1] = generic_read_16bit_axis_from_bytes(i2c_bytes[2],i2c_bytes[3]);
          g_coll_evo_thr_buf[1] = thr[1];
        }
        generic_check_16_bit_axis_val(thr[1]);
      }
    } else  {
      if (g_struct_coll_attr.throttles >= 2) {
        thr[0] = generic_read_16bit_axis_from_bytes(i2c_bytes[2],i2c_bytes[3]);
        thr[1] = generic_read_16bit_axis_from_bytes(i2c_bytes[4],i2c_bytes[5]);
        generic_check_16_bit_axis_val(thr[0]);
        generic_check_16_bit_axis_val(thr[1]);
      } else {
        thr[0] = generic_read_16bit_axis_from_bytes(i2c_bytes[2],i2c_bytes[3]);
        generic_check_16_bit_axis_val(thr[0]);
      }
    }       
  }

#endif
