#define SIMPLE_PEDESTAL_I2C_ADDRESS 30 // do not change this!

#include <Wire.h>

//#define CALIBRATE
byte data[2];


byte pins = 13;

uint8_t b = 0b00000000; //digital pins 0 to 7; x ^= (1 << n); - toggles nth bit of x.  all other bits left alone.
uint8_t b1 = 0b00000000; //digital pins 8 to 15
uint8_t x,y;

void setup()
{
  Wire.begin(SIMPLE_PEDESTAL_I2C_ADDRESS);                // join i2c bus with address #8
  Wire.onRequest(requestEvent); // register event
  #if defined(CALIBRATE)
    Serial.begin(115200);           // start serial for output
  #endif
  
  for (int i = 3; i <= pins; i++)
  {
    pinMode(i, INPUT_PULLUP);
  }
  // pin 2 powers KY-023 board
  pinMode(2,OUTPUT);
  digitalWrite(2,HIGH);
}

void loop()
{
  for (int i = 0; i <= pins; i++)
  {
    bool pin = !digitalRead(i);
    if (pin == 1)
    {
     if (i < 8)
     {
        b |= (1 << i);       // forces ith bit of b to be 1.  all other bits left alone.
     }
     else if ( (i >= 8) && (i < 16))
     {
        b1 |= (1 <<  (i - 8));
     }
    }
    else
    {
      if ( i < 8)
      {
        b &= ~(1 << i);      // forces ith bit of b to be 0.  all other bits left alone.
      }
      else if ( (i >= 8) && (i < 16))
      {
        b1 &= ~(1 << (i - 8));
      }
    }
    x = analogRead(A0) >> 2;
    y = analogRead(A1) >> 2;

    
 }
  

  #if defined(CALIBRATE)
    printBits(b);
    Serial.print(" ");
    printBits(b1);
    Serial.println();
  #endif
}



// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void requestEvent() {
  data[0] = x;
  data[1] = y;
  data[2] = b;
  data[3] = b1;

  Wire.write(data,4);
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

