
#ifndef IMU_HEADER
#define IMU_HEADER

#include "ICM42688.h"
#include <Arduino.h>

extern double accX, accY, accZ, gyrX, gyrY, gyrZ;

void setupIMU();
void measureIMU();

#endif