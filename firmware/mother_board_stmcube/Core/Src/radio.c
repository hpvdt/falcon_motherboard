/*
 * radio.c
 *
 *  Created on: Jan 22, 2026
 *      Author: savo
 */

#include "radio.h"
#include "spi.h"
#include "tim.h"
#include "main.h"
#include "falcon_data.h"
#include "../../stm32_hal_nrf24_library/NRF24.h"

struct NRFConfig radio_ext = {
	.hspiX = &hspi1,
	.spi_w_timeout = 1000,
	.spi_r_timeout = 1000,
	.spi_rw_timeout = 1000,

	.csn_gpio_port = RADIO_EXT_CS_GPIO_Port,
	.csn_gpio_pin = RADIO_EXT_CS_Pin,

	.ce_gpio_port = RADIO_EXT_CE_GPIO_Port,
	.ce_gpio_pin = RADIO_EXT_CE_Pin,
};

struct NRFConfig radio_int = {
	.hspiX = &hspi1,
	.spi_w_timeout = 1000,
	.spi_r_timeout = 1000,
	.spi_rw_timeout = 1000,

	.csn_gpio_port = RADIO_INT_CS_GPIO_Port,
	.csn_gpio_pin = RADIO_INT_CS_Pin,

	.ce_gpio_port = RADIO_INT_CE_GPIO_Port,
	.ce_gpio_pin = RADIO_INT_CE_Pin,
};

#define PLD_S 4

uint8_t rx_addr[5] = {'1', 'N', 'o', 'd', 'e'};
uint8_t tx_addr[5] = {'2', 'N', 'o', 'd', 'e'};

uint8_t radio_tx_buffer[PLD_S];
uint8_t radio_rx_buffer[PLD_S];

HAL_StatusTypeDef radio_setup() {
	struct NRFConfig* target_radio = &radio_int;
	nrf24_init(*target_radio, &htim8);

	nrf24_pipe_pld_size(*target_radio, 0, PLD_S);
	nrf24_pipe_pld_size(*target_radio, 1, PLD_S);

	nrf24_open_tx_pipe(*target_radio, tx_addr);
	nrf24_open_rx_pipe(*target_radio, 1, rx_addr);

	nrf24_listen(*target_radio);

	target_radio = &radio_ext;
	nrf24_init(*target_radio, &htim8);

	nrf24_pipe_pld_size(*target_radio, 0, PLD_S);
	nrf24_pipe_pld_size(*target_radio, 1, PLD_S);

	nrf24_open_tx_pipe(*target_radio, rx_addr);
	nrf24_open_rx_pipe(*target_radio, 0, rx_addr);

	nrf24_stop_listen(*target_radio);
	ce_high(radio_ext);

	return HAL_OK;
}

HAL_StatusTypeDef radio_receive_internal(struct CompleteAircraftData* target) {
	if(!nrf24_data_available(radio_int)) return HAL_TIMEOUT;

	uint32_t data_received = 0;
	nrf24_receive(radio_int, radio_rx_buffer, sizeof(radio_rx_buffer));
	data_received = nrf24_uint8_t_to_type(radio_rx_buffer, sizeof(data_received));

	return HAL_OK;
}

HAL_StatusTypeDef radio_broadcast_data(struct CompleteAircraftData* target) {
	nrf24_transmit(radio_ext, (uint8_t*) &target->pilot.propellor_pitch_angle_rad, 4);

	return HAL_OK;
}
