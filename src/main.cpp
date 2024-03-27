#include <USBSerial.h>
#include <Wire.h>

#include "co2.h"
#include "dht22.h"

void setup()
{
  SerialUSB.begin();
  setupCO2();
  setupDHT();
}

void loop()
{
  // should we be calling CO2change() function? Nah I think it's called by interrupts
  SerialUSB.print("CO2 PPM: ");
  SerialUSB.println(CO2ppm);
  // Delay between measurements.
  measureDHT();
  SerialUSB.println("DHT22 Readings");
  SerialUSB.print("Temperature: ");
  SerialUSB.println(temperature);
  SerialUSB.print("Humidity: ");
  SerialUSB.println(humidity);
  delay(1000);
}
