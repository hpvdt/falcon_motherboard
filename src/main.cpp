#include "6-Axis-IMU.h"
/* Using an Arduino library for the ICM42688 maintained by Inhwan Wee.
   https://github.com/finani/ICM42688/
*/

void setup()
{
  /*    some debugging LED code
  pinMode(PC8, OUTPUT);   // Top green
  pinMode(PC9, OUTPUT);   // Middle green
  pinMode(PB12, OUTPUT);  // Bottom red

  digitalWrite(PC8, HIGH);
  digitalWrite(PC9, HIGH);
  digitalWrite(PB12, LOW);
  */
  
  while (!SerialUSB) delay(10); 
  // Wait for USB connection to be made to the computer before continuing

  IMU_setup();

  delay(10);
  SerialUSB.begin();
}

void loop()
{
  IMU_measurements();
  delay(1000);
}
