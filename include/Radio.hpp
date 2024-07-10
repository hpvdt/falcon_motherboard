#ifndef RADIO_HEADER_H
#define RADIO_HEADER_H

#include <Arduino.h>
#include <SPI.h>
#include <RF24.h>

int setupRadios(SPIClass* radio_bus);

int defaultSetupRadio(SPIClass* radio_bus, RF24* radio);
void setReceiver(RF24* radio, uint8_t pipe, uint64_t address);
void setTransmitter(RF24* radio, uint64_t address);
void radioRecieved(RF24* radio);
bool radioSend(RF24* radio, const char* outputMessage, byte messageLen);
#endif