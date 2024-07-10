#include "Radio.hpp"
#include <Arduino.h>
#include <SPI.h>
#include <RF24.h>

byte pipeNumber;
char radioMessage[32];
volatile bool recievedRadioData = false; 

int setupRadio(SPIClass* radio_bus, uint8_t CE_PIN, uint8_t CSN_PIN){
    RF24 radio(CE_PIN, CSN_PIN);
    if (!radio.begin(radio_bus)) {
        SerialUSB.println(F("radio hardware is not responding!!"));
        while (1) {}  // hold in infinite loop
    }
    radio.setAutoAck(true);
    radio.setPALevel(RF24_PA_MAX);
    radio.setDataRate(RF24_250KBPS);
    radio.enableAckPayload();               // Allow optional ack payloads
    radio.setRetries(5, 15);                // Smallest time between retries, max no. of retries
    radio.enableDynamicPayloads();
    radio.setChannel(100);
}

void setReceiver(RF24 radio, uint8_t pipe, uint8_t address){
    radio.openReadingPipe(pipe, address);    
    radio.startListening();
}

void setTransmitter(RF24 radio, uint8_t address){
    radio.openWritingPipe(address);
    radio.stopListening();
}

void radioRecieved(RF24 radio) {
  recievedRadioData = true;

  // Start by getting where the message is from
  radio.available(&pipeNumber); // Get pipe the message was recieved on in the event a reply is needed

  // Read the message. Reads both messages and acknoledgements. 
  radio.read(&radioMessage, radio.getDynamicPayloadSize());
}

void radioSend(const char* outputMessage, byte messageLen) {
    
}