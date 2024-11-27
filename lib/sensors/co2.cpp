#include <Arduino.h>
#include "co2.h"

volatile int co2_ppm = 0; 

const byte CO2_PIN = PB0;   // CO2 pin on aircraft is PB0
const int CO2_SPAN = 5000;  // Span in ppm of CO2 of sensor

void co2_change() {
    const bool cur_state = digitalRead(CO2_PIN);
    const unsigned long cur_time = millis();

    // Used to record previous edge times
    static unsigned long last_co2_rising = 0;
    static unsigned long last_co2_falling = 0;

    // Determine edge type based on current state
    if (cur_state == HIGH) {
        // Treat it as a rising edge

        // Calculate CO2 level from last pulse
        // CO2 = ppm span * (Th - 2ms) / (Th + Tl - 4ms)

        unsigned long time_high = last_co2_falling - last_co2_rising;
        unsigned long time_low = cur_time - last_co2_falling;

        co2_ppm = CO2_SPAN * (time_high - 2);
        co2_ppm = co2_ppm / (time_high + time_low - 4);

        last_co2_rising = cur_time; // Update rising time
    }
    else {
        // Falling edge
        last_co2_falling = millis();
    }
}

void co2_setup() {
    pinMode(CO2_PIN, INPUT);

    attachInterrupt(CO2_PIN, co2_change, CHANGE);
}

void co2_record(int* ppm) {
    *ppm = co2_ppm;
}

void co2_print() {
    SerialUSB.println("CO2 Sensor Readings: ~~~~~~~~~~~~~~~~~~~~~~");
    int temp;
    co2_record(&temp);
    SerialUSB.printf("CO2 ppm: %d", temp);
}