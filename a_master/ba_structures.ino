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


