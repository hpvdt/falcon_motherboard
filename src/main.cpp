#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <USBSerial.h>
#include <Wire.h>
#include <Adafruit_BusIO_Register.h>

#include "co2.h"
#include <DHT.h>
#include <DHT_U.h>

#define DHT_PIN PB1
#define DHT_TYPE DHT22

const int LEDPIN = PC8;
const int LEDPIN2 = PC9;

uint32_t delayMS;
DHT_Unified dht(DHT_PIN, DHT_TYPE);

void setup()
{
  SerialUSB.begin();
  setupCO2();

  dht.begin();

  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  SerialUSB.println(F("------------------------------------"));
  SerialUSB.println(F("Temperature Sensor"));
  SerialUSB.print(F("Sensor Type: "));
  SerialUSB.println(sensor.name);
  SerialUSB.print(F("Driver Ver:  "));
  SerialUSB.println(sensor.version);
  SerialUSB.print(F("Unique ID:   "));
  SerialUSB.println(sensor.sensor_id);
  SerialUSB.print(F("Max Value:   "));
  SerialUSB.print(sensor.max_value);
  SerialUSB.println(F("°C"));
  SerialUSB.print(F("Min Value:   "));
  SerialUSB.print(sensor.min_value);
  SerialUSB.println(F("°C"));
  SerialUSB.print(F("Resolution:  "));
  SerialUSB.print(sensor.resolution);
  SerialUSB.println(F("°C"));
  SerialUSB.println(F("------------------------------------"));

  // Print humidity sensor details.
  dht.humidity().getSensor(&sensor);
  SerialUSB.println(F("Humidity Sensor"));
  SerialUSB.print(F("Sensor Type: "));
  SerialUSB.println(sensor.name);
  SerialUSB.print(F("Driver Ver:  "));
  SerialUSB.println(sensor.version);
  SerialUSB.print(F("Unique ID:   "));
  SerialUSB.println(sensor.sensor_id);
  SerialUSB.print(F("Max Value:   "));
  SerialUSB.print(sensor.max_value);
  SerialUSB.println(F("%"));
  SerialUSB.print(F("Min Value:   "));
  SerialUSB.print(sensor.min_value);
  SerialUSB.println(F("%"));
  SerialUSB.print(F("Resolution:  "));
  SerialUSB.print(sensor.resolution);
  SerialUSB.println(F("%"));
  SerialUSB.println(F("------------------------------------"));
  // Set delay between sensor readings based on sensor details.
  delayMS = sensor.min_delay / 1000;
}

void loop()
{

  SerialUSB.print("CO2 PPM: ");
  SerialUSB.println(CO2ppm);
  // Delay between measurements.
  delay(delayMS);
  // Get temperature event and print its value.
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature))
  {
    Serial.println(F("Error reading temperature!"));
  }
  else
  {
    Serial.print(F("Temperature: "));
    Serial.print(event.temperature);
    Serial.println(F("°C"));
  }
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity))
  {
    Serial.println(F("Error reading humidity!"));
  }
  else
  {
    Serial.print(F("Humidity: "));
    Serial.print(event.relative_humidity);
    Serial.println(F("%"));
  }
}
