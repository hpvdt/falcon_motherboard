#ifndef GPS_HEADER
#define GPS_HEADER

#include <Arduino.h>

void gps_setup();
void gps_get_data(float *lattitude, float *longitude, float *speedGPS, float *altitude);

#endif