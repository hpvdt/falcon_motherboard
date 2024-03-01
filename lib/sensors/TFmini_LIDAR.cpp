// Code originally written by George Wang, imported from
// the Aircraft_Data repository

//code and walkthrough from https://how2electronics.com/how-to-use-tfmini-s-lidar-distance-sensor-with-arduino/#Overview
//sensor documentation available here: https://www.sparkfun.com/products/16977 
 

#include "TFmini_LIDAR.h"

TFMini tfmini;
 
SoftwareSerial SerialTFMini(2, 3);          //The only value that matters here is the first one, 2, Rx
  //pin related stuff

float wrapper_distance = -1;//global vars updated by wrapper function, -1 placeholder, hopefully the code isnt too sketchy
  //dist is in cm
float wrapper_signal_strength = -1;
  //strength can be ignored

void getTFminiData(int *distance, int *strength)
{
    static char i = 0;
    char j = 0;
    int checksum = 0;
    static int rx[9];
    if (SerialTFMini.available())
    {
        rx[i] = SerialTFMini.read();
        if (rx[0] != 0x59)
        {
            i = 0;
        }
        else if (i == 1 && rx[1] != 0x59)
        {
            i = 0;
        }
        else if (i == 8)
        {
            for (j = 0; j < 8; j++)
            {
                checksum += rx[j];
            }
            if (rx[8] == (checksum % 256))
            {
                *distance = rx[2] + rx[3] * 256;
                *strength = rx[4] + rx[5] * 256;
            }
            i = 0;
        }
        else
        {
            i++;
        }
    }
    // added code to make the function a wrapper function:
    wrapper_distance = *distance;
    wrapper_signal_strength = *strength; // updates global vars
}

void TFminisetup()
{
  Serial.begin(115200);       //Initialize hardware serial port (serial debug port)
  
  SerialTFMini.begin(TFMINI_BAUDRATE);    //Initialize the data rate for the SoftwareSerial port
  tfmini.begin(&SerialTFMini);            //Initialize the TF Mini sensor  
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