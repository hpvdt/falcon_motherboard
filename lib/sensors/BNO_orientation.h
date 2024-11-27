// Code originally written by George Wang, imported from 
// the Aircraft_Data repository

#ifndef ORIENTATION_HEADER
#define ORIENTATION_HEADER

#include <Arduino.h>

#include <Wire.h>

void BNO_measurements(float* pitch, float* roll, float* heading); 
void BNO_setup(TwoWire* bno_bus); 

#endif