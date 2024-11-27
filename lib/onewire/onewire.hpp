#ifndef ONEWIRE_HEADER_H
#define ONEWIRE_HEADER_H

#include <Arduino.h>

void ow_setup(uint8_t RX, uint8_t TX, uint8_t address = 0, bool isListener = true);
bool ow_request(uint8_t targetAdd, int32_t *destination);
void ow_set_payload(int32_t newPayload);

#endif