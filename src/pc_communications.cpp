#include <Arduino.h>
#include "pc_communications.hpp"
#include "aircraft_data.h"

/**
 * \brief Set the up USB/PC communications
 * 
 * \param block Block until connection is confirmed or not
 */
void setup_pc_comms(bool block) {
    SerialUSB.begin(115200);

    if (block == false) return; // Return immediately once set up

    // Wait for USB connection to be made to the computer before continuing
    while (!SerialUSB) delay(10); 
}

void float_by_bytes(float data) {
    char * dataPointer = (char *) & data;

    //for (int i = 4; i > 0; i--) SerialUSB.write(dataPointer[i-1]); // Most significant byte first
    for (int i = 0; i < 4; i++) SerialUSB.write(dataPointer[i]); // Least significant byte first
}

void send_pc_packet (AircraftState* cur) {
    /* the order of data going into the vision system:
  
    Airspeed, Altimeter,
    GyroX, GyroY, GyroZ, GyroW, (-1 , 1)
    heading, (0,360)
    GPSX, GPSY, (-180, 180)
    totalEnergy, energyLossRate, efficiency,
    temp, pressure,
    windX, windY, windZ
    */

    float_by_bytes(cur->estimate.roll); 
    float_by_bytes(cur->estimate.pitch);
    float_by_bytes(cur->estimate.yaw);

    float_by_bytes(cur->gps.longitude);
    float_by_bytes(cur->gps.latitude);

    float_by_bytes(cur->power.shaft);
    float_by_bytes(cur->power.propellor);

    float_by_bytes(cur->environment.temperature);
    float_by_bytes(cur->environment.pressure);

    float_by_bytes(cur->wind.x);
    float_by_bytes(cur->wind.y);
    float_by_bytes(cur->wind.z);
}

void send_pc_packet_test () {
    AircraftState test;

    test.estimate.roll      = 1.0;
    test.estimate.pitch     = 2.0;
    test.estimate.yaw       = 3.0;
    test.estimate.altitude = 4.0;
    test.estimate.speed     = 6.0;
    test.estimate.height    = 7.0;

    test.orientation.pitch  = 32.0; // 32 is 0x4200_0000 in hex, if the byte order is reversed (0x0000_0042) then it will be interpreted as 9.24856986454e-44
    test.orientation.roll   = 23.151; // 0x41b9353f, if reversed then 0.709858000278
    test.orientation.yaw    = (millis()/1000) % 360;

    test.gps.latitude       = (random(35999)-18000)/1000.0;
    test.gps.longitude      = (random(35999)-18000)/1000.0;
    test.gps.speed          = 25.3;
    test.gps.altitude      = 60.3;  

    test.power.shaft        = 567.32;
    test.power.propellor    = 555.50;

    test.environment.temperature    = 18.74;
    test.environment.humidity       = 72.1;
    test.environment.pressure       = 1020.57;
    test.environment.co2            = 486;

    test.wind.x             = 2.7;
    test.wind.y             = -6.8;
    test.wind.z             = 34.93;

    send_pc_packet(&test);
}

