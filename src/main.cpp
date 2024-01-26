#include <Arduino.h>

const int testPin = PB5;

void setup() {
  pinMode(testPin, OUTPUT);
}

void loop() {
  digitalWrite(testPin, HIGH);
  delay(2000);
  digitalWrite(testPin, LOW);
  delay(2000);
}
