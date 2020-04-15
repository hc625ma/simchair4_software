int parse_hat_sw (int x, int y, byte dirs) {
  if(dirs == 8) {
    if(x < PC10_HATSW_VAL && y > PC90_HATSW_VAL ) {
      //top-left
      return 315;
    } else if (x < PC10_HATSW_VAL && y > PC25_HATSW_VAL && y < PC75_HATSW_VAL ) {
      //left
      return 270;
    } else if (x < PC10_HATSW_VAL && y < PC10_HATSW_VAL ) {
      //bottom-left
      return 225;
    } else if (x > PC25_HATSW_VAL && x < PC75_HATSW_VAL && y > PC90_HATSW_VAL) {
      //top
      return 0;
    } else if (x > PC25_HATSW_VAL && x < PC75_HATSW_VAL && y < PC10_HATSW_VAL ) {
      //bottom
      return 180;
    } else if (x > PC90_HATSW_VAL && y > PC90_HATSW_VAL ) {
      //top-right
      return 45;
    } else if (x > PC90_HATSW_VAL && y > PC25_HATSW_VAL && y < PC75_HATSW_VAL ) {
      //right
      return 90;
    } else if (x > PC90_HATSW_VAL && y < PC10_HATSW_VAL ) {
      //bottom-right
      return 135;
    } else {//if ( x > 90 && x < 150 && y > 90 && y < 150 ) { // or use as else...
      //center 
      return JOYSTICK_HATSWITCH_RELEASE;
    }
  } else { // dir 4
    if (x > PC25_HATSW_VAL && x < PC75_HATSW_VAL && y > PC90_HATSW_VAL) {
      //top
      return 0;
    } else if (x < PC10_HATSW_VAL && y > PC25_HATSW_VAL && y < PC75_HATSW_VAL ) {
      //left
      return 270;
    } else if (x > PC25_HATSW_VAL && x < PC75_HATSW_VAL && y < PC10_HATSW_VAL ) {
      //bottom
      return 180;
    } else if (x > PC90_HATSW_VAL && y > PC25_HATSW_VAL && y < PC75_HATSW_VAL ) {
      //right
      return 90;
    } else {//if ( x > 256 && x < 768 && y > 256 && y < 768 ) { // or use as else...
      //center 
      return JOYSTICK_HATSWITCH_RELEASE;
    }
  }
}

#if ((defined CYCLIC_BASE) || (defined B8_GRIP) || (defined PEDALS))

  int adjust_sensitivity (long val, int percent) {
   // Serial.println(percent);
    int center = CBASE_ADC_RANGE / 2;
    int adj_range = (CBASE_ADC_RANGE / 100) * percent;
  
    val = ((val - center) * percent / 100) + center; // here we at first substract ADC_RANGE / 2 to move our value to the range with center at 0, then make our calculations, and adjust it back by adding ADC_RANGE/2
    val = constrain(val,0,CBASE_ADC_RANGE);  
    return val;
  }

#endif
