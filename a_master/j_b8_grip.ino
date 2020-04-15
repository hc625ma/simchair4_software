#if (defined B8_GRIP)
  Joystick_ j_b8grip(0x31, 0x05, 6, 1, false, false, false, false, false, false, false, false, false, false, false);

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
//    if (B8_POT_MODE == "ANALOG") {
//      j_b8grip.setXAxis(rx);
//      j_b8grip.setYAxis(ry);
//    } else {
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
    //}
    for (byte i = 0; i < 6; i++) {
      bool v = (b >> i) & 1;
      if (v != g_b8grip_lastButtonState[i]) {
        if ((RATES_POTS_ENABLED == 1) || (PTT_KEYBOARD_PRESS == 1) || (PSEUDO_FORCE_TRIM == 1)) {
          if ((i != HAT_SWITCH_BUTTON - 1) && (i != PTT_BUTTON - 1) && (i != PSEUDO_FORCE_TRIM_BUTTON - 1)) {
            j_b8grip.setButton(i, v);
          } else {
            if (i == HAT_SWITCH_BUTTON - 1) {
                if (HAT_SWITCH_TRIM_RESET == 1) {
                  if (v == 1) {
                    g_ftcr = 1;
                  } else {
                    g_ftcr = 0;
                  }
                } else {
                  j_b8grip.setButton(i, v);
                }
            }
            else if ((i == PTT_BUTTON - 1) && (PTT_KEYBOARD_PRESS == 1)) {
              if (v == 1) {
                Keyboard.press(PTT_KEYBOARD_KEY_MOD);
                Keyboard.press(PTT_KEYBOARD_KEY);
              } else {
                Keyboard.releaseAll();
              }
            } else if ((i == PSEUDO_FORCE_TRIM_BUTTON - 1) && (PSEUDO_FORCE_TRIM == 1) && (v == 1)) {
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
