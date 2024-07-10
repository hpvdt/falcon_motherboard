#include "TinyGPS.h"
#include <Arduino.h>

const uint32_t GPS_TX = PA15;
const uint32_t GPS_RX = PB3;
HardwareSerial gps_serial(GPS_TX, GPS_RX);

TinyGPS gps;
const unsigned long GPS_BAUDRATE = 9600;

void GPSSetup() {
    gps_serial.begin(GPS_BAUDRATE);
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
    