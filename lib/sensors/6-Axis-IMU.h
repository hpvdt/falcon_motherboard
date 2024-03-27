
#ifndef IMU_HEADER
#define IMU_HEADER

#include "ICM42688.h"
#include <Arduino.h>

extern double accX, accY, accZ, gyrX, gyrY, gyrZ, tempIMU;

void setupIMU();
void measureIMU();
void printIMU();

#endif