
#ifndef IMU_HEADER
#define IMU_HEADER

#include "ICM42688.h"
#include <Arduino.h>

void setupIMU();
void measureIMU(double*, double*, double*, double*, double*, double*, double*);
void printIMU();

#endif