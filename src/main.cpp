#include <Arduino.h>
#include "ICM42688.h"
/* Using an Arduino library for the ICM42688 maintained by Inhwan Wee.
   https://github.com/finani/ICM42688/
*/
ICM42688 IMU(SPI, 10); // initializes an ICM chip object, specifying
                       // protocol + pin select for serial communication
                       // defaults to 8MHz clock, but a third parameter can be
                       // passed to specify up to 24 MHz.
                       // probably refer to motherboard schematic for specific pin

void setup() {
  SerialUSB.begin();
  while (!SerialUSB) delay(10); // Wait for USB connection to be made to the computer before continuing
}

void loop() {
  // start communication with IMU
  SerialUSB.println("initializing...");
  int status = IMU.begin();
  if (status < 0)
  {
    SerialUSB.println("IMU initialization failed");
    SerialUSB.print("Status: ");
    SerialUSB.println(status);
    while (1)
    {
    }
  }

  SerialUSB.println("good to go");

  delay(20);
}
