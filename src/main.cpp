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
#include "pc_communications.hpp"
#include "Radio.hpp"

const uint32_t LEDPIN1 = PB12;
const uint32_t LEDPIN2 = PC8;
const uint32_t LEDPIN3 = PC9;

const uint32_t SDA1 = PB7;
const uint32_t SCL1 = PB6;
TwoWire maini2c(SDA1, SCL1);

// OneWire addresses
const uint8_t owAdd = 2;
const uint32_t owTX  = PC3;
const uint32_t owRX  = PC2;

const uint32_t SPI_BUS_SCLK = PA5;
const uint32_t SPI_BUS_MISO = PA6;
const uint32_t SPI_BUS_MOSI = PA7;
SPIClass spi_bus_1(SPI_BUS_MOSI, SPI_BUS_MISO, SPI_BUS_SCLK);

float distance = 0.0;
float press = 0.0;
float pitch, roll, heading;
double humidity, DHTtemp;
double accX, accY, accZ, gyrX, gyrY, gyrZ, IMUtemp;
float latitude = 0;         // Latitude (degrees)
float longitude = 0;        // Longitude (degrees)
float altitudeGPS = 0;      // Altitude (m)
float speedGPS = 0;         // GPS speed in km/h

void setup() {
    pinMode(LEDPIN1, OUTPUT);
    pinMode(LEDPIN2, OUTPUT);
    pinMode(LEDPIN3, OUTPUT);

    int32_t rec;

    digitalWrite(LEDPIN3, HIGH);

    setup_pc_comms(true); // Setup PC communications, blocking until connected

    lidar_setup(&maini2c);      // TFmini LIDAR setup
    DPS_setup(&maini2c);        // DPS310 barometer setup
    bno_setup(&maini2c);        // BNO055 orientation setup
    co2_setup();
    dht_setup();
    imu_setup();
    gps_setup();
    ow_setup(owRX, owTX, owAdd, false);

    setupRadios(&spi_bus_1);
}

void loop()
{
    digitalWrite(LEDPIN2, HIGH);
    
    bno_record(&pitch, &roll, &heading);
    lidar_record(&distance);
    pressure_record(&press);
    dht_record(&DHTtemp, &humidity);
    imu_record(&accX, &accY, &accZ, &gyrX, &gyrY, &gyrZ, &IMUtemp);
    gps_get_data(&latitude, &longitude, &speedGPS, &altitudeGPS);

    SerialUSB.println(press);
    SerialUSB.println(distance);
    SerialUSB.println(pitch);
    SerialUSB.println(roll);
    SerialUSB.println(heading);
    imu_print();
    dht_print();
    co2_print();

    send_pc_packet();

    digitalWrite(LEDPIN2, LOW);

    delay(1000);
}
