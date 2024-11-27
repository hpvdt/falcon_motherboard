#ifndef GPS_HEADER
#define GPS_HEADER

#include <Arduino.h>

void setup_gps();
void gps_get_data(float *lattitude, float *longitude, float *speedGPS, float *altitude);

#endif