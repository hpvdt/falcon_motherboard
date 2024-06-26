#ifndef DHT_HEADER
#define DHT_HEADER

#include <DHT.h>
#include <DHT_U.h>
#include <Adafruit_BusIO_Register.h>
#include <Adafruit_Sensor.h>

#define DHT_PIN PB1
#define DHT_TYPE DHT22



void setupDHT();
void measureDHT(double* temperature, double* humidity); // These will grab new readings from the DHT.
void printDHT();

#endif