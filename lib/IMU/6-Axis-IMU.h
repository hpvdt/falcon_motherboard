
#ifndef IMU_HEADER
#define IMU_HEADER

#include <Arduino.h>

void imu_setup();
void imu_record(double* accX, double* accY, double* accZ, double* gyrX, double* gyrY, double* gyrZ, double* tempIMU);
void imu_print();

#endif