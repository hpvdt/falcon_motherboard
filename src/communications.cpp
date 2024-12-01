#include <Arduino.h>
#include "communications.hpp"
#include "aircraft_data.h"

void setup_pc_comms(bool block) {
    SerialUSB.begin(115200);

    if (block == false) return; // Return immediately once set up

    // Wait for USB connection to be made to the computer before continuing
    while (!SerialUSB) delay(10); 
}

uint_fast8_t append_type(MessageType msg_type, uint8_t* buffer, uint_fast8_t location) {
    buffer[location] = msg_type;

    return location + 1;
}

uint_fast8_t append_float(float data, uint8_t* buffer, uint_fast8_t location) {
    uint8_t * dataPointer = (uint8_t *) & data;

    for (int i = 0; i < 4; i++) buffer[location + i] = dataPointer[i]; // Least significant byte first

    return location + 4;
}

uint_fast8_t append_32_bits(uint32_t data, uint8_t* buffer, uint_fast8_t location) {
    for (int i = 0; i < 4; i++) buffer[location + i] = (data >>  (8 * i)) & 0xFF; 
    
    return location + 4;
}

uint_fast8_t append_24_bits(uint32_t data, uint8_t* buffer, uint_fast8_t location) {
    for (int i = 0; i < 3; i++) buffer[location + i] = (data >>  (8 * i)) & 0xFF; 
    
    return location + 3;
}

uint_fast8_t append_16_bits(uint16_t data, uint8_t* buffer, uint_fast8_t location) {
    for (int i = 0; i < 2; i++) buffer[location + i] = (data >>  (8 * i)) & 0xFF; 
    
    return location + 2;
}

uint_fast8_t append_8_bits(uint8_t data, uint8_t* buffer, uint_fast8_t location) {
    buffer[location] = data & 0xFF; 
    
    return location + 1;
}

uint_fast8_t append_checksum(uint8_t* buffer, uint_fast8_t location) {
    uint8_t sum = 0;

    for (int i = 0; i < location; i++) sum = sum + buffer[i];

    location = append_8_bits(sum, buffer, location);

    return location + 1;
}

size_t pack_strain_message(AircraftStrain* data, uint8_t* buffer) {
    uint_fast8_t location = 0;

    location = append_type(MESSAGE_TYPE_STRAIN, buffer, location);
    
    location = append_24_bits(data->left_wing_strain[0] , buffer, location);
    location = append_24_bits(data->left_wing_strain[1] , buffer, location);
    location = append_24_bits(data->left_wing_strain[2] , buffer, location);
    location = append_24_bits(data->right_wing_strain[0], buffer, location);
    location = append_24_bits(data->right_wing_strain[1], buffer, location);
    location = append_24_bits(data->right_wing_strain[2], buffer, location);
    location = append_24_bits(data->tail_strain         , buffer, location);
    location = append_24_bits(data->left_wing_torsion   , buffer, location);
    location = append_24_bits(data->right_wing_torsion  , buffer, location);
    location = append_24_bits(data->tail_torsion        , buffer, location);

    location = append_checksum(buffer, location);

    return location;
}

size_t pack_main_message(AircraftState* data, uint8_t* buffer) {
    uint8_t location = 0;

    location = append_type(MESSAGE_TYPE_MAIN, buffer, location);

    location = append_float(data->estimate.roll, buffer, location); 
    location = append_float(data->estimate.pitch, buffer, location);
    location = append_float(data->estimate.yaw, buffer, location);

    location = append_float(data->power.shaft, buffer, location);
    location = append_float(data->power.propellor, buffer, location);

    location = append_float(data->environment.temperature, buffer, location);
    location = append_float(data->environment.pressure, buffer, location);

    location = append_float(data->wind.x, buffer, location);
    location = append_float(data->wind.y, buffer, location);
    location = append_float(data->wind.z, buffer, location);

    location = append_checksum(buffer, location);

    return location;
}

size_t pack_gps_message(AircraftState* data, uint8_t* buffer) {
    uint8_t location = 0;

    location = append_type(MESSAGE_TYPE_GPS, buffer, location);

    location = append_float(data->gps.altitude, buffer, location);
    location = append_float(data->gps.speed, buffer, location);
    location = append_float(data->gps.latitude, buffer, location);
    location = append_float(data->gps.longitude, buffer, location);

    location = append_checksum(buffer, location);

    return location;
}

int send_mesage(MessageType msg_type, CommunicationChannel chn, AircraftState* state) {
    int ret_val = 0; // Default to 0 for OK
    uint8_t buffer[50];
    size_t length;

    switch (msg_type) {
    case MESSAGE_TYPE_MAIN:
        length = pack_main_message(state, buffer);
        break;
    case MESSAGE_TYPE_GPS:
        length = pack_gps_message(state, buffer);
        break;
    case MESSAGE_TYPE_STRAIN:
        length = pack_strain_message(&state->strain, buffer);
        break;
    
    default:
        return -1; // Not supported
        break;
    }

    size_t sent_length = 0;
    switch (chn) {
    case COMM_CHANNEL_USB:
        sent_length = SerialUSB.write(buffer, length);
        if (sent_length != length) return -3;
        break;

    default:
        return -2;
        break;
    }


    return ret_val;
}

int send_test_mesage(MessageType msg_type, CommunicationChannel chn) {
    AircraftState test;

    test.estimate.roll      = 1.0;
    test.estimate.pitch     = 2.0;
    test.estimate.yaw       = 3.0;
    test.estimate.altitude  = 4.0;
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

    return send_mesage(msg_type, chn, &test);
}