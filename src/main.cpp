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
// const int testPin = PB5;   from test code

void setup()
{
  // serial to display data
  Serial.begin(115200);
  while (!Serial)
  {
  }

  // start communication with IMU
  Serial.println("initializing...");
  int status = IMU.begin();
  if (status < 0)
  {
    Serial.println("IMU initialization failed");
    Serial.print("Status: ");
    Serial.println(status);
    while (1)
    {
    }
  }

  Serial.println("good to go");
}

void loop()       // fill in loop
{
  /* from test code
  digitalWrite(testPin, HIGH);
  delay(2000);
  digitalWrite(testPin, LOW);
  delay(2000);
  */
}
