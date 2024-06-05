#include <Arduino.h>
#include "onewire.hpp"

/* Pin allocations
    PB0 - Resistive dividers for address
    PB1 - One Wire TX
    PB2 - Data from HX711
    PB3 - One Wire RX
    PB4 - Clock to HX711
    PB5 - RESET
*/
const int owAdd = 2;
const int owAddTest = 0b1010; // Hardcoded address for spar boards.
const int owTX  = PC3;
const int owRX  = PC2;


void setup() {
  setupOneWire(owRX, owTX, owAdd, false);
  Serial.begin(115200);

  int32_t rec;
  requestOneWire(owAddTest, &rec);
}

void loop() {
  int32_t rec = 0;
  requestOneWire(owAddTest, &rec);
}
