// Code originally written by George Wang, imported from 
// the Aircraft_Data repository

#ifndef ALTITUDE_HEADER
#define ALTITUDE_HEADER

#include <Arduino.h>
#include <Adafruit_DPS310.h>//arduino yay
#include <Wire.h>

//void measurements();//altitude measures
void pressureCheck(float *press);
void DPS_setup(TwoWire * dpsbus);

extern byte pressure;
extern byte temperature;

#endif