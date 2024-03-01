// Code originally written by George Wang, imported from 
// the Aircraft_Data repository

#ifndef ORIENTATION_HEADER
#define ORIENTATION_HEADER

#include <Arduino.h>

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <utility/imumaths.h>

void measurements(); // altitude measures
void setup_BNO(); 

#endif