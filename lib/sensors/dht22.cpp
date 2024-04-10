#include "dht22.h"

DHT_Unified dht(DHT_PIN, DHT_TYPE);
uint32_t delayMS_DHT;
double lastDHT;  // is the value of lastDHT preserved here?

void setupDHT() {

    dht.begin();
    sensor_t sensor;
    delayMS_DHT = sensor.min_delay / 1000;

}
void measureDHT(double* humidity, double* temperature) {

  if (millis() < lastDHT + delayMS_DHT) return;
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
    *temperature = event.temperature;
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
    *humidity = event.relative_humidity;
  }
  lastDHT = millis();
}

void printDHT() {

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
