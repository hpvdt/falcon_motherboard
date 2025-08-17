#include "dht22.h"

#include <DHT.h>
#include <DHT_U.h>
#include <Adafruit_BusIO_Register.h>
#include <Adafruit_Sensor.h>

const byte DHT_PIN = PB1;
static const uint8_t DHT_TYPE = DHT22;

DHT_Unified dht(DHT_PIN, DHT_TYPE);
static uint32_t period_dht_check;
static uint32_t last_dht_check;  

void dht_setup() {
    dht.begin();
    sensor_t sensor;
    period_dht_check = sensor.min_delay / 1000;
}

void dht_record(float* humidity, float* temperature) {

    if (millis() < last_dht_check + period_dht_check) return;
        // Get temperature event and print its value.
    sensors_event_t event;
    dht.temperature().getEvent(&event);
    if (isnan(event.temperature)) {
        SerialUSB.println(F("Error reading temperature!"));
    }
    else {
        /*
        SerialUSB.print(F("Temperature: "));
        SerialUSB.print(event.temperature);
        SerialUSB.println(F("°C"));
        */
        *temperature = event.temperature;
    }
    // Get humidity event and print its value.
    dht.humidity().getEvent(&event);
    if (isnan(event.relative_humidity)) {
        SerialUSB.println(F("Error reading humidity!"));
    }
    else {
        /*
        SerialUSB.print(F("Humidity: "));
        SerialUSB.print(event.relative_humidity);
        SerialUSB.println(F("%"));
        */
        *humidity = event.relative_humidity;
    }
    last_dht_check = millis();
}

void dht_print() {

    sensors_event_t event;
    dht.temperature().getEvent(&event);
    double temperature = event.temperature;

    dht.humidity().getEvent(&event);
    double humidity = event.relative_humidity;
    
    SerialUSB.println("DHT22 Readings ~~~~~~~~~~~~~~~~~~~~~~");
    SerialUSB.print("DHT22 Temperature: ");
    SerialUSB.print(temperature);
    SerialUSB.println("°C");
    SerialUSB.print("DHT22 Humidity: ");
    SerialUSB.print(humidity);
    SerialUSB.println("%");
}
