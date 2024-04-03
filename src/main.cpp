#include <arduino.h>
#include <TinyGPS.h>
#include <SoftwareSerial.h>

#define CE_PIN PB5
#define CSN_PIN PB9
#define LEDPIN1 PB12
#define LEDPIN2 PC8
#define LEDPIN3 PC9
#define GPS_TX PA15
#define GPS_RX PB3

TinyGPS gps;
SoftwareSerial gps_serial(GPS_TX,GPS_RX);

float latitude = 0;         // Latitude (degrees)
float longitude = 0;        // Longitude (degrees)
float altitudeGPS = 0;      // Altitude (m)
float speedGPS = 0;         // GPS speed in km/h
float distanceGPS = 0;      // GPS distance in km

void setup(){
  SerialUSB.begin(115200);
  gps_serial.begin(9600);
}

void loop(){
  bool newData = false;
  while (gps_serial.available()) {
    char c = gps_serial.read();
    // Serial.write(c); // uncomment this line if you want to see the GPS data flowing
    if (gps.encode(c)) newData = true; // Did a new valid sentence come in?
  }
  if (newData) {
    unsigned long age;
    // Get coordinates
    gps.f_get_position(&latitude, &longitude, &age);
    latitude = (latitude == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : latitude);
    longitude = (longitude == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : longitude);
    
    altitudeGPS = gps.f_altitude();
    speedGPS = gps.f_speed_kmph();

    SerialUSB.println(latitude);
    SerialUSB.println(longitude);
    SerialUSB.println(altitudeGPS);
    SerialUSB.println(speedGPS);
    // Get the distance in KM from current to start point
    // distanceGPS = (latitude == TinyGPS::GPS_INVALID_F_ANGLE ? 1000 : (unsigned long)TinyGPS::distance_between(latitude, longitude, startLat, startLong) / 1000.0);
  }
}