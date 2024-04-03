
#ifndef IMU_HEADER
#define IMU_HEADER

#include "ICM42688.h"
#include <Arduino.h>

extern SPIClass IMUspi;
extern ICM42688 IMU;

void IMU_setup();
void IMU_measurements();

#endif