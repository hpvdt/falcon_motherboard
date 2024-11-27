
#include <Arduino.h>
#include "aircraft_data.h"

void setup_pc_comms(bool block);
void send_pc_packet(AircraftState* cur);
void send_pc_packet_test();