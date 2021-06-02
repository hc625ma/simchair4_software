//<VRMAX_AB412_COLL_HEAD_EXTENSION>//

#if (defined VRMAX_AB412_COLL_HEAD_EXTENSION)

    //#define RADIO_PANEL_SETUP // <--- UNCOMMENT TO SEE BUTTON NUMBERS IN JOY.CPL, COMMENT OUT FOR NORMAL OPERATION!
  
    #define RADIO_DEVICE_SWITCH_XPDR_POSITION_JOY_BUTTON 3 // put the lower button number here
    #define RADIO_MODE_SWITCH_LOWER_POSITION_JOY_BUTTON 10 // put the lower button number here
    #define ALTIMETER_SELECTOR_JOY_BUTTON 22 // put the lower button number here
    #define NAV_MODE_SELECTOR_JOY_BUTTON 19 // put the lower button number here
    #define XPDR_MODE_SW_JOY_BUTTON 7 // put the lower button number here
    #define XPDR_MODE_C_JOY_BUTTON 9 // that button will be pressed in middle xpdr switch position
    #define OBS_RATE_SELECTOR_JOY_BUTTON 18 // switches between high and low OBS knob turn rate
    #define DIR_GYRO_RATE_SELECTOR_JOY_BUTTON 17 // switches between high and low DIR G knob turn rate
    #define OBS_HIGH_RATE_BUTTON_PRESSES 200; // how many times the OBS / DIR GYRO knobs buttons will be pressed on knob turn

    #define RP_COM_FINE_PB_JOY_BUTTON 24 // top left knob on the radio panel
    #define RP_COM_COARSE_PB_JOY_BUTTON 9 // bottom left knob
    #define RP_NAV_FINE_PB_JOY_BUTTON 5 // top right
    #define RP_NAV_COARSE_PB_JOY_BUTTON 2 // bottom right

//    const t_struct_rp_e_matrix radio_panel_knob_matrix[] =
//    {
//    // e  r0l r0r r1l r1r r2l r2r  m0l m0r  m1l m1r  m2l m2r adfl adfr  r0lpm1 r0rpm1 r1lpm1 r1rpm1 r2lpm1 r2rpm1 r0lpm2 r0rpm2 r1lpm2 r1rpm2 r2lpm2 r2rpm2
//      {0, 25, 26, 33, 34, 41, 42,  49, 50,  57, 58,  65, 66, 87, 88,    95,    96,    103,   104,   111,   112,   119,   120,   127,   128,   135,   136, },
//      {1, 27, 28, 35, 36, 43, 44,  51, 52,  59, 60,  67, 68, 89, 90,    97,    98,    105,   106,   113,   114,   121,   122,   129,   130,   137,   138, },      
//      {2, 29, 30, 37, 38, 45, 46,  53, 54,  61, 62,  69, 70, 91, 92,    99,    100,   107,   108,   115,   116,   123,   124,   131,   132,   139,   140, },
//      {3, 31, 32, 39, 40, 47, 48,  55, 56,  63, 64,  71, 72, 93, 94,    101,   102,   109,   110,   117,   118,   125,   126,   133,   134,   141,   142, },
//    };

const t_struct_rp_e_matrix radio_panel_knob_matrix[] =
    {
    // e  r0l r0r r1l r1r r2l r2r  m0l m0r  m1l m1r  m2l m2r adfl adfr  r0lpm1 r0rpm1 r1lpm1 r1rpm1 r2lpm1 r2rpm1 r0lpm2 r0rpm2 r1lpm2 r1rpm2 r2lpm2 r2rpm2
      {0, 25, 26, 33, 34, 41, 42,  49, 50,  57, 58,  65, 66, 87, 88,    95,    96,    103,   104,   111,   112,   119,   120,   127,   128,   135,   136, },
      {1, 27, 28, 35, 36, 43, 44,  51, 52,  59, 60,  67, 68, 89, 90,    97,    98,    105,   106,   113,   114,   121,   122,   129,   130,   137,   138, },      
      {2, 29, 30, 37, 38, 45, 46,  53, 54,  61, 62,  69, 70, 91, 92,    99,    100,   107,   108,   115,   116,   123,   124,   131,   132,   139,   140, },
      {3, 31, 32, 39, 40, 47, 48,  55, 56,  63, 64,  71, 72, 93, 94,    101,   102,   109,   110,   117,   118,   125,   126,   133,   134,   141,   142, },
    };

    const t_struct_r_matrix radio_panel_pb_matrix[] =
    {
    // 0  c1f  c1c  n1f  n1c   c2f  c2c  n2f  n2c  x1   x2   x3   x4
      {0, 143, 144, 145, 146,  147, 148, 149, 150, 151, 152, 153, 154, },
    };

    const t_struct_mag_conf mag_knob_conf[] =
    {
    // e  l  r
      {3, 73,74}
    };

    const t_struct_nav_conf nav_knob_conf[] =
    {
    // e  n0l n0r n1l n1r n2l n2r
      {4, 75, 76, 77, 78, 79, 80, },
    };

    const t_struct_alt_conf alt_knob_conf[] =
    {
    // e  a0l a0r a1l a1r a2l a2r
      {5, 81, 82, 83, 84, 85, 86, },
    };

#endif
//</VRMAX_AB412_COLL_HEAD_EXTENSION>
