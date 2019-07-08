#if (defined B8_GRIP)
  Joystick_ j_b8grip(0x31, 0x05, 6, 1, true, true, false, false, false, false, false, false, false, false, false);

  void setup_b8grip() {
    j_b8grip.begin();
    j_b8grip.setXAxisRange(0, 255);
    j_b8grip.setYAxisRange(0, 255);
  }
  
  void poll_b8grip() {
    uint8_t rx;
    uint8_t ry;
    uint8_t b;
    Wire.requestFrom(B8GRIP_I2C_ADDRESS, 3);
    while (Wire.available()) {
      byte b1 = Wire.read();
      byte b2 = Wire.read();
      b = Wire.read();
      rx = b1;
      ry = b2;
    }
    if (B8_POT_MODE == "ANALOG") {
      j_b8grip.setXAxis(rx);
      j_b8grip.setYAxis(ry);
    } else {
      if (B8_HAT_SWITCH_MODE == "HAT") {
        int16_t hat_val = parse_hat_sw(rx, ry, 8);
        j_b8grip.setHatSwitch(0, hat_val);
      }
      else if (B8_HAT_SWITCH_MODE == "BOTH") {
        if (g_controls_freezed == 0) {
          int16_t hat_val = parse_hat_sw(rx, ry, 8);
          j_b8grip.setHatSwitch(0, hat_val);
        } else {
          int16_t hat_val = parse_hat_trim(rx, ry, INVERT_HAT_TRIM_X, INVERT_HAT_TRIM_Y);
        }
      } else {
        int16_t hat_val = parse_hat_trim(rx, ry, INVERT_HAT_TRIM_X, INVERT_HAT_TRIM_Y);
      }
    }
    for (byte i = 0; i < 6; i++) {
      bool v = (b >> i) & 1;
      if (v != g_b8grip_lastButtonState[i]) {
        if (((SENS_SWITCH_ENABLED == 1) && (SENS_DEVICE == "B8_GRIP")) || (PTT_KEYBOARD_PRESS == 1) || (PSEUDO_FORCE_TRIM == 1)) {
          if ((i != SENS_SWITCH_BUTTON) && (i != PTT_BUTTON) && (i != PSEUDO_FORCE_TRIM_BUTTON)) {
            j_b8grip.setButton(i, v);
          } else {
            if ((v == 1) && (i != PTT_BUTTON) && (i != PSEUDO_FORCE_TRIM_BUTTON)) {
              if (g_cyclic_sens == 100) {
                g_cyclic_sens = CUSTOM_CYCLIC_SENS;
                g_rudder_sens = CUSTOM_RUDDER_SENS;
                g_physical_cyclic_center_x = adjust_sensitivity (g_physical_cyclic_center_x, CUSTOM_CYCLIC_SENS);
                g_physical_cyclic_center_y = adjust_sensitivity (g_physical_cyclic_center_y, CUSTOM_CYCLIC_SENS);
                if (SENS_SWITCH_TRIM_RESET == 1) {
                  g_force_trim_on = 0;
                  g_force_trim_position_set = 0;
                  g_force_trim_rudder_on = 0;
                  g_force_trim_rudder_position_set = 0;
                }
              } else {
                g_cyclic_sens = 100;
                g_rudder_sens = 100;
                g_physical_cyclic_center_x = adjust_sensitivity(g_physical_cyclic_center_x, 100);
                g_physical_cyclic_center_y = adjust_sensitivity(g_physical_cyclic_center_y, 100);
                if (SENS_SWITCH_TRIM_RESET == 1) {
                  g_x_diff = 0;
                  g_y_diff = 0;
                  g_cyclic_x_adj = 0;
                  g_cyclic_y_adj = 0;
                  g_force_trim_on = 0;
                  g_force_trim_position_set = 0;
                  g_force_trim_rudder_on = 0;
                  g_force_trim_rudder_position_set = 0;
  
                  g_force_trim_x = CBASE_ADC_RANGE / 2;
                  g_force_trim_y = CBASE_ADC_RANGE / 2 ;
                  g_force_trim_rudder = PEDALS_ADC_RANGE / 2;
                  g_cyclic_force_trim_state = 0;
                  g_pedals_force_trim_state = 0;
                }
              }
            }
            else if ((i == PTT_BUTTON) && (PTT_KEYBOARD_PRESS == 1)) {
              if (v == 1) {
                Keyboard.press(PTT_KEYBOARD_KEY_MOD);
                Keyboard.press(PTT_KEYBOARD_KEY);
              } else {
                Keyboard.releaseAll();
              }
            } else if ((i == PSEUDO_FORCE_TRIM_BUTTON) && (PSEUDO_FORCE_TRIM == 1) && (v == 1)) {
              g_force_trim_on = !g_force_trim_on;
              g_controls_freezed = !g_controls_freezed;
  
              g_force_trim_rudder_on = !g_force_trim_rudder_on;
              g_force_trim_button_pressed = 1;
            } else {
              j_b8grip.setButton(i, v);
              g_force_trim_button_pressed = 0;
            }
          }
        } else {
          j_b8grip.setButton(i, v);
        }
      }
      g_b8grip_lastButtonState[i] = v;
    }
  }

  int parse_hat_trim (int x, int y, bool invert_x, bool invert_y) {
    int hat_val;
    int adj_step_x = g_one_percent_cbase_range * ATT_TRIM_STEP_X;
    int adj_step_y = g_one_percent_cbase_range * ATT_TRIM_STEP_Y;
    if (y > 145) {
      //hat up
      if (invert_y == 0) {
        g_cyclic_y_adj = g_cyclic_y_adj + adj_step_y;
        if ((FORCE_TRIM_BUTTON_MODE == "MOMENTARY") && (g_controls_freezed == 1)) {
          j_b8grip.setYAxis(g_yval + g_cyclic_y_adj);
        }
      } else {
        g_cyclic_y_adj = g_cyclic_y_adj - adj_step_y;
        if ((FORCE_TRIM_BUTTON_MODE == "MOMENTARY") && (g_controls_freezed == 1)) {
          j_b8grip.setYAxis(g_yval + g_cyclic_y_adj);
        }
      }
    } else if (y < 105) {
      //hat down
      if (invert_y == 0) {
       g_cyclic_y_adj = g_cyclic_y_adj - adj_step_y;
       if ((FORCE_TRIM_BUTTON_MODE == "MOMENTARY") && (g_controls_freezed == 1)) {
          j_b8grip.setYAxis(g_yval + g_cyclic_y_adj);
        }
      } else {
        g_cyclic_y_adj = g_cyclic_y_adj + adj_step_y;
        if ((FORCE_TRIM_BUTTON_MODE == "MOMENTARY") && (g_controls_freezed == 1)) {
          j_b8grip.setYAxis(g_yval + g_cyclic_y_adj);
        }
      }
    } else if (x > 145) {
      //hat right
      if (invert_x == 0) {
        g_cyclic_x_adj = g_cyclic_x_adj + adj_step_x;
        if ((FORCE_TRIM_BUTTON_MODE == "MOMENTARY") && (g_controls_freezed == 1)) {
          j_b8grip.setXAxis(g_xval + g_cyclic_x_adj);
        }
      } else {
        g_cyclic_x_adj = g_cyclic_x_adj - adj_step_x;
        if ((FORCE_TRIM_BUTTON_MODE == "MOMENTARY") && (g_controls_freezed == 1)) {
          j_b8grip.setXAxis(g_xval + g_cyclic_x_adj);
        }
      }
    }
    else if (x < 105) {
      //hat left
      if (invert_x == 0) {
        g_cyclic_x_adj = g_cyclic_x_adj - adj_step_x;
        if ((FORCE_TRIM_BUTTON_MODE == "MOMENTARY") && (g_controls_freezed == 1)) {
          j_b8grip.setXAxis(g_xval + g_cyclic_x_adj);
        }
      } else {
        g_cyclic_x_adj = g_cyclic_x_adj + adj_step_x;
        if ((FORCE_TRIM_BUTTON_MODE == "MOMENTARY") && (g_controls_freezed == 1)) {
          j_b8grip.setXAxis(g_xval + g_cyclic_x_adj);
        }
      }
    }
  
  }

#endif
