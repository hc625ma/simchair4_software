#define B206_HEAD_I2C_ADDRESS 18 // do not change this!

#include <Wire.h>

//#define CALIBRATE
byte data[1];


byte pins = 6;

uint8_t b = 0b00000000; //digital pins 0 to 7; x ^= (1 << n); - toggles nth bit of x.  all other bits left alone.
uint8_t b1 = 0b00000000; //digital pins 8 to 15
uint8_t x,y;

void setup() {
  Wire.begin(B206_HEAD_I2C_ADDRESS);                // join i2c bus with address #8
  Wire.onRequest(requestEvent); // register event
  #if defined(CALIBRATE)
    Serial.begin(115200);           // start serial for output
  #endif
  
  for (int i = 2; i <= pins; i++) {
    pinMode(i, INPUT_PULLUP);
  }
}

void loop() {
  for (int i = 2; i <= pins; i++){
    bool pin = !digitalRead(i);
    if (pin == 1) {
        b |= (1 << i - 2);       // forces ith bit of b to be 1.  all other bits left alone.
    } else {
        b &= ~(1 << i - 2);      // forces ith bit of b to be 0.  all other bits left alone.
    }    
  }
  //b >> 2;
  
  #if defined(CALIBRATE)
    printBits(b);
    Serial.println();
  #endif
}



// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void requestEvent() {
  data[0] = b;

  Wire.write(data,1);
}

void printBits(byte myByte)
{
  for(byte mask = 0x80; mask; mask >>= 1)
  {
    if(mask  & myByte)
       Serial.print('1');
    else
       Serial.print('0');
  }
}

