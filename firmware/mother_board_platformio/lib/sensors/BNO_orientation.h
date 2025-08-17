// Code originally written by George Wang, imported from 
// the Aircraft_Data repository

#ifndef ORIENTATION_HEADER
#define ORIENTATION_HEADER

#include <Arduino.h>

#include <Wire.h>

void bno_record(float* pitch, float* roll, float* heading); 
void bno_setup(TwoWire* bno_bus); 

#endif