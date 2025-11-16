/*
 * orientation.h
 *
 *  Created on: Nov 13, 2025
 *      Author: savo
 */

#ifndef INC_ORIENTATION_H_
#define INC_ORIENTATION_H_

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_fmpi2c.h"
#include "falcon_data.h"


HAL_StatusTypeDef pos_setup(FMPI2C_HandleTypeDef* bus, uint32_t i2c_timeout_ms);

HAL_StatusTypeDef pos_orientation_update(struct OrientationState* target);

#endif /* INC_ORIENTATION_H_ */
