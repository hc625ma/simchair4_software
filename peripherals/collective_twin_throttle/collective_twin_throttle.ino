#define COLL_TWIN_THROTTLE_I2C_ADDRESS 15 // do not change this!

#include <Wire.h>

//#define CALIBRATE

uint16_t z,rz,ry;
uint8_t b = 0b00000000;

byte data[7];

uint8_t filter_counter_z = 6;
uint8_t filter_counter_rz = 6;
uint8_t filter_counter_ry = 6;

void setup() {
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  Wire.begin(COLL_TWIN_THROTTLE_I2C_ADDRESS);                // join i2c bus with address #8
  Wire.onRequest(requestEvent); // register event
  #if defined(CALIBRATE)
    Serial.begin(115200);           // start serial for output
  #endif
}

void loop() {
  z = filteredRead(A0,filter_counter_z);
  rz = filteredRead(A1,filter_counter_rz);
  ry = filteredRead(A2,filter_counter_ry);
  // uncomment #define CALIBRATE statement and change 2nd and 3rd values to physical min and max
  // of your lever; change the order of 2 last values to invert an axis
  //z = map(z,1002,0,0,1023);
  //rz = map(rz,347,938,1023,0);
  //rz = map(ry,347,938,1023,0);

  if ((z > 1023) && (z < 30000)) {
    z = 1023;
  }
  if ((rz > 1023) && (rz < 30000)) {
    rz = 1023;
  }
  if ((ry > 1023) && (ry < 30000)) {
    ry = 1023;
  }

  if (z > 30000) {
    z = 0;
  }
  if (rz > 30000){
    rz = 0;
  }
  if (ry > 30000){
    ry = 0;
  }

  for (int i = 2; i <=3 ; i++) {
    bool pin = !digitalRead(i);
    if (pin == 1) {
      b |= (1 << (i - 2));
    } else {
      b &= ~(1 << (i - 2));
    }
  }
  

  #if defined(CALIBRATE)
    Serial.print(z);
    Serial.print(" ");
    Serial.print(rz);
    Serial.print(" ");
    Serial.print(ry);
    Serial.print(" ");
    Serial.println(b);
  #endif
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void requestEvent() {
  data[0] = (z >> 8) & 0xFF;
  data[1] = z & 0xFF;
  data[2] = (rz >> 8) & 0xFF;
  data[3] = rz & 0xFF;
  data[4] = (ry >> 8) & 0xFF;
  data[5] = ry & 0xFF;
  data[6] = b;
  Wire.write(data,7);
}

uint16_t filteredRead (uint16_t input,uint8_t filter_counter)
{
  uint32_t filter = 0;
  for (uint8_t i=0;i<filter_counter;i++)
  {

      filter+= analogRead(input);
      delay(1);
  }

  uint16_t val = filter/filter_counter;
  return val;

}
