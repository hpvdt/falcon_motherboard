// Code originally written by George Wang, imported from
// the Aircraft_Data repository

// This example shows how to read temperature/pressure from adafruit library
// walkthrough and code: https://learn.adafruit.com/adafruit-dps310-precision-barometric-pressure-sensor/arduino

#include "DPS_barometer.h"
#include "Wire.h"

// debug to figure out how to include this, maybe use setup function
Adafruit_DPS310 dps;
Adafruit_Sensor *dps_pressure = dps.getPressureSensor();
TwoWire barometer_bus(PB7, PB6);

void DPS_setup()
{
  SerialUSB.begin(115200);
  if (!dps.begin_I2C(119, &barometer_bus))
  {
    // Something went wrong...
    SerialUSB.println("DPS not worketh");
    while (1)
      ;
  }

  dps.configurePressure(DPS310_64HZ, DPS310_64SAMPLES);
  dps.configureTemperature(DPS310_64HZ, DPS310_64SAMPLES);
}

void pressureCheck(int *press)
{
 sensors_event_t temp_event, pressure_event;
  if (dps.pressureAvailable()){
    dps_pressure->getEvent(&pressure_event);
    *press = pressure_event.pressure;
  }
}