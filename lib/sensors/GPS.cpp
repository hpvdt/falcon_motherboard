#include "TinyGPS.h"

#define CE_PIN PB5
#define CSN_PIN PB9
#define LEDPIN1 PB12
#define LEDPIN2 PC8
#define LEDPIN3 PC9
#define GPS_TX PA15
#define GPS_RX PB3

TinyGPS gps;
HardwareSerial gps_serial(GPS_TX,GPS_RX);

void GPSSetup(){
    gps_serial.begin(9600);
}

void getGPSdata(float *latitude, float *longitude, float *speedGPS, float *altitude){
    bool newData = false;
  while (gps_serial.available()) {
    char c = gps_serial.read();
    if (gps.encode(c)) newData = true; // Did a new valid sentence come in?
  }
  if (newData) {
    unsigned long age;
    gps.f_get_position(latitude, longitude, &age);
    *latitude = (*latitude == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : *latitude);
    *longitude = (*longitude == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : *longitude);
    *altitude = gps.f_altitude();
    *speedGPS = gps.f_speed_kmph();
  }
}
    