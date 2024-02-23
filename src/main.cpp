#include <Arduino.h>
#include "ICM42688.h"
/* Using an Arduino library for the ICM42688 maintained by Inhwan Wee.
   https://github.com/finani/ICM42688/
*/

SPIClass IMUspi(PB15, PB14, PB13); // Initialize specific SPI bus.
ICM42688 IMU(IMUspi, PB10);        // initializes an ICM chip object, specifying
                                   // protocol + pin select for serial communication
                                   // defaults to 8MHz clock, but a third parameter can be
                                   // passed to specify up to 24 MHz.
                                   // PB10 is chip select pin.

void setup()
{

  pinMode(PC8, OUTPUT);   // Top green
  pinMode(PC9, OUTPUT);   // Middle green
  pinMode(PB12, OUTPUT);  // Bottom red

  digitalWrite(PC8, HIGH);
  digitalWrite(PC9, HIGH);
  digitalWrite(PB12, LOW);

      SerialUSB.begin();
  while (!SerialUSB)
    delay(10); // Wait for USB connection to be made to the computer before continuing

  // start communication with IMU
  SerialUSB.println("Initializing...");
  int status = IMU.begin();

  if (status < 0)
  {
    SerialUSB.println("IMU initialization failed");
    SerialUSB.print("Status: ");
    SerialUSB.println(status);
    while (1)
    {
      IMU.begin();
      digitalWrite(PB12, HIGH); // If initialization failed, red LED turns on
      delay(10);
    }
  }

  SerialUSB.println("good to go");

  delay(20);
}

void loop()
{
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
