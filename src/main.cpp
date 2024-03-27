/*
 * See documentation at https://nRF24.github.io/RF24
 * See License information at root directory of this library
 * Author: Brendan Doherty (2bndy5)
 */

/**
 * A simple example of sending data from 1 nRF24L01 transceiver to another.
 *
 * This example was written to be used on 2 devices acting as "nodes".
 * Use the Serial Monitor to change each node's behavior.
 */
#include <arduino.h>
#include <SPI.h>
#include "printf.h"
#include "RF24.h"
#include <Adafruit_DPS310.h>

#define CE_PIN PB5
#define CSN_PIN PB9
#define LEDPIN1 PB12
#define LEDPIN2 PC8
#define LEDPIN3 PC9
// instantiate an object for the nRF24L01 transceiver
RF24 radio(CE_PIN, CSN_PIN);
SPIClass int_telemetry_spi(PA7, PA6, PA5);
Adafruit_DPS310 dps;
Adafruit_Sensor *dps_temp = dps.getTemperatureSensor();
Adafruit_Sensor *dps_pressure = dps.getPressureSensor();
TwoWire mainbus(PB7, PB6);

// Let these addresses be used for the pair
uint8_t address[][6] = { "1Node", "2Node" };
// It is very helpful to think of an address as a path instead of as
// an identifying device destination

// to use different addresses on a pair of radios, we need a variable to
// uniquely identify which address this radio will use to transmit
bool radioNumber = 1;  // 0 uses address[0] to transmit, 1 uses address[1] to transmit

// For this example, we'll be using a payload containing
// a single float number that will be incremented
// on every successful transmission
struct PayloadStruct {
  long data[2];  // only using 6 characters for TX & ACK payloads
};
PayloadStruct payload;

float data = 0.0;
void setup() {

  SerialUSB.begin(115200);
  while (!SerialUSB) {
    // some boards need to wait to ensure access to serial over USB
  }
  digitalWrite(LEDPIN2, HIGH);
  Serial.println("DPS310");
  if (! dps.begin_I2C(119U, &mainbus)) {
    Serial.println("Failed to find DPS");
    while (1) yield();
  }
  Serial.println("DPS OK!");
  // Setup highest precision
  dps.configurePressure(DPS310_64HZ, DPS310_64SAMPLES);
  dps.configureTemperature(DPS310_64HZ, DPS310_64SAMPLES);
  
  dps_temp->printSensorDetails();
  dps_pressure->printSensorDetails();

  // initialize the transceiver on the SPI bus
  if (!radio.begin(&int_telemetry_spi)) {
    SerialUSB.println(F("radio hardware is not responding!!"));
    while (1) {}  // hold in infinite loop
  }

  // Set the PA Level low to try preventing power supply related problems
  // because these examples are likely run with nodes in close proximity to
  // each other.
  radio.setPALevel(RF24_PA_HIGH);  // RF24_PA_MAX is default.

  // save on transmission time by setting the radio to only transmit the
  // number of bytes we need to transmit a float
  radio.setPayloadSize(sizeof(payload));  // float datatype occupies 4 bytes
  radio.enableAckPayload();

  // set the TX address of the RX node into the TX pipe
  radio.openWritingPipe(address[radioNumber]);  // always uses pipe 0

  // additional setup specific to the node's role
  radio.stopListening();  // put radio in TX mode
  radio.setAutoAck(true);
 
  // For debugging info
  printf_begin();             // needed only once for printing details
  radio.printDetails();       // (smaller) function that prints raw register values
  radio.printPrettyDetails(); // (larger) function that prints human readable data
} // setup

void loop() {

  // This device is a TX node
  digitalWrite(LEDPIN1, HIGH);
  unsigned long start_timer = micros();                  // start the timer
  bool report = radio.write(&payload, sizeof(float));    // transmit & save the report
  unsigned long end_timer = micros();                    // end the timer

  if (report) {
    SerialUSB.println(data);  // print payload sent
    data = data + 0.1;     // increment float payload
    uint8_t pipe;
    if (radio.available(&pipe)) {  // is there an ACK payload? grab the pipe number that received it
        PayloadStruct received;
        radio.read(&received, sizeof(received));  // get incoming ACK payload
        SerialUSB.println(received.data[0]);    // print incoming message
      } else {
        SerialUSB.println(F(" Recieved: an empty ACK packet"));  // empty ACK packet received
      }
  } else {
    SerialUSB.println(F("Transmission failed or timed out"));  // payload was not delivered
  }
    // to make this example readable in the serial monitor
    delay(1000);  // slow transmissions down by 1 second
}  // loop
