#include "dht22.h"

DHT_Unified dht(DHT_PIN, DHT_TYPE);
uint32_t delayMS_DHT;
double humidity, temperature;

void setupDHT() {

    dht.begin();
    sensor_t sensor;
    delayMS_DHT = sensor.min_delay / 1000;

}
void measureDHT() {

    // Get temperature event and print its value.
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature))
  {
    SerialUSB.println(F("Error reading temperature!"));
  }
  else
  {
    /*
    SerialUSB.print(F("Temperature: "));
    SerialUSB.print(event.temperature);
    SerialUSB.println(F("°C"));
    */
    temperature = event.temperature;
  }
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity))
  {
    SerialUSB.println(F("Error reading humidity!"));
  }
  else
  {
    /*
    SerialUSB.print(F("Humidity: "));
    SerialUSB.print(event.relative_humidity);
    SerialUSB.println(F("%"));
    */
    humidity = event.relative_humidity;
  }

}

void printDHT() {
  SerialUSB.println("DHT22 Readings ~~~~~~~~~~~~~~~~~~~~~~");
  SerialUSB.print("DHT22 Temperature: ");
  SerialUSB.print(temperature);
  SerialUSB.println("°C");
  SerialUSB.print("DHT22 Humidity: ");
  SerialUSB.print(humidity);
  SerialUSB.println("%");
}