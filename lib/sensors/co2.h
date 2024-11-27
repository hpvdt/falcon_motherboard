// Code lifted from Savo Bajic's work on Titan;
// source: https://github.com/hpvdt/titan_2022/blob/main/Microcontroller%20Code/titan/lib/sensors/co2.h


#ifndef CO2_HEADER
#define CO2_HEADER

#include <Arduino.h>

extern volatile int co2_ppm; // CO2 level in ppm
// Is it possible to update a dereferenced pointer in an interrupt?

void setup_co2();
void co2_change();
void co2_print();

#endif