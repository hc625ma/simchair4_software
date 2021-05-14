// <TWIN_COLLECTIVE_MKIII>
    #define TCOLL_MKIII_PHYSICAL_THROTTLE_LATCH 0 // if your collective has a physical idle stop latch, set this to 1, otherwise set it to 0 for advanced throttle related functions to work

    #define TWIN_COLLECTIVE_MKIII_MIN 1023
    #define TWIN_COLLECTIVE_MKIII_MAX 0

    uint16_t TWIN_COLLECTIVE_MKIII_THR_MIN[] = {1023,1023};
    uint16_t TWIN_COLLECTIVE_MKIII_THR_MAX[] = {0,0};


    uint16_t TWIN_COLLECTIVE_MKIII_THR_IDLE_DETENT_AXIS_VAL[] = {976,959}; // throttle axis value at the detent mark

    byte TWIN_COLLECTIVE_MKIII_PHYSICAL_LATCH_MOD_JOY_BUTTON[] = {1,2};
    
    //#define SINGLE_COLLECTIVE_MKIII_PHYSICAL_LATCH_MOD_JOY_BUTTON 1 // this button will be pressed on throttle cutoff, if IDLE REL button was pressed
    
  // </TWIN_COLLECTIVE_MKIII>
