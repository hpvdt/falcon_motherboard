#include <stdint.h>
#include "onewireConfig.hpp"

const uint_fast8_t OW_BIT_PERIOD = 55;
const uint_fast8_t OW_PULSE_PERIOD = 15;
const uint_fast8_t OW_ADDRESS_WIDTH = 4;
const uint_fast8_t OW_DATA_WIDTH = 24;

const uint_fast8_t OW_NUM_ATTEMPTS = 3;
const uint_fast16_t OW_TIMEOUT_COMMS = 2000;

const uint_fast8_t OW_ADDR_TEST_ENABLE = 0x00;
const uint_fast8_t OW_ADDR_TEST_DISABLE = 0x01;