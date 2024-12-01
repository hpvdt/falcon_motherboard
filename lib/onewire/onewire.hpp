#ifndef ONEWIRE_HEADER_H
#define ONEWIRE_HEADER_H

#include <Arduino.h>

/**
 * \brief Setups up a one wire interface
 * 
 * \param RX Pin for reading one wire bus
 * \param TX Pin connected to one wire transistor
 * \param address Address for device (default is 0)
 * \param isListener Set to true if device is listener (default is true). If a listener, it will set the handler interrupt routine up.
 */
void ow_setup(uint8_t RX, uint8_t TX, uint8_t address = 0, bool isListener = true);

/**
 * \brief Handles potential requests from the one wire bus
 * 
 * \note Meant to be an interrupt
 * \warning Blocks for the entirety of a transmission to host if responding
 */
bool ow_request(uint8_t targetAdd, int32_t *destination);

/**
 * \brief Requests and receives data from device on the one wire bus
 * 
 * \warning Leaves interrupts enabled once completed
 * 
 * \param targetAdd Address of the unit of interest
 * \param destination Pointer to location to store response from target
 * 
 * \return True if message was received
 */
void ow_set_payload(int32_t newPayload);

#endif