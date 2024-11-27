#include <Arduino.h>
#include "pc_communications.hpp"

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

void send_pc_packet () {
    /* the order of data going into the vision system:
  
    Airspeed, Altimeter,
    GyroX, GyroY, GyroZ, GyroW, (-1 , 1)
    heading, (0,360)
    GPSX, GPSY, (-180, 180)
    totalEnergy, energyLossRate, efficiency,
    temp, pressure,
    windX, windY, windZ

    Realistically we will make a function for this in the final aircraft
    to hold all these function calls with a bunch of parameters passed in
    for all the system characteristics.
    */

    float_by_bytes(32.000); // 32 is 0x4200_0000 in hex, if the byte order is reversed (0x0000_0042) then it will be interpreted as 9.24856986454e-44
    float_by_bytes(23.151); // 0x41b9353f, if reversed then 0.709858000278

    float_by_bytes((millis()/1000) % 360);

    float_by_bytes((random(35999)-18000)/1000.0);
    float_by_bytes((random(35999)-18000)/1000.0);

    float_by_bytes(567.32);
    float_by_bytes(555.50);
    float_by_bytes(0.9875);

    float_by_bytes(18.74);
    float_by_bytes(1020.57);

    float_by_bytes(2.7);
    float_by_bytes(-6.8);
    float_by_bytes(34.93);
}

