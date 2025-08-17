#include "radio.hpp"
#include <Arduino.h>
#include <SPI.h>
#include <RF24.h>

const uint8_t INTERNAL_CE_PIN = PB5;
const uint8_t INTERNAL_CSN_PIN = PB9;
const uint8_t INTERNAL_IRQ_PIN = PB4;
RF24 int_radio(INTERNAL_CE_PIN, INTERNAL_CSN_PIN); // Radio used for internal communications

const uint8_t EXTERNAL_CE_PIN = PA9;
const uint8_t EXTERNAL_CSN_PIN = PA8;
const uint8_t EXTERNAL_IRQ_PIN = PA10;
RF24 ext_radio(EXTERNAL_CE_PIN, EXTERNAL_CSN_PIN); // Radio used for external communications

byte pipeNumber;
char radioMessage[32];
volatile bool recievedRadioData = false; 

int setupRadios(SPIClass* radio_bus) {
    defaultSetupRadio(radio_bus, &int_radio);
    defaultSetupRadio(radio_bus, &ext_radio);

    return 0; // A-okay radio start
}

int defaultSetupRadio(SPIClass* radio_bus, RF24* radio) {
    
    if (!radio->begin(radio_bus)) {
        // SerialUSB.println(F("radio hardware is not responding!!"));
        // while (1) {}  // hold in infinite loop
        return -1; // Failed to start radio
    }
    radio->setAutoAck(true);
    radio->setPALevel(RF24_PA_MAX);
    radio->enableAckPayload();               // Allow optional ack payloads
    radio->setRetries(5, 15);                // Smallest time between retries, max no. of retries
    radio->enableDynamicPayloads();
    radio->setChannel(100);
    if(radio->setDataRate(RF24_250KBPS) != true) return -2; // Failed to set radio data rate
    
    return 0; // Radio setup sans issue
}

void setReceiver(RF24* radio, uint8_t pipe, uint64_t address) {
    radio->openReadingPipe(pipe, address);    
    radio->startListening();
}

void setTransmitter(RF24* radio, uint64_t address) {
    radio->openWritingPipe(address);
    radio->stopListening();
}

void radioRecieved(RF24* radio) {
  recievedRadioData = true;

  // Start by getting where the message is from
  radio->available(&pipeNumber); // Get pipe the message was recieved on in the event a reply is needed

  // Read the message. Reads both messages and acknoledgements. 
  radio->read(&radioMessage, radio->getDynamicPayloadSize());
}

bool radioSend(RF24* radio, const char* outputMessage, byte messageLen) {
    return radio->write(outputMessage, messageLen);
}