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
const int owAddMain = 2;
const int owAddTest = 0b1010; // Hardcoded address for spar boards.
const int LEDPIN = PC8;
const int owTX  = PC3;
const int owRX  = PC2;
const int owAdd = owAddMain;


void setup() {
  pinMode(LEDPIN, OUTPUT);
  setupOneWire(owRX, owTX, owAdd, false);
  Serial.begin(115200);
  delay(100);
  Serial.println("RUNNING THE TEST STATION CODE");
  Serial.println("MISSES / TESTS");

  int32_t rec;
  requestOneWire(owAddTest, &rec);
  Serial.println(rec);
}

void loop() {
  digitalWrite(LEDPIN, HIGH);
  static unsigned int count = 0;
  static unsigned int missed = 0;

  int32_t rec = 0;
  if (requestOneWire(owAddTest, &rec) == false) missed++;
  count++;

  // Serial.print(missed);
  // Serial.print("/");
  // Serial.println(count);
  Serial.println(rec);
  delay(5);
}
