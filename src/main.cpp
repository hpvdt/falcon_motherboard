#include <Arduino.h>
#include "ICM42688.h"

#include "TFmini_LIDAR.cpp"
#include "DPS_barometer.cpp"
#include "BNO_orientation.cpp"

void setup()
{
  SerialUSB.begin();
  while (!SerialUSB)
    delay(10); // Wait for USB connection to be made to the computer before continuing

  TFminisetup(); // TFmini LIDAR setup

  DPS_setup(); // DPS330 barometer setup

  BNO_setup(); // BNO055 orientation setup

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
  float pitch, roll, heading;

  BNO_measurements(pitch, roll, heading); // grab BNO readings,
  static int distance = 0;
  getTFminidata(&distance);
  

  SerialUSB.printf("Pitch: %f \nRoll: %f \nHeading: %f", pitch, roll, heading);

  // Test code for LIDAR
}
