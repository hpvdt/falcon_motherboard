#include <Arduino.h>

const int testPin = PB5;

void setup() {
  pinMode(testPin, OUTPUT);

  SerialUSB.begin();
  while (!SerialUSB) delay(10); // Wait for USB connection to be made to the computer before continuing
}

void loop() {
  SerialUSB.println(millis());

  digitalWrite(testPin, HIGH);
  delay(20);
  digitalWrite(testPin, LOW);
  delay(20);
}
