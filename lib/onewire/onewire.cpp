#include <Arduino.h>

#include "onewire.hpp"
#include "onewireConfig.hpp"

void handleOneWireInput(); // Since it is only meant to be used as an interrupt it is locally scoped
void ow_send_data(uint32_t data, uint8_t width);

volatile uint8_t pinRX, pinTX;
volatile uint8_t oneWireAddress;
volatile int32_t oneWirePayloadOut;
volatile int32_t oneWirePayloadIn;
volatile bool oneWireMessageReceived;
volatile bool oneWireListener;

void ow_setup(uint8_t RX, uint8_t TX, uint8_t address, bool isListener) {
    pinRX = RX;
    pinTX = TX;

    pinMode(pinRX, INPUT);
    pinMode(pinTX, OUTPUT);
    digitalWrite(pinTX, LOW);

    oneWireAddress = address;
    oneWireListener = isListener;

    attachInterrupt(digitalPinToInterrupt(pinRX), handleOneWireInput, CHANGE);
}

void handleOneWireInput() {
    static unsigned long lastEdge = 0; // Store previous edge timestamp
    unsigned long present = micros();

    bool reading = digitalRead(pinRX);
    unsigned long delta = present - lastEdge;

    static uint8_t bitCount = 0;
    static uint8_t ignoreCount = 0;
    static uint32_t tempData = 0;

    // Too short since last edge, ignore. Probably setting up the next actual edge
    if (delta < (3 * OW_PULSE_PERIOD)) return;
    else lastEdge = present;

    // See if the edge is late (new message or timeout)
    if (delta > (2 * OW_BIT_PERIOD)) {
        bitCount = 0;
        ignoreCount = 0;
        tempData = 0;
        return;
    }

    // Are we ignoring data edges? (From other responders)
    if (ignoreCount != 0) {
        ignoreCount--;
        return;
    }
    
    // Process the edge
    tempData = (tempData << 1) + reading;
    bitCount++;
    
    // Process depending on if it's a caller or not
    if (oneWireListener) {
        // If there's an address check for a match
        if (bitCount == OW_ADDRESS_WIDTH) {

            if (tempData == oneWireAddress) ow_send_data(oneWirePayloadOut, OW_DATA_WIDTH);
            else {
                // Ignore the other device's response
                ignoreCount = OW_DATA_WIDTH;
            }

            // Reset for next message
            bitCount = 0;
            tempData = 0;
        }
    }
    else {
        // If awaiting a response
        if (bitCount == OW_DATA_WIDTH) {
            oneWireMessageReceived = true;

            // Extend sign by prefixing ones as needed prior to recording it
            if (tempData & (1L << (OW_DATA_WIDTH - 1))) {
                oneWirePayloadIn = tempData | (0xFFFFFFFF << (OW_DATA_WIDTH - 1));
            }
            else oneWirePayloadIn = tempData; 

            // Reset for next message
            bitCount = 0;
            ignoreCount = 0;
            tempData = 0;
        }
    }
}

bool ow_request(uint8_t targetAdd, int32_t *destination) {

    uint8_t attempts = 0;

    do {
        noInterrupts(); // Don't want it catching it's own messages
        oneWireMessageReceived = false; // Clear received flag

        // Pull line down for a half period to get attention of all devices
        digitalWrite(pinTX, HIGH);
        delayMicroseconds(OW_PULSE_PERIOD);

        // Send out address
        ow_send_data(targetAdd, OW_ADDRESS_WIDTH);

        // Read data in from line
        unsigned long timeoutMark = micros() + OW_TIMEOUT;
        while (!oneWireMessageReceived && (micros() < timeoutMark)) {
            //delayMicroseconds(1000);
        }

        attempts++;
    } while (!oneWireMessageReceived && (attempts < OW_NUMBER_ATTEMPTS));

    if (oneWireMessageReceived) *(destination) = oneWirePayloadIn; 
    //else *(destination) = 0;

    return oneWireMessageReceived;
}

/**
 * \brief Send data over one wire interface
 * 
 * \note Shifts data out MSB first. Positive dominant edges are for 1.
 * 
 * \warning Leaves interrupts enabled on completion
 * 
 * \param data Payload to send
 * \param width The width of the data to send in bits
 */
void ow_send_data(uint32_t data, uint8_t width) {
    noInterrupts(); // Don't want interrupts to catch outgoing message

    for (uint8_t i = width; i > 0; i--) {
        uint32_t mask = 1L << (i - 1); // Needs the `1L` otherwise mask will be 16 bits wide

        bool currentBit = ((mask & data) != 0);

        digitalWrite(pinTX, currentBit);
        delayMicroseconds(OW_BIT_PERIOD - OW_PULSE_PERIOD);
        digitalWrite(pinTX, !currentBit);
        delayMicroseconds(OW_PULSE_PERIOD);
    }

    digitalWrite(pinTX, LOW); // Release line
    interrupts();
}

void ow_set_payload(int32_t new_payload) {
    noInterrupts();
    oneWirePayloadOut = new_payload;
    interrupts();
}