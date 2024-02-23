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
  while (!SerialUSB)
    delay(10); // Wait for USB connection to be made to the computer before continuing
  
  // start communication with IMU
  SerialUSB.println("initializing...");
  int status = IMU.begin();

  if (status < 0) {
    SerialUSB.println("IMU initialization failed");
    SerialUSB.print("Status: ");
    SerialUSB.println(status);
    while (1) { }
  }

  SerialUSB.println("good to go");

  delay(20);

}

void loop() {
  IMU.getAGT(); // grab newest readings for accel. and gyro.
  SerialUSB.println("Accelerometer Readings");
  SerialUSB.printf("a_x: %f \t a_y: %f \t a_z: %f \n", IMU.accX(), IMU.accY(), IMU.accZ());
  SerialUSB.println("Gyroscope Readings");
  SerialUSB.print("g_x");
  SerialUSB.println(IMU.gyrX());
  SerialUSB.print("g_y");
  SerialUSB.println(IMU.gyrY());
  SerialUSB.print("g_z");
  SerialUSB.println(IMU.gyrZ());
  SerialUSB.print("Temperature: ");
  SerialUSB.println(IMU.temp());
  delay(100);
}
