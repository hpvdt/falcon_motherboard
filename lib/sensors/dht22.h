#ifndef DHT_HEADER
#define DHT_HEADER

#include <DHT.h>
#include <DHT_U.h>
#include <Adafruit_BusIO_Register.h>
#include <Adafruit_Sensor.h>

#define DHT_PIN PB1
#define DHT_TYPE DHT22

extern uint32_t delayMS;
extern DHT_Unified dht;

extern double temperature, humidity;

void setupDHT();
void DHT_measurements(); // These will grab new readings from the DHT.

#endif