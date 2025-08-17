#include <Arduino.h>

#include "onewire.hpp"
#include "onewireConfig.hpp"

void ow_handle_input(); // Since it is only meant to be used as an interrupt it is locally scoped
void ow_send_data(uint32_t data, uint8_t width);

/**
 * \brief Check if the message was received correctly
 * 
 * \param data Paylod to verify
 * \param origin_node THe node the message was meant to originate from
 *
 * \return True if the message checksum is correctly verified, flase on failure to verify 
 */
bool ow_verify_checksum(int32_t data, uint_fast8_t origin_node);

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

    attachInterrupt(digitalPinToInterrupt(pinRX), ow_handle_input, CHANGE);

    // Command into normal mode
    int32_t discard;
    ow_request(OW_ADDR_TEST_DISABLE, &discard);
}

void ow_handle_input() {
    static unsigned long lastEdge = 0; // Store previous edge timestamp
    unsigned long present = micros();

    bool reading = digitalRead(pinRX);
    unsigned long delta = present - lastEdge;

    static uint8_t bitCount = 0;
    static uint8_t ignoreCount = 0;
    static uint32_t tempData = 0;

    // Too short since last edge, ignore. Probably setting up the next actual edge
    if (delta < (3 * OW_PULSE_PERIOD)) return;
    lastEdge = present;

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

OWReceiveStatus ow_request(uint8_t targetAdd, int32_t *destination) {
    uint8_t attempts = 0;

    do {
        noInterrupts(); // Don't want it catching it's own messages
        oneWireMessageReceived = false; // Clear received flag

        // Pull line down for to get attention of all devices
        digitalWrite(pinTX, HIGH);
        delayMicroseconds(OW_PULSE_PERIOD / 2);

        // Send out address
        ow_send_data(targetAdd, OW_ADDRESS_WIDTH);

        // Read data in from line
        unsigned long timeoutMark = micros() + OW_TIMEOUT_COMMS;
        while (!oneWireMessageReceived && (micros() < timeoutMark)) {
            //delayMicroseconds(1000);
        }

        attempts++;
    } while (!oneWireMessageReceived && (attempts < OW_NUM_ATTEMPTS));

    if (oneWireMessageReceived == false) return OW_RX_TIMEOUT;

    if (ow_verify_checksum(oneWirePayloadIn, targetAdd) == false) return OW_RX_BAD_CHECKSUM;

    *(destination) = oneWirePayloadIn;

    return OW_RX_OK;
}

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

/**
 * \brief Checks if a test payload is valid or not
 * 
 * \param test Returned test payload
 * \param address Address of expected responder
 * 
 * \return True if the payload was valid and from expected source
 */
bool ow_verify_test_payload(int32_t test, uint8_t address) {
    uint8_t received_address = (test >> 24) & 0xFF;

    return (address = received_address);
}

void ow_test_comms(uint8_t start_addr, uint8_t end_address, unsigned int trials) {
    int32_t discard;

    SerialUSB.printf("OneWire scanning test. %d requests each for nodes %d to %d.\n", trials, start_addr, end_address);

    for (uint8_t node = start_addr; node <= end_address; node++) {
        int messages_requested = 0;
        int responses_received = 0;
        int checksums_failed = 0;
        int wrong_nodes = 0;
        int messages_passed = 0;
        ow_request(OW_ADDR_TEST_ENABLE, &discard);

        for (int i = 0; i < trials; i++) {
            int32_t test_data = 0;

            OWReceiveStatus status = ow_request(node, &test_data);
            messages_requested++;

            if (status != OW_RX_TIMEOUT) responses_received++;
            if (status == OW_RX_BAD_CHECKSUM) checksums_failed++;
            if (status != OW_RX_OK) continue;

            if (ow_verify_test_payload(test_data, node) == true) messages_passed++;
            else wrong_nodes++;

            delayMicroseconds(100);
        }

        SerialUSB.printf("\tNode %2d | RQ:%4d RX:%4d PASS:%4d | FCS:%4d FADD:%4d\n", node, messages_requested, 
            responses_received, messages_passed, checksums_failed, wrong_nodes);
    }

    ow_request(OW_ADDR_TEST_DISABLE, &discard);
}

bool ow_verify_checksum(int32_t data, uint_fast8_t origin_node) {
    uint8_t calculated_checksum = origin_node;
    calculated_checksum += data & 0xFF;
    calculated_checksum += (data >> 8) & 0xFF;
    calculated_checksum += (data >> 16) & 0xFF;

    uint8_t received_checksum = (data >> 24) & 0xFF;

    return received_checksum == calculated_checksum;
}