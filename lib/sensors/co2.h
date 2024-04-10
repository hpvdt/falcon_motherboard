// Code lifted from Savo Bajic's work on Titan;
// source: https://github.com/hpvdt/titan_2022/blob/main/Microcontroller%20Code/titan/lib/sensors/co2.h


#ifndef TITAN_CO2_HEADER
#define TITAN_CO2_HEADER

#include <Arduino.h>

extern volatile int CO2ppm; // CO2 level in ppm
// Is it possible to update a dereferenced pointer in an interrupt?

void setupCO2();
void CO2change();
void printCO2();

#endif