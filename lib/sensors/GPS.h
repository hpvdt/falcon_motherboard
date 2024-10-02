#ifndef GPS_HEADER
#define GPS_HEADER

#include <Arduino.h>

void GPSSetup();
void getGPSdata(float *lattitude, float *longitude, float *speedGPS, float *altitude);

#endif