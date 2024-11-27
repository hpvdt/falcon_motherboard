
#ifndef IMU_HEADER
#define IMU_HEADER

#include <Arduino.h>

void setupIMU();
void measureIMU(double* accX, double* accY, double* accZ, double* gyrX, double* gyrY, double* gyrZ, double* tempIMU);
void printIMU();

#endif