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
#include "communications.hpp"
#include "radio.hpp"
#include "aircraft_data.h"

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

AircraftState state;
IMUAxes onboard_imu;

float lidar_distance = 0.0;
float onboard_imu_temperature;

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

void strain_record(AircraftStrain* target) {
    ow_request( 4, &target->left_wing_strain[0]);
    ow_request( 4, &target->left_wing_strain[1]);
    ow_request( 4, &target->left_wing_strain[2]);
    ow_request( 4, &target->left_wing_torsion);

    ow_request( 4, &target->right_wing_strain[0]);
    ow_request( 4, &target->right_wing_strain[1]);
    ow_request( 4, &target->right_wing_strain[2]);
    ow_request( 4, &target->right_wing_torsion);

    ow_request( 5, &target->tail_strain);
    ow_request( 5, &target->tail_torsion);
}

void loop() {
    digitalWrite(LEDPIN2, HIGH);
    
    imu_record(&onboard_imu.lin.x, &onboard_imu.lin.y, &onboard_imu.lin.z, &onboard_imu.rot.x, &onboard_imu.rot.y, &onboard_imu.rot.z, &onboard_imu_temperature);
    bno_record(&state.orientation.pitch, &state.orientation.roll, &state.orientation.yaw);
    lidar_record(&lidar_distance);

    pressure_record(&state.environment.pressure);
    co2_record(&state.environment.co2);
    dht_record(&state.environment.temperature, &state.environment.humidity);

    gps_get_data(&state.gps.latitude, &state.gps.longitude, &state.gps.speed, &state.gps.altitude);

    // strain_record(&state.strain);

    // imu_print();
    // dht_print();
    // co2_print();

    // send_test_mesage(MESSAGE_TYPE_MAIN, COMM_CHANNEL_USB);

    ow_test_comms(2, 6, 100);
    
    digitalWrite(LEDPIN2, LOW);

    delay(1000);
}
