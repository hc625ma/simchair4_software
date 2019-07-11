int parse_hat_sw (int x, int y, byte dirs) {
  int hat_val;
  if (dirs == 8) {
    if (y > 145) {
      if (x > 145) {
        hat_val = 45;
      } else if (x < 105) {
        hat_val = 315;
      } else {
        hat_val = 0;
      }
    } else if (y < 105) {
      if (x > 160) {
        hat_val = 135;
      } else if (x < 105) {
        hat_val = 225;
      } else {
        hat_val = 180;
      }
    } else if (x > 145) {
      hat_val = 90;
    } else if (x < 105) {
      hat_val = 270;
    } else {
      hat_val = JOYSTICK_HATSWITCH_RELEASE;
    }
  } else {
    if (y > 145) {
      hat_val = 0;
    } else if (y < 105) {
       hat_val = 180;
    } else if (x > 145) {
      hat_val = 90;
    } else if (x < 105) {
      hat_val = 270;
    } else {
      hat_val = JOYSTICK_HATSWITCH_RELEASE;
    }
  }
  return hat_val;
}

#if ((defined CYCLIC_BASE) || (defined B8_GRIP) || (defined PEDALS))

  int adjust_sensitivity (int val, int param) {
  //  if ((SENS_ADJ_METHOD == "LINEAR") && (param < 100))
    if (param < 100) {
      int percent = param;
      // this will simply limit your controls throw range by the given percent,
      // and map full ADC range for it.
      // may be a good option for flying helicopters in x-plane
      int center = CBASE_ADC_RANGE / 2;
      int adj_range = (CBASE_ADC_RANGE / 100) * percent;
      val = map(val, 0, CBASE_ADC_RANGE, center - (adj_range / 2), center + (adj_range / 2));
    }
  
    return val;
  }

#endif
