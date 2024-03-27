#include <USBSerial.h>
#include <Wire.h>
#include "co2.h"
#include "dht22.h"
#include "6-Axis-IMU.h"
/* Using an Arduino library for the ICM42688 maintained by Inhwan Wee.
   https://github.com/finani/ICM42688/
*/
void setup()
{
  SerialUSB.begin();
  setupCO2();
  setupDHT();
  setupIMU();
}

void loop()
{
  // should we be calling CO2change() function? Nah I think it's called by interrupts
  SerialUSB.print("CO2 PPM: ");
  SerialUSB.println(CO2ppm);
  // Delay between measurements.
  measureDHT();
  measureIMU();
  SerialUSB.println("DHT22 Readings");
  SerialUSB.print("Temperature: ");
  SerialUSB.println(temperature);
  SerialUSB.print("Humidity: ");
  SerialUSB.println(humidity);
  delay(1000);
}
