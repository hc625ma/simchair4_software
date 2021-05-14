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
  uint16_t thr_max[3] = {1023,1023,1023};
  uint16_t thr_min[3] = {0,0,0};
  uint16_t idle_detent_axis_val[3] = {0,0,0};
  bool btn_press_on_thr_cutoff;
  bool dcs_huey_compat_mode;
  uint8_t phys_thr_latch_joy_btn[3];
  uint8_t base_version;
  bool lastButtonState[10];
  uint16_t thr_buf[3] = {0,0,0};
  bool phys_thr_latch_btn_state[3] = {0,0,0};
  bool thr_latch_pressed[3] = {1,1,1};
   bool phys_thr_latch = 0;
} t_struct_coll_attr;


#if defined (VRMAX_AB412_COLL_HEAD_EXTENSION)
typedef struct __attribute__((__packed__)) 
{
   uint8_t sw_id;
   uint8_t sw_type;
   uint8_t sw_middle_b;
   bool sw_val;
   long sw_ts;
   bool sw_tr_state;
}  t_struct_sw_matrix;

typedef struct __attribute__((__packed__)) 
{
  bool sw_val;
  uint8_t c1f;
  uint8_t c1c;
  uint8_t n1f;
  uint8_t n1c;
  uint8_t c2f;
  uint8_t c2c;
  uint8_t n2f;
  uint8_t n2c;
  uint8_t x1;
  uint8_t x2;
  uint8_t x3;
  uint8_t x4;
}  t_struct_r_matrix;

typedef struct __attribute__((__packed__)) 
{
   uint8_t enc_id;
   uint8_t val;
   uint8_t last_val;
   long enc_ts;
   byte button_id;
   bool button_val;
   byte press_counter;
   bool last_dir;
}  t_struct_enc_state;
typedef struct __attribute__((__packed__)) 
{
  uint8_t e; // encoder id
  uint8_t r0l; // radio stack selector pos 0 left
  uint8_t r0r; // radio stack selector pos 0 right
  uint8_t r1l; // radio stack selector pos 1 left
  uint8_t r1r; // radio stack selector pos 1 right
  uint8_t r2l; // radio stack selector pos 2 left
  uint8_t r2r; // radio stack selector pos 2 right
  
  uint8_t m0l; // panel mode switch pos 0 left
  uint8_t m0r; // panel mode switch pos 0 right
  uint8_t m1l; // panel mode switch pos 1 left
  uint8_t m1r; // panel mode switch pos 1 right
  uint8_t m2l; // panel mode switch pos 2 left
  uint8_t m2r; // panel mode switch pos 2 right
  uint8_t adfl; // nav mode switch pos middle + radio stack selector pos 2 (xpdr) - adf mode left
  uint8_t adfr; // nav mode switch pos middle + radio stack selector pos 2 (xpdr) - adf mode right

  uint8_t r0lpm1; // radio stack selector pos 0 left panel mode sw in pos 1
  uint8_t r0rpm1; // radio stack selector pos 0 right panel mode sw in pos 1
  uint8_t r1lpm1; // radio stack selector pos 1 left panel mode sw in pos 1
  uint8_t r1rpm1; // radio stack selector pos 1 right panel mode sw in pos 1
  uint8_t r2lpm1; // radio stack selector pos 2 left panel mode sw in pos 1
  uint8_t r2rpm1; // radio stack selector pos 2 right panel mode sw in pos 1

  uint8_t r0lpm2; // radio stack selector pos 0 left panel mode sw in pos 2
  uint8_t r0rpm2; // radio stack selector pos 0 right panel mode sw in pos 2
  uint8_t r1lpm2; // radio stack selector pos 1 left panel mode sw in pos 2
  uint8_t r1rpm2; // radio stack selector pos 1 right panel mode sw in pos 2
  uint8_t r2lpm2; // radio stack selector pos 2 left panel mode sw in pos 2
  uint8_t r2rpm2; // radio stack selector pos 2 right panel mode sw in pos 2
} t_struct_rp_e_matrix;

typedef struct __attribute__((__packed__)) 
{
  uint8_t e; // encoder id
  uint8_t l; // MAG left
  uint8_t r; // MAG right
} t_struct_mag_conf;

typedef struct __attribute__((__packed__)) // e  n0l n0r n1l n1r n2l n2r
{
  uint8_t e;
  uint8_t n0l; // VOR 1 OBS left
  uint8_t n0r; // VOR 1 OBS right
  uint8_t n1l; // ADF HDG left
  uint8_t n1r; // ADF HDG right
  uint8_t n2l; // VOR 2 OBS left
  uint8_t n2r; // VOR 2 OBS right
} t_struct_nav_conf;

typedef struct __attribute__((__packed__)) // e  n0l n0r n1l n1r n2l n2r
{
  uint8_t e;
  uint8_t a0l;
  uint8_t a0r;
  uint8_t a1l;
  uint8_t a1r;
  uint8_t a2l;
  uint8_t a2r;
} t_struct_alt_conf;
#endif
