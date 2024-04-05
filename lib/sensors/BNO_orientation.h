// Code originally written by George Wang, imported from 
// the Aircraft_Data repository

#ifndef ORIENTATION_HEADER
#define ORIENTATION_HEADER

#include <Arduino.h>

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>

void BNO_measurements(float pitch, float roll, float heading); // altitude measures
void BNO_setup(); 

#endif