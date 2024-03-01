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
  
  DPS_setup();  // DPS330 barometer setup
  
  setup_BNO(); // BNO055 orientation setup
  
}

void loop()
{
  // Test code for BNO055 orientation module.
  float pitch, roll, heading;

  BNO_measurements(pitch, roll, heading); // grab BNO readings, 

  // Test code for barometer.
}
