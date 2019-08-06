#define VRMAX_PEDESTAL_A_I2C_ADDRESS 41 // do not change this!

#include <Wire.h>

//#define CALIBRATE

uint16_t x,y,z;
byte data[4];


byte pins = 14;

byte encoderPinA = 3;
byte encoderPinB = 4;
byte n0;

uint8_t b = 0b00000000; //digital pins 0 to 7; x ^= (1 << n); - toggles nth bit of x.  all other bits left alone.
uint8_t b1 = 0b00000000; //digital pins 8 to 15
byte b2 = 0b00000000;

uint8_t encoderPos = 0;
uint8_t encoderPinALast = LOW;



void setup()
{
  Wire.begin(VRMAX_PEDESTAL_A_I2C_ADDRESS);                // join i2c bus with address #8
  Wire.onRequest(requestEvent); // register event
  #if defined(CALIBRATE)
    Serial.begin(115200);           // start serial for output
  #endif
  
  for (int i = 0; i <= pins+2; i++)
  {
    pinMode(i, INPUT_PULLUP);
  }
}

void loop()
{
  n0 = !digitalRead(encoderPinA);
  if ((encoderPinALast == LOW) && (n0 == HIGH)) 
  {
    if (!digitalRead(encoderPinB) == LOW) 
    {
      encoderPos--;
    } 
    else 
    {
      encoderPos++;
    }
   }
  encoderPinALast = n0;


  for (int i = 5; i <= pins; i++)
  {
    bool pin = !digitalRead(i);
    if (pin == 1)
    {
     if ((i - 5 < 8) && ((i != encoderPinA) && (i != encoderPinB)))
     {
        b |= (1 << i - 5);       // forces ith bit of b to be 1.  all other bits left alone.
     }
     else if ( (i - 5 >= 8) && (i - 5 < 15))
     {
        b1 |= (1 <<  (i - 5 - 8));
     }
    }
    else
    {
      if ( i - 5 < 8)
      {
        b &= ~(1 << i - 5);      // forces ith bit of b to be 0.  all other bits left alone.
      }
      else if ( (i - 5 >= 8) && (i - 5 < 15))
      {
        b1 &= ~(1 << (i - 5 - 8));
      }
    }

    pin = !digitalRead(0);
    if (pin == 1) {
       b1 |= (1 << 2);
    } else {
      b1 &= ~(1 << 2);
    }
    pin = !digitalRead(2);
    if (pin == 1) {
       b1 |= (1 << 3);
    } else {
      b1 &= ~(1 << 3);
    }
    pin = !digitalRead(15);
    if (pin == 1) {
       b2 |= (1 << 0);
    } else {
      b2 &= ~(1 << 0);
    }
    pin = !digitalRead(16);
    if (pin == 1) {
       b2 |= (1 << 1);
    } else {
      b2 &= ~(1 << 1);
    }

    
 }
  
  x = analogRead(A6) >> 2;
  y = analogRead(A7) >> 2;
  z = analogRead(A3);

  #if defined(CALIBRATE)
    Serial.print (encoderPos);
    Serial.print(" ");
    Serial.print(x);
    Serial.print(" ");
    Serial.print(y);
    Serial.print(" ");
    Serial.print(z);
    Serial.print(" ");
    printBits(b);
    Serial.print(" ");
    printBits(b1);
    Serial.print(" ");
    printBits(b2);
    Serial.println(" ");
  #endif
}



// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void requestEvent() {
  data[0] = x;
  data[1] = y;
  data[2] = (z >> 8) & 0xFF;
  data[3] = z & 0xFF;
  data[4] = b;
  data[5] = b1;
  data[6] = b2;
  data[7] = encoderPos;
  Wire.write(data,8);
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

