
#include <Arduino.h>
#include "aircraft_data.h"

enum MessageType : uint8_t {
    MESSAGE_TYPE_MAIN     = 'm',
    MESSAGE_TYPE_STRAIN   = 's',
    MESSAGE_TYPE_GPS      = 'g',
};

enum CommunicationChannel {
    COMM_CHANNEL_USB,
    COMM_CHANNEL_RADIO,
};

/**
 * \brief Set the up USB/PC communications
 * 
 * \param block Block until connection is confirmed or not
 */
void setup_pc_comms(bool block);

int send_mesage(MessageType msg_type, CommunicationChannel chn, AircraftState* state);
int send_test_mesage(MessageType msg_type, CommunicationChannel chn);
