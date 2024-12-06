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
 * \brief Perform a series of trial exchanges with a range of one wire nodes
 * 
 * \param address Target address
 * \param trials Number of trials to attempt
 * \param start_addr Start address of scan
 * \param end_addr End address of scan
 * 
 * \note Prints a summary over USB
 */
void ow_test_comms(uint8_t start_addr, uint8_t end_addr, unsigned int trials);

/**
 * \brief Send data over one wire interface
 * 
 * \param data Payload to send
 * \param width The width of the data to send in bits
 * 
 * \note Shifts data out MSB first. Positive dominant edges are for 1.
 * 
 * \warning Leaves interrupts enabled on completion
 */
void ow_send_data(uint32_t data, uint8_t width);

#endif