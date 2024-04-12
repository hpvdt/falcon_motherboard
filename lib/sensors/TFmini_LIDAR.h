

#ifndef LIDAR_HEADER
#define LIDAR_HEADER

#include <Arduino.h>
#include "TFMPI2C.h"
#include <Wire.h>

void TFminisetup(TwoWire* tfbus);

void getTFminidata(float *distance);


#endif