/*
 * communications.c
 *
 *  Created on: Nov 16, 2025
 *      Author: savo
 */
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "falcon_data.h"

enum MessageType {
    MESSAGE_TYPE_MAIN           = 'm',
    MESSAGE_TYPE_STRAIN         = 's',
    MESSAGE_TYPE_GPS            = 'g',
    MESSAGE_TYPE_PRETTY_STRAIN  = 'S',
};

int print_nice_wing_loading(struct WingLoading load, uint8_t* buffer, uint16_t buff_len) {
	snprintf((char*) buffer, buff_len, "%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f\r\n",
			load.strain_port[2],
			load.strain_port[1],
			load.strain_port[0],
			load.strain_center,
			load.strain_starboard[0],
			load.strain_starboard[1],
			load.strain_starboard[2],
			load.torsion
			);
	return strnlen((char*) buffer, buff_len);
}

uint_fast8_t append_type(enum MessageType msg_type, uint8_t* buffer, uint_fast8_t location) {
    buffer[location] = (uint8_t) msg_type;

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

size_t pack_strain_message(struct WingLoading* data, uint8_t* buffer) {
    uint_fast8_t location = 0;

    location = append_type(MESSAGE_TYPE_STRAIN, buffer, location);

    location = append_float(data->strain_port[0]		, buffer, location);
    location = append_float(data->strain_port[1]		, buffer, location);
    location = append_float(data->strain_port[2]		, buffer, location);
    location = append_float(data->strain_starboard[0]	, buffer, location);
    location = append_float(data->strain_starboard[1]	, buffer, location);
    location = append_float(data->strain_starboard[2]	, buffer, location);
    location = append_float(data->strain_center			, buffer, location);
    location = append_float(data->torsion				, buffer, location);

    location = append_checksum(buffer, location);

    return location;
}

size_t pack_main_message(struct CompleteAircraftData* data, uint8_t* buffer) {
    uint8_t location = 0;

    location = append_float(data->orientation.roll_deg, buffer, location);
    location = append_float(data->orientation.pitch_deg, buffer, location);
    location = append_float(data->orientation.yaw_deg, buffer, location);

    location = append_float(data->power.shaft_w, buffer, location);
    location = append_float(data->power.propellor_w, buffer, location);

    location = append_float(data->atmo.temperature_c, buffer, location);
    location = append_float(data->atmo.static_pressure_pa, buffer, location);

    location = append_float(data->atmo.wind.x_m_s, buffer, location);
    location = append_float(data->atmo.wind.y_m_s, buffer, location);
    location = append_float(data->atmo.wind.z_m_s, buffer, location);

    location = append_checksum(buffer, location);

    return location;
}

size_t pack_gps_message(struct GPSState* data, uint8_t* buffer) {
    uint8_t location = 0;

    location = append_type(MESSAGE_TYPE_GPS, buffer, location);

    location = append_float(data->altitude_m, buffer, location);
    location = append_float(data->speed_m_s, buffer, location);
    location = append_float(data->latitude_deg, buffer, location);
    location = append_float(data->longitude_deg, buffer, location);

    location = append_checksum(buffer, location);

    return location;
}
