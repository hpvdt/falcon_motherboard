#include <arduino.h>
#include <TinyGPS.h>
#include "GPS.h"

float latitude = 0;         // Latitude (degrees)
float longitude = 0;        // Longitude (degrees)
float altitudeGPS = 0;      // Altitude (m)
float speedGPS = 0;         // GPS speed in km/h
float distanceGPS = 0;      // GPS distance in km

void setup(){
  SerialUSB.begin(115200);
  GPSSetup();
}

void loop(){
  getGPSdata(&latitude, &longitude, &speedGPS, &altitudeGPS);
}