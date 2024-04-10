#include <Arduino.h>
#include "ICM42688.h"
#include <Wire.h>
#include "TFmini_LIDAR.h"
#include "DPS_barometer.h"
#include "BNO_orientation.h"

#define LEDPIN1 PB12
#define LEDPIN2 PC8
#define LEDPIN3 PC9

const uint32_t SDA1 = PB7;
const uint32_t SCL1 = PB6;

TwoWire maini2c(SDA1, SCL1);

int distance = 0;

void setup()
{
  pinMode(LEDPIN1, OUTPUT);
  pinMode(LEDPIN2, OUTPUT);
  pinMode(LEDPIN3, OUTPUT);
  digitalWrite(LEDPIN1, HIGH);
  SerialUSB.begin(115200);
  while (!SerialUSB)
    delay(10); // Wait for USB connection to be made to the computer before continuing

  digitalWrite(LEDPIN2, HIGH);

  TFminisetup(maini2c); // TFmini LIDAR setup

  DPS_setup(maini2c); // DPS310 barometer setup

  //BNO_setup(); // BNO055 orientation setup

  //////////////////////////////////////////////////////////////////////////
  // Probably fix setup functions, I think we need to keep the sensor objects
  // in the scope of the main function
  //
  // Done for BNO_setup... may need to pass by reference ?
  //////////////////////////////////////////////////////////////////////////
}

void loop()
{
  // Test code for BNO055 orientation module.
  //float pitch, roll, heading;
  int press = 0;

  //BNO_measurements(pitch, roll, heading); // grab BNO readings,
  digitalWrite(LEDPIN3, HIGH);
  pressureCheck(&press);
  SerialUSB.println(press);
  getTFminidata(&distance);

  SerialUSB.println(distance);


  delay(1000);
  
  //SerialUSB.printf("Pitch: %f \nRoll: %f \nHeading: %f", pitch, roll, heading);
}
