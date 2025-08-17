#ifndef IMU_HEADER
#define IMU_HEADER

void imu_setup();
void imu_record(float* accX, float* accY, float* accZ, float* gyrX, float* gyrY, float* gyrZ, float* tempIMU);
void imu_print();

#endif