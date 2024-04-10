

#ifndef LIDAR_HEADER
#define LIDAR_HEADER

#include <Arduino.h>
#include "TFMPI2C.h"

void TFminisetup();

void getTFminidata(int *distance);


#endif