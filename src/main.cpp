#include <Arduino.h>
#include <Adafruit_DPS310.h>
#include <USBSerial.h>
#include <Wire.h>


Adafruit_DPS310 dps;
Adafruit_Sensor *dps_pressure = dps.getPressureSensor();
HardwareSerial Serial1(PA3, PA2);
const int LEDPIN = PC8;
const int LEDPIN2 = PC9;

void setup() {
  SerialUSB.begin();
  SerialUSB.begin(9600);
  while (!SerialUSB) delay(10); 
  pinMode(LEDPIN, OUTPUT);
  pinMode(LEDPIN2, OUTPUT);

  SerialUSB.println("DPS310");

  if (! dps.begin_I2C()) {
    SerialUSB.println("Failed to find DPS");
    while (1) yield();
  }
  SerialUSB.println("DPS OK!");

  // Setup highest precision
  dps.configurePressure(DPS310_64HZ, DPS310_64SAMPLES);
  dps_pressure->printSensorDetails();
}

void loop() {
  digitalWrite(LEDPIN, HIGH);
  digitalWrite(LEDPIN2, HIGH);
  delay(1000);
  sensors_event_t pressure_event;
  if (dps.pressureAvailable()) {
    dps_pressure->getEvent(&pressure_event);
    SerialUSB.print(F("Pressure = "));
    SerialUSB.print(pressure_event.pressure);
    SerialUSB.println(" hPa"); 
    SerialUSB.println();
  }
}
