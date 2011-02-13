// Wire Master Writer
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Writes data to an I2C/TWI slave device
// Refer to the "Wire Slave Receiver" example for use with this

// Created 29 March 2006

// This example code is in the public domain.


#include <Wire.h>


void setup()
{
  Wire.begin(); // join i2c bus (address optional for master)
  Serial.begin(9600);
}

byte mainScore = 0;
byte slaveScore = 1;
byte mainToServe = 1;

void loop()
{
  Serial.print((int)mainScore);
  Serial.print((int)slaveScore);
  Serial.print((int)mainToServe);

  Wire.beginTransmission(15); // transmit to device #15
  Wire.send(mainScore);            // sends one byte
  Wire.send(slaveScore);
  if (mainScore % 2 == 0) {
    mainToServe ^= 1;
  }
  Wire.send(mainToServe);
  Wire.endTransmission();     // stop transmitting
  

  mainScore++;
  slaveScore += 2;
  delay(2000);
}
