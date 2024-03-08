

#ifndef LIDAR_HEADER
#define LIDAR_HEADER

#include <Arduino.h>
#include <SoftwareSerial.h>
#include "TFMini.h"

void getTFminidata(int *distance, int *strength);

void TFminisetup();

#endif