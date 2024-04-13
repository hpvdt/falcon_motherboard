#ifndef RADIO_HEADER_H
#define RADIO_HEADER_H

#include <Arduino.h>

bool setupRadio(SPIClass* radio_bus, uint8_t CE_PIN, uint8_t CSN_PIN, uint8_t level);


#endif