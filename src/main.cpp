#include "6-Axis-IMU.h"
/* Using an Arduino library for the ICM42688 maintained by Inhwan Wee.
   https://github.com/finani/ICM42688/
*/

void setup()
{

  while (!SerialUSB) delay(10); 
  // Wait for USB connection to be made to the computer before continuing

  setupIMU();

  delay(10);
  SerialUSB.begin();
}

void loop()
{
  measureIMU();
  delay(1000);
}
