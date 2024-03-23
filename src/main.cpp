#include <USBSerial.h>
#include <Wire.h>

#include "co2.h"
#include "dht22.h"

const int LEDPIN = PC8;
const int LEDPIN2 = PC9;

void setup()
{
  setupCO2();
  setupDHT();
  SerialUSB.begin();

}

void loop()
{
    
  // should we be calling CO2change() function?
  SerialUSB.print("CO2 PPM: ");
  SerialUSB.println(CO2ppm);
  // Delay between measurements.
  DHT_measurements();
  delay(delayMS);
}
