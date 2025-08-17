

#ifndef LIDAR_HEADER
#define LIDAR_HEADER

#include <Arduino.h>
#include <Wire.h>

void lidar_setup(TwoWire* tfbus);

void lidar_record(float *distance);


#endif