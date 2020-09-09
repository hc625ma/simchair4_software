# if defined (VRMAXII_PEDESTAL)

  typedef struct __attribute__((__packed__)) {
    uint8_t enc0;
    uint8_t enc1; 
    uint8_t enc2;
    uint8_t enc3;
  } t_struct_vrm2_enc_lastVal;
  
#endif

# if defined (CYCLIC_BASE)

  typedef struct __attribute__((__packed__)) {
    uint16_t pot_val;
    uint8_t sens_val; 
  } t_struct_cbase_rates;
  
#endif

typedef struct __attribute__((__packed__)) {
    uint8_t btn_idle_stop;
    uint8_t btn_modesw;
    uint8_t *switches_w_middle_btn; 
  } t_struct_coll_head_attr;

typedef struct __attribute__((__packed__)) {
  uint8_t i2c_addr;
  uint8_t i2c_bytes;
  uint8_t throttles;
  uint16_t lvr_max;
  uint16_t lvr_min;
  uint16_t thr_max[2] = {1023,1023};
  uint16_t thr_min[2] = {0,0};
  uint16_t idle_detent_axis_val[2];
  bool btn_press_on_thr_cutoff;
  bool dcs_huey_compat_mode;
  uint8_t phys_thr_latch_joy_btn[2];
  uint8_t base_version;
  uint8_t *lastButtonState;
  uint16_t thr_buf[2];
  bool phys_thr_latch_btn_state[2] = {0,0};
  bool thr_latch_pressed[2] = {1,1};
} t_struct_coll_attr;


