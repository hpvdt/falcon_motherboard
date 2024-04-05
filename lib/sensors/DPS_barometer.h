// Code originally written by George Wang, imported from 
// the Aircraft_Data repository

#ifndef ALTITUDE_HEADER
#define ALTITUDE_HEADER

#include <Arduino.h>
#include <Adafruit_DPS310.h>//arduino yay

//void measurements();//altitude measures
void pressureCheck(int *press);
void DPS_setup();

extern byte pressure;
extern byte temperature;

#endif