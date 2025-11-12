/*
 * atmosphere.h
 *
 *  Created on: Nov 12, 2025
 *      Author: savo
 */

#ifndef INC_ATMOSPHERE_H_
#define INC_ATMOSPHERE_H_

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_fmpi2c.h"

struct AtmoConditions {
	float temperature_c;
	float humidity_rel;
	float static_pressure_pa;
	uint16_t co2_ppm;
};

HAL_StatusTypeDef atmo_setup(FMPI2C_HandleTypeDef* bus, uint32_t i2c_timeout_ms);

HAL_StatusTypeDef atmo_conditions_update(struct AtmoConditions* target);

#endif /* INC_ATMOSPHERE_H_ */
