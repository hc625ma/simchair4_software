#define COLL_SINGLE_THROTTLE_I2C_ADDRESS 10 // do not change this!

#include <Wire.h>

//#define CALIBRATE

uint16_t z,rz;
uint8_t b = 0b00000000;
uint8_t h = 0b00000000;

byte data[6];

uint8_t filter_counter_z = 6;
uint8_t filter_counter_rz = 6;

void setup() {
   for (int i = 4; i <=12 ; i++) {
    pinMode(i, INPUT_PULLUP);
   }
  Wire.begin(COLL_SINGLE_THROTTLE_I2C_ADDRESS);                // join i2c bus with address #8
  Wire.onRequest(requestEvent); // register event
  #if defined(CALIBRATE)
    Serial.begin(115200);           // start serial for output
  #endif
}

void loop() {
  z = filteredRead(A0,filter_counter_z);
  rz = filteredRead(A1,filter_counter_rz);
  // uncomment #define CALIBRATE statement and change 2nd and 3rd values to physical min and max
  // of your lever; change the order of 2 last values to invert an axis
  z = map(z,1002,0,0,1023);
  rz = map(rz,722,150,0,1023);

  if ((z > 1023) && (z < 30000)) {
    z = 1023;
  }

  if ((rz > 1023) && (rz < 30000)) {
    rz = 1023;
  }

  if (z > 30000) {
    z = 0;
  }

  if (rz > 30000){
    rz = 0;
  }

  for (int i = 4; i <=7 ; i++) {
    bool pin = !digitalRead(i);
    if (pin == 1) {
      b |= (1 << (i - 4));
    } else {
      b &= ~(1 << (i - 4));
    }
  }

  for (int i = 8; i <=12 ; i++) {
    bool pin = !digitalRead(i);
    if (pin == 1) {
      h |= (1 << (i - 8));
    } else {
      h &= ~(1 << (i - 8));
    }
  }
  

  #if defined(CALIBRATE)
    Serial.print(z);
    Serial.print(" ");
    Serial.print(rz);
    Serial.print(" ");
    printBits(b);
    Serial.print(" ");
    printBits(h);
    Serial.println("");
  #endif
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void requestEvent() {
  data[0] = (z >> 8) & 0xFF;
  data[1] = z & 0xFF;
  data[2] = (rz >> 8) & 0xFF;
  data[3] = rz & 0xFF;
  data[4] = b;
  data[5] = h;
  Wire.write(data,6);
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

void printBits(byte myByte){
  for(byte mask = 0x80; mask; mask >>= 1){
    if(mask  & myByte)
      Serial.print('1');
    else
      Serial.print('0');
  }
}
