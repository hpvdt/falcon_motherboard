// Code originally written by George Wang, imported from the Aircraft_Data repository

#include "TFMPI2C.h"
#include "Wire.h"

TFMPI2C tfmini;

int16_t tfDist = 0;       // Distance to object in centimeters
int16_t tfFlux = 0;       // Signal strength or quality of return signal
int16_t tfTemp = 0;       // Internal temperature of Lidar sensor chip

  // Pin related stuff
  //  PB6 - I2C_SCK
  //  PB7 - I2C_SDA

void TFminisetup(TwoWire * tfbus)
{
  tfmini.setI2Cbus(tfbus);
  tfbus->begin();              // Initialize two-wire interface  
}

void getTFminidata(float *distance){
  tfmini.getData( tfDist, tfFlux, tfTemp); // Get a frame of data
  if( tfmini.status == TFMP_READY){
    *distance = tfDist;
  }
}
