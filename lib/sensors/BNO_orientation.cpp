// Code originally written by George Wang, imported from the Aircraft_Data repository

#include "BNO_orientation.h"

Adafruit_BNO055 bno = Adafruit_BNO055(55);
uint16_t BNO055_SAMPLERATE_DELAY_MS = 100;

void BNO_setup(TwoWire * bno_bus){
    if (!bno.begin(0x28, bno_bus)){
        while (1);
    }
    delay(1000);
}

void BNO_measurements(float* pitch, float* roll, float* heading){ 
    sensors_event_t orientationData;
    bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER); 
    *pitch = orientationData.orientation.z;
    *roll = orientationData.orientation.y;
    *heading = orientationData.orientation.x;
}

