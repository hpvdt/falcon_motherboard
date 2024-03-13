#include <Arduino.h>
#include <RF24.h>
#include <Wire.h>
#include <Adafruit_DPS310.h>
#include <SPI.h>

const int LEDPIN = PC8;
const int LEDPIN2 = PC9;
RF24 radio1(PB5, PB9);
uint8_t interior_address[6] = "inter";
Adafruit_DPS310 dps;
Adafruit_Sensor *dps_temp = dps.getTemperatureSensor();
Adafruit_Sensor *dps_pressure = dps.getPressureSensor();
TwoWire i2c1(PB7, PB6);
SPIClass int_telemetry_bus(PA7, PA6, PA5);

void setup() {
  pinMode(LEDPIN, OUTPUT);
  pinMode(LEDPIN2, OUTPUT);

  SerialUSB.begin(9600); 
  while(!SerialUSB){
    delay(100);
  }
  digitalWrite(LEDPIN, HIGH);

  if (!radio1.begin(&int_telemetry_bus, PB5, PB9)) {
    SerialUSB.println("Radio not responding");
    while (1) {};
  }
  SerialUSB.println("Radio connected");
  digitalWrite(LEDPIN2, HIGH);

  if (!dps.begin_I2C(119U, &i2c1)){
    SerialUSB.println("DPS310 initalization failed");
    while (1) {
    };
  }
  SerialUSB.println("DPS310 connected");

  radio1.setPALevel(RF24_PA_LOW);
  radio1.openWritingPipe(interior_address);
  radio1.stopListening();

  dps.configurePressure(DPS310_64HZ, DPS310_64SAMPLES);
  dps.configureTemperature(DPS310_64HZ, DPS310_64SAMPLES);

  dps_temp->printSensorDetails();
  dps_pressure->printSensorDetails();
}

void loop() {
  sensors_event_t temp_event;

  digitalWrite(LEDPIN, HIGH);
  digitalWrite(LEDPIN2, HIGH);
  
  if (dps.temperatureAvailable()) {
    dps_temp->getEvent(&temp_event);
  }

  float T = temp_event.temperature;

  float data[] = {T};
  radio1.write(&data, sizeof(data));
}
