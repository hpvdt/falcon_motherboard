// Code originally written by George Wang, imported from
// the Aircraft_Data repository

//code and walkthrough from https://how2electronics.com/how-to-use-tfmini-s-lidar-distance-sensor-with-arduino/#Overview
//sensor documentation available here: https://www.sparkfun.com/products/16977 
 
// The following link has an example for I2C communication with the LIDAR; will probably
// look to implement this.
// https://github.com/TFmini/TFmini-Arduino/blob/master/TFmini_Arduino_I2C_1Master1Slave/TFmini_I2C_Master/TFmini_I2C_Master.ino

#include "TFMPI2C.h"
#include "Wire.h"

TFMPI2C tfmini;

int16_t tfDist = 0;       // Distance to object in centimeters
int16_t tfFlux = 0;       // Signal strength or quality of return signal
int16_t tfTemp = 0;       // Internal temperature of Lidar sensor chip

  //pin related stuff
  // TFmini's pins: PB6 - I2C_SCK
  //                PB7 - I2C_SDA

void TFminisetup(TwoWire tfbus)
{
  tfmini.setI2Cbus(&tfbus);
  Serial.begin(115200);       // Initialize hardware serial port (serial debug port)
  tfbus.begin();              // Initialize two-wire interface  
}

void getTFminidata(int *distance){
  tfmini.getData( tfDist, tfFlux, tfTemp); // Get a frame of data
  if( tfmini.status == TFMP_READY){
    *distance = tfDist;
  }
}

/*
example code for TFmini LIDAR
void loop()
{
  int distance = 0;
  int strength = 0;
 
  getTFminiData(&distance, &strength);
  while (!distance)
  {
    getTFminiData(&distance, &strength);
    if (distance)
    {
      Serial.print(distance);//dist to object
      Serial.print("cm\t");
      Serial.print("signal strength: "); //signal strength (mostly ignore)
      Serial.println(strength);
      
      these two lines were also commented.
      Serial.println(wrapper_distance);
      Serial.println(wrapper_signal_strength);
      
    }
  }
  delay(100);
}
*/
//Example code for Benewake TFMini time-of-flight distance sensor. 
//library by Peter Jansen (December 11/2017)