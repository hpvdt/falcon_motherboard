#include <Arduino.h>
#include "ICM42688.h"
#include <Wire.h>
#include "TFmini_LIDAR.h"
#include "DPS_barometer.h"
#include "BNO_orientation.h"
#include <USBSerial.h>
#include "co2.h"
#include "dht22.h"
#include "6-Axis-IMU.h"
#include <TinyGPS.h>
#include "GPS.h"
#include "onewire.hpp"

#define LEDPIN1 PB12
#define LEDPIN2 PC8
#define LEDPIN3 PC9

const uint32_t SDA1 = PB7;
const uint32_t SCL1 = PB6;

// OneWire addresses
const int owAdd = 2;
const int owAddTest = 0b1010; // Hardcoded address for spar boards.
const int owTX  = PC3;
const int owRX  = PC2;

TwoWire maini2c(SDA1, SCL1);

float distance = 0.0;
float press = 0.0;
float pitch, roll, heading;
double humidity, DHTtemp;
double accX, accY, accZ, gyrX, gyrY, gyrZ, IMUtemp;
float latitude = 0;         // Latitude (degrees)
float longitude = 0;        // Longitude (degrees)
float altitudeGPS = 0;      // Altitude (m)
float speedGPS = 0;         // GPS speed in km/h
float distanceGPS = 0;      // GPS distance in km

void setup()
{
  pinMode(LEDPIN1, OUTPUT);
  pinMode(LEDPIN2, OUTPUT);
  pinMode(LEDPIN3, OUTPUT);

  int32_t rec;

  digitalWrite(LEDPIN3, HIGH);

  SerialUSB.begin(115200);

  while (!SerialUSB)
    delay(10);                // Wait for USB connection to be made to the computer before continuing

  TFminisetup(&maini2c);      // TFmini LIDAR setup
  DPS_setup(&maini2c);        // DPS310 barometer setup
  BNO_setup(&maini2c);        // BNO055 orientation setup
  setupCO2();
  setupDHT();
  setupIMU();
  GPSSetup();
  setupOneWire(owRX, owTX, owAdd, false);
  requestOneWire(owAddTest, &rec);
}

void loop()
{
  digitalWrite(LEDPIN2, HIGH);
  
  BNO_measurements(&pitch, &roll, &heading); // grab BNO readings,
  getTFminidata(&distance);
  pressureCheck(&press);
  measureDHT(&DHTtemp, &humidity);
  measureIMU(&accX, &accY, &accZ, &gyrX, &gyrY, &gyrZ, &IMUtemp);
  getGPSdata(&latitude, &longitude, &speedGPS, &altitudeGPS);
  int32_t rec = 0;
  requestOneWire(owAddTest, &rec);

  digitalWrite(LEDPIN1, HIGH);

  SerialUSB.println(press);
  SerialUSB.println(distance);
  SerialUSB.println(pitch);
  SerialUSB.println(roll);
  SerialUSB.println(heading);
  printIMU();
  printDHT();
  printCO2();

  digitalWrite(LEDPIN2, LOW);

  delay(1000);
}
