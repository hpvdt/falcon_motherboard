#ifndef DHT_HEADER
#define DHT_HEADER

void dht_setup();
void dht_record(float* temperature, float* humidity); // These will grab new readings from the DHT.
void dht_print();

#endif