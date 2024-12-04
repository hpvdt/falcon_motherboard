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
 * \brief Set the one wire response payload
 * 
 * \param new_payload What to repsond with next one wire query
 */
void ow_set_payload(int32_t new_payload);

/**
 * \brief Perform a series of trial exchanges with a device
 * 
 * \param address Target address
 * \param trials Number of trials to aatempt
 * 
 * \note Prints a summary over USB
 */
void ow_test_comms(uint8_t address, unsigned int trials);

#endif