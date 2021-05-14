#include <Wire.h>

//#define DEBUG // uncomment to test - set serial baud rate to 115200

byte data[5];

byte encoder0PinA = 6;
byte encoder0PinB = 7;
byte encoder1PinA = 12;
byte encoder1PinB = 13;
byte encoder2PinA = 16;
byte encoder2PinB = 17;


uint8_t b = 0b00000000;
uint8_t b1 = 0b00000000;

uint8_t encoder0Pos = 0;
uint8_t encoder0PinALast = LOW;
uint8_t encoder1Pos = 0;
uint8_t encoder1PinALast = LOW;
uint8_t encoder2Pos = 0;
uint8_t encoder2PinALast = LOW;

uint8_t n0,n1,n2 = LOW;
uint8_t button_pins[] = {1,2,3,4,5,8,9,10,11,14,15};

void setup() {
  Wire.begin(22);                // join i2c bus with address #8
  Wire.onRequest(requestEvent); // register event

  
  for (int i = 0; i < sizeof(button_pins); i++)
  {
    pinMode(button_pins[i], INPUT_PULLUP);
  }

  pinMode (encoder0PinA,INPUT_PULLUP);
  pinMode (encoder0PinB, INPUT_PULLUP); 

  pinMode (encoder1PinA,INPUT_PULLUP);
  pinMode (encoder1PinB, INPUT_PULLUP);

  pinMode (encoder2PinA,INPUT_PULLUP);
  pinMode (encoder2PinB, INPUT_PULLUP);


  #if defined(DEBUG)
    Serial.begin (115200);
  #endif
}

void loop() 
{
  for (int i = 0; i < sizeof(button_pins); i++)
  {
    bool pin = !digitalRead(button_pins[i]);
    if (pin == 1)
    {
     if (i < 8)
     {
        b |= (1 << i);       // forces ith bit of b to be 1.  all other bits left alone.
     }
     else
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
      else
      {
        b1 &= ~(1 << (i - 8));
      }
    }
  }


  n0 = !digitalRead(encoder0PinA);
  if ((encoder0PinALast == LOW) && (n0 == HIGH)) 
  {
    if (!digitalRead(encoder0PinB) == LOW) 
    {
      encoder0Pos--;
    } 
    else 
    {
      encoder0Pos++;
    }
   }
  encoder0PinALast = n0;

  n1 = !digitalRead(encoder1PinA);
  if ((encoder1PinALast == LOW) && (n1 == HIGH)) 
  {
    if (!digitalRead(encoder1PinB) == LOW) 
    {
      encoder1Pos--;
    } 
    else 
    {
      encoder1Pos++;
    }
   }
  encoder1PinALast = n1;
  
  n2 = !digitalRead(encoder2PinA);
  if ((encoder2PinALast == LOW) && (n2 == HIGH)) 
  {
    if (!digitalRead(encoder2PinB) == LOW) 
    {
      encoder2Pos--;
    } 
    else 
    {
      encoder2Pos++;
    }
   }
  encoder2PinALast = n2;
  


// DEBUG
#if defined(DEBUG)
    printBits(b);
    Serial.print(" ");
    printBits(b1);
    Serial.print(" ");
    Serial.print (encoder0Pos);
    Serial.print(" ");
    Serial.print (encoder1Pos);
    Serial.print(" ");
    Serial.print (encoder2Pos);
    Serial.println();
#endif
  
}

void requestEvent() 
{
  data[0] = b;
  data[1] = b1;
  data[2] = encoder0Pos;
  data[3] = encoder1Pos;
  data[4] = encoder2Pos;
  
  Wire.write(data,5);
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





