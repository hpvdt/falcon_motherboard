// Code originally written by George Wang, imported from
// the Aircraft_Data repository

#include "DPS_barometer.h"
#include "Wire.h"
#include <Adafruit_DPS310.h>//arduino yay

Adafruit_DPS310 dps;
Adafruit_Sensor *dps_pressure = dps.getPressureSensor();

void DPS_setup(TwoWire * dpsbus) {
    SerialUSB.begin(115200);
    if (!dps.begin_I2C(119, dpsbus)) {
        // Something went wrong...
        SerialUSB.println("DPS not worketh");
        while (1);
    }

    dps.configurePressure(DPS310_64HZ, DPS310_64SAMPLES);
    dps.configureTemperature(DPS310_64HZ, DPS310_64SAMPLES);
}

void pressure_record(float *press) {
    sensors_event_t temp_event, pressure_event;
    if (dps.pressureAvailable()){
        dps_pressure->getEvent(&pressure_event);
        *press = pressure_event.pressure;
    }
}